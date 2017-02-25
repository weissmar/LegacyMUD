/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/24/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include <cstdlib>
#include <thread>
#include <chrono>
#include <iostream>
#include "parser.hpp"
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
#include "EffectType.hpp"
#include "Container.hpp"
#include "Item.hpp"
#include "ItemType.hpp"

namespace legacymud { namespace engine {

const std::string ADMIN_PASSWORD = "default";

GameLogic::GameLogic() 
: accountManager(nullptr)
, theServer(nullptr)
, startArea("Start Area", "You are surrounded by grey mist.", "You are surrounded by grey mist. You appear to be floating in the air.", AreaSize::LARGE){
    manager = new GameObjectManager;
}


GameLogic::GameLogic(const GameLogic &otherGameLogic)
: accountManager(nullptr)
, theServer(nullptr)
, startArea("Start Area", "You are surrounded by grey mist.", "You are surrounded by grey mist. You appear to be floating in the air.", AreaSize::LARGE){
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


bool GameLogic::startGame(bool newGame, const std::string &fileName, telnet::Server *aServer, account::Account *anAccount){
    PlayerClass *aClass;
    ItemType *anItemType;
    Item *anItem;

    accountManager = anAccount;
    theServer = aServer;

    // create a default player class
    aClass = new PlayerClass(0, "default class", nullptr, 5, 5, DamageType::NONE, DamageType::NONE, 1.0);
    manager->addObject(aClass, -1);

    // create a test item type and item
    anItemType = new ItemType(5, ItemRarity::COMMON, "a bright and shiny apple", "apple", 1, EquipmentSlot::NONE); 
    manager->addObject(anItemType, -1);
    anItem = new Item(&startArea, ItemPosition::GROUND, "red apple", anItemType);
    manager->addObject(anItem, -1);
    startArea.addItem(anItem);

    return false;
}


bool GameLogic::newPlayerHandler(int fileDescriptor){
    bool success = false;
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
        success = getValueFromUser(fileDescriptor, "Please enter your username or [new] if you are new to this world.", username);
        if (!success)
            return false;

        // check if new 
        if (username.compare("new") == 0){

            // get username
            success = getValueFromUser(fileDescriptor, "Please enter your email address. You will use this as your username in the future.", username);
            if (!success)
                return false;

            // validate username
            validUsername = accountManager->uniqueUsername(username);
            while (!validUsername){
                success = getValueFromUser(fileDescriptor, "That username is already in use. Please enter an alternate username.", username);
                if (!success)
                    return false;
                validUsername = accountManager->uniqueUsername(username);
            }

            // get password
            theServer->setPlayerEcho(fileDescriptor, false);
            success = getValueFromUser(fileDescriptor, "Please enter a password. (Note: This is not a secure connection. Please use a password that you don't mind others potentially seeing.", password);
            if (!success)
                return false;
            // check if admin password was entered
            if (password.compare(ADMIN_PASSWORD) == 0){
                isAdmin = true;
                success = getValueFromUser(fileDescriptor, "Welcome Administrator, please enter a new password.", password);
                if (!success)
                    return false;
            }
            // verify password
            while (!validPassword){
                success = getValueFromUser(fileDescriptor, "Please verify the password.", passwordVerify);
                if (!success)
                    return false;
                if (password.compare(passwordVerify) == 0){
                    validPassword = true;
                } else {
                    success = getValueFromUser(fileDescriptor, "The passwords didn't match. Please enter a password.", password);
                    if (!success)
                        return false;
                }
            }
            theServer->setPlayerEcho(fileDescriptor, true);

            // get player name
            success = getValueFromUser(fileDescriptor, "What would you like your character name to be?", playerName);
            if (!success)
                return false;

            // get player class
            message = "What would you like your character class to be? Your options are: ";
            pClasses = manager->getPlayerClasses();
            for (size_t i = 0; i < pClasses.size(); i++){
                message += "[";
                message += std::to_string(i + 1);
                message += "] ";
                message += pClasses[i]->getName();
                if (i < (pClasses.size() - 1))
                    message += ", ";
            }
            message += ". Please enter the number that corresponds to your choice.";
            success = getValueFromUser(fileDescriptor, message, number);
            if (!success)
                return false;
            pClassNumber = validateStringNumber(number, 1, pClasses.size());
            while (pClassNumber == -1){
                success = getValueFromUser(fileDescriptor, "Invalid input. Please enter the number that corresponds to your choice.", number);
                if (!success)
                    return false;
                pClassNumber = validateStringNumber(number, 1, pClasses.size());
            }

            // get player size
            success = getValueFromUser(fileDescriptor, "What size is your character? Your options are: [1] Tiny, [2] Small, [3] Medium, [4] Large, [5] Huge. Please enter the number that corresponds to your choice.", number);
            if (!success)
                return false;
            playerSize = validateStringNumber(number, 1, 5);
            while (playerSize == -1){
                success = getValueFromUser(fileDescriptor, "Invalid input. Please enter the number that corresponds to your choice.", number);
                if (!success)
                    return false;
                playerSize = validateStringNumber(number, 1, 5);
            }

            // get player description
            success = getValueFromUser(fileDescriptor, "Please enter a description of your character (from a third-person perspective).", pDescription);
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
            newPlayer->setLocation(&startArea);
            messagePlayer(newPlayer, startArea.getFullDescription(newPlayer->getID()));
            message = "You see a player named " + playerName + " enter the area.";
            messageAreaPlayers(newPlayer, message, &startArea);

            validUser = true;
        } else {
            // ask for password
            theServer->setPlayerEcho(fileDescriptor, false);
            success = getValueFromUser(fileDescriptor, "Please enter your password.", password);
            if (!success)
                return false;
            theServer->setPlayerEcho(fileDescriptor, true);

            // check account info
            success = accountManager->verifyAccount(username, password);
            if (success){
                // check if user is already logged in
                aPlayer = manager->getPlayerByUsername(username);
                if (aPlayer != nullptr){
                    // load player into game
                    manager->loadPlayer(username, fileDescriptor);
                    aPlayer = manager->getPlayerByFD(fileDescriptor);
                    aPlayer->activate(fileDescriptor);

                    // move player to current location
                    anArea = aPlayer->getLocation();
                    anArea->addCharacter(aPlayer);
                    messagePlayer(aPlayer, anArea->getFullDescription(aPlayer->getID()));
                    message = "You see a player named " + aPlayer->getName() + " enter the area.";
                    messageAreaPlayers(aPlayer, message, anArea);

                    validUser = true;
                } else {
                    theServer->sendMsg(fileDescriptor, "That account is already logged in. Please log in with a different account.");
                }
            } else {
                theServer->sendMsg(fileDescriptor, "Your username and password didn't match any accounts we have on file. Please try again.");
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


bool GameLogic::getValueFromUser(int FD, std::string outMessage, std::string &response){
    bool success;

    theServer->sendMsg(FD, outMessage);
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
                resultVector = parser::TextParser::parse(aMessage.first, aPlayer->getLexicalData(), anArea->getLexicalData(), isAdmin, aPlayer->isEditMode());

                // check results
                if (resultVector.size() == 1){
                    if (resultVector.front().status == parser::ParseStatus::VALID){
                        std::thread aThread(&GameLogic::executeCommand, this, aPlayer, resultVector.front());
                        aThread.detach();
                    } else {
                        handleParseError(aPlayer, resultVector.front());
                    }
                } else if (resultVector.size() == 0){
                    std::cout << "DEBUG:: parser returned empty result vector\n";
                } else {
                    std::thread aThread(&GameLogic::handleParseErrorMult, this, aPlayer, resultVector);
                    aThread.detach();
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
        aPlayer->getLocation()->removeCharacter(aPlayer);
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
    theServer->sendMsg(aPlayer->getFileDescriptor(), message);
}


void GameLogic::messageAllPlayers(Player *aPlayer, std::string message){
    int excludeFD = -1;
    std::vector<int> fileDescriptors = manager->getPlayersFDs();

    if (aPlayer != nullptr){
        excludeFD = aPlayer->getFileDescriptor();
    }

    for (auto FD : fileDescriptors){
        if (FD != excludeFD)
            theServer->sendMsg(FD, message);
    }
}


void GameLogic::messageAreaPlayers(Player *aPlayer, std::string message, Area *anArea){
    int excludeFD = -1;
    std::vector<Character*> areaCharacters = anArea->getCharacters();
    std::vector<int> fileDescriptors;

    if (aPlayer != nullptr){
        excludeFD = aPlayer->getFileDescriptor();
    }

    for (auto character : areaCharacters){
        if (character->getObjectType() == ObjectType::PLAYER){
            fileDescriptors.push_back(dynamic_cast<Player*>(character)->getFileDescriptor());
        }
    }

    for (auto FD : fileDescriptors){
        if (FD != excludeFD)
            theServer->sendMsg(FD, message);
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
    printParseResult(result);
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

void GameLogic::printParseResult(parser::ParseResult result){
    std::cout << "Result from parser:\n";
    std::cout << "status = "; 
    switch (result.status){
        case parser::ParseStatus::UNPARSED:
            std::cout << "UNPARSED\n";
            break;
        case parser::ParseStatus::INVALID_VERB:
            std::cout << "INVALID_VERB\n";
            break;
        case parser::ParseStatus::INVALID_DIRECT:
            std::cout << "INVALID_DIRECT\n";
            break;
        case parser::ParseStatus::INVALID_INDIRECT:
            std::cout << "INVALID_INDIRECT\n";
            break;
        case parser::ParseStatus::INVALID_PREPOSITION:
            std::cout << "INVALID_PREPOSITION\n";
            break;
        case parser::ParseStatus::UNAVAILABLE_VERB:
            std::cout << "UNAVAILABLE_VERB\n";
            break;
        case parser::ParseStatus::UNAVAILABLE_DIRECT:
            std::cout << "UNAVAILABLE_DIRECT\n";
            break;
        case parser::ParseStatus::UNAVAILABLE_INDIRECT:
            std::cout << "UNAVAILABLE_INDIRECT\n";
            break;
        case parser::ParseStatus::VALID:
            std::cout << "VALID\n";
            break;
        default:
            std::cout << "\n";
    }
    std::cout << "command = ";
    switch (result.command){
        case CommandEnum::HELP:
            std::cout << "HELP\n";
            break;
        case CommandEnum::INVALID:
            std::cout << "INVALID\n";
            break;
        case CommandEnum::LOOK:
            std::cout << "LOOK\n";
            break;
        case CommandEnum::LISTEN:
            std::cout << "LISTEN\n";
            break;
        case CommandEnum::TAKE:
            std::cout << "TAKE\n";
            break;
        case CommandEnum::PUT:
            std::cout << "PUT\n";
            break;
        case CommandEnum::DROP:
            std::cout << "DROP\n";
            break;
        case CommandEnum::INVENTORY:
            std::cout << "INVENTORY\n";
            break;
        case CommandEnum::MORE:
            std::cout << "MORE\n";
            break;
        case CommandEnum::EQUIPMENT:
            std::cout << "EQUIPMENT\n";
            break;
        case CommandEnum::EQUIP:
            std::cout << "EQUIP\n";
            break;
        case CommandEnum::UNEQUIP:
            std::cout << "UNEQUIP\n";
            break;
        case CommandEnum::TRANSFER:
            std::cout << "TRANSFER\n";
            break;
        case CommandEnum::SPEAK:
            std::cout << "SPEAK\n";
            break;
        case CommandEnum::SHOUT:
            std::cout << "SHOUT\n";
            break;
        case CommandEnum::WHISPER:
            std::cout << "WHISPER\n";
            break;
        case CommandEnum::QUIT:
            std::cout << "QUIT\n";
            break;
        case CommandEnum::GO:
            std::cout << "GO\n";
            break;
        case CommandEnum::MOVE:
            std::cout << "MOVE\n";
            break;
        case CommandEnum::STATS:
            std::cout << "STATS\n";
            break;
        case CommandEnum::QUESTS:
            std::cout << "QUESTS\n";
            break;
        case CommandEnum::SKILLS:
            std::cout << "SKILLS\n";
            break;
        case CommandEnum::ATTACK:
            std::cout << "ATTACK\n";
            break;
        case CommandEnum::TALK:
            std::cout << "TALK\n";
            break;
        case CommandEnum::SHOP:
            std::cout << "SHOP\n";
            break;
        case CommandEnum::BUY:
            std::cout << "BUY\n";
            break;
        case CommandEnum::SELL:
            std::cout << "SELL\n";
            break;
        case CommandEnum::SEARCH:
            std::cout << "SEARCH\n";
            break;
        case CommandEnum::USE_SKILL:
            std::cout << "USE_SKILL\n";
            break;
        case CommandEnum::READ:
            std::cout << "READ\n";
            break;
        case CommandEnum::BREAK:
            std::cout << "BREAK\n";
            break;
        case CommandEnum::CLIMB:
            std::cout << "CLIMB\n";
            break;
        case CommandEnum::TURN:
            std::cout << "TURN\n";
            break;
        case CommandEnum::PUSH:
            std::cout << "PUSH\n";
            break;
        case CommandEnum::PULL:
            std::cout << "PULL\n";
            break;
        case CommandEnum::EAT:
            std::cout << "EAT\n";
            break;
        case CommandEnum::DRINK:
            std::cout << "DRINK\n";
            break;
        case CommandEnum::EDIT_MODE:
            std::cout << "EDIT_MODE\n";
            break;
        case CommandEnum::WARP:
            std::cout << "WARP\n";
            break;
        case CommandEnum::COPY:
            std::cout << "COPY\n";
            break;
        case CommandEnum::CREATE:
            std::cout << "CREATE\n";
            break;
        case CommandEnum::EDIT_ATTRIBUTE:
            std::cout << "EDIT_ATTRIBUTE\n";
            break;
        case CommandEnum::EDIT_WIZARD:
            std::cout << "EDIT_WIZARD\n";
            break;
        case CommandEnum::SAVE:
            std::cout << "SAVE\n";
            break;
        case CommandEnum::LOAD:
            std::cout << "LOAD\n";
            break;
        case CommandEnum::DELETE:
            std::cout << "DELETE\n";
            break;
        default:
            std::cout << "\n";
    }
    std::cout << "direct = ";
    for (size_t i = 0; i < result.direct.size(); i++){
        if (result.direct[i] != nullptr){
            std::cout << result.direct[i]->getName();
        } else {
            std::cout << "nullptr";
        }
        std::cout << ", ";
    }
    
    std::cout << "\ndirectAlias = " << result.directAlias;
    std::cout << "indirect = ";
    for (size_t i = 0; i < result.indirect.size(); i++){
        if (result.indirect[i] != nullptr){
            std::cout << result.indirect[i]->getName();
        } else {
            std::cout << "nullptr";
        }
        std::cout << ", ";
    }
    std::cout << "\nindirectAlias = " << result.indirectAlias;
    std::cout << "\n position = ";
    switch (result.position){
        case ItemPosition::NONE:
            std::cout << "none\n";
            break;
        case ItemPosition::GROUND:
            std::cout << "ground\n";
            break;
        case ItemPosition::INVENTORY:
            std::cout << "inventory\n";
            break;
        case ItemPosition::EQUIPPED:
            std::cout << "equipped\n";
            break;
        case ItemPosition::IN:
            std::cout << "in\n";
            break;
        case ItemPosition::ON:
            std::cout << "on\n";
            break;
        case ItemPosition::UNDER :
            std::cout << "under\n";
            break;
        default:
            std::cout << "\n";
    }
    std::cout << "unparsed = ";
    std::cout << result.unparsed;
    std::cout << "\n";
}

void GameLogic::handleParseErrorMult(Player *aPlayer, std::vector<parser::ParseResult> results){
    std::map<InteractiveNoun*, parser::ParseResult> directPtrMap;
    std::map<InteractiveNoun*, parser::ParseResult> indirectPtrMap;
    std::vector<InteractiveNoun*> directPtrs;
    std::vector<InteractiveNoun*> indirectPtrs;
    std::string message;
    InteractiveNoun *directChoice = nullptr;
    InteractiveNoun *indirectChoice = nullptr;
    parser::ParseResult chosenResult;

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
        directChoice = clarifyChoice(aPlayer, directPtrs);
        chosenResult = directPtrMap.at(directChoice);
        if (chosenResult.status == parser::ParseStatus::VALID){
            executeCommand(aPlayer, chosenResult);
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    } else if (indirectPtrs.size() > 1){
        // choose between multiple indirect options
        indirectChoice = clarifyChoice(aPlayer, indirectPtrs);
        chosenResult = indirectPtrMap.at(indirectChoice);
        if (chosenResult.status == parser::ParseStatus::VALID){
            executeCommand(aPlayer, chosenResult);
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    } else {
        // options all have the same direct and indirect options, use first
        chosenResult = results[0];
        if (chosenResult.status == parser::ParseStatus::VALID){
            executeCommand(aPlayer, chosenResult);
        } else {
            handleParseError(aPlayer, chosenResult);
        }
    }
}


void GameLogic::sendClarifyingQuery(Player *aPlayer, std::vector<InteractiveNoun*> optionsVector){
    std::string message;

    message = "Did you mean ";
    for (size_t i = 0; i < optionsVector.size(); i++){
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


InteractiveNoun* GameLogic::clarifyChoice(Player *aPlayer, std::vector<InteractiveNoun*> optionsVector){
    std::string message;
    int choice;
    InteractiveNoun *finalChoice;

    addPlayerMessageQueue(aPlayer);
    sendClarifyingQuery(aPlayer, optionsVector);
    message = blockingGetMsg(aPlayer);
    choice = validateStringNumber(message, 1, optionsVector.size());
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        message = blockingGetMsg(aPlayer);
        choice = validateStringNumber(message, 1, optionsVector.size());
    }
    removePlayerMessageQueue(aPlayer);
    finalChoice = optionsVector[choice - 1];

    return finalChoice;
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


bool GameLogic::executeCommand(Player *aPlayer, parser::ParseResult result){
    bool success = false;
    InteractiveNoun *param = nullptr;
    InteractiveNoun *directObj = nullptr;
    InteractiveNoun *indirectObj = nullptr;

    printParseResult(result);

    switch (result.command) {
        case CommandEnum::HELP:           
            // Displays a list of some available commands.
            success = helpCommand(aPlayer);
            break;
        case CommandEnum::LOOK:           
            // clarify the object being looked at
            param = clarifyDirect(aPlayer, result);
            if (param == nullptr){
                param = clarifyIndirect(aPlayer, result);
            }
            // Displays the full description of the current area or specified direct object.
            success = lookCommand(aPlayer, param);
            break;
        case CommandEnum::LISTEN:         
            // Displays an optional description of any sounds in the current area.
            success = listenCommand(aPlayer);
            break;
        case CommandEnum::TAKE:
        std::cout << "inside case CommandEnum::TAKE in executeCommand\n";
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);
            // clarify indirect
            indirectObj = clarifyIndirect(aPlayer, result);
            // Puts the specified item into inventory.
            success = takeCommand(aPlayer, directObj, indirectObj);
            break;
        case CommandEnum::PUT:  
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);
            // clarify indirect
            indirectObj = clarifyIndirect(aPlayer, result);         
            // Puts the specified item in, on, or under the specified container.
            success = putCommand(aPlayer, directObj, indirectObj, result.position);
            break;
        case CommandEnum::DROP:       
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);
            // Drops the specified item onto the ground.
            success = dropCommand(aPlayer, directObj);
            break;
        case CommandEnum::INVENTORY:      
            // Displays the player's inventory and equipped items.
            success = inventoryCommand(aPlayer);
            break;
        case CommandEnum::MORE:
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);       
            // Displays details of the specified item or skill.
            success = moreCommand(aPlayer, directObj);
            break;
        case CommandEnum::EQUIPMENT:      
            // Display the player's equipped items.
            success = equipmentCommand(aPlayer);
            break;
        case CommandEnum::EQUIP:    
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);    
            // Equips the specified item.
            success = equipCommand(aPlayer, directObj);
            break;
        case CommandEnum::UNEQUIP:  
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);     
            // Unequips the specified item.
            success = unequipCommand(aPlayer, directObj);
            break;
        case CommandEnum::TRANSFER:
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);
            // clarify indirect
            indirectObj = clarifyIndirect(aPlayer, result); 
            // Gives the specified item to the specified character.
            success = transferCommand(aPlayer, directObj, indirectObj);
            break;
        case CommandEnum::SPEAK:          
            // Says the specified text to all players in the current area.
            success = speakCommand(aPlayer, result.directAlias);
            break;
        case CommandEnum::SHOUT:          
            // Shouts the specified text to all players within n links of the current area.
            success = shoutCommand(aPlayer, result.directAlias);
            break;
        case CommandEnum::WHISPER:        
            indirectObj = clarifyIndirect(aPlayer, result);
            // Whispers the specified text to the specified player.
            success = whisperCommand(aPlayer, indirectObj, result.directAlias);
            break;
        case CommandEnum::QUIT:           
            // Logs the player out of the game.
            success = quitCommand(aPlayer);
            break;
        case CommandEnum::GO:             
            // clarify where to go
            param = clarifyDirect(aPlayer, result);
            if (param == nullptr){
                param = clarifyIndirect(aPlayer, result);
            }
            // Moves the player to the specified area.
            success = goCommand(aPlayer, param);
            break;
        case CommandEnum::MOVE:     
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);      
            // Moves the specified item.
            success = moveCommand(aPlayer, directObj);    
            break;
        case CommandEnum::STATS:          
            // Displays the player's stats.
            success = statsCommand(aPlayer);    
            break;
        case CommandEnum::QUESTS:         
            // Displays the player's quests.
            success = questsCommand(aPlayer);    
            break;
        case CommandEnum::SKILLS:         
            // Displays the player's skills.
            success = skillsCommand(aPlayer);    
            break;
        case CommandEnum::ATTACK:    
            // clarify direct 
            directObj = clarifyDirect(aPlayer, result);
            // clarify indirect
            indirectObj = clarifyIndirect(aPlayer, result);     
            // Initiates or continues combat with the specified combatant, using either the default attack or the specified attack skill.
            success = attackCommand(aPlayer, directObj, indirectObj);    
            break;
        case CommandEnum::TALK:        
            // clarify who is being talked to
            param = clarifyDirect(aPlayer, result);
            if (param == nullptr){
                param = clarifyIndirect(aPlayer, result);
            } 
            // Initiates a conversation with the specified non-combatant.
            success = talkCommand(aPlayer, param);    
            break;
        case CommandEnum::SHOP:           
            // Lists any items the non-combatant the player is talking to has for sale.
            success = shopCommand(aPlayer);    
            break;
        case CommandEnum::BUY:    
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);
            // Purchases the specified item from the non-combatant the player is talking to.
            success = buyCommand(aPlayer, directObj);    
            break;
        case CommandEnum::SELL:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);           
            // Sells the specified item to the non-combatant the player is speaking to.
            success = sellCommand(aPlayer, directObj);    
            break;
        case CommandEnum::SEARCH:  
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);       
            // Lists any items in the specified container.
            success = searchCommand(aPlayer, directObj);    
            break;
        case CommandEnum::USE_SKILL:  
            // clarify direct and indirect
            directObj = clarifyDirect(aPlayer, result);
            indirectObj = clarifyIndirect(aPlayer, result);    
            // Activates the specified skill.
            success = useSkillCommand(aPlayer, directObj, indirectObj);    
            break;
        case CommandEnum::READ:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);           
            // Reads the specified item.
            success = readCommand(aPlayer, directObj);    
            break;
        case CommandEnum::BREAK:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);          
            // Breaks the specified item.
            success = breakCommand(aPlayer, directObj);    
            break;
        case CommandEnum::CLIMB:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);          
            // Climbs the specified item.
            success = climbCommand(aPlayer, directObj);    
            break;
        case CommandEnum::TURN:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);           
            // Turns the specified item.
            success = turnCommand(aPlayer, directObj);    
            break;
        case CommandEnum::PUSH:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);           
            // Pushes the specified item.
            success = pushCommand(aPlayer, directObj);    
            break;
        case CommandEnum::PULL:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);           
            // Pulls the specified item.
            success = pullCommand(aPlayer, directObj);    
            break;
        case CommandEnum::EAT:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);            
            // Eats the specified item.
            success = eatCommand(aPlayer, directObj);    
            break;
        case CommandEnum::DRINK:
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);          
            // Drinks the specified item.
            success = drinkCommand(aPlayer, directObj);    
            break;
        case CommandEnum::EDIT_MODE:      
            // Toggles between edit mode and normal mode.
            success = editModeCommand(aPlayer);    
            break;
        case CommandEnum::WARP: 
            // clarify where to warp to
            param = clarifyDirect(aPlayer, result);
            if (param == nullptr){
                param = clarifyIndirect(aPlayer, result);
            }          
            // World builder command. Moves the player to the specified area.
            success = warpCommand(aPlayer, param);    
            break;
        case CommandEnum::COPY:       
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);    
            // World builder command. Creates a copy of the specified object and places it in the current area.
            success = copyCommand(aPlayer, directObj);    
            break;
        case CommandEnum::CREATE:         
            // World builder command. Starts the creation wizard for the specified object type.
            success = createCommand(aPlayer, result.directAlias);    
            break;
        case CommandEnum::EDIT_ATTRIBUTE: 
            // clarify indirect
            indirectObj = clarifyIndirect(aPlayer, result);
            // World builder command. Edits the specified attribute of the specified object (or the current area if not specified).
            success = editAttributeCommand(aPlayer, indirectObj, result.directAlias);    
            break;
        case CommandEnum::EDIT_WIZARD:    
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);
            // World builder command. Starts the edit wizard for the specified object.
            success = editWizardCommand(aPlayer, directObj);    
            break;
        case CommandEnum::SAVE:           
            // World builder command. Saves the game to the specified file (or the default file if not specified).
            success = saveCommand(aPlayer, result.indirectAlias);    
            break;
        case CommandEnum::LOAD:           
            // World builder command. Loads the game from the specified file (or the default file if not specified).
            success = loadCommand(aPlayer, result.indirectAlias);    
            break;
        case CommandEnum::DELETE:         
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);
            // World builder command. Deletes the specified object from the game.
            success = deleteCommand(aPlayer, directObj);  
            break;
        case CommandEnum::INVALID:
            std::cout << "DEBUG: executeCommand received an INVALID command.\n";
            break;
    }
    return success;
}


