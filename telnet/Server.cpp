/*!
  \file     Server.cpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/22/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the Server class.
*/


#include <stdio.h>          // sprintf and other input,output operations
#include <stdlib.h>         // general purpose functions
#include <string.h>         // c strings
#include <string>           // std::string
#include <unistd.h>         // close
#include <sys/types.h>      // data types used in system calls
#include <sys/socket.h>     // sockets
#include <netinet/in.h>     // internet domain addresses
#include <thread>           // threading
#include <mutex>            // mutex locks
#include <iostream>         // displaying messages on server
#include "Server.hpp"
#include <GameLogic.hpp>


namespace legacymud { namespace telnet {


/******************************************************************************
* Constructor:    Server()                     
*****************************************************************************/
Server::Server() { 
    /* Sets default private member variables.  initServer needs to be called afterwards */
    /* to initialize the server. */
    _serverPort = 0;     
    _maxPlayers = 0;  
    _playerCount = 0;
    _timeOut = 0;
    _listenSocketFd = 0;
    _serverPause = false;
    _gameLogicPt = 0;       
}


/******************************************************************************
* Function:    initServer                 
*****************************************************************************/
bool Server::initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt) {   
    struct sockaddr_in serverAddr;         // address structure for server  
    
    /* Set member variables. */
    if (!_setServerPort(serverPort) || !setMaxPlayers(maxPlayers) || !setTimeOut(timeOut) || !_setGameLogicPt(gameLogicPt) ) {
        std::cout << "Error setting server parameters." << std::endl; // Error setting member variables
        return false;    
    }        
    
    /* Initialize the server. Create a socket. TCP/IP. */
    _listenSocketFd = socket(AF_INET, SOCK_STREAM, 0);   // IP, TCP
    if (_listenSocketFd < 0) {
        std::cout << "Error opening socket." << std::endl; // Error opening socket
        return false;
    }           
    
    /* Fill server address struct. */
    bzero((char *) &serverAddr, sizeof(serverAddr));     // initialize the server address struct to zeros
    serverAddr.sin_family = AF_INET;                     // IP
    serverAddr.sin_addr.s_addr = INADDR_ANY;             // open to any IP
    serverAddr.sin_port = htons(_serverPort);            // server port number
    
    /* Bind the socket to the port. */
    if (bind(_listenSocketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error cannot connect to port." << std::endl; // Error cannot connect to port  
        return false;          
    }    
    
    std::cout << "Server initialized on port " << _serverPort << std::endl;
    return true;   
}


/******************************************************************************
* Function:    startListening
*****************************************************************************/
void Server::startListening() {   
    struct sockaddr_in clientAddr;         // address structure for client
    socklen_t clientLength;                // length of a client's address structure   
    int newClientSocketFd;                 // new socket connected to client
    struct timeval timeOut;                // timeout timer for receiving messages on a socket.
    timeOut.tv_sec = getTimeOut();         // sets a player time-out period in seconds for disconnect 
   
    while (_listenSocketFd > 0 ) {
        
        /* Listen for connection. */
        std::cout << "Listening for new connection..." << std::endl;
        listen(_listenSocketFd, 10);      // maximum connection requests at a time set to 10
        
        /* Accept connection. */
        clientLength = sizeof(clientAddr);
        newClientSocketFd = accept(_listenSocketFd, (struct sockaddr *) &clientAddr, &clientLength);
        if (newClientSocketFd < 0 && _listenSocketFd > 0) {   
            std::cout << "Error accepting connection" << std::endl;     // error accepting connection
        }
        else {
            std::cout << "Connection established. PlayerFd: " << newClientSocketFd << std::endl;              
            
            /* Add player to player map. This map is a list of players on the server. */
            if (_addPlayerToMap(newClientSocketFd) == false) {
                std::cout << "Error adding player to map." << std::endl;    // error adding player to a map
                disconnectPlayer(newClientSocketFd);
            }
            /* Set the player's Telnet terminal into character mode. */
            else if (_setCharacterMode(newClientSocketFd) == false) {
                std::cout << "Error setting character mode in the client." << std::endl; // error setting character mode
                disconnectPlayer(newClientSocketFd);
            }
            /* Disconnect the player if the player cap is exceeded. */
            else if (_playerCount > _maxPlayers) {
                sendMsg(newClientSocketFd, "Server is full.  Please try again later.");    // server is full
                disconnectPlayer(newClientSocketFd);
            }
            /* Disconnect the new player if game backup is in progress. */
            else if (_serverPause == true) {
                sendMsg(newClientSocketFd, "Game backup in progress.  Please try again later."); // game backup in progress
                disconnectPlayer(newClientSocketFd);    
            }
            /* Set disconnect timeout on player's socket. */
            else if (setsockopt (newClientSocketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut, sizeof(timeOut)) < 0) {
                std::cout << "Error setting socket time-out." << std::endl;   // error setting time-out
                disconnectPlayer(newClientSocketFd);                
            }
            /* Create a new thread for this player and send to the game logic new player handler. */
            else {
                std::thread t(&legacymud::engine::GameLogic::newPlayerHandler, _gameLogicPt, newClientSocketFd);    
                t.detach();                        
            }  
        }   
    }
}


/******************************************************************************
* Function:    disconnectPlayer
*****************************************************************************/
bool Server::disconnectPlayer(int playerFd) {   
    
    /* Only close and decrement player count when player is in map. */
    /* Protects against accidental multiple calls to disconnect a player. */   
    if (_removePlayerFromMap(playerFd) == false )     
        return false;
 
    else {      
        _playerCount--;                     // decrement player count
        shutdown(playerFd, SHUT_RDWR);      // needed to break any blocked reads.

        /* Recycle file descriptors through a que to prevent reuse. */
        std::lock_guard<std::mutex> lock(_mu_fd_held_que);   // Lock fd que. Lock is released when it goes out of scope. 
        _fd_held_que.push_back(playerFd);
        if (_fd_held_que.size() > 250) {   // Limit held file descriptors to 250.  OSU Flip server limit is 1024.
            close(_fd_held_que.front());   // destroy held socket
            _fd_held_que.pop_front();    
        }
        
        std::cout << "Player disconnected. PlayerFd: " << playerFd << std::endl;
        return true;
    }
}


/******************************************************************************
* Function:    shutDownServer
*****************************************************************************/
bool Server::shutDownServer() {   
    
    /* Only shutdown if there are no players on the server and the listening socket 
       is greater than 0. The game logic would have to first pause the server and
       disconnect all the players before calling this. */
    
    if (_playerCount > 0 && _listenSocketFd > 0) {
        return false;
    }
    else {
        /* Close the listening socket. */
        shutdown(_listenSocketFd, SHUT_RDWR);   // break blocked read/write
        close(_listenSocketFd);                 // destroy socket
        
        /* Return parameters to default state. */
        _serverPort = 0;     
        _maxPlayers = 0;  
        _playerCount = 0;
        _timeOut = 0;
        _listenSocketFd = 0;
        _serverPause = false;
        _gameLogicPt = 0; 
        
        return true;
    }
}


/******************************************************************************
* Function:    sendQuestion
*****************************************************************************/
bool Server::sendQuestion(int playerFd, std::string outQuestion) {
   
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_player_map);    

    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If the player is in the map, send the message. */
    if(player == _playerMap.end()) 
        return false;   // the player is not in the game
        
    else {

        /* Add the question to the player's question buffer. */
        player->second.questionBuffer = outQuestion;
        
        /* If a player is entering text, clear that text from their display. */
        unsigned char eraseStr[3] = {8,32,8};     // ASCII backspace, space, backspace
        for (unsigned int i = 0; i < player->second.readBuffer.size(); i++ ) {
            if (write(playerFd, eraseStr, 3) < 0) 
                return false;      
        }         
        
        /* Add a newline at the end of the question. */
        outQuestion += "\015\012";       // attach carriage return  and linefeed
        
        /* Write to the socket. */ 
        if (write(playerFd,outQuestion.c_str(),strlen(outQuestion.c_str())) < 0) 
            return false;   // Error writing to socket               
        
        /* Write the player's read buffer to the socket if it is not empty. */
        else {
            if (player->second.readBuffer.size() > 0 ) {
                if (write(playerFd,player->second.readBuffer.c_str(),strlen(player->second.readBuffer.c_str())) < 0) 
                    return false;   // Error writing to socket             
            }
            return true;     
        }       
    }                  
} 


/******************************************************************************
* Function:    sendMsg
*****************************************************************************/
bool Server::sendMsg(int playerFd, std::string outMsg) {
   
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_player_map);    

    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If the player is in the map, send the message. */
    if(player == _playerMap.end()) 
        return false;   // the player is not in the game
        
