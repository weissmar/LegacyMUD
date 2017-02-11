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

#include <map>


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
        
        /*!
          \brief Server class constructor.
          
          This function is the default constructor for the server class.
          
          \post The server class is constructed.  
        */     
        Server();
        
        /*!
          \brief Initializes the server.
          
          This function initializes the server, getting it ready to start listening for connections.
          
          \param[in]  serverPort        port to start the server on
          \param[in]  maxPlayers        max number of players that can be on the server
          \param[in]  timeOut           amount of time in seconds to wait for a response from a player before disconnect
          \param[in]  gameLogicPt       pointer to the game logic object         
          \pre serverPort  should be from 1000 to 65535
          \pre maxPlayers  should be a positive integer
          \pre timeOut     should be greater than 0
          \post Returns true if the server is successfully initialized.  Otherwise it returns false.
        */        
        bool initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt);
        
        /*!
          \brief Causes the server to start listening for connections.
          
          This function listens for new player connections.  It runs in an infinite loop.
          Each new player connection is detached as a thread to a Game Logic object newPlayerHandler.
                   
          \pre      The server should be first initiazed with initServer
          \post The server is in an infinite loop listening for new player connections.
        */
        void startListening();
        
        /*!
          \brief Disconnects a player from the server.
          
          This function disconnects a player from the server.
                   
          \pre none
          \post A players is disconnected from the server.
        */        
        void disconnectPlayer(int playerFd);
        
        /*!
          \brief Sends a message to a player.
          
          This function sends a message to a player.
          
          \param[in]  playerFd          a player identifier 
          \param[in]  outMsg            message to be sent to a player
          \param[in]  newLine           indicates where the cursor is to be located on the player's screen 
                                        after the message is displayed                                   
          \pre none
          \post Returns true if the message is successfully sent.  Otherwise it returns false.
        */        
        bool sendMsg(int playerFd, std::string outMsg, Server::NewLine newLine);
        
        /*!
          \brief Receives a message from a player.  
          
          This function receives a message from a player.  It will not return until a message is received 
          or the player disconnects or times-out.
          
          \param[in]  playerFd          a player identifier 
          \param[in]  inMsg             message received from a player                                 
          \pre none
          \post Returns true if the message is successfully received.  Returns false if there an issue with
                reading from the player's socket, or if the player disconnects or times-out.
        */         
        bool receiveMsg(int playerFd, std::string &inMsg);
        
        /*!
          \brief Listens for received messages from a player.  
          
          This function listens for received messages from a player.  It enters an infinite loop that constantly listens. 
          This infinite loop is broken if a player disconnects or times-out.  Each received message is sent to a Game Logic 
          object receivedMessageHandler. 
          
          \param[in]  playerFd          a player identifier                               
          \pre none
          \post Returns false if a player disconnects or times-out. The Game Logic newPlayerHandler should call 
                Server::disconnectPlayer(playerFd) if a false is received.
        */ 
        bool listenForMsgs(int playerFd);
        
        /* Setters. */
        void pause(bool pause);
        bool setMaxPlayers(int maxPlayers);
        bool setTimeOut(int timeOut);
        bool setServerPort(int timeOut);
        bool setGameLogicPt(legacymud::engine::GameLogic* gameLogicPt);
        bool setPlayerEcho(int playerFd, bool echo);
        
        /* Getters. */
        bool getServerPause() const;
        int getMaxPlayers() const;
        int getPlayerCount() const;
        int getTimeOut() const;
        int getServerPort() const;
        legacymud::engine::GameLogic* getGameLogicPt() const;
        bool getPlayerEcho(int playerFd);

    private:
        bool _setCharacterMode(int playerFd);
        bool _addPlayerToMap(int playerFd);
        bool _removePlayerFromMap(int playerFd);
        int _serverPort;
        int _maxPlayers; 
        int _timeOut;
        int _listenSocketFd; 
        int _playerCount;
        bool _serverPause;
        legacymud::engine::GameLogic* _gameLogicPt;  
        std::map<int, bool> _playerEcho;        
};

}}

#endif