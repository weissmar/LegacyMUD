/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/10/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.cpp
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Implementation.
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "GameLogic.hpp"
#include "Server.hpp"

namespace legacymud {namespace engine {

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
    
    serverPt->sendMsg(playerFd, "Enter Username: ", serverPt->NO_NEWLINE);  
    if (serverPt->receiveMsg(playerFd, username ) == false) {
        /* Player either timed out or disconnected. */
        std::cout << "Player timed out or disconnected." << std::endl;
        /* Note: all calls to disconnectPlayer should come from the game logic. The server
        just returns false. */
        serverPt->disconnectPlayer(playerFd); 
        std::cout << "Disconnect.  Player count: " << serverPt->getPlayerCount() << std::endl;    
    }
    else {
        std::cout << "Username received: " << username << std::endl;
        serverPt->setPlayerEcho(playerFd, false);
        serverPt->sendMsg(playerFd, "Enter Password: ", serverPt->NO_NEWLINE);
        if(serverPt->receiveMsg(playerFd, password) == false) {
            /* Player either timed out or disconnected. */
            std::cout << "Player timed out or disconnected." << std::endl;        
            serverPt->disconnectPlayer(playerFd);  
            std::cout << "Disconnect.  Player count: " << serverPt->getPlayerCount() << std::endl;            
        }
        else {
            serverPt->setPlayerEcho(playerFd, true);
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

    std::cout << "End of newPlayerHandler." << std::endl;

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
    msgDeque.push_back(rm);
    return true;
}


}}