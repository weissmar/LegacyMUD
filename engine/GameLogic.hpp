/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/12/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.hpp
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Declarations.
 ************************************************************************/
 
#ifndef LEGACYMUD_ENGINE_GAMELOGIC_HPP
#define LEGACYMUD_ENGINE_GAMELOGIC_HPP

#include <deque>
#include <condition_variable>


namespace legacymud {
    namespace telnet {
        class Server;       // forward declaration
    }

    namespace engine {


class GameLogic {
    public:
        void newPlayerHandler(int playerFd);
        bool receivedMessageHandler(std::string inMsg, int playerFd);          
        legacymud::telnet::Server* serverPt;        
        struct ReceivedMsg {                // received message struct   
            std::string msg;
            int playerFd;
        };
        std::deque<ReceivedMsg> msgDeque;   // message deque for received messages
        std::condition_variable cond_msg;   // conditional variable for information game engine a message has arrived
        std::mutex mu_msg;                  // mutex for making msgDeque thread safe
};

}}

#endif