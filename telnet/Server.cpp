/*!
  \file     Server.cpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/10/2017
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
#include "GameLogic.hpp"


namespace legacymud {namespace telnet {


/******************************************************************************
* Function:    Server()
* Description: Server class constructor
* 
* Input:            none         
* Preconditions:    none                      
*****************************************************************************/
Server::Server() {   
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
* Description: This function initializes the server.
* 
* Input:            int serverPort     Port to start the server on.
* Input:            int maxPlayers     Max number of people that can be on the server. 
* Input:            gameLogicPt          
* Preconditions:    serverPort         Should be an integer from 1000 to 65535 
*                   maxPlayers         Should be an integer > 0.     
* Return:           true if successful.  otherwise false.                   
*****************************************************************************/
bool Server::initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt) {   
    struct sockaddr_in serverAddr;         // address structure for server  
    
    /* Set member variables. */
    if (!setServerPort(serverPort) || !setMaxPlayers(maxPlayers) || !setTimeOut(timeOut) || !setGameLogicPt(gameLogicPt) ) {
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
* Description: This function listens for connections.
*
* Input:            none           
* Preconditions:    none                        
* Return:           none 
*****************************************************************************/
void Server::startListening() {   
    struct sockaddr_in clientAddr;         // address structure for client
    socklen_t clientLength;                // length of a client's address structure   
    int newClientSocketFd;                 // new socket connected to client
    struct timeval timeOut;                // timeout timer for receiving messages on a socket.
    timeOut.tv_sec = getTimeOut();                  // set a 15 minute player timeout for disconnect 
   
    while (1) {
        
        /* Listen for connection. */
        std::cout << "Listening for new connection..." << std::endl;
        listen(_listenSocketFd, 10);      // maximum connection requests at a time: 10
        
        /* Accept connection. */
        clientLength = sizeof(clientAddr);
        newClientSocketFd = accept(_listenSocketFd, (struct sockaddr *) &clientAddr, &clientLength);
        if (newClientSocketFd < 0) {   
            std::cout << "Error accepting connection" << std::endl;
        }
        else {
            std::cout << "Connection established. Socket: " << newClientSocketFd << std::endl;              
            
            /* Add player to player map. This map is a list of players on the server. */
            if (_addPlayerToMap(newClientSocketFd) == false) {
                std::cout << "Error adding player to map." << std::endl; 
                disconnectPlayer(newClientSocketFd);
            }
            /* Set the player's Telnet terminal into character mode. */
            else if (_setCharacterMode(newClientSocketFd) == false) {
                std::cout << "Error setting character mode in the client" << std::endl;
                disconnectPlayer(newClientSocketFd);
            }
            /* Disconnect the player if the player cap is exceeded. */
            else if (_playerCount > _maxPlayers) {
                sendMsg(newClientSocketFd, "Server is full.  Please try again later.", NEWLINE);
                disconnectPlayer(newClientSocketFd);
            }
            /* Disconnect the new player if game backup is in progress. */
            else if (_serverPause == true) {
                sendMsg(newClientSocketFd, "Server backup in progress.  Please try again later.", NEWLINE);
                disconnectPlayer(newClientSocketFd);    
            }
            /* Set disconnect timeout on player's socket. */
            else if (setsockopt (newClientSocketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut, sizeof(timeOut)) < 0) {
                std::cout << "Error setting socket timeout" << std::endl; 
                disconnectPlayer(newClientSocketFd);                
            }
            /* Create a new thread for this player and send to a new player handler in the game logic. */
            else {
                std::thread t(&legacymud::engine::GameLogic::newPlayerHandler, _gameLogicPt, newClientSocketFd);    
                t.detach();                        
            }  
        }   
    }
}


/******************************************************************************
* Function:    disconnectPlayer
* Description: This function disconnects a player.
*
* Input:            int playerFd   Socket to close.        
* Preconditions:    none                      
* Return:           Returns true if player disconnected successfully. Otherwise false.
*****************************************************************************/
void Server::disconnectPlayer(int playerFd) {   
    
    /* Only close and decrement player count when player is in map.  Protects */
    /* against accidental multiple calls to disconnect a player. */
    
    if (_removePlayerFromMap(playerFd) == true ) {
        _playerCount--;     // decrement player count
        close(playerFd);     
    }
}


/******************************************************************************
* Function:    sendMsg
* Description: This function sends a message to a client.
*
* Input:            int playerFd   client socket 
* Input:            std::string outMsg   message to be sent  
* Input:            Server::NewLine newLine
* Preconditions:    none                      
* Return:           Returns true if successful send. Otherwise false.
*****************************************************************************/
bool Server::sendMsg(int playerFd, std::string outMsg, Server::NewLine newLine) {
   
    if (newLine == NEWLINE) { 
        outMsg += "\015\012";       // attach carriage return  and linefeed
    }
    /* Write to the socket. */ 
    if (write(playerFd,outMsg.c_str(),strlen(outMsg.c_str())) < 0) 
        return false;   // Error writing to socket               
    else         
        return true;         
}  
    

/******************************************************************************
* Function:    receiveMsg
* Description: This function receives a message from a client.
*
* Input:            std::string inMsg    message received
* Input:            int playerFd   client socket        
* Preconditions:    none                      
* Return:           Returns true if successful send. Otherwise false.
*****************************************************************************/
bool Server::receiveMsg(int playerFd, std::string &inMsg ) {
    unsigned char ch = 0;
    
    inMsg = "";               // initialize inMsg
    
    // Read socket.
    while (ch != 13) {  // ASCII carriage return
        if (read(playerFd,&ch,1) <= 0)  // returns -1 when error reading, returns 0 if client disconnected.
            return false;        
        
        /* Read an ANSI escape code.  Needed for up arrow. */
        else if (ch == 27 ) {
            /* Read the rest if an ansi code. */
            unsigned char ansiCode[10];
            if (read(playerFd,ansiCode,10) <= 0)   // returns -1 when error reading, returns 0 if client disconnected.    
                return false;    
             
            /* This is an up arrow. */
            else if (ansiCode[0] == 91 && ansiCode[1] == 65) {

                /* Redisplay the message on the player's terminal. */
                if (inMsg.size() > 0 && getPlayerEcho(playerFd))
                    if (sendMsg(playerFd, inMsg, NO_NEWLINE) == false) 
                        return false;      
            }
        }
            
        /* Backspace character received. Remove the last character from the in message. */
        else if (ch == 8 && inMsg.size() > 0) {
            inMsg.pop_back();               
            
            /* Send the delete character to the player's terminal. */
            if (getPlayerEcho(playerFd)) {
                ch = 127;
                if (write(playerFd, &ch, 1) < 0) 
                    return false;                
            }  
        }
            
        /* Character to be added to the message. */
        else if (ch >=32 && ch <=126) {
            inMsg += ch;
            
            /* Echo the character back to the player's terminal. */
            if (getPlayerEcho(playerFd)) 
                if (write(playerFd, &ch, 1) < 0) 
                    return false;             
        }                   
    }
    /* Echo new line to the player's terminal. */
    if (sendMsg(playerFd, "", NEWLINE) == false ) 
        return false;     
    
    return true;
}


/******************************************************************************
* Function:    listenForMsgs
* Description: This function listens for messages from a client
*
* Input:            int playerFd   client socket        
* Preconditions:    none                      
* Return:           none
*****************************************************************************/
bool Server::listenForMsgs(int playerFd) {
    std::string inMsg;             // initialize inMsg
    std::mutex m;
    
    while (1) {
        if (receiveMsg(playerFd, inMsg) == false )
            return false;
        
        /* Send the message to the message handler. */
        m.lock();   // make thread safe
        _gameLogicPt->receivedMessageHandler(inMsg, playerFd); 
        m.unlock();
    }
}


/******************************************************************************
* Function:    pause
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
void Server::pause(bool pause) {   
    _serverPause = pause;
}


/******************************************************************************
* Function:    setMaxPlayers
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::setMaxPlayers(int maxPlayers) {   
    if (maxPlayers <= 0) {
        return false;    
    }
    else {
        _maxPlayers = maxPlayers; 
        return true;
    }   
}


/******************************************************************************
* Function:    setTimeOut
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::setTimeOut(int timeOut) {    
    if (timeOut <= 0) {
        return false;    
    }
    else {
        _timeOut = timeOut; 
        return true;
    }
} 


/******************************************************************************
* Function:    setServerPort
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::setServerPort(int serverPort) {   

    if (serverPort < 1000 || serverPort > 65535) {
        return false;    
    }
    else {
        _serverPort = serverPort; 
        return true;
    }
}


/******************************************************************************
* Function:    setGameLogicPt
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::setGameLogicPt(legacymud::engine::GameLogic* gameLogicPt) {   

    if (gameLogicPt == 0) {
        return false;    
    }
    else {
        _gameLogicPt = gameLogicPt; 
        return true;
    }
}


/******************************************************************************
* Function:    setPlayerEcho
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::setPlayerEcho(int playerFd, bool echo) {   
    
    /* Verify that the player is in the Map. */
    if(_playerEcho.find(playerFd) == _playerEcho.end()) 
        return false;
    else {
        /* Set the player's echo value. */
        _playerEcho[playerFd] = echo;            
        return true;
    }   
}


/******************************************************************************
* Function:    getServerPause
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::getServerPause() const {   
    return _serverPause;
}        


/******************************************************************************
* Function:    getMaxPlayers
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
int Server::getMaxPlayers() const {   
    return _maxPlayers;
}        


/******************************************************************************
* Function:    getPlayerCount
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
int Server::getPlayerCount() const {   
    return _playerCount;
}     


/******************************************************************************
* Function:    getTimeOut
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
int Server::getTimeOut() const {   
    return _timeOut;  
}  


/******************************************************************************
* Function:    getServerPort
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
int Server::getServerPort() const {   
    return _serverPort;  
} 


/******************************************************************************
* Function:    getGameLogicPt
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
legacymud::engine::GameLogic* Server::getGameLogicPt() const {   
    return _gameLogicPt;  
} 


/******************************************************************************
* Function:    getPlayerEcho
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::getPlayerEcho(int playerFd) {   

    /* Verify that the player is in the Map. */
    if(_playerEcho.find(playerFd) == _playerEcho.end()) 
        return false;     // Don't care.  This player is not in the game.
    else
        /* Get the player's echo value. */
        return _playerEcho[playerFd];        
}


