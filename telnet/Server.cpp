/*!
  \file     Server.cpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/08/2017
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
bool Server::initServer(int serverPort, int maxPlayers, legacymud::engine::GameLogic* gameLogicPt) {   
    struct sockaddr_in serverAddr;         // address structure for server  
      
    /* Validate serverPort. */
    if (serverPort < 1000 || serverPort > 65535) {
        std::cout << "Error port value must be from 1000 to 65535" << std::endl;
        return false;
    }
    
    /* Validate max players. */
    if (maxPlayers <=0) {
        std::cout << "Error max player cap must be greater than zero" << std::endl;
        return false;
    }
    
    /* Set member variables. */
    _serverPort = serverPort;     
    _maxPlayers = maxPlayers;  
    _gameLogicPt = gameLogicPt;
    
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
    timeOut.tv_sec = 900;                  // set a 15 minute player timeout for disconnect  
   
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
            _playerCount++;    // increment player count               
            /* Disconnect the player if the player cap is exceeded. */
            if (_playerCount > _maxPlayers) {
                sendMsg("Server is full.  Please try again later.\n", newClientSocketFd);
                disconnectPlayer(newClientSocketFd);
            }
            /* Disconnect the player if game backup is in progress. */
            else if (_serverPause == true) {
                sendMsg("Server backup in progress.  Please try again later.\n", newClientSocketFd);
                disconnectPlayer(newClientSocketFd);    
            }
            /* Otherwise, send the player to the new player handler. */
            else {
                /* Set disconnect timeout on player's socket. */
                if (setsockopt (newClientSocketFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeOut, sizeof(timeOut)) < 0)
                    std::cout << "Error setting socket timeout" << std::endl;  
                else {
                    /* Create a new thread for this player and send to a new player handler in the game logic. */
                    std::thread t(&legacymud::engine::GameLogic::newPlayerHandler, _gameLogicPt, newClientSocketFd);    
                    t.detach();      
                }
            }
        }
    }
}


/******************************************************************************
* Function:    setServerPause
* Description: 
*
* Input:                   
* Preconditions:                         
* Return:          
*****************************************************************************/
void Server::setServerPause(bool pause) {   
    _serverPause = pause;
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
* Function:    disconnectPlayer
* Description: This function disconnects a player.
*
* Input:            int clientSocketFd   Socket to close.        
* Preconditions:    none                      
* Return:           Returns true if player disconnected successfully. Otherwise false.
*****************************************************************************/
void Server::disconnectPlayer(int clientSocketFd) {   
    _playerCount--;     // decrement player count
    close(clientSocketFd); 
}


/******************************************************************************
* Function:    sendMsg
* Description: This function sends a message to a client.
*
* Input:            std::string outMsg   message to be sent
* Input:            int clientSocketFd   client socket        
* Preconditions:    none                      
* Return:           Returns true if successful send. Otherwise false.
*****************************************************************************/
bool Server::sendMsg(std::string outMsg, int clientSocketFd) {
    
    /* Write to the socket. */ 
    if (write(clientSocketFd,outMsg.c_str(),strlen(outMsg.c_str())) < 0)
        return false;       // Error writing to socket         
    else 
        return true;
}


/******************************************************************************
* Function:    receiveMsg
* Description: This function receives a message from a client.
*
* Input:            std::string inMsg    message received
* Input:            int clientSocketFd   client socket        
* Preconditions:    none                      
* Return:           Returns true if successful send. Otherwise false.
*****************************************************************************/
bool Server::receiveMsg(std::string &inMsg, int clientSocketFd) {
    unsigned char ch = 0;
    inMsg = "";               // initialize inMsg
    
    // Read socket.
    while (ch !='\n') {
        if (read(clientSocketFd,&ch,1) <= 0) // returns -1 when error reading, returns 0 if client disconnected.
            return false;        
        else {
            if (ch >=32 && ch <=127) {
                inMsg += ch;
            }
        }             
    } 
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
    unsigned char ch = 0;
    std::string inMsg;             // initialize inMsg
    std::mutex m;
    
    while (1) {
        ch = 0;         // initialize
        inMsg = "";     // initialize
        // Read socket.
        while (ch !='\n') {
            if (read(playerFd,&ch,1) <= 0) {     // returns -1 when error reading, returns 0 if client disconnected.          
                disconnectPlayer(playerFd);  
                return false;
            }
            else {
                if (ch >=32 && ch <=127) {
                    inMsg += ch;
                }
            }             
        }
        m.lock();   // make thread safe
        _gameLogicPt->receivedMsgHandler(inMsg, playerFd); 
        m.unlock();
    }
}

}}