    else {
        unsigned char eraseStr[3] = {8,32,8};     // ASCII backspace, space, backspace

        /* If a player is entering text clear that text from their display. */
        int readBufferSize = player->second.readBuffer.size();
        if (readBufferSize > 0) {
            for (int i = 0; i < readBufferSize; i++ ) {
                if (write(playerFd, eraseStr, 3) < 0) 
                    return false;      
            }             
        }
               
        /* If a has a question to be answered, clear that text from their display. */
        int questionBufferSize = player->second.questionBuffer.size();
        if (questionBufferSize > 0) {
            for (int i = 0; i < questionBufferSize; i++ ) {
                if (write(playerFd, eraseStr, 3) < 0) 
                    return false;      
            }  
            /* Erase rest of current line.  Additional characters remain depending on how wide a user's terminal display is. */
            std::string clearLineandMoveCursorToLeft = "\033\[2K\033\[2000D";  // ANSI code to eraseline and moves cursor to beginning of this line
            if (write(playerFd, clearLineandMoveCursorToLeft.c_str(), strlen(clearLineandMoveCursorToLeft.c_str())) < 0) 
                    return false; 
            
        }
         
        /* Erase current line. */
        std::string moveLeft = "\033\[2K\033\[2000D";  // erases line and moves cursor to beginning of this line
        if (write(playerFd, moveLeft.c_str(), strlen(moveLeft.c_str())) < 0) 
                return false; 
        
        /* Add a newline at the end of the message. */ 
        outMsg += "\015\012";       // attach carriage return  and linefeed
        
        /* Write to the socket. */ 
        if (write(playerFd,outMsg.c_str(),strlen(outMsg.c_str())) < 0) 
            return false;   // Error writing to socket               
               
        else {
            /* Write the player's question buffer to the socket if it is not empty. */
            if (player->second.questionBuffer.size() > 0 ) {
                std::string questionStr = player->second.questionBuffer + "\015\012";   // end with a carriage return linefeed
                if (write(playerFd,questionStr.c_str(),strlen(questionStr.c_str())) < 0) 
                    return false;   // Error writing to socket             
            }
            
            /* Write the player's read buffer to the socket if it is not empty. */
            if (player->second.readBuffer.size() > 0 ) {
                if (write(playerFd,player->second.readBuffer.c_str(),strlen(player->second.readBuffer.c_str())) < 0) 
                    return false;   // Error writing to socket             
            }
            
            return true;     
        }       
    }                  
}  


