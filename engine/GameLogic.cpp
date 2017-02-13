/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/12/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.cpp
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Implementation.
 ************************************************************************/

#include <iostream>
#include <string>
#include <mutex>            // mutex locks
#include "GameLogic.hpp"
#include <Server.hpp>

namespace legacymud { namespace engine {
    

/******************************************************************************
* Function:    newPlayerHandler
* Description: This function handles new players.
*
* Input:            int playerFd           
* Preconditions:                           
* Return:           none 
*****************************************************************************/
void GameLogic::newPlayerHandler(int playerFd) {   

    std::string username;
    std::string password;
    std::string newMsg;

    serverPt->sendMsg(playerFd, "Welcome to Server Testing", serverPt->NEWLINE);
    std::cout << "Player added.  Player count: " << serverPt->getPlayerCount() << std::endl;
    
    /* Prompt for a username. */
    serverPt->sendMsg(playerFd, "Enter Username: ", serverPt->NO_NEWLINE);  
    if (serverPt->receiveMsg(playerFd, username ) == false) {
        /* Player either timed out or disconnected. */
        std::cout << "Player timed out or disconnected." << std::endl;
        /* Note: all calls to disconnectPlayer should come from the game logic. */
        serverPt->disconnectPlayer(playerFd); 
        std::cout << "Disconnect.  Player count: " << serverPt->getPlayerCount() << std::endl;    
    }
    /* Display server name, turn off text echo mode, and prompt for a password. */
    else {
        std::cout << "Username received: " << username << std::endl;
        serverPt->setPlayerEcho(playerFd, false);       // turn off  text echo mode for this player
        serverPt->sendMsg(playerFd, "Enter Password: ", serverPt->NO_NEWLINE);
        if(serverPt->receiveMsg(playerFd, password) == false) {
            /* Player either timed out or disconnected. */
            std::cout << "Player timed out or disconnected." << std::endl;        
            /* Note: all calls to disconnectPlayer should come from the game logic. */
            serverPt->disconnectPlayer(playerFd);  
            std::cout << "Disconnect.  Player count: " << serverPt->getPlayerCount() << std::endl;            
        }
        /* Display password, turn on text echo mode, and enter game play. */
        else {
            serverPt->setPlayerEcho(playerFd, true);    // turn text echo mode for this player back on
            std::cout << "Password received: " << password << std::endl;
            serverPt->sendMsg(playerFd, "Let's play!", serverPt->NEWLINE);     
            
            /* Send thread back to server to listen for messages. */
            if (serverPt->listenForMsgs(playerFd) == false) {
                /* Player either timed out or disconnected. */
                std::cout << "Player timed out or disconnected." << std::endl;  
                serverPt->disconnectPlayer(playerFd); 
                std::cout << "Disconnect.  Player count: " << serverPt->getPlayerCount() << std::endl;
            }  
        }
    }

    /* This is only reached if a player's listener detects a disconnect.  The player's listen thread ends here. */
    std::cout << "End of newPlayerHandler. A player's listen thread has ended due to disconnect." << std::endl;

}

/******************************************************************************
* Function:    receivedMessageHandler
* Description: This function handles received messages from a player.
*
* Input:            std::string inMsg  
* Input:            int playerFd           
* Preconditions:    none                        
* Return:           true if success.  Otherwise fail. 
*****************************************************************************/

bool GameLogic::receivedMessageHandler(std::string inMsg, int playerFd) {
    
    ReceivedMsg rm;
    rm.msg = inMsg;
    rm.playerFd = playerFd;
    
    std::unique_lock<std::mutex> locker(mu_msg);    
    msgDeque.push_back(rm);
    locker.unlock();
    cond_msg.notify_one();      // notify the game engine that a message is here
    
    return true;
}


}}