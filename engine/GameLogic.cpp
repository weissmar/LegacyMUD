/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/14/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include <cstdlib>
#include <parser.hpp>
#include <Account.hpp>
#include <Server.hpp>
#include "GameLogic.hpp"
#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "NonCombatant.hpp"
#include "Area.hpp"
#include "PlayerClass.hpp"

namespace legacymud { namespace engine {

const std::string ADMIN_PASSWORD = "default";

GameLogic::GameLogic() : accountManager(nullptr), theTextParser(nullptr), theServer(nullptr){
    manager = new GameObjectManager;
}


GameLogic::GameLogic(const GameLogic &otherGameLogic){
    manager = new GameObjectManager(*otherGameLogic.manager);
}


GameLogic & GameLogic::operator=(const GameLogic &otherGameLogic){
    if (this != &otherGameLogic){
        delete manager;
        manager = new GameObjectManager(*otherGameLogic.manager);
    }

    return *this;
}


GameLogic::~GameLogic(){
    delete manager;
}


bool GameLogic::startGame(bool newGame, const std::string &fileName){
    return false;
}


bool GameLogic::newPlayerHandler(int fileDescriptor){
    bool success = false;
    bool newUser = false;
    bool validUsername = false;
    bool validPassword = false;
    bool isAdmin = false;
    std::string username, password, passwordVerify, playerName, message, number, pDescription;
    std::vector<PlayerClass*> pClasses;
    int pClassNumber, playerSize;
    Player *newPlayer;

    // get username
    success = getValueFromUser(fileDescriptor, "Please enter your username or [new] if you are new to this world.", username, true);
    if (!success)
        return false;

    // check if new 
    if (username.compare("new") == 0){
        newUser = true;

        // get username
        success = getValueFromUser(fileDescriptor, "Please enter your email address. You will use this as your username in the future.", username, true);
        if (!success)
            return false;

        // validate username
        validUsername = accountManager->validateUsername(username);
        while (!validUsername){
            success = getValueFromUser(fileDescriptor, "That username is already in use. Please enter an alternate username.", username, true);
            if (!success)
                return false;
            validUsername = accountManager->validateUsername(username);
        }

        // get password
        theServer->setPlayerEcho(fileDescriptor, false);
        success = getValueFromUser(fileDescriptor, "Please enter a password. (Note: This is not a secure connection. Please use a password that you don't mind others potentially seeing.", password, true);
        if (!success)
            return false;
        // check if admin password was entered
        if (password.compare(ADMIN_PASSWORD) == 0){
            isAdmin = true;
            success = getValueFromUser(fileDescriptor, "Welcome Administrator, please enter a new password.", password, true);
            if (!success)
                return false;
        }
        // verify password
        while (!validPassword){
            success = getValueFromUser(fileDescriptor, "Please verify the password.", passwordVerify, true);
            if (!success)
                return false;
            if (password.compare(passwordVerify) == 0){
                validPassword = true;
            } else {
                success = getValueFromUser(fileDescriptor, "The passwords didn't match. Please enter a password.", password, true);
                if (!success)
                    return false;
            }
        }
        theServer->setPlayerEcho(fileDescriptor, true);

        // get player name
        success = getValueFromUser(fileDescriptor, "What would you like your character name to be?", playerName, true);
        if (!success)
            return false;

        // get player class
        message = "What would you like your character class to be? Your options are: ";
        pClasses = manager->getPlayerClasses();
        for (int i = 0; i < pClasses.size(); i++){
            message += "[";
            message += std::to_string(i + 1);
            message += "] ";
            message += pClasses[i]->getName();
            if (i < (pClasses.size() - 1))
                message += ", ";
        }
        message += ". Please enter the number that corresponds to your choice.";
        success = getValueFromUser(fileDescriptor, message, number, true);
        if (!success)
            return false;
        pClassNumber = validateStringNumber(number, 1, pClasses.size());
        while (pClassNumber == -1){
            success = getValueFromUser(fileDescriptor, "Invalid input. Please enter the number that corresponds to your choice.", number, true);
            if (!success)
                return false;
            pClassNumber = validateStringNumber(number, 1, pClasses.size());
        }

        // get player size
        success = getValueFromUser(fileDescriptor, "What size is your character? Your options are: [1] Tiny, [2] Small, [3] Medium, [4] Large, [5] Huge. Please enter the number that corresponds to your choice.", number, true);
        if (!success)
            return false;
        playerSize = validateStringNumber(number, 1, 5);
        while (playerSize == -1){
            success = getValueFromUser(fileDescriptor, "Invalid input. Please enter the number that corresponds to your choice.", number, true);
            if (!success)
                return false;
            playerSize = validateStringNumber(number, 1, 5);
        }

        // get player description
        success = getValueFromUser(fileDescriptor, "Please enter a description of your character (from a third-person perspective).", pDescription, true);
        if (!success)
            return false;

        // create player
        newPlayer = new Player(playerSize - 1, pClasses[pClassNumber - 1], username, fileDescriptor, playerName, pDescription);
        manager->addObject(newPlayer);

        // create account
        success = accountManager->createAccount(username, password, isAdmin, newPlayer->getID());
        if (!success){
            // username got taken in the interim DO SOMETHING
        }

    }

    // ask for password

    // get response

    // check account info

    return false;
}


int GameLogic::validateStringNumber(std::string number, int min, int max){
    int intNum;
    bool validNumber = true;

    try {
        intNum = std::stoi(number);
    } catch (std::invalid_argument){
        validNumber = false;
    }
    if ((validNumber) && ((intNum < min) || (intNum > max))){
        validNumber = false;
    }

    if (validNumber){
        return intNum;
    } else {
        return -1;
    }
}


bool GameLogic::getValueFromUser(int FD, std::string outMessage, std::string response, bool newline){
    bool success;
    telnet::Server::NewLine useNewline = telnet::Server::NEWLINE;

    if (!newline){
        useNewline = telnetServer::NO_NEWLINE;
    }

    theServer->sendMsg(FD, outMessage, useNewline);
    success = theServer->receiveMsg(FD, response);
    if (!success){
        theServer->disconnectPlayer(FD);
        return false;
    }

    return true;
}


void GameLogic::processInput(int numToProcess){
    Player *aPlayer = nullptr;
    Area *anArea = nullptr;
    bool isAdmin = false;
    std::pair<std::string, int> aMessage;
    std::vector<parser::ParseResult> resultVector;
    bool commandSuccess = false;

    // lock for thread-safety
    std::unique_lock<std::mutex> lockQueue(queueMutex);

    for(int i = 0; i < numToProcess; i++){
        if (!messageQueue.empty()){
            // get the next message
            aMessage = messageQueue.front();
            messageQueue.pop();
            lockQueue.unlock();

            // get pointer to player the message is from
            aPlayer = manager->getPlayerByFD(aMessage.second);
            if (aPlayer != nullptr){
                // get pointer to area player is currently in
                anArea = aPlayer->getLocation();

                // check if player is admin
                isAdmin = accountManager->confirmAdmin(aPlayer->getUser());

                // send message to parser
                resultVector = theTextParser->parse(aMessage.first, aPlayer->getLexicalData(), anArea->getLexicalData(), isAdmin, aPlayer->isEditMode());

                // check results
                if (resultVector.size() == 1){
                    if (resultVector.front().status == parser::ParseStatus::VALID){
                        commandSuccess = executeCommand(resultVector.front());
                        if (!commandSuccess){
                            messagePlayer(aPlayer, "You can't do that. (Command failed.)");
                        }
                    } else {
                        handleParseError(resultVector.front());
                    }
                } else {
                    handleParseError(resultVector);
                }
            }
            lockQueue.lock();
        }
    }
}


bool GameLogic::receivedMessageHandler(std::string message, int fileDescriptor){
    std::pair<std::mutex, std::queue<std::string>> *playerQueue;

    // check if there's a player-specific queue
    int specific = playerMessageQueues.count(fileDescriptor);

    if (specific == 1){
        playerQueue = &playerMessageQueues.at(fileDescriptor);
        std::unique_lock<std::mutex> playerQueueLock(playerQueue->first);
        playerQueue->second.push(message);
        playerQueueLock.unlock();
    } else {
        std::lock_guard<std::mutex> lockGuard(queueMutex);
        messageQueue.push(std::make_pair(message, fileDescriptor));
    }

    return true;
}


bool GameLogic::updateCreatures(){
    return false;
}


bool GameLogic::updatePlayersInCombat(){
    return false;
}


bool GameLogic::loadPlayer(Player *aPlayer, int fileDescriptor){
    if ((aPlayer != nullptr) && (fileDescriptor >= 0)){
        aPlayer->setFileDescriptor(fileDescriptor);
        aPlayer->setActive(true);
        manager->loadPlayer(aPlayer->getUser(), fileDescriptor);
        return true;
    }
    return false;
}


bool GameLogic::hibernatePlayer(Player *aPlayer){
    if (aPlayer != nullptr){
        aPlayer->setActive(false);
        manager->hibernatePlayer(aPlayer->getFileDescriptor());
        aPlayer->setFileDescriptor(-1);
        return true;
    }
    return false;
}


int GameLogic::rollDice(int numSides, int numDice){
    int diceTotal = 0;

    for (int i = 0; i < numDice; i++){
        diceTotal += (std::rand() % numSides) + 1;
    }

    return diceTotal;
}


bool GameLogic::createObject(Player *aPlayer, ObjectType type){
    return false;
}


ObjectType GameLogic::getObjectType(const std::string &input){
    ObjectType type = ObjectType::NONE;

    if ((input.compare("Area") == 0) || (input.compare("AREA") == 0) || (input.compare("area") == 0))
        type = ObjectType::AREA;
    // TO DO: Finish this

    return type;
}


void GameLogic::messagePlayer(Player *aPlayer, std::string message){
    theServer->sendMsg(aPlayer->getFileDescriptor(), message, telnet::Server::NEWLINE);
}


void GameLogic::messageAllPlayers(std::string message){
    std::vector<int> fileDescriptors = manager->getPlayersFDs();

    for (auto FD : fileDescriptors){
        theServer->sendMsg(FD, message, telnet::Server::NEWLINE);
    }
}


bool GameLogic::startCombat(Player* aPlayer, Creature *aCreature){
    return false;
}


bool GameLogic::endCombat(Player *aPlayer, Creature *aCreature){
    return false;
}


bool GameLogic::startConversation(Player *aPlayer, NonCombatant *aNPC){
    return false;
}


bool GameLogic::endConversation(Player *aPlayer){
    return false;
}

void GameLogic::handleParseError(parser::ParseResult result){

}

void GameLogic::handleParseError(std::vector<parser::ParseResult> results){
    
}

bool GameLogic::executeCommand(parser::ParseResult){
    return false;
}


bool GameLogic::helpCommand(Player *aPlayer){
    return false;
}


bool GameLogic::lookCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::listenCommand(Player *aPlayer){
    return false;
}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::putCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, ItemPosition aPosition){
    return false;
}


bool GameLogic::dropCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::inventoryCommand(Player *aPlayer){
    return false;
}


bool GameLogic::moreCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::equipmentCommand(Player *aPlayer){
    return false;
}


bool GameLogic::equipCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::unequipCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::transferCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::speakCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *indirectObj, const std::string &stringParam){
    return false;
}


bool GameLogic::quitCommand(Player *aPlayer){
    return false;
}


bool GameLogic::goCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::moveCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::statsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::questsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::skillsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::attackCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::talkCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::shopCommand(Player *aPlayer){
    return false;
}


bool GameLogic::buyCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::sellCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::searchCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::useSkillCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::readCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::breakCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::climbCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::turnCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::pushCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::pullCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::eatCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::drinkCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::editModeCommand(Player *aPlayer){
    return false;
} 


bool GameLogic::warpCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::copyCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::createCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::editAttributeCommand(Player *aPlayer, InteractiveNoun *directObj, const std::string &stringParam){
    return false;
}


bool GameLogic::editWizardCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::saveCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::loadCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::deleteCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}

}}