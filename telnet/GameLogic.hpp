/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/10/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.h
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Declarations.
 ************************************************************************/
 
#ifndef LEGACYMUD_ENGINE_GAMELOGIC_HPP
#define LEGACYMUD_ENGINE_GAMELOGIC_HPP

#include <deque>

namespace legacymud {
    namespace telnet {
        class Server;       // forward declaration
    }

    namespace engine {


class GameLogic {
    public:
        void newPlayerHandler(int playerId);
        bool receivedMsgHandler(std::string inMsg, int playerId);          
        legacymud::telnet::Server* serverPt;
        struct ReceivedMsg {
            std::string msg;
            int playerId;
        };
        std::deque<ReceivedMsg> msgDeque;
       
};

}}

#endif