InteractiveNoun* GameLogic::clarifyDirect(Player *aPlayer, parser::ParseResult result){
    InteractiveNoun *directObj = nullptr;
std::cout << "inside clarifyDirect\n";

    if (result.direct.size() == 1){
        directObj = result.direct[0];
    } else if (result.direct.size() > 0){
        directObj = clarifyChoice(aPlayer, result.direct);
    }

    return directObj;
}


InteractiveNoun* GameLogic::clarifyIndirect(Player *aPlayer, parser::ParseResult result){
    InteractiveNoun *indirectObj = nullptr;
std::cout << "inside clarifyIndirect\n";

    if (result.indirect.size() == 1){
        indirectObj = result.indirect[0];
    } else if (result.indirect.size() > 0){
        indirectObj = clarifyChoice(aPlayer, result.indirect);
    }

    return indirectObj;
}


bool GameLogic::helpCommand(Player *aPlayer){
    std::string message;

    std::cout << "inside help command\n";

    if (aPlayer != nullptr){
        if (aPlayer->isEditMode()){
            message = "Available Edit Mode Commands:\015\012";
            message += "EDIT_MODE\015\012";
            message += "WARP to [an area]\015\012";
            message += "COPY [an object]\015\012";
            message += "CREATE [ObjectType]\015\012";
            message += "EDIT_ATTRIBUTE [an attribute] of [an object]\015\012";
            message += "EDIT_WIZARD [an object]\015\012";
            message += "SAVE [a file name]\015\012";
            message += "LOAD [a file name]\015\012";
            message += "DELETE [an object]\015\012";
        } else {
            message = "Some Available Commands:\015\012";
            message += "look\015\012";
            message += "listen\015\012";
            message += "take\015\012";
            message += "drop\015\012";
            message += "inventory\015\012";
            message += "equipment\015\012";
            message += "speak\015\012";
            message += "go\015\012";
            message += "stats\015\012";
            message += "skills\015\012";
            message += "equip\015\012";
            message += "unequip\015\012";
        }
        messagePlayer(aPlayer, message);
        return true;
    }
    return false;
}


