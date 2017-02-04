/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/2/2017
 * Course:        CS467, Winter 2017
 * Filename:      Server.cpp
 *
 * Overview:
 *     Implementation file for the Server class.
 ************************************************************************/

#include <stdio.h>          // sprintf and other input,output operations
#include <stdlib.h>         // general purpose functions
#include <string.h>         // for c strings
#include <string>           // for std::string
#include <unistd.h>         // includes close
#include <sys/types.h>      // includes data types used in system calls
#include <sys/socket.h>     // sockets
#include <netinet/in.h>     // needed for internet domain addresses
// #include <netdb.h>           // perhaps not needed? more for a client?
#include <thread>           // threading
#include <mutex>
#include <iostream>
#include <vector>
#include "Server.h"


namespace legacymud {
    namespace engine {
        class GameLogic;
    }

    namespace telnet {


/******************************************************************************
* Function:    Server class constructor
* Description: This function starts the server.
*
* Input:            int newServerPort     Port to start the server on.
* Input:            int newMaxLoad        Max number of people that can be on the server.           
* Preconditions:    newServerPort         Should be an integer from 1000 to 65535 
*                   newMaxLoad            Should be an integer > 0.                        
*****************************************************************************/
Server::Server(int newServerPort, int newMaxPlayers, legacymud::engine::GameLogic* glpt) {   
    _serverPort = newServerPort;     
    _maxPlayers = newMaxPlayers;  
    _playerCount = 0;
    _gameLogicPt = glpt;    
}

/******************************************************************************
* Function:    startServer
* Description: This function starts the server.
*
* Input:            none           
* Preconditions:    Server object should be constructed with a valid serverport
*                   and maxload.                        
* Return:           Returns true if server started successfully. 
*****************************************************************************/
void Server::startServer() {   
    struct sockaddr_in serverAddr;         // address structure for server
    struct sockaddr_in clientAddr;         // address structure for client
    socklen_t clientLength;                // length of a client's address structure   
    int newClientSocketFd;                 // new socket connected to client
      
    /* Initialize the server. Create a socket. TCP/IP. */
    _listenSocketFd = socket(AF_INET, SOCK_STREAM, 0);   // IP, TCP
    if (_listenSocketFd < 0) {
        std::cout << "Error opening socket." << std::endl; // Error opening socket
        std::terminate();           // terminate this thread         
    }           
    
    /* Fill server address struct. */
    bzero((char *) &serverAddr, sizeof(serverAddr));     // initialize the server address struct to zeros
    serverAddr.sin_family = AF_INET;                     // IP
    serverAddr.sin_addr.s_addr = INADDR_ANY;             // open to any IP
    serverAddr.sin_port = htons(_serverPort);            // server port number
    
    /* Bind the socket to the port. */
    if (bind(_listenSocketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Error cannot connect to port." << std::endl; // Error cannot connect to port  
        std::terminate();                                        // terminate this thread           
    }    
    
    std::cout << "Server started on port " << _serverPort << std::endl;
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
            if (_playerCount < _maxPlayers) {
                _playerCount++;
                /* Pass client off to its own thread. */
                std::thread t(&legacymud::engine::GameLogic::newPlayerHandler, _gameLogicPt, newClientSocketFd);    
                t.detach();  
            }
            else {
                sendMsg("Server is full.  Try again later.\n", newClientSocketFd);
                disconnectPlayer(newClientSocketFd);
            }
        }
    }
}


/******************************************************************************
* Function:    disconnectPlayer
* Description: This function disconnects a player.
*
* Input:            int clientSocketFd   Socket to close.        
* Preconditions:    none                      
* Return:           Returns true if player disconnected successfully. Otherwise false.
*****************************************************************************/
bool Server::disconnectPlayer(int clientSocketFd) {   
    if (close(clientSocketFd) < 0 ) 
        return false;   // Error closing socket
    else
        return true;
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
        return false; // Error writing to socket
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
    unsigned char ch=0;
    inMsg = "";               // initialize inMsg
    
    // Read socket.
    while (ch !='\n') {
        if (read(clientSocketFd,&ch,1) < 0) 
            return false; // error reading from socket 
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
void Server::listenForMsgs(int playerFd) {
    unsigned char ch=0;
    std::string inMsg;             // initialize inMsg
    std::mutex m;
    
    while (1) {
        ch = 0;         // initialize
        inMsg = "";     // initialize
        // Read socket.
        while (ch !='\n') {
            if (read(playerFd,&ch,1) < 0) {
                _playerCount--;             // decrement player count
                disconnectPlayer(playerFd); // disconnect player
                std::terminate();           // terminate this thread
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