/******************************************************************************
* Function:    sendNewLine
*****************************************************************************/
bool Server::sendNewLine(int playerFd) {   
    return sendMsg(playerFd, "");   
} 
    

/******************************************************************************
* Function:    receiveMsg
*****************************************************************************/
bool Server::receiveMsg(int playerFd, std::string &inMsg ) {
    unsigned char ch = 0;       // character read from the socket
      
    /* Set player map lock. Lock is released when it goes out of scope. */
    std::unique_lock<std::mutex> lock_player_map(_mu_player_map);
      
    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If this player file descriptor is not in the map, return false. */
    if(player == _playerMap.end()) 
        return false;       
    
    /* Player is in the player map. Receive a message from this player. */
    else {
        player->second.readBuffer.clear();  // initialize this player's read buffer    
        inMsg.clear();                      // initialize inMsg
        
        // Read the socket.  Continue to read until a carriage return (ASCII 13) is received.
        while (ch != 13) {  // ASCII carriage return

            /* Unlock the player map mutex for the next read */
            lock_player_map.unlock(); 
            
            /* Read a character. */
            if (read(playerFd,&ch,1) <= 0) {              
                return false;     // error: returned -1 when error reading, or 0 if client disconnected.   
            }
            /* Character received. */
            else {              
                
                /* Lock the player map mutex. */
                lock_player_map.lock();             
                
                /* Find this player again in case disconnectPlayer was called while blocked. */
                auto player = _playerMap.find(playerFd); 
 
                /* If this player file descriptor is not in the map, return false. */
                if(player == _playerMap.end()) 
                    return false; 

                /* Read an ANSI escape code and clear it so it doesn't display in the player's terminal. */
                if (ch == 27 ) {
                    /* Capture the rest if the ansi code. */
                    unsigned char ansiCode[10];
                    if (read(playerFd,ansiCode,10) <= 0)   // returns -1 when error reading, returns 0 if client disconnected.    
                        return false;    
                }
                
                /* Backspace character received. Supports terminals that send 8 or 127. */ 
                else if (ch == 8 || ch == 127) {
                    
                    /* If the player's receive buffer has something in it, remove the last character and echo to that player. */
                    if (player->second.readBuffer.size() > 0) {
                        
                        /* Remove the last character from the in message. */
                        player->second.readBuffer.pop_back();               
                    
                        /* Delete player's previous character. */
                        unsigned char eraseStr[3] = {8,32,8};     // ASCII backspace, space, backspace
                        if (write(playerFd, eraseStr, 3) < 0) 
                            return false;  
                    }
                }
                    
                /* Character to be added to the message. */
                else if (ch >=32 && ch <=126) {
                    
                    /* Add the character to the read buffer. */
                    player->second.readBuffer += ch;
                    
                    /* If echo is set to false, set ch to '*'. */
                    if (player->second.echo == false)
                        ch = '*';
                    
                    /* Display the character on the player's terminal. */
                    if (write(playerFd, &ch, 1) < 0) 
                        return false; 
                }              
            }   
        }
        
        /* Return the read string by setting it to inMsg and clear this player's buffers. */        
        inMsg = player->second.readBuffer;    // set the return message
        player->second.readBuffer.clear();    // clear the read buffer
        if (player->second.questionBuffer.size() > 0 ) 
            player->second.questionBuffer.clear();    // clear the question buffer
        
        /* Unlock the player map mutex.  Need to unlock before calling sendNewLine since it also uses this mutex. */
        lock_player_map.unlock();            
        
        /* Send new line to the player's terminal. */
        if (sendNewLine(playerFd) == false ) 
            return false;     
        
        return true;
    }
}


