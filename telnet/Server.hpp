/*!
  \file     Server.hpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/26/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the Server class.
*/

 
#ifndef LEGACYMUD_TELNET_SERVER_HPP
#define LEGACYMUD_TELNET_SERVER_HPP

#include <map>
#include <deque>
#include <mutex>

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
          \brief Server class constructor.
          
          This function is the default constructor for the server class.
          
          \post The server class is constructed.  
        */     
        Server();
        
        virtual ~Server() { }

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
        virtual bool initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt);
        
        /*!
          \brief Causes the server to start listening for connections.
          
          This function listens for new player connections.  It runs in an infinite loop.
          Each new player connection is detached as a thread to a Game Logic object newPlayerHandler.
                   
          \pre      The server should be first initiazed with initServer
          \post The server is in an infinite loop listening for new player connections.
        */
        virtual void startListening();
        
        /*!
          \brief Disconnects a player from the server.
          
          This function disconnects a player from the server.  This should be called by the game engine every time
          receiveMsg or listenForMsgs returns false. 
                   
          \pre none
          \post Returns true if a player is disconnected from the server.  Otherwise returns false.
        */        
        virtual bool disconnectPlayer(int playerFd);
        
        /*!
          \brief Closes the server listen socket and sets member variables to default values.
                   
          \pre All players on the server have to be disconnected before calling this.
          \post True is returned if the server listen socket is closed and member variables are reset to default values.
                Otherwise, false is returned.
        */        
        virtual bool shutDownServer();

        
        /*!
          \brief Sends a quetion to a player. 
          
          This function sends a message to a player.  Puts the cursor on a new line after the message.
          
          \param[in]  playerFd          a player identifier 
          \param[in]  outQuestion       message to be sent to a player
          \param[in]  newLine           indicates where the cursor is to be located on the player's screen 
                                        after the message is displayed                                   
          \pre none
          \post Returns true if the question is successfully sent.  Otherwise it returns false.
        */        
        virtual bool sendQuestion(int playerFd, std::string outQuestion);        

        
        /*!
          \brief Sends a message to a player.  Puts the cursor on a new line after the message.
          
          \param[in]  playerFd          a player identifier 
          \param[in]  outMsg            message to be sent to a player
          \param[in]  newLine           indicates where the cursor is to be located on the player's screen 
                                        after the message is displayed                                   
          \pre none
          \post Returns true if the message is successfully sent.  Otherwise it returns false.
        */        
        virtual bool sendMsg(int playerFd, std::string outMsg);
        

        /*!
          \brief Sends a newline to a player's terminal. 
          
          \param[in]  playerFd          a player identifier                                  
          \pre none
          \post Returns true if the newline was successfully sent.  Otherwise it returns false.
        */        
        virtual bool sendNewLine(int playerFd);        
        
        /*!
          \brief Receives a message from a player.  
          
          This function receives a message from a player.  It will not return until either a message is received, 
          the player disconnects, or the player times-out.  
          
          \param[in]  playerFd          a player identifier 
          \param[in]  inMsg             message received from a player                                 
          \pre none
          \post Returns true if the message is successfully received.  Returns false if there an issue with
                reading from the player's socket, or if the player disconnects or times-out.
        */         
        virtual bool receiveMsg(int playerFd, std::string &inMsg);
        
        /*!
          \brief Listens for received messages from a player.  
          
          This function listens for received messages from a player.  It enters an infinite loop that constantly listens 
          for received messages from one player. This infinite loop is broken if a player disconnects or times-out.  
          Each received message is sent to a Game Logic object receivedMessageHandler function. 
          
          \param[in]  playerFd          a player identifier                               
          \pre none
          \post Returns false if a player disconnects, times-outs, or if this playerId is not in the player map. 
                The Game Logic newPlayerHandler should call disconnectPlayer(playerFd) if a false is received.
        */ 
        virtual bool listenForMsgs(int playerFd);
        
        /*!
          \brief Sets the server into a pause state. 
          
          This function sets the server into a paused state to support game back-ups.  In a paused state, the server no longer accepts
          new player connections.  
          
          \param[in]  pause     sets the pause state of the server                            
          \pre none
          \post If pause is set to true, the server will be in a paused state.  If pause is set to false, the server will not
                be in a paused state.
        */
        virtual void pause(bool pause);
        
        /*!
          \brief Sets the max number of players that can be concurrently on the server.  
          
          \param[in]  maxPlayers     max number of players that can be on the server                           
          \pre maxplayers must be an integer greater than 0.
          \post Returns false if maxPlayers is an invalid ammount.  Otherwise the player cap is set and true is returned.
        */        
        virtual bool setMaxPlayers(int maxPlayers);
        
        /*!
          \brief Sets the time-out period in seconds that the server waits before disconnecting an inactive player.    
          
          \param[in]  timeOut     time in seconds to set the time-out period to                          
          \pre timeOut must be an integer greater than 0.
          \post Returns false if timeOut is an invalid ammount.  Otherwise the time-out period is set and true is returned.
        */ 
        virtual bool setTimeOut(int timeOut);
        
        
        /*!
          \brief Sets a players text display echo mode.
          
          This function sets a players text display echo mode.  For example, it can be used to prevent the
          display of a password as it's entered on a player's terminal display.
          
          \param[in]  playerFd        player identifier 
          \param[in]  echo            text echo display mode of a player's terminal display                        
          \pre none
          \post Returns false if the player is not on the server.  Otherwise the echo mode is set and true is returned.
        */ 
        virtual bool setPlayerEcho(int playerFd, bool echo);
        
        /*!
          \brief Gets the server's pause state.
                                
          \pre none
          \post Returns the server's pause state.
        */ 
        virtual bool getServerPause() const;
        
        /*!
          \brief Gets the server's max player setting.
                                
          \pre none
          \post Returns the server's max player setting.
        */
        virtual int getMaxPlayers() const;
        
        /*!
          \brief Gets the server's current player count.
          
          This function gets the server's current player count.
                                
          \pre none
          \post Returns the server's current player count.
        */
        virtual int getPlayerCount() const;
        
        /*!
          \brief Gets the server's player time-out period in seconds.
                                
          \pre none
          \post Returns the server's time-out period in seconds.
        */        
        virtual int getTimeOut() const;
        
        /*!
          \brief Gets the server's port.
                                
          \pre none
          \post Returns the server's port.
        */ 
        virtual int getServerPort() const;
        
        /*!
          \brief Gets the game logic pointer that the server is using.
          
          This function gets the game logic pointer that the server is using.
                                
          \pre none
          \post Returns the game logic pointer that the server is using.
        */ 
        virtual legacymud::engine::GameLogic* getGameLogicPt() const;      
        
    private:
        bool _setServerPort(int serverPort);        // function that sets and validates the server port
        bool _setGameLogicPt(legacymud::engine::GameLogic* gameLogicPt);    // function that sets a pointer to a game logic object
        bool _setCharacterMode(int playerFd);       // function that sets a player's telnet terminal to character mode
        bool _addPlayerToMap(int playerFd);         // function that adds a players to the _playerEcho map
        bool _removePlayerFromMap(int playerFd);    // function that removes a players from the _playerEcho map
        int _serverPort;                            // server port
        int _maxPlayers;                            // max number of players that can be on the server
        int _timeOut;                               // time in seconds the server waits before removing an inactive player
        int _listenSocketFd;                        // socket the server uses to listen for new connections
        int _playerCount;                           // a count of the number of players on the server
        bool _serverPause;                          // pause state of the server
        legacymud::engine::GameLogic* _gameLogicPt; // game logic pointer the server is using
        struct _Player {                            // struct for a player's server info 
            bool echo;                              // flag that indicates a player's text echo display mode           
            std::string readBuffer;                 // a player's read string buffer
            std::string questionBuffer;             // a question sent to the player that's waiting for a response
        }; 
        std::map<int, _Player> _playerMap;          // map used track player's on the server and to capture player specific server data
        std::mutex _mu_player_map;                  // mutex used for the player map
        std::deque<int> _fd_held_que;               // que of file descriptors that haven't been released yet
        std::mutex _mu_fd_held_que;                 // mutex used for the held file descriptor que
};

}}

#endif