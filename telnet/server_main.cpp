/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/2/2017
 * Course:        CS467, Winter 2017
 * Filename:      server_main.cpp
 *
 * Overview:
 *     This main function for for local testing of the server class.
 ************************************************************************/

#include <string>
#include <iostream>
#include <thread>           // threading
#include <mutex>
#include "GameLogic.h"
#include "Server.h"

int main() {   
    legacymud::engine::GameLogic gl;
    legacymud::telnet::Server ts(50500,10, &gl);
    gl.serverPt = &ts; 
    std::mutex m;
    
    
    std::cout << "Hello.  Welcome to Server Testing." << std::endl;
    
    std::thread serverThread(&legacymud::telnet::Server::startServer, &ts);    // pass client off to its own thread
    serverThread.detach();    
    
    // Need to call game logic.
    std::cout << "And the game is on! Enter 'quit' to quit." << std::endl;
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