/******************************************************************************
* Function:    listenForMsgs
*****************************************************************************/
bool Server::listenForMsgs(int playerFd) {
    std::string inMsg;             // initialize inMsg
    
    while (1) {
        if (receiveMsg(playerFd, inMsg) == false ) {
            return false;
        }
        
        /* Send the message to the message handler. */
        _gameLogicPt->receivedMessageHandler(inMsg, playerFd);       
    }
}


/******************************************************************************
* Function:    pause          
*****************************************************************************/
void Server::pause(bool pause) {   
    _serverPause = pause;
}


/******************************************************************************
* Function:    setMaxPlayers       
*****************************************************************************/
bool Server::setMaxPlayers(int maxPlayers) {   
    
    /* Validate the maxPlayer value. */
    if (maxPlayers <= 0) {
        return false;    
    }
    /* Set the maxPlayer value. */
    else {
        _maxPlayers = maxPlayers; 
        return true;
    }   
}


/******************************************************************************
* Function:    setTimeOut         
*****************************************************************************/
bool Server::setTimeOut(int timeOut) {    
    
    /* Validate the time-out value. */
    if (timeOut <= 0) {
        return false;    
    }
    /* Set the time-out value. */
    else {
        _timeOut = timeOut; 
        return true;
    }
} 


/******************************************************************************
* Function:    setPlayerEcho          
*****************************************************************************/
bool Server::setPlayerEcho(int playerFd, bool echo) {  
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_player_map);   
    
    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If the player is in the map, set the echo. */
    if(player == _playerMap.end())
        return false;   // the player is not in the map
    else { 
        player->second.echo = echo;
        return true;
    }
  
}


/******************************************************************************
* Function:    getServerPause        
*****************************************************************************/
bool Server::getServerPause() const {   
    return _serverPause;
}        


/******************************************************************************
* Function:    getMaxPlayers          
*****************************************************************************/
int Server::getMaxPlayers() const {   
    return _maxPlayers;
}        


/******************************************************************************
* Function:    getPlayerCount        
*****************************************************************************/
int Server::getPlayerCount() const {   
    return _playerCount;
}     


/******************************************************************************
* Function:    getTimeOut        
*****************************************************************************/
int Server::getTimeOut() const {   
    return _timeOut;  
}  


/******************************************************************************
* Function:    getServerPort          
*****************************************************************************/
int Server::getServerPort() const {   
    return _serverPort;  
} 


/******************************************************************************
* Function:    getGameLogicPt         
*****************************************************************************/
legacymud::engine::GameLogic* Server::getGameLogicPt() const {   
    return _gameLogicPt;  
} 


/******************************************************************************
* Private Function:    _setServerPort         
*****************************************************************************/
bool Server::_setServerPort(int serverPort) {   

    /* Validate the server port value. */
    if (serverPort < 1000 || serverPort > 65535) {
        return false;    
    }
    /* Set the server port value. */
    else {
        _serverPort = serverPort; 
        return true;
    }
}


/******************************************************************************
* Private Function:    _setGameLogicPt          
*****************************************************************************/
bool Server::_setGameLogicPt(legacymud::engine::GameLogic* gameLogicPt) {   

    /* Validate the pointer. */
    if (gameLogicPt == 0) {
        return false;    
    }
    /* Set the game logic pointer. */
    else {
        _gameLogicPt = gameLogicPt; 
        return true;
    }
}


/******************************************************************************
* Private Function:    _setCharacterMode          
*****************************************************************************/
bool Server::_setCharacterMode(int playerFd) {
    unsigned char code[6] = {255,251,1,255,251,3};     // Telnet command IAC WILL ECHO, IAC WILL SUPPRESS_GO_AHEAD
    unsigned char inCode[6];                           // will hold returned Telnet code from terminal
    
    /* Write to the socket. */ 
    if (write(playerFd,code,6) < 0)
        return false;       // Error writing to socket         
    else {
        /* Get confirmation from client that character mode is set. */
        if (read(playerFd, inCode, 6) <= 0) {
            return false;       // Error writing to socket    
        }
        else if (inCode[0] == 255 && inCode[1] == 253 && inCode[2] == 1 && inCode[3] == 255 && inCode[4] == 253 && inCode[5] == 3)   
            return true;
        else 
            return false;       // client doesn't support character mode
    }
}


/******************************************************************************
* Private Function:    _addPlayerToMap         
*****************************************************************************/
bool Server::_addPlayerToMap(int playerFd) {  

    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_player_map);        

    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If the player is not in the map, add the player. */
    if(player != _playerMap.end()) 
        return false;   // This player fd is alread in the map.
    else {
        _Player newPlayer;
        newPlayer.echo = true;              // default echo is set to true
        newPlayer.readBuffer.clear();       // clear the buffer
        _playerMap[playerFd] = newPlayer;   // add the player  
        _playerCount++;                     // increment the player count        
        return true;        
    }    
}


/******************************************************************************
* Private Function:    _removePlayerFromMap         
*****************************************************************************/
bool Server::_removePlayerFromMap(int playerFd ) { 
    
    /* Set lock. Lock is released when it goes out of scope. */
    std::lock_guard<std::mutex> lock(_mu_player_map);    
    
    /* Find the player. */
    auto player = _playerMap.find(playerFd);
    
    /* If the player is in the map, delete player and return true. */
    if(player == _playerMap.end())
        return false;   // the player isn't in the list
    else {
        /* Remove the player from the map. */
        _playerMap.erase(playerFd);
        return true;
    } 
}


}}  

    