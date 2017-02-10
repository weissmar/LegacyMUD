/*!
  \file     Server.hpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/10/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the Server class.
*/

 
#ifndef LEGACYMUD_TELNET_SERVER_HPP
#define LEGACYMUD_TELNET_SERVER_HPP


namespace legacymud {
    namespace engine{
        class GameLogic;    // forward declaration
    }
    namespace telnet {

    /*!
      \brief Telnet server class for legacyMUD.  
      
      This class is a telnet server to be used for legacyMUD.  Multiplayer support is provided 
      through threading.  The server supports ANSI terminals that use BSD telnet commands.
    */
    class Server {
    public:
        /*!
          \enum Available new line options.
        */
        enum NewLine {
            NEWLINE, NO_NEWLINE, 
        };
              
        Server();
        bool initServer(int serverPort, int maxPlayers, legacymud::engine::GameLogic* gameLogicPt);
        void startListening();
        void disconnectPlayer(int playerFd);
        bool sendMsg(int playerFd, std::string outMsg, Server::NewLine newLine);
        bool receiveMsg(int playerFd, std::string &inMsg);
        bool listenForMsgs(int playerFd);
        void setServerPause(bool pause);
        bool getServerPause() const;
        bool setMaxPlayers(int maxPlayers);
        int getMaxPlayers() const;
        int getPlayerCount() const;
        void setEcho(bool echo);
        bool getEcho() const;
    
    private:
        int _serverPort;
        int _maxPlayers; 
        int _listenSocketFd; 
        int _playerCount;
        bool _serverPause;
        bool _echo;
        legacymud::engine::GameLogic* _gameLogicPt;        
};

}}

#endif