/******************************************************************************
* Function:    _setCharacterMode
* Description: This helper function sends a telnet command to a client to set 
* character mode.
*
* Input:            
* Input:                    
* Preconditions:                          
* Return:           
*****************************************************************************/
bool Server::_setCharacterMode(int playerFd) {
    unsigned char code[6] = {255,251,1,255,251,3};     // Telnet command IAC WILL ECHO, IAC WILL SUPPRESS_GO_AHEAD
    unsigned char inCode[6];                           // returned Telnet code from terminal
    
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
* Function:    _addPlayerToMap
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::_addPlayerToMap(int playerFd) {   

    /* Verify that the player isn't already in the Map. */
    if(_playerEcho.find(playerFd) != _playerEcho.end()) 
        return false;     // This player fd is alread in the map.
    else {
        /* Add the player.  Default echo is set to true. */
        _playerCount++;     // increment the player count
        _playerEcho[playerFd] = true;            
        return true;
    }
}


/******************************************************************************
* Function:    _removePlayerFromMap
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
bool Server::_removePlayerFromMap(int playerFd ) { 
    
    /* Verify that the player is in the Map. */
    if(_playerEcho.find(playerFd) == _playerEcho.end()) 
        return false;       // the player isn't in the list
    else {
        /* Remove the player from the map. */
        _playerEcho.erase(playerFd);     
        return true;
    }
}


}}  

    