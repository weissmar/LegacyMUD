/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/2/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.cpp
 *
 * Overview:
 *     Stub of GameLogic class for testing Server class. Implementation.
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "GameLogic.h"

namespace legacymud { 
    namespace telnet {
        class Server;
    }
    
    namespace engine {

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

    serverPt->sendMsg("Welcome to Server Testing\n", playerFd);
    serverPt->sendMsg("Enter Username: ", playerFd);
    serverPt->receiveMsg(username, playerFd);
    std::cout << "Username received: " << username << std::endl;
    serverPt->sendMsg("Enter Password: ", playerFd);
    serverPt->receiveMsg(password, playerFd);
    std::cout << "Password received: " << password << std::endl;
    
    serverPt->sendMsg("Let's play!\n", playerFd);  
    
    /* Send thread back to server to listen for messages. */
    serverPt->listenForMsgs(playerFd);

}

/******************************************************************************
* Function:    receivedMsgHandler
* Description: This function handles received messages from a player.
*
* Input:            std::string inMsg  
* Input:            int playerFd           
* Preconditions:    none                        
* Return:           true if success.  Otherwise fail. 
*****************************************************************************/

bool GameLogic::receivedMsgHandler(std::string inMsg, int playerFd) {   
    ReceivedMsg rm;
    rm.msg = inMsg;
    rm.playerFd = playerFd;
    msgDeque.push_back(rm);
    return true;
}


}}