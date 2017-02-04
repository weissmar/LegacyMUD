/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/2/2017
 * Course:        CS467, Winter 2017
 * Filename:      Server.h
 *
 * Overview:
 *     Declaration file for the Server class.
 ************************************************************************/
 
#ifndef LEGACYMUD_TELNET_SERVER_H
#define LEGACYMUD_TELNET_SERVER_H

#include <string>
#include "GameLogic.h"

namespace legacymud {
    namespace engine{
        class GameLogic;
    }

    namespace telnet {

/*!
  \brief A telnet server class.
*/
class Server {
    public:
        Server(int newServerPort, int newMaxPlayers, legacymud::engine::GameLogic* glpt);
        void startServer();
        bool disconnectPlayer(int clientSocketFd);
        bool sendMsg(std::string outMsg, int clientSocketFd);
        bool receiveMsg(std::string &inMsg, int clientSocketFd);
        void listenForMsgs(int clientSocketFd);
    
    private:
        int _serverPort;
        int _maxPlayers; 
        int _listenSocketFd; 
        int _playerCount;
        legacymud::engine::GameLogic* _gameLogicPt;        
};

}}

#endif