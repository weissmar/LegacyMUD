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
* Input:            int playerId           
* Preconditions:                           
* Return:           none 
*****************************************************************************/
void GameLogic::newPlayerHandler(int playerId) {   

    std::string username;
    std::string password;
    std::string newMsg;

    serverPt->sendMsg(playerId, "Welcome to Server Testing", serverPt->NEWLINE);
    serverPt->sendMsg(playerId, "Enter Username: ", serverPt->NO_NEWLINE);  
    if (serverPt->receiveMsg(playerId, username ) == false) {
        /* Player either timed out or disconnected. */
        std::cout << "Player timed out." << std::endl;
        serverPt->sendMsg(playerId, "", serverPt->NEWLINE);
        serverPt->sendMsg(playerId, "Disconnected for inactivity.", serverPt->NEWLINE);        
        serverPt->disconnectPlayer(playerId);      
    }
    else {
        std::cout << "Username received: " << username << std::endl;
        serverPt->setEcho(false);
        serverPt->sendMsg(playerId, "Enter Password: ", serverPt->NO_NEWLINE);
        if(serverPt->receiveMsg(playerId, password) == false) {
            /* Player either timed out or disconnected. */
            std::cout << "Player timed out." << std::endl;
            serverPt->sendMsg(playerId, "", serverPt->NEWLINE);
            serverPt->sendMsg(playerId, "Disconnected for inactivity.", serverPt->NEWLINE );        
            serverPt->disconnectPlayer(playerId);    
        }
        serverPt->setEcho(true);
        std::cout << "Password received: " << password << std::endl;
        
        serverPt->sendMsg(playerId, "Let's play!", serverPt->NEWLINE);  
        
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