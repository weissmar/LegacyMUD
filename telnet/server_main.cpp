/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/10/2017
 * Course:        CS467, Winter 2017
 * Filename:      server_main.cpp
 *
 * Overview:
 *     This main function for local testing of the server class.
 ************************************************************************/

#include <string>
#include <string.h>
#include <iostream>
#include <thread>           // threading
#include <mutex>
#include "GameLogic.hpp"
#include "Server.hpp"


int main(int argc, char *argv[]) {   
    int serverPort;
    legacymud::engine::GameLogic gl;
    legacymud::telnet::Server ts;
    int playerCap = 10;
    int timeOut = 900;           // in seconds.
    std::string message;
    int sender;
    
    gl.serverPt = &ts; 
 
    /* Validate command line entry. */
	if (argc < 2 || argc > 2) {
		std::cout << "Error: Usage is server [port number]" << std::endl;
		return 1;
	}
    
    /* Convert from c-string to int. */
    serverPort = atoi(argv[1]);
    
    
    /* Initialize the server. */
    if (ts.initServer(serverPort, playerCap, timeOut, &gl) == false) {
        return 1;     // error initializing the server
    }
    /* Send and detach a thread to listen for new server connections. */
    else {
        std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);    
        serverThread.detach();    
               
        /*  Some game engine function would be called here. */
        std::cout << "In the game play thread..." << std::endl;
        
        /* Continuously monitor for new messages. */
        while(1) {
            std::unique_lock<std::mutex> locker(gl.mu_msg);   // game logic's mutex, this locks
            
            /* This unlocks and waits for a new message using Game Logic's condition variable. */
            if (gl.msgDeque.empty() )
                gl.cond_msg.wait(locker);       // called by receivedMessageHandler

            /* Mutex is locked again. Get message from the queue and pop it. */
            message = gl.msgDeque.front().msg;
            sender = gl.msgDeque.front().playerFd; 
            gl.msgDeque.pop_front();  
            locker.unlock();    // unlock
            std::cout << "Message: " << message << " from " << sender << std::endl;     
                      
        }
        return 0;        
    }
 
}

