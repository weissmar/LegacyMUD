/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/2/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.h
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Declarations.
 ************************************************************************/
 
#ifndef LEGACYMUD_ENGINE_GAMELOGIC_H
#define LEGACYMUD_ENGINE_GAMELOGIC_H

#include <string>
#include <deque>
#include "Server.h"

namespace legacymud {
    namespace telnet {
        class Server;
    }

    namespace engine {


class GameLogic {
    public:
        void newPlayerHandler(int playerFd);
        bool receivedMsgHandler(std::string inMsg, int playerFd);          
        legacymud::telnet::Server* serverPt;
        struct ReceivedMsg {
            std::string msg;
            int playerFd;
        };
        std::deque<ReceivedMsg> msgDeque;
       
};

}}

#endif