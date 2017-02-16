/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/15/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include <cstdlib>
#include <thread>
#include <chrono>
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

GameLogic::GameLogic() 
: accountManager(nullptr)
, theTextParser(nullptr)
, theServer(nullptr)
, startArea("Start Area", "You are surrounded by grey mist.", "You are surrounded by grey mist. You appear to be floating in the air.", AreaSize::LARGE){
    manager = new GameObjectManager;
}


GameLogic::GameLogic(const GameLogic &otherGameLogic)
: accountManager(nullptr)
, theTextParser(nullptr)
, theServer(nullptr)
, startArea(otherGameLogic.startArea){
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
    std::vector<Player*> players = manager->getPlayersPtrs();

    for (auto player : players){
        removePlayerMessageQueue(player);
    }

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
    bool validUser = false;
    std::string username, password, passwordVerify, playerName, message, number, pDescription;
    std::vector<PlayerClass*> pClasses;
    int pClassNumber, playerSize;
    Player *newPlayer = nullptr;
    Player *aPlayer = nullptr;
    Area *anArea = nullptr;

    while (!validUser){
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
            validUsername = accountManager->uniqueUsername(username);
            while (!validUsername){
                success = getValueFromUser(fileDescriptor, "That username is already in use. Please enter an alternate username.", username, true);
                if (!success)
                    return false;
                validUsername = accountManager->uniqueUsername(username);
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
            newPlayer = new Player(static_cast<CharacterSize>(playerSize - 1), pClasses[pClassNumber - 1], username, fileDescriptor, playerName, pDescription, &startArea);
            manager->addObject(newPlayer, fileDescriptor);

            // create account
            success = accountManager->createAccount(username, password, isAdmin, newPlayer->getID());
            if (!success){
                // username got taken in the interim DO SOMETHING
            }

            // move player to start area
            startArea.addCharacter(newPlayer);
            messagePlayer(newPlayer, startArea.getFullDescription());
            message = "You see a player named " + playerName + " enter the area.";
            messageAreaPlayers(message, &startArea);

            validUser = true;
        } else {
            // ask for password
            theServer->setPlayerEcho(fileDescriptor, false);
            success = getValueFromUser(fileDescriptor, "Please enter your password.", password, true);
            if (!success)
                return false;
            theServer->setPlayerEcho(fileDescriptor, true);

            // check account info
            success = accountManager->verifyAccount(username, password);
            if (success){
                // load player into game
                manager->loadPlayer(username, fileDescriptor);
                aPlayer = manager->getPlayerByFD(fileDescriptor);
                aPlayer->activate(fileDescriptor);

                // move player to current location
                anArea = aPlayer->getLocation();
                anArea->addCharacter(aPlayer);
                messagePlayer(aPlayer, anArea->getFullDescription());
                message = "You see a player named " + aPlayer->getName() + " enter the area.";
                messageAreaPlayers(message, anArea);

                validUser = true;
            } 
        }
    }

    success = theServer->listenForMsgs(fileDescriptor);
    if (!success){
        theServer->disconnectPlayer(fileDescriptor);
    }

    return true;
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
        useNewline = telnet::Server::NO_NEWLINE;
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
                isAdmin = accountManager->verifyAdmin(aPlayer->getUser());

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
                        handleParseError(aPlayer, resultVector.front());
                    }
                } else {
                    handleParseError(aPlayer, resultVector);
                }
            }
            lockQueue.lock();
        }
    }
}


bool GameLogic::receivedMessageHandler(std::string message, int fileDescriptor){
    std::pair<std::mutex*, std::queue<std::string>*> *playerQueue;

    // check if there's a player-specific queue
    std::unique_lock<std::mutex> playerMsgQLock(playerMsgQMutex);
    int specific = playerMessageQueues.count(fileDescriptor);

    if (specific == 1){
        playerQueue = &playerMessageQueues.at(fileDescriptor);
        std::lock_guard<std::mutex> playerQueueLock(*playerQueue->first);
        playerQueue->second->push(message);
    } else {
        playerMsgQLock.unlock();
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
        aPlayer->activate(fileDescriptor);
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
        removePlayerMessageQueue(aPlayer);
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

    if ((input.compare("Area") == 0) || (input.compare("AREA") == 0) || (input.compare("area") == 0)) {
        type = ObjectType::AREA;
    } else if ((input.compare("Exit") == 0) || (input.compare("armor type")  == 0) || (input.compare("ARMOR_TYPE") == 0)){
        type = ObjectType::ARMOR_TYPE;  
    } else if ((input.compare("Container") == 0) || (input.compare("CONTAINER")  == 0) || (input.compare("container") == 0)){
        type = ObjectType::CONTAINER;  
    } else if ((input.compare("Creature") == 0) || (input.compare("CREATURE")  == 0) || (input.compare("creature") == 0)) {
        type = ObjectType::CREATURE;   
    } else if ((input.compare("Creature Type") == 0) || (input.compare("creature type")  == 0) || (input.compare("CREATURE_TYPE") == 0)){
        type = ObjectType::CREATURE_TYPE;  
    } else if ((input.compare("Exit") == 0) || (input.compare("EXIT")  == 0) || (input.compare("exit") == 0)){
        type = ObjectType::EXIT;   
    } else if ((input.compare("Feature") == 0) || (input.compare("FEATURE")  == 0) || (input.compare("feature") == 0)){
        type = ObjectType::FEATURE;   
    } else if ((input.compare("Item") == 0) || (input.compare("ITEM")  == 0) || (input.compare("item") == 0)){
        type = ObjectType::ITEM;   
    } else if ((input.compare("Item Type") == 0) || (input.compare("item type")  == 0) || (input.compare("ITEM_TYPE") == 0)){
        type = ObjectType::ITEM_TYPE;  
    } else if ((input.compare("Non Combatant") == 0) || (input.compare("non combatant")  == 0) || (input.compare("NON_COMBATANT") == 0)){
        type = ObjectType::NON_COMBATANT;  
    } else if ((input.compare("Player") == 0) || (input.compare("PLAYER")  == 0) || (input.compare("player") == 0)){
        type = ObjectType::PLAYER;   
    } else if ((input.compare("Player Class") == 0) || (input.compare("player class")  == 0) || (input.compare("PLAYER_CLASS") == 0)){
        type = ObjectType::PLAYER_CLASS; 
    } else if ((input.compare("Quest") == 0) || (input.compare("QUEST")  == 0) || (input.compare("quest") == 0)) {
        type = ObjectType::QUEST;   
    } else if ((input.compare("Quest Step") == 0) || (input.compare("quest step")  == 0) || (input.compare("QUEST_STEP") == 0)){
        type = ObjectType::QUEST_STEP;  
    } else if ((input.compare("Special Skill") == 0) || (input.compare("special skill")  == 0) || (input.compare("SPECIAL_SKILL") == 0)){
        type = ObjectType::SPECIAL_SKILL;  
    } else if ((input.compare("Weapon Type") == 0) || (input.compare("weapon type")  == 0) || (input.compare("WEAPON_TYPE") == 0)){
        type = ObjectType::WEAPON_TYPE;
    } else {
        type = ObjectType::NONE;
    }

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


void GameLogic::messageAreaPlayers(std::string message, Area *anArea){
    std::vector<Character*> areaCharacters = anArea->getCharacters();
    std::vector<int> fileDescriptors;

    for (auto character : areaCharacters){
        if (character->getObjectType() == ObjectType::PLAYER){
            fileDescriptors.push_back(dynamic_cast<Player*>(character)->getFileDescriptor());
        }
    }

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

void GameLogic::handleParseError(Player *aPlayer, parser::ParseResult result){
    if (result.status == parser::ParseStatus::INVALID_VERB){
        // Input string contains invalid verb.
        messagePlayer(aPlayer, "I don't know what you mean by that.");
    } else if (result.status == parser::ParseStatus::UNAVAILABLE_VERB){
        // Input string contains valid verb but not in current area.
        messagePlayer(aPlayer, "You can't do that here.");
    } else if (result.status == parser::ParseStatus::INVALID_DIRECT){
        // Input string contains an invalid noun alias in the direct object position.
        messagePlayer(aPlayer, "I don't know what you mean by that.");
    } else if (result.status == parser::ParseStatus::UNAVAILABLE_DIRECT){
        // Input string contains a valid noun alias in the direct object position but not in current area.
        messagePlayer(aPlayer, "I don't see that here.");
    } else if (result.status == parser::ParseStatus::INVALID_PREPOSITION){
        // Input string contains an invalid preposition.
        messagePlayer(aPlayer, "I don't know what you mean by that.");
    } else if (result.status == parser::ParseStatus::INVALID_INDIRECT){
        // Input string contains an invalid noun alias in the indirect object position.
        messagePlayer(aPlayer, "I don't know what you mean by that.");
    } else if (result.status == parser::ParseStatus::UNAVAILABLE_INDIRECT){
        // Input string contains a valid noun alias in the indirect object position but not in current area.
        messagePlayer(aPlayer, "I don't see that here.");
    }
}

void GameLogic::handleParseError(Player *aPlayer, std::vector<parser::ParseResult> results){
    std::map<InteractiveNoun*, parser::ParseResult> directPtrMap;
    std::map<InteractiveNoun*, parser::ParseResult> indirectPtrMap;
    std::vector<InteractiveNoun*> directPtrs;
    std::vector<InteractiveNoun*> indirectPtrs;
    std::string message;
    InteractiveNoun *directChoice = nullptr;
    InteractiveNoun *indirectChoice = nullptr;
    parser::ParseResult chosenResult;
    int choice;
    bool commandSuccess = false;

    // get maps and vectors of distinct direct and indirect options
    for (auto result : results){
        for (auto ptr : result.direct){
            directPtrMap[ptr] = result;
        }
        for (auto ptr : result.indirect){
            indirectPtrMap[ptr] = result;
        }
    }
    for (auto directP : directPtrMap){
        directPtrs.push_back(directP.first);
    }
    for (auto indirectP : indirectPtrMap){
        indirectPtrs.push_back(indirectP.first);
    }

    // choose between multiple direct options
    if (directPtrs.size() > 1){
        addPlayerMessageQueue(aPlayer);
        sendClarifyingQuery(aPlayer, directPtrs);
        message = blockingGetMsg(aPlayer);
        choice = validateStringNumber(message, 1, directPtrs.size());
        while (choice == -1){
            messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
            message = blockingGetMsg(aPlayer);
            choice = validateStringNumber(message, 1, directPtrs.size());
        }
        directChoice = directPtrs[choice - 1];
        chosenResult = directPtrMap.at(directChoice);
        if (chosenResult.status == parser::ParseStatus::VALID){
            commandSuccess = executeCommand(chosenResult);
            if (!commandSuccess){
                messagePlayer(aPlayer, "You can't do that. (Command failed.)");
            }
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    } else if (indirectPtrs.size() > 1){
        // choose between multiple indirect options
        addPlayerMessageQueue(aPlayer);
        sendClarifyingQuery(aPlayer, indirectPtrs);
        message = blockingGetMsg(aPlayer);
        choice = validateStringNumber(message, 1, indirectPtrs.size());
        while (choice == -1){
            messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
            message = blockingGetMsg(aPlayer);
            choice = validateStringNumber(message, 1, directPtrs.size());
        }
        indirectChoice = indirectPtrs[choice - 1];
        chosenResult = indirectPtrMap.at(indirectChoice);
        if (chosenResult.status == parser::ParseStatus::VALID){
            commandSuccess = executeCommand(chosenResult);
            if (!commandSuccess){
                messagePlayer(aPlayer, "You can't do that. (Command failed.)");
            }
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    } else {
        // options all have the same direct and indirect options, use first
        chosenResult = results[0];
        if (chosenResult.status == parser::ParseStatus::VALID){
            commandSuccess = executeCommand(chosenResult);
            if (!commandSuccess){
                messagePlayer(aPlayer, "You can't do that. (Command failed.)");
            }
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    }
}


void GameLogic::sendClarifyingQuery(Player *aPlayer, std::vector<InteractiveNoun*> optionsVector){
    std::string message;

    message = "Did you mean ";
    for (int i = 0; i < optionsVector.size(); i++){
        message += "[";
        message += std::to_string(i + 1);
        message += "] ";
        message += optionsVector[i]->getName();
        if (i < (optionsVector.size() - 1)){
            message += ", ";
        }
    }
    message += "? Please enter the number that corresponds to your choice.";
    messagePlayer(aPlayer, message);
}


std::string GameLogic::blockingGetMsg(Player *aPlayer){
    bool msgRecived = false;
    std::string message = "";

    while (!msgRecived){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        message = getMsgFromPlayerQ(aPlayer);
        if (message != ""){
            msgRecived = true;
        }
    }

    return message;
}


void GameLogic::addPlayerMessageQueue(Player *aPlayer){
    int addPlayer;
    std::lock_guard<std::mutex> playerMsgQLock(playerMsgQMutex);
    int FD = aPlayer->getFileDescriptor();

    addPlayer = playerMessageQueues.count(FD);

    if (addPlayer == 0){
        playerMessageQueues[FD] = std::make_pair(new std::mutex, new std::queue<std::string>);
    }
}


void GameLogic::removePlayerMessageQueue(Player *aPlayer){
    int removePlayer;
    std::mutex *aMutex;
    std::queue<std::string> *aQueue;
    std::lock_guard<std::mutex> playerMsgQLock(playerMsgQMutex);
    int FD = aPlayer->getFileDescriptor();

    removePlayer = playerMessageQueues.count(FD);

    if (removePlayer == 1){
        aMutex = (playerMessageQueues.at(FD).first);
        aQueue = (playerMessageQueues.at(FD).second);

        playerMessageQueues.erase(FD);
        delete aMutex;
        delete aQueue;
    }
}


std::string GameLogic::getMsgFromPlayerQ(Player *aPlayer){
    std::string message;
    std::pair<std::mutex*, std::queue<std::string>*> *playerQueue;
    int FD = aPlayer->getFileDescriptor();
    std::lock_guard<std::mutex> playerMsgQLock(playerMsgQMutex);
    int specific = playerMessageQueues.count(FD);

    if (specific == 1){
        playerQueue = &playerMessageQueues.at(FD);
        std::lock_guard<std::mutex> playerQueueLock(*playerQueue->first);
        if (!playerQueue->second->empty()){
            message = playerQueue->second->front();
            playerQueue->second->pop();
        } else {
            message = "";
        }
    }

    return message;
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
    removePlayerMessageQueue(aPlayer);
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