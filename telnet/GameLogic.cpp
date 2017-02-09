/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/8/2017
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
* Input:            int playerId           
* Preconditions:                           
* Return:           none 
*****************************************************************************/
void GameLogic::newPlayerHandler(int playerId) {   

    std::string username;
    std::string password;
    std::string newMsg;

    serverPt->sendMsg("Welcome to Server Testing\n", playerId);
    serverPt->sendMsg("Enter Username: ", playerId);  
    if (serverPt->receiveMsg(username, playerId) == false) {
        /* Player either timed out or disconnected. */
        std::cout << "Player timed out." << std::endl;
        serverPt->sendMsg("\nDisconnected for inactivity.\n", playerId);        
        serverPt->disconnectPlayer(playerId);      
    }
    else {
        std::cout << "Username received: " << username << std::endl;
        serverPt->sendMsg("Enter Password: ", playerId);
        if(serverPt->receiveMsg(password, playerId) == false) {
            /* Player either timed out or disconnected. */
            std::cout << "Player timed out." << std::endl;
            serverPt->sendMsg("\nDisconnected for inactivity.\n", playerId);        
            serverPt->disconnectPlayer(playerId);    
        }
        std::cout << "Password received: " << password << std::endl;
        
        serverPt->sendMsg("Let's play!\n", playerId);  
        
        /* Send thread back to server to listen for messages. */
        if (serverPt->listenForMsgs(playerId) == false) {
            /* Player either timed out or disconnected. */
            std::cout << "Player disconnected. " << std::endl;
        }   
    }
    std::cout << "End of newPlayerHandler." << std::endl;

}

/******************************************************************************
* Function:    receivedMsgHandler
* Description: This function handles received messages from a player.
*
* Input:            std::string inMsg  
* Input:            int playerId           
* Preconditions:    none                        
* Return:           true if success.  Otherwise fail. 
*****************************************************************************/

bool GameLogic::receivedMsgHandler(std::string inMsg, int playerId) {   
    ReceivedMsg rm;
    rm.msg = inMsg;
    rm.playerId = playerId;
    msgDeque.push_back(rm);
    return true;
}


}}