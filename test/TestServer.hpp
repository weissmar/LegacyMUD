/*!
  \file     TestServer.hpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the TestServer class.
*/

 
#ifndef LEGACYMUD_TEST_TESTSERVER_HPP
#define LEGACYMUD_TEST_TESTSERVER_HPP

#include <Server.hpp>

#include <string>
#include <atomic>
#include <mutex>
#include <thread>

namespace legacymud {
    namespace test {

    /*!
      \brief Stub telnet server used for testing GameLogic functionality.
    */
    class TestServer : public virtual telnet::Server {
    public:
        
        /*!
          \brief Default constructor.
        */     
        TestServer();
        
        /*!
          \brief Initializes the stub server.
          
          The only parameter that matters here is the \a gameLogicPt value.
          
          \param[in]  gameLogicPt       pointer to the game logic object         
        */        
        virtual bool initServer(int serverPort, int maxPlayers, int timeOut, legacymud::engine::GameLogic* gameLogicPt);
        
        /*!
          \brief Causes the server to listen for messages set by simulatePlayerInput().
        */
        virtual void startListening();
        
        /*!
          \brief Simulates disconnecting the player.
        */        
        virtual bool disconnectPlayer(int playerFd);
        
        /*!
          \brief Simulates shutting down the server.
        */        
        virtual bool shutDownServer();
        
        /*!
          \brief Simulates sending a question to the player.
        */        
        virtual bool sendQuestion(int playerFd, std::string outQuestion);        
        
        /*!
          \brief Simulates sending a message to the player.
          
          \param[in]  outMsg            message to be sent to a player
        */        
        virtual bool sendMsg(int playerFd, std::string outMsg);
        
        /*!
          \brief Simulates sending a newline to the player.
        */        
        virtual bool sendNewLine(int playerFd);        
        
        /*!
          \brief Receives the message set in simulatePlayerInput().

          \param[in]  inMsg             message received from a player                                 
        */         
        virtual bool receiveMsg(int playerFd, std::string &inMsg);
        
        /*!
          \brief Listens for messages set with simulatePlayerInput().
        */ 
        virtual bool listenForMsgs(int playerFd);
        
        /*!
          \brief Does nothing.
        */
        virtual void pause(bool pause);
        
        /*!
          \brief Does nothing.
        */        
        virtual bool setMaxPlayers(int maxPlayers);
        
        /*!
          \brief Does nothing.
        */ 
        virtual bool setTimeOut(int timeOut);
        
        /*!
          \brief Does nothing.
        */ 
        virtual bool setPlayerEcho(int playerFd, bool echo);
        
        /*!
          \brief Does nothing.
        */ 
        virtual bool getServerPause() const;
        
        /*!
          \brief Always returns 1.
        */
        virtual int getMaxPlayers() const;
        
        /*!
          \brief Returns 1 if a simulated player is connected, or 0 otherwise.
        */
        virtual int getPlayerCount() const;
        
        /*!
          \brief Does nothing.
        */        
        virtual int getTimeOut() const;
        
        /*!
          \brief Does nothing.
        */ 
        virtual int getServerPort() const;
        
        /*!
          \brief Does nothing.
        */ 
        virtual legacymud::engine::GameLogic* getGameLogicPt() const;

        /*!
          \brief Use this to get the text that was sent to the player.
        */
        std::string getToPlayerMsg();
        std::string tryGetToPlayerMsg();

        /*!
          \brief Use this to simulate text sent by the player.
        */
        void simulatePlayerInput(std::string input);

        /*!
          \brief Use this to simulate a player connection.
        */
        void simulatePlayerConnection();
        
    private:
        int _player;
        std::atomic<bool> _disconnect;
        std::thread _thread;
        mutable std::mutex _waitForConnection;
        mutable std::mutex _waitForDisconnect;
        mutable std::mutex _waitForReceive;
        mutable std::mutex _waitForSend;
        mutable std::string _toPlayer;
        mutable std::string _fromPlayer;
        legacymud::engine::GameLogic* _gameLogicPt;
};

}}

#endif