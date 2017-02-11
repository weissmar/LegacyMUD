/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/10/2017
 * Course:        CS467, Winter 2017
 * Filename:      server_main.cpp
 *
 * Overview:
 *     This main function for for local testing of the server class.
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
    int playerCap = 5;
    int timeOut = 900;           // in seconds.
    std::mutex m;
    
    gl.serverPt = &ts; 
        
    
    
    /* Validate command line entry. */
	if (argc < 2 || argc > 2) {
		std::cout << "Error: Usage is server [port number]" << std::endl;
		return 1;
	}
    
    serverPort = atoi(argv[1]);
    
    
    if (ts.initServer(serverPort, playerCap, timeOut, &gl) == false) {
        return 1;     // error initializing the server
    }
    else {
        std::thread serverThread(&legacymud::telnet::Server::startListening, &ts);    // pass client off to its own thread
        serverThread.detach();    
        
        
        // Need to call game logic.
        std::cout << "In the game play thread... Enter 'q' to quit." << std::endl;
        char ch;
        while(ch != 'q' ) {
            std::cin >> ch;
            if (gl.msgDeque.size() > 0) {
                std::cout << "Message: " << gl.msgDeque.front().msg << " " << gl.msgDeque.front().playerFd << std::endl;
                m.lock();
                gl.msgDeque.pop_front();
                m.unlock();
            }
        }
        return 0;        
    }
 
}