std::string GameLogic::handleEffects(Player *aPlayer, std::vector<EffectType> effects){
    std::string message, allMessages;
    int health;
    int money;
    int specialPts;
    Item *anItem;

    for (auto effect : effects){
        switch (effect){
            case EffectType::NONE:
                message = "";
                break;
            case EffectType::DAMAGE:
                health = aPlayer->subtractFromCurrentHealth(5);
                message = "You take 5 damage, leaving you with ";
                message += std::to_string(health);
                message += " health.";
                break;
            case EffectType::FALL:
                health = aPlayer->subtractFromCurrentHealth(5);
                message = "You fall a short distance and loose 5 health, leaving you with ";
                message += std::to_string(health);
                message += " health.";
                break;
            case EffectType::LONG_FALL:
                health = aPlayer->subtractFromCurrentHealth(10);
                message = "You fall a long distance and loose 10 health, leaving you with ";
                message += std::to_string(health);
                message += " health.";
                break;
            case EffectType::LOST_ITEM:
                anItem = aPlayer->removeRandomFromInventory();
                message = "You trip and drop your ";
                message += anItem->getName();
                message += ".";
                break;
            case EffectType::DROP_ALL_ITEMS:
                aPlayer->removeAllFromInventory();
                message = "You trip and drop everything you were carrying.";
                break;
            case EffectType::GAIN_MONEY:
                money = aPlayer->addMoney(10);
                message = "You gain 10 coins, bringing your total money to ";
                message += std::to_string(money);
                message += " coins.";
                break;
            case EffectType::LOSE_MONEY:
                money = aPlayer->subtractMoney(10);
                message = "You lose 10 coins, bringing your total money to ";
                message += std::to_string(money);
                message += " coins.";
                break;
            case EffectType::HEAL:
                health = aPlayer->addToCurrentHealth(10);
                message = "You gain 10 health, bringing your total to ";
                message += std::to_string(health);
                message += " health.";
                break;
            case EffectType::GET_SPECIAL_POINTS:
                specialPts = aPlayer->addToCurrentSpecialPts(10);
                message = "You gain 10 special points, bringing your total to ";
                message += std::to_string(specialPts);
                message += " special points.";
                break;
            default:
                message = "";
                break;
        }
        allMessages += message;
        allMessages += " ";
    }
    return allMessages;
}


bool GameLogic::lookCommand(Player *aPlayer, InteractiveNoun *param){
    std::string message = "";
    std::vector<EffectType> effects;

    // if command is look
    if (param == nullptr){
        message = aPlayer->getLocation()->getFullDescription(aPlayer->getID());
    } else {
        // command is look at
        message = param->look(&effects);
    }
    
    message += " ";
    message += handleEffects(aPlayer, effects);

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::listenCommand(Player *aPlayer){
    std::vector<EffectType> effects;
    std::string message = aPlayer->getLocation()->listen(&effects);
    messagePlayer(aPlayer, message);

    message += " ";
    message += handleEffects(aPlayer, effects);

    return true;
}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    bool success = false;
    std::vector<EffectType> effects;
    std::string message;

std::cout << "inside GameLogic::takeCommand\n";

    if (directObj != nullptr){
        message += "You pick up the " + directObj->getName() + ". ";
        if ((indirectObj != nullptr) && (indirectObj->getObjectType() == ObjectType::CONTAINER) && ((directObj->getObjectType() == ObjectType::ITEM) || (directObj->getObjectType() == ObjectType::CONTAINER))){
            // command is of the form: take ___ from ___
            message += directObj->take(aPlayer, nullptr, indirectObj, nullptr, &effects);
        } else {
            message += directObj->take(aPlayer, nullptr, nullptr, nullptr, &effects);
std::cout << "after item::take\n";
        }
        if (message.compare("false") != 0){
            success = true;
        }
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }
    
    return success;
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
    std::string message = aPlayer->getName() + " says " + stringParam;

    messageAreaPlayers(aPlayer, message, aPlayer->getLocation());
    return true;
}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *indirectObj, const std::string &stringParam){
    return false;
}


bool GameLogic::quitCommand(Player *aPlayer){
    std::string message = "A player named " + aPlayer->getName() + " leaves the area.";
    int FD = aPlayer->getFileDescriptor();
    messagePlayer(aPlayer, "Logging you out...");
    messageAreaPlayers(aPlayer, message, aPlayer->getLocation());
    hibernatePlayer(aPlayer);
    theServer->disconnectPlayer(FD);
    return true;
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
    if (aPlayer->isEditMode()){
        aPlayer->setEditMode(false);
        messagePlayer(aPlayer, "Leaving edit mode...");
    } else {
        aPlayer->setEditMode(true);
        messagePlayer(aPlayer, "Entering edit mode...");
    }

    return true;
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