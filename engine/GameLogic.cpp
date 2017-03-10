/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    03/08/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include <cstdlib>
#include <thread>
#include <chrono>
#include <iostream>
#include <limits>
#include <cctype>
#include <algorithm>
#include <unistd.h>
#include <atomic>
#undef HUGE
#include <parser.hpp>
#include <DataManager.hpp>
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
#include "Exit.hpp"
#include "WeaponType.hpp"
#include "Quest.hpp"
#include "QuestStep.hpp"
#include "SpecialSkill.hpp"
#include "ItemType.hpp"
#include "CreatureType.hpp"
#include "ArmorType.hpp"
#include "Feature.hpp"
#include "EnumToString.hpp"
#include "Action.hpp"

namespace legacymud { namespace engine {

const std::string ADMIN_PASSWORD = "default";
const int SAVE_TIMEOUT = 60;
std::atomic<bool> saving;

bool waitForSaveOrTimeout() {
    int counter = 0;
    while (saving.load()) {
        ::sleep(1);
        if (counter++ > SAVE_TIMEOUT) {
            return false;
        }
    }
    return true;
}


GameLogic::GameLogic() 
: accountManager(nullptr)
, theServer(nullptr)
{
    saving.store(false);
    manager = new GameObjectManager;
    startArea = nullptr;
}


GameLogic::GameLogic(const GameLogic &otherGameLogic)
: accountManager(nullptr)
, theServer(nullptr)
{
    saving.store(false);
    manager = new GameObjectManager(*otherGameLogic.manager);
    startArea = nullptr;
}


GameLogic & GameLogic::operator=(const GameLogic &otherGameLogic){
    if (this != &otherGameLogic){
        delete manager;
        manager = new GameObjectManager(*otherGameLogic.manager);
    }

    return *this;
}


GameLogic::~GameLogic(){
    saving.store(false);
    std::vector<Player*> players = manager->getPlayersPtrs();

    for (auto player : players){
        removePlayerMessageQueue(player);
    }

    delete manager;
}


bool GameLogic::startGame(bool newGame, const std::string &fileName, telnet::Server *aServer, account::Account *anAccount){
    accountManager = anAccount;
    theServer = aServer;
    gamedata::DataManager dm;

    // initialize saving flag
    saving.store(false);

    bool success = false;
    int startAreaId = -1;

    // try to load file if one is specified
    if (!fileName.empty()) {
        currentFilename = fileName;
        std::cout << "Loading " << fileName << std::endl;
        success = dm.loadGame(fileName, manager, startAreaId);
        if (success) {
            std::cout << "Successfully loaded " << fileName << std::endl;

            // Set starting area
            startArea = static_cast<Area*>(manager->getPointer(startAreaId));
        }
        else {
            std::cerr << "Failed to load " << fileName << std::endl;
        }
    }

    if (!success) {
        std::cout << "Starting new game" << std::endl;

        // Create the objects                                                                     
        // Starting Area (name, short desciption, long description, area size)    
        startArea = new legacymud::engine::Area("default area name", "short description of area", "longer description of area", 
                                                                    legacymud::engine::AreaSize::LARGE);   
                                                                    
        // SpecialSkill(name, damage, damageType, cost, cooldown);
        legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("default special skill name", 20, 
                                                                                    legacymud::engine::DamageType::PIERCING, 
                                                                                    10,2);

        // PlayerClass (primaryStat, name, special skill, attackBonus, armorBonus, resistantTo, weakTo, float healPoints);
        legacymud::engine::PlayerClass* playerClass = new legacymud::engine::PlayerClass(45, "playerClass name", skill, 10, 20, 
                                                                                    legacymud::engine::DamageType::FIRE,
                                                                                    legacymud::engine::DamageType::WATER,
                                                                                    35.5); 

        // put the objects in the GameObjectManager
        manager->addObject(startArea, -1); 
        manager->addObject(skill, -1);
        manager->addObject(playerClass, -1);

        success = dm.saveGame(fileName, manager, startArea->getID());
        // save account data file as well
        if (success) {
            accountManager->setFileName(fileName + ".accounts");
            accountManager->saveToDisk();
        }
    }
    return success;
}


bool GameLogic::newPlayerHandler(int fileDescriptor){
    std::cout << "inside new player handler\n";
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
            newPlayer = new Player(static_cast<CharacterSize>(playerSize - 1), pClasses[pClassNumber - 1], username, fileDescriptor, playerName, pDescription, startArea);
            
            // Wait until not saving before adding object
            if (!waitForSaveOrTimeout()) {
                messagePlayer(newPlayer, "Timed out while waiting for game to save.");
                return false;
            }
            manager->addObject(newPlayer, fileDescriptor);

            // create account
            success = accountManager->createAccount(username, password, isAdmin, newPlayer->getID());
            if (!success){
                // username got taken in the interim DO SOMETHING
            }

            // move player to start area
            startArea->addCharacter(newPlayer);
            newPlayer->setLocation(startArea);
            messagePlayer(newPlayer, startArea->getFullDescription(newPlayer));
            message = "You see a player named " + playerName + " enter the area.";
            messageAreaPlayers(newPlayer, message, startArea);

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
                    std::cout << "before loading player into game\n";
                    // load player into game
                    manager->loadPlayer(username, fileDescriptor);
                    std::cout << "after loading player\n";
                    aPlayer = manager->getPlayerByFD(fileDescriptor);
                    aPlayer->activate(fileDescriptor);
                    std::cout << "after activate\n";

                    // move player to current location
                    anArea = aPlayer->getLocation();
                    std::cout << "after get location\n";
                    anArea->addCharacter(aPlayer);
                    std::cout << "after addCharacter\n";
                    messagePlayer(aPlayer, anArea->getFullDescription(aPlayer));
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
    std::cout << "inside getValueFromUser\n";
    std::cout << "file descriptor = " + std::to_string(FD) + "\n";
    std::cout << "message = " + outMessage + "\n";
    bool success;

    theServer->sendMsg(FD, outMessage);
    std::cout << "after send message\n";
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

// check cooldown, move creatures, attack players (randomly), update health and special points
bool GameLogic::updateCreatures(){
    bool ready = false;
    bool inCombat = false;
    std::vector<Creature*> allCreatures = manager->getCreatures();
    Area *location = nullptr;
    std::vector<Character*> characters;
    std::vector<Character*> players;
    int spotCheck = 0;
    int hideCheck = 0;
    size_t index;

    for (auto creature : allCreatures){
        // check cooldown
        ready = creature->cooldownIsZero();
        if (ready){
            // check to see if already in combat
            if (creature->getInCombat() != nullptr){
                inCombat = true;
            } else {
                inCombat = false;
            }

            if (!inCombat){
                // check if there are players in this location
                location = creature->getLocation();
                characters = location->getCharacters();
                for (auto character : characters){
                    if (character->getObjectType() == ObjectType::PLAYER){
                        players.push_back(character);
                    }
                }

                if (players.size() != 0){
                    index = 0;
                    while ((!inCombat) && (index < players.size())){
                        // if the player isn't already in combat
                        if (players[index]->getInCombat() == nullptr){
                            // creature rolls spot check and player rolls hide check
                            spotCheck = rollDice(20, 1);
                        }
                    }

                    // start combat

                    // roll to see which attack is used

                    // attack
                } else {
                    // roll to see if leaves the area

                    // roll to see which exit to take

                    // move creature
                }
            } else {
                // roll to see which attack is used

                // attack
            }
        }
    }
    

    return false;
}


bool GameLogic::updatePlayersInCombat(){
    // check cooldown, check command queue, otherwise default attack, update health and special points
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
    bool success = false;

    switch (type){
        case ObjectType::NONE:
            break;
        case ObjectType::AREA:
            success = createArea(aPlayer);
            break;
        case ObjectType::ARMOR_TYPE:
            success = createArmorType(aPlayer);
            break;
        case ObjectType::CONTAINER:
            success = createContainer(aPlayer);
            break;
        case ObjectType::CREATURE:
            success = createCreature(aPlayer);
            break;
        case ObjectType::CREATURE_TYPE:
            success = createCreatureType(aPlayer);
            break;
        case ObjectType::EXIT:
            success = createExit(aPlayer);
            break;
        case ObjectType::FEATURE:
            success = createFeature(aPlayer);
            break;
        case ObjectType::ITEM:
            success = createItem(aPlayer);
            break;
        case ObjectType::ITEM_TYPE:
            success = createItemType(aPlayer);
            break;
        case ObjectType::NON_COMBATANT:
            success = createNonCombatant(aPlayer);
            break;
        case ObjectType::PLAYER:
            success = createPlayer(aPlayer);
            break;
        case ObjectType::PLAYER_CLASS:
            success = createPlayerClass(aPlayer);
            break;
        case ObjectType::QUEST:
            success = createQuest(aPlayer);
            break;
        case ObjectType::QUEST_STEP:
            success = createQuestStep(aPlayer);
            break;
        case ObjectType::SPECIAL_SKILL:
            success = createSpecialSkill(aPlayer);
            break;
        case ObjectType::WEAPON_TYPE:
            success = createWeaponType(aPlayer);
            break;
    }

    return success;
}


bool GameLogic::createArea(Player *aPlayer){
    Area *newArea = nullptr;
    std::string name = "";
    std::string shortDescription = "";
    std::string longDescription = "";
    AreaSize size;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Area Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    shortDescription = getStringParameter(aPlayer, "short description");
    longDescription = getStringParameter(aPlayer, "long description");
    size = getAreaSizeParameter(aPlayer, "size");

    removePlayerMessageQueue(aPlayer);

    newArea = new Area(name, shortDescription, longDescription, size);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newArea, -1);

    messagePlayer(aPlayer, "You have created a new area. The ID of the new area is " + std::to_string(newArea->getID()) + ".");

    return true;
}


bool GameLogic::createArmorType(Player *aPlayer){
    ArmorType *newArmorType = nullptr;
    int bonus;
    DamageType resistantTo;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Armor Type Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    bonus = getIntParameter(aPlayer, "armor bonus");
    resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
    weight = getIntParameter(aPlayer, "weight");
    rarity = getItemRarityParameter(aPlayer, "rarity");
    cost = getIntParameter(aPlayer, "cost");
    slotType = getEquimentSlotParameter(aPlayer, "slot type");

    removePlayerMessageQueue(aPlayer);

    newArmorType = new ArmorType(bonus, resistantTo, weight, rarity, description, name, cost, slotType);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newArmorType, -1);

    messagePlayer(aPlayer, "You have created a new armor type. The ID of the new armor type is " + std::to_string(newArmorType->getID()) + ".");

    return true;
}


// would be better without dynamic casts 
bool GameLogic::createContainer(Player *aPlayer){
    Container *newContainer = nullptr;
    ObjectType anObjectType = ObjectType::NONE;
    Area *anArea = nullptr;
    Character *aCharacter = nullptr;
    Container *aContainer = nullptr;
    int capacity;
    InteractiveNoun *location = nullptr;
    ItemPosition position;
    std::string name;
    ItemType *type = nullptr;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Container Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    type = getItemTypeParameter(aPlayer, "type");
    capacity = getIntParameter(aPlayer, "capacity");
    location = getInteractiveNounParameter(aPlayer, "location");
    
    if (location != nullptr){
        // make sure that the given combo of location and position make sense
        anObjectType = location->getObjectType();
        if (anObjectType == ObjectType::AREA){
            position = ItemPosition::GROUND;  
        } else if (anObjectType == ObjectType::CONTAINER){
            // position should be IN, ON, or UNDER
            position = getItemPositionParameter(aPlayer, "position", 0);
        } else {
            // position should be either INVENTORY or EQUIPPED
            position = getItemPositionParameter(aPlayer, "position", 1);
        }
    } else {
        position = ItemPosition::NONE;
    }

    removePlayerMessageQueue(aPlayer);

    newContainer = new Container(capacity, location, position, name, type);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newContainer, -1);
    if (location != nullptr){
        // add to location
        if (anObjectType == ObjectType::AREA){
            anArea = dynamic_cast<Area*>(location);
            if (anArea != nullptr){
                anArea->addItem(newContainer);
                messageAreaPlayers(nullptr, "A new " + name + " appears on the ground.", anArea);
            }
        } else if (anObjectType == ObjectType::CONTAINER){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                aContainer->place(newContainer, position);
            }
        } else {
            aCharacter = dynamic_cast<Character*>(location);
            if (aCharacter != nullptr){
                aCharacter->addToInventory(newContainer);
                if (position == ItemPosition::EQUIPPED){
                    aCharacter->equipItem(newContainer);
                }
            }
        }
    }
    
    messagePlayer(aPlayer, "You have created a new container. The ID of the new container is " + std::to_string(newContainer->getID()) + ".");

    return true;
}


bool GameLogic::createCreature(Player *aPlayer){
    Creature *newCreature = nullptr;
    CreatureType *aType = nullptr;
    bool ambulatory;
    int maxHealth;
    Area *spawnLocation = nullptr;
    int maxSpecialPts;
    std::string name;
    std::string description;
    int money;
    Area *aLocation = nullptr;
    int maxInventoryWeight;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Creature Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    aType = getCreatureTypeParameter(aPlayer, "creature type");
    ambulatory = getBoolParameter(aPlayer, "ambulatory");
    aLocation = getAreaParameter(aPlayer, "current location");
    spawnLocation = getAreaParameter(aPlayer, "spawn location");
    maxHealth = getIntParameter(aPlayer, "maximum health");
    maxSpecialPts = getIntParameter(aPlayer, "maximum special points");
    money = getIntParameter(aPlayer, "money");
    maxInventoryWeight = getIntParameter(aPlayer, "maximum inventory weight");

    removePlayerMessageQueue(aPlayer);

    newCreature = new Creature(aType, ambulatory, maxHealth, spawnLocation, maxSpecialPts, name, description, money, aLocation, maxInventoryWeight);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newCreature, -1);
    aLocation->addCharacter(newCreature);
    messageAreaPlayers(nullptr, "A creature named " + name + " appears out of nowhere.", aLocation);

    messagePlayer(aPlayer, "You have created a new creature. The ID of the new creature is " + std::to_string(newCreature->getID()) + ".");

    return true;
}


bool GameLogic::createCreatureType(Player *aPlayer){
    CreatureType *newCreatureType = nullptr;
    CharacterSize size;
    XPTier difficulty;
    std::string name;
    SpecialSkill *skill = nullptr;
    int attackBonus;
    int armorBonus;
    DamageType resistantTo;
    DamageType weakTo;
    float healPoints;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Creature Type Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    size = getCharacterSizeParameter(aPlayer, "size");
    difficulty = getXPTierParameter(aPlayer, "difficulty");
    skill = getSpecialSkillParameter(aPlayer, "special skill");
    attackBonus = getIntParameter(aPlayer, "attack bonus");
    armorBonus = getIntParameter(aPlayer, "armor bonus");
    resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
    weakTo = getDamageTypeParameter(aPlayer, "weakness type");
    healPoints = getFloatParameter(aPlayer, "healing point rate");

    removePlayerMessageQueue(aPlayer);

    newCreatureType = new CreatureType(size, difficulty, name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newCreatureType, -1);

    messagePlayer(aPlayer, "You have created a new creature type. The ID of the new creature type is " + std::to_string(newCreatureType->getID()) + ".");

    return true;
}


bool GameLogic::createExit(Player *aPlayer){
    Exit *newExit = nullptr;
    ExitDirection direction;
    Area *location = nullptr;
    Area *connectArea = nullptr;
    bool isConditional;
    ItemType *anItemType = nullptr;
    std::string description;
    std::string altDescription = "";

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Exit Creation Wizard...");

    direction = getExitDirectionParameter(aPlayer, "direction");
    description = getStringParameter(aPlayer, "description");
    location = getAreaParameter(aPlayer, "location");
    connectArea = getAreaParameter(aPlayer, "connecting area");
    isConditional = getBoolParameter(aPlayer, "conditional");
    // if conditional is false, then don't need conditional item type or alt description
    if (isConditional){
        anItemType = getItemTypeParameter(aPlayer, "conditional item type");
        altDescription = getStringParameter(aPlayer, "alternate description");
    }

    removePlayerMessageQueue(aPlayer);

    newExit = new Exit(direction, location, connectArea, isConditional, anItemType, description, altDescription);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newExit, -1);
    location->addExit(newExit);
    messageAreaPlayers(nullptr, newExit->getDirectionString() + " you see " + description + " appear out of nowhere.", location);
    
    messagePlayer(aPlayer, "You have created a new exit. The ID of the new exit is " + std::to_string(newExit->getID()) + ".");

    return true;
}


bool GameLogic::createFeature(Player *aPlayer){
    Feature *newFeature = nullptr;
    std::string name;
    std::string placement;
    Area *location = nullptr;
    bool isConditional;
    ItemType *anItemType = nullptr;
    std::string description;
    std::string altDescription;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Feature Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    placement = getStringParameter(aPlayer, "placement");
    location = getAreaParameter(aPlayer, "location");
    isConditional = getBoolParameter(aPlayer, "conditional");
    // if conditional is false, then don't need conditional item type or alt description
    if (isConditional){
        anItemType = getItemTypeParameter(aPlayer, "conditional item type");
        altDescription = getStringParameter(aPlayer, "alternate description");
    }

    removePlayerMessageQueue(aPlayer);

    newFeature = new Feature(name, placement, location, isConditional, anItemType, description, altDescription);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newFeature, -1);
    location->addFeature(newFeature);
    messageAreaPlayers(nullptr, "You see a " + name + " appear out of nowhere " + placement + ".", location);

    messagePlayer(aPlayer, "You have created a new feature. The ID of the new feature is " + std::to_string(newFeature->getID()) + ".");

    return true;

}


bool GameLogic::createItem(Player *aPlayer){
    ObjectType anObjectType = ObjectType::NONE;
    Area *anArea = nullptr;
    Character *aCharacter = nullptr;
    Container *aContainer = nullptr;
    Item *newItem = nullptr;
    InteractiveNoun* location = nullptr;
    ItemPosition position;
    std::string name;
    ItemType *type = nullptr;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Item Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    type = getItemTypeParameter(aPlayer, "type");
    location = getInteractiveNounParameter(aPlayer, "location");
    
    if (location != nullptr){
        // make sure that the given combo of location and position make sense
        anObjectType = location->getObjectType();
        if (anObjectType == ObjectType::AREA){
            position = ItemPosition::GROUND;  
        } else if (anObjectType == ObjectType::CONTAINER){
            // position should be IN, ON, or UNDER
            position = getItemPositionParameter(aPlayer, "position", 0);
        } else {
            // position should be either INVENTORY or EQUIPPED
            position = getItemPositionParameter(aPlayer, "position", 1);
        }
    } else {
        position = ItemPosition::NONE;
    }

    removePlayerMessageQueue(aPlayer);

    newItem = new Item(location, position, name, type);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newItem, -1);
    if (location != nullptr){
        // add to location
        if (anObjectType == ObjectType::AREA){
            anArea = dynamic_cast<Area*>(location);
            if (anArea != nullptr){
                anArea->addItem(newItem);
                messageAreaPlayers(nullptr, "A new " + name + " appears on the ground.", anArea);
            }
        } else if (anObjectType == ObjectType::CONTAINER){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                aContainer->place(newItem, position);
            }
        } else {
            aCharacter = dynamic_cast<Character*>(location);
            if (aCharacter != nullptr){
                aCharacter->addToInventory(newItem);
                if (position == ItemPosition::EQUIPPED){
                    aCharacter->equipItem(newItem);
                }
            }
        }
    }

    messagePlayer(aPlayer, "You have created a new item. The ID of the new item is " + std::to_string(newItem->getID()) + ".");

    return true;
}


bool GameLogic::createItemType(Player *aPlayer){
    ItemType *newItemType = nullptr;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Item Type Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    weight = getIntParameter(aPlayer, "weight");
    rarity = getItemRarityParameter(aPlayer, "rarity");
    cost = getIntParameter(aPlayer, "cost");
    slotType = getEquimentSlotParameter(aPlayer, "slot type");

    removePlayerMessageQueue(aPlayer);

    newItemType = new ItemType(weight, rarity, description, name, cost, slotType);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newItemType, -1);

    messagePlayer(aPlayer, "You have created a new item type. The ID of the new item type is " + std::to_string(newItemType->getID()) + ".");

    return true;
}


bool GameLogic::createNonCombatant(Player *aPlayer){
    NonCombatant *newNonCombatant = nullptr;
    Quest *aQuest = nullptr;
    std::string name;
    std::string description;
    int money;
    Area *aLocation = nullptr;
    int maxInventoryWeight;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting NonCombatant Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    money = getIntParameter(aPlayer, "money");
    aLocation = getAreaParameter(aPlayer, "location");
    maxInventoryWeight = getIntParameter(aPlayer, "maximum inventory weight");
    aQuest = getQuestParameter(aPlayer, "quest", true);

    removePlayerMessageQueue(aPlayer);

    newNonCombatant = new NonCombatant(aQuest, name, description, money, aLocation, maxInventoryWeight);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newNonCombatant, -1);
    aLocation->addCharacter(newNonCombatant);
    messageAreaPlayers(nullptr, "A person named " + name + " appears out of nowhere.", aLocation);

    messagePlayer(aPlayer, "You have created a new non-combatant. The ID of the new non-combatant is " + std::to_string(newNonCombatant->getID()) + ".");

    return true;
}


bool GameLogic::createPlayer(Player *aPlayer){
    messagePlayer(aPlayer, "You can't create a new player this way. Use the login process to create new user/player pairs.");

    return false;
}


bool GameLogic::createPlayerClass(Player *aPlayer){
    PlayerClass *newPlayerClass = nullptr;
    int primaryStat;
    std::string name;
    SpecialSkill* skill = nullptr;
    int attackBonus;
    int armorBonus;
    DamageType resistantTo;
    DamageType weakTo;
    float healPoints;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Player Class Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    primaryStat = getIntParameter(aPlayer, "primary stat (enter [0] for dexterity, [1] for intelligence, or [2] for strength)", 2);
    skill = getSpecialSkillParameter(aPlayer, "special skill");
    attackBonus = getIntParameter(aPlayer, "attack bonus");
    armorBonus = getIntParameter(aPlayer, "armor bonus");
    resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
    weakTo = getDamageTypeParameter(aPlayer, "weakness type");
    healPoints = getFloatParameter(aPlayer, "healing point rate");

    removePlayerMessageQueue(aPlayer);

    newPlayerClass = new PlayerClass(primaryStat, name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newPlayerClass, -1);

    messagePlayer(aPlayer, "You have created a new player class. The ID of the new player class is " + std::to_string(newPlayerClass->getID()) + ".");

    return true;
}


bool GameLogic::createQuest(Player *aPlayer){
    Quest *newQuest = nullptr;
    std::string name;
    std::string description;
    int rewardMoney;
    Item *rewardItem = nullptr;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Quest Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    rewardMoney = getIntParameter(aPlayer, "reward money");
    rewardItem = getItemParameter(aPlayer, "reward item", true); // may want to change this so they can't choose ANY item ANYWHERE in the game

    removePlayerMessageQueue(aPlayer);

    newQuest = new Quest(name, description, rewardMoney, rewardItem);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newQuest, -1);

    messagePlayer(aPlayer, "You have created a new quest. The ID of the new quest is " + std::to_string(newQuest->getID()) + ".");

    return true;
}


bool GameLogic::createQuestStep(Player *aPlayer){
    QuestStep *newQuestStep = nullptr;
    int ordinalNumber;
    std::string description;
    ItemType *anItemType = nullptr;
    NonCombatant *giver = nullptr;
    NonCombatant *receiver = nullptr;
    std::string completionText;
    Quest *aQuest = nullptr;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Quest Step Creation Wizard...");

    aQuest = getQuestParameter(aPlayer, "quest", false);
    ordinalNumber = getIntParameter(aPlayer, "ordinal number");
    description = getStringParameter(aPlayer, "description");
    anItemType = getItemTypeParameter(aPlayer, "fetch item type");
    giver = getNonCombatantParameter(aPlayer, "giver", aQuest);
    giver->setQuest(aQuest);
    receiver = getNonCombatantParameter(aPlayer, "receiver", aQuest);
    receiver->setQuest(aQuest);
    completionText = getStringParameter(aPlayer, "completion text");

    removePlayerMessageQueue(aPlayer);

    newQuestStep = new QuestStep(ordinalNumber, description, anItemType, giver, receiver, completionText);
    aQuest->addStep(newQuestStep);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newQuestStep, -1);

    messagePlayer(aPlayer, "You have created a new quest step. The ID of the new quest step is " + std::to_string(newQuestStep->getID()) + ".");

    return true;
}


bool GameLogic::createSpecialSkill(Player *aPlayer){
    SpecialSkill *newSpecialSkill = nullptr;
    std::string name;
    int damage;
    DamageType type;
    int cost;
    time_t cooldown;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Special Skill Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    damage = getIntParameter(aPlayer, "damage");
    type = getDamageTypeParameter(aPlayer, "damage type");
    cost = getIntParameter(aPlayer, "cost");
    cooldown = getIntParameter(aPlayer, "cooldown (in seconds)");

    removePlayerMessageQueue(aPlayer);

    newSpecialSkill = new SpecialSkill(name, damage, type, cost, cooldown);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newSpecialSkill, -1);

    messagePlayer(aPlayer, "You have created a new special skill. The ID of the new special skill is " + std::to_string(newSpecialSkill->getID()) + ".");

    return true;
}


bool GameLogic::createWeaponType(Player *aPlayer){
    WeaponType *newWeaponType = nullptr;
    int damage;
    DamageType type;
    AreaSize range;
    int critMultiplier;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;

    addPlayerMessageQueue(aPlayer);
    messagePlayer(aPlayer, "\015\012Starting Weapon Type Creation Wizard...");

    name = getStringParameter(aPlayer, "name");
    description = getStringParameter(aPlayer, "description");
    damage = getIntParameter(aPlayer, "damage");
    type = getDamageTypeParameter(aPlayer, "damage type");
    range = getAreaSizeParameter(aPlayer, "range");
    critMultiplier = getIntParameter(aPlayer, "crit multiplier");
    weight = getIntParameter(aPlayer, "weight");
    rarity = getItemRarityParameter(aPlayer, "rarity");
    cost = getIntParameter(aPlayer, "cost");
    slotType = getEquimentSlotParameter(aPlayer, "slot type");

    removePlayerMessageQueue(aPlayer);

    newWeaponType = new WeaponType(damage, type, range, critMultiplier, weight, rarity, description, name, cost, slotType);

    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }
    manager->addObject(newWeaponType, -1);

    messagePlayer(aPlayer, "You have created a new weapon type. The ID of the new weapon type is " + std::to_string(newWeaponType->getID()) + ".");

    return true;
}


std::string GameLogic::editActionAttribute(Player *aPlayer, InteractiveNoun *objectToEdit){
    std::string message = "";
    std::string response;
    int choice = -1;
    int actionChoice = -1;
    int attributeChoice = -1;
    std::vector<Action*> allActions = objectToEdit->getAllActions();
    Action *anAction = nullptr;
    bool valid;
    std::string flavorText;
    EffectType anEffect;
    CommandEnum aCommand;

    messagePlayer(aPlayer, "What would you like to do? Your options are: [1] edit action, [2] add action, or [3] remove action. Please enter the number that corresponds to your choice.");
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 3);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 3);
    }

    switch (choice){
        case 1:
            // edit 
            messagePlayer(aPlayer, "Starting Edit Action Wizard...");
            actionChoice = getPointerParameter<Action*>(aPlayer, "", allActions, true, "Which action would you like to edit?");
            if (actionChoice == 0){
                messagePlayer(aPlayer, "Aborting Edit Action Wizard...");
            } else if (actionChoice != -1){
                anAction = allActions[actionChoice];
                messagePlayer(aPlayer, anAction->getToString());
                messagePlayer(aPlayer, "Which action attribute would you like to edit? Your options are: [1] valid, [2] flavor text, or [3] effect. Please enter the number that corresponds to your choice.");
                response = blockingGetMsg(aPlayer);
                attributeChoice = validateStringNumber(response, 1, 3);
                while (attributeChoice == -1){
                    messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
                    response = blockingGetMsg(aPlayer);
                    attributeChoice = validateStringNumber(response, 1, 3);
                }
                switch (attributeChoice){
                    case 1:
                        // edit valid
                        valid = getBoolParameter(aPlayer, "valid");
                        anAction->setValid(valid);
                        message = "Valid is now ";
                        if (valid){
                            message += "true.";
                        } else {
                            message += "false.";
                        }
                        break;
                    case 2:
                        // edit flavor text
                        flavorText = getStringParameter(aPlayer, "flavor text");
                        anAction->setFlavorText(flavorText);
                        message = "The flavor text is now " + flavorText + ".";
                        break;
                    case 3:
                        // edit effect
                        anEffect = getEffectTypeParameter(aPlayer, "effect");
                        anAction->setEffect(anEffect);
                        message = "The effect is now " + effectTypeToString(anEffect) + "."; 
                        break;
                    default:
                        break;
                }
            }
            break;
        case 2:
            // add
            messagePlayer(aPlayer, "Starting Action Creation Wizard...");
            aCommand = getCommandEnumParameter(aPlayer, "command");
            valid = getBoolParameter(aPlayer, "valid");
            flavorText = getStringParameter(aPlayer, "flavor text");
            anEffect = getEffectTypeParameter(aPlayer, "effect");
            anAction = objectToEdit->addAction(aCommand, valid, flavorText, anEffect);
            if ((anAction->getValid() == valid) && (anAction->getFlavorText().compare(flavorText) == 0) && (anAction->getEffect() == anEffect)){
                message = "The action has been successfully created.";
            } else {
                messagePlayer(aPlayer, "There is already an action with that command. \015\012Aborting Action Creation Wizard...");
            }
            break;
        case 3:
            // remove
            messagePlayer(aPlayer, "Starting Action Removal Wizard...");
            actionChoice = getPointerParameter<Action*>(aPlayer, "", allActions, true, "Which action would you like to remove?");
            if (actionChoice == 0){
                messagePlayer(aPlayer, "Aborting Action Removal Wizard...");
            } else if (actionChoice != -1){
                anAction = allActions[actionChoice];
                objectToEdit->removeAction(anAction->getCommand());
                message = "The action has been successfully removed.";
            }
            break;
        default:
            break;
    }

    return message;
}


std::string GameLogic::editAliasAttribute(Player *aPlayer, InteractiveNoun *objectToEdit){
    std::string message = "";
    std::string response;
    int choice1 = -1;
    int choice2 = -1;
    int choice = -1;
    bool success;
    std::string alias;
    CommandEnum aCommand;
    parser::Grammar::Support directSupport, indirectSupport;
    std::map<std::string, parser::PrepositionType> prepositions;
    bool complete = false;
    std::string prepAlias;
    parser::PrepositionType prepType;

    messagePlayer(aPlayer, "What kind of alias would you like to edit? Your options are: [1] noun alias or [2] verb alias. Please enter the number that corresponds to your choice.");
    response = blockingGetMsg(aPlayer);
    choice1 = validateStringNumber(response, 1, 2);
    while (choice1 == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice1 = validateStringNumber(response, 1, 2);
    }

    messagePlayer(aPlayer, "What would you like to do to the alias? Your options are: [1] add alias or [2] remove alias. Please enter the number that corresponds to your choice.");
    response = blockingGetMsg(aPlayer);
    choice2 = validateStringNumber(response, 1, 2);
    while (choice2 == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice2 = validateStringNumber(response, 1, 2);
    }

    if ((choice1 == 1) && (choice2 == 1)){
        //add noun alias
        alias = getStringParameter(aPlayer, "noun alias to add");
        success = objectToEdit->addNounAlias(alias);
        if (success){
            message = "The noun alias was successfully added.";
        } else {
            messagePlayer(aPlayer, "The noun alias could not be added.");
        }
    } else if ((choice1 == 1) && (choice2 == 2)){
        // remove noun alias
        alias = getStringParameter(aPlayer, "noun alias to remove");
        success = objectToEdit->removeNounAlias(alias);
        if (success){
            message = "The noun alias was successfully removed.";
        } else {
            messagePlayer(aPlayer, "The noun alias could not be removed.");
        }
    } else if ((choice1 == 2) && (choice2 == 1)){
        // add verb alias
        alias = getStringParameter(aPlayer, "verb alias to add");
        aCommand = getCommandEnumParameter(aPlayer, "command to alias");
        directSupport = getSupportParameter(aPlayer, "direct object support");
        indirectSupport = getSupportParameter(aPlayer, "indirect object support");
        while (!complete){
            messagePlayer(aPlayer, "Would you like to add a preposition? Your options are: [1] yes or [2] no. Please enter the number that corresponds to your choice.");
            response = blockingGetMsg(aPlayer);
            choice = validateStringNumber(response, 1, 2);
            while (choice == -1){
                messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
                response = blockingGetMsg(aPlayer);
                choice = validateStringNumber(response, 1, 2);
            }
            if (choice == 1){
                prepAlias = getStringParameter(aPlayer, "preposition");
                prepType = getPrepositionTypeParameter(aPlayer, "preposition type");
                prepositions[prepAlias] = prepType;
            } else {
                complete = true;
            }
        }
        success = objectToEdit->addVerbAlias(aCommand, alias, directSupport, indirectSupport, prepositions);
        if (success){
            message = "The verb alias was successfully added.";
        } else {
            messagePlayer(aPlayer, "The verb alias could not be added.");
        }
    } else {
        //remove verb alias
        alias = getStringParameter(aPlayer, "verb alias to remove");
        aCommand = getCommandEnumParameter(aPlayer, "command that was aliased");
        success = objectToEdit->removeVerbAlias(aCommand, alias);
        if (success){
            message = "The verb alias was successfully removed.";
        } else {
            messagePlayer(aPlayer, "The verb alias could not be removed.");
        }
    }

    return message;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfArea(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    std::string name = "";
    std::string shortDescription = "";
    std::string longDescription = "";
    AreaSize size;
    Area *anArea = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::AREA)){
        anArea = dynamic_cast<Area*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Area Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            anArea->setName(name);
            message = "The name of the area is now " + name + ".";
            success = true;
        } else if (attribute.compare("short description") == 0){
            shortDescription = getStringParameter(aPlayer, "short description");
            anArea->setShortDesc(shortDescription);
            message = "The short description of the area is now " + shortDescription + ".";
            success = true;
        } else if (attribute.compare("long description") == 0){
            longDescription = getStringParameter(aPlayer, "long description");
            anArea->setLongDesc(longDescription);
            message = "The long description of the area is now " + longDescription + ".";
            success = true;
        } else if (attribute.compare("size") == 0){
            size = getAreaSizeParameter(aPlayer, "size");
            anArea->setSize(size);
            message = "The size of the area is now " + areaSizeToString(size) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfArmorType(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int bonus;
    DamageType resistantTo;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;
    ArmorType *anArmorType = nullptr; 

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::ARMOR_TYPE)){
        anArmorType = dynamic_cast<ArmorType*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Armor Type Attribute Editing Wizard...");

        if (attribute.compare("armor bonus") == 0){
            bonus = getIntParameter(aPlayer, "armor bonus");
            anArmorType->setArmorBonus(bonus);
            message = "The armor bonus of the armor type is now " + std::to_string(bonus) + ".";
            success = true;
        } else if (attribute.compare("resistance type") == 0){
            resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
            anArmorType->setResistantTo(resistantTo);
            message = "The resistance type of the armor type is now " + damageTypeToString(resistantTo) + ".";
            success = true;
        } else if (attribute.compare("weight") == 0){
            weight = getIntParameter(aPlayer, "weight");
            anArmorType->setWeight(weight);
            message = "The weight of the armor type is now " + std::to_string(weight) + ".";
            success = true;
        } else if (attribute.compare("rarity") == 0){
            rarity = getItemRarityParameter(aPlayer, "rarity");
            anArmorType->setRarity(rarity);
            message = "The rarity of the armor type is now " + itemRarityToString(rarity) + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            anArmorType->setDescription(description);
            message = "The description of the armor type is now " + description + ".";
            success = true;
        } else if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            anArmorType->setName(name);
            message = "The name of the armor type is now " + name + ".";
            success = true;
        } else if (attribute.compare("cost") == 0){
            cost = getIntParameter(aPlayer, "cost");
            anArmorType->setCost(cost);
            message = "The cost of the armor type is now " + std::to_string(cost) + ".";
            success = true;
        } else if (attribute.compare("slot type") == 0){
            slotType = getEquimentSlotParameter(aPlayer, "slot type");
            anArmorType->setSlotType(slotType);
            message = "The slot type of the armor type is now " + equipmentSlotToString(slotType) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfContainer(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int capacity;
    //InteractiveNoun *location = nullptr;
    //ItemPosition position;
    std::string name;
    ItemType *type = nullptr;
    Container *aContainer = nullptr;
    //ObjectType anObjectType;
    //InteractiveNoun *oldLocation = nullptr;


    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::CONTAINER)){
        aContainer = dynamic_cast<Container*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting  Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aContainer->setName(name);
            message = "The name of the container is now " + name + ".";
            success = true;
        } else if (attribute.compare("type") == 0){
            type = getItemTypeParameter(aPlayer, "type");
            aContainer->setType(type);
            message = "The type of the container is now " + type->getName() + ".";
            success = true;
        } else if (attribute.compare("capacity") == 0){
            capacity = getIntParameter(aPlayer, "capacity");
            aContainer->setInsideCapacity(capacity);
            message = "The capacity of the container is now " + std::to_string(capacity) + ".";
            success = true;
        } else if (attribute.compare("location") == 0){
            // disallow editing location for the time being
            message = "You can't edit the location of the container.";
            /*location = getInteractiveNounParameter(aPlayer, "location");
            oldLocation = aContainer->getLocation();
            // remove from old location and add to new location
            aContainer->setLocation(location);
            message = "The location of the container is now " + location->getName() + ".";
            success = true;*/
        } else if (attribute.compare("position") == 0){
            // disallow editing position for the time being
            message = "You can't edit the position of the container.";
            /*// make sure that the given combo of location and position make sense
            anObjectType = aContainer->getLocation()->getObjectType();
            if (anObjectType == ObjectType::AREA){
                // position can only be ground (can't be changed)
                message = "You can't change the position of this container before changing its location.";
            } else if (anObjectType == ObjectType::CONTAINER){
                // position should be IN, ON, or UNDER
                position = getItemPositionParameter(aPlayer, "position", 0);
                aContainer->setPosition(position);
                message = "The position of the container is now " + itemPositionToString(position) + ".";
                success = true;
            } else {
                // position should be either INVENTORY or EQUIPPED
                position = getItemPositionParameter(aPlayer, "position", 1);
                aContainer->setPosition(position);
                message = "The position of the container is now " + itemPositionToString(position) + ".";
                success = true;
            }*/
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfCreature(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    CreatureType *aType = nullptr;
    bool ambulatory;
    int maxHealth;
    Area *spawnLocation = nullptr;
    int maxSpecialPts;
    std::string name;
    std::string description;
    int money;
    Area *aLocation = nullptr;
    int maxInventoryWeight;
    Creature *aCreature = nullptr;
    Area *oldLocation = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::CREATURE)){
        aCreature = dynamic_cast<Creature*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Creature Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aCreature->setName(name);
            message = "The name of the creature is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aCreature->setDescription(description);
            message = "The description of the creature is now " + description + ".";
            success = true;
        } else if (attribute.compare("creature type") == 0){
            aType = getCreatureTypeParameter(aPlayer, "creature type");
            aCreature->setType(aType);
            message = "The type of the creature is now " + aType->getName() + ".";
            success = true;
        } else if (attribute.compare("ambulatory") == 0){
            ambulatory = getBoolParameter(aPlayer, "ambulatory");
            aCreature->setAmbulatory(ambulatory);
            message = "The ambulatory of the creature is now ";
            if (ambulatory){
                message += "true.";
            } else {
                message += "false.";
            }
            success = true;
        } else if (attribute.compare("current location") == 0){
            aLocation = getAreaParameter(aPlayer, "current location");
            oldLocation = aCreature->getLocation();
            oldLocation->removeCharacter(aCreature);
            messageAreaPlayers(nullptr, "A creature named " + aCreature->getName() + " leaves the area.", oldLocation);
            aLocation->addCharacter(aCreature);
            messageAreaPlayers(nullptr, "A creature named " + aCreature->getName() + " leaves the area.", aLocation);
            aCreature->setLocation(aLocation);
            message = "The location of the creature is now " + aLocation->getName() + ".";
            success = true;
        } else if (attribute.compare("spawn location") == 0){
            spawnLocation = getAreaParameter(aPlayer, "spawn location");
            aCreature->setSpawnLocation(spawnLocation);
            message = "The spawn location of the creature is now " + spawnLocation->getName() + ".";
            success = true;
        } else if (attribute.compare("maximum health") == 0){
            maxHealth = getIntParameter(aPlayer, "maximum health");
            aCreature->setMaxHealth(maxHealth);
            message = "The maximum health of the creature is now " + std::to_string(maxHealth) + ".";
            success = true;
        } else if (attribute.compare("maximum special points") == 0){
            maxSpecialPts = getIntParameter(aPlayer, "maximum special points");
            aCreature->setMaxSpecialPts(maxSpecialPts);
            message = "The maximum special points of the creature is now " + std::to_string(maxSpecialPts) + ".";
            success = true;
        } else if (attribute.compare("money") == 0){
            money = getIntParameter(aPlayer, "money");
            aCreature->setMoney(money);
            message = "The money of the creature is now " + std::to_string(money) + ".";
            success = true;
        } else if (attribute.compare("maximum inventory weight") == 0){
            maxInventoryWeight = getIntParameter(aPlayer, "maximum inventory weight");
            aCreature->setMaxInventoryWeight(maxInventoryWeight);
            message = "The weight of the creature is now " + std::to_string(maxInventoryWeight) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfCreatureType(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    CharacterSize size;
    XPTier difficulty;
    std::string name;
    SpecialSkill *skill = nullptr;
    int attackBonus;
    int armorBonus;
    DamageType resistantTo;
    DamageType weakTo;
    float healPoints;
    CreatureType *aCreatureType = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::CREATURE_TYPE)){
        aCreatureType = dynamic_cast<CreatureType*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Creature Type Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aCreatureType->setName(name);
            message = "The name of the creature type is now " + name + ".";
            success = true;
        } else if (attribute.compare("size") == 0){
            size = getCharacterSizeParameter(aPlayer, "size");
            aCreatureType->setSize(size);
            message = "The size of the creature type is now " + characterSizeToString(size) + ".";
            success = true;
        } else if (attribute.compare("difficulty") == 0){
            difficulty = getXPTierParameter(aPlayer, "difficulty");
            aCreatureType->setDifficulty(difficulty);
            message = "The difficulty of the creature type is now " + xpTierToString(difficulty) + ".";
            success = true;
        } else if (attribute.compare("special skill") == 0){
            skill = getSpecialSkillParameter(aPlayer, "special skill");
            aCreatureType->setSpecialSkill(skill);
            message = "The special skill of the creature type is now " + skill->getName() + ".";
            success = true;
        } else if (attribute.compare("attack bonus") == 0){
            attackBonus = getIntParameter(aPlayer, "attack bonus");
            aCreatureType->setAttackBonus(attackBonus);
            message = "The attack bonus of the creature type is now " + std::to_string(attackBonus) + ".";
            success = true;
        } else if (attribute.compare("armor bonus") == 0){
            armorBonus = getIntParameter(aPlayer, "armor bonus");
            aCreatureType->setArmorBonus(armorBonus);
            message = "The armor bonus of the creature type is now " + std::to_string(armorBonus) + ".";
            success = true;
        } else if (attribute.compare("resistance type") == 0){
            resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
            aCreatureType->setResistantTo(resistantTo);
            message = "The resistance type of the creature type is now " + damageTypeToString(resistantTo) + ".";
            success = true;
        } else if (attribute.compare("weakness type") == 0){
            weakTo = getDamageTypeParameter(aPlayer, "weakness type");
            aCreatureType->setWeakTo(weakTo);
            message = "The weakness type of the creature type is now " + damageTypeToString(weakTo) + ".";
            success = true;
        } else if (attribute.compare("healing point rate") == 0){
            healPoints = getFloatParameter(aPlayer, "healing point rate");
            aCreatureType->setHealPoints(healPoints);
            message = "The healing point rate of the creature type is now " + std::to_string(healPoints) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfExit(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    ExitDirection direction;
    Area *location = nullptr;
    Area *connectArea = nullptr;
    bool isConditional;
    ItemType *anItemType = nullptr;
    std::string description;
    std::string altDescription = "";
    Exit *anExit = nullptr;
    Area *oldLocation = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::EXIT)){
        anExit = dynamic_cast<Exit*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Exit Attribute Editing Wizard...");

        if (attribute.compare("direction") == 0){
            direction = getExitDirectionParameter(aPlayer, "direction");
            anExit->setDirection(direction);
            message = "The direction of the exit is now " + exitDirectionToString(direction) + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            anExit->setDescription(description);
            message = "The description of the exit is now " + description + ".";
            success = true;
        } else if (attribute.compare("location") == 0){
            location = getAreaParameter(aPlayer, "location");
            oldLocation = anExit->getLocation();
            oldLocation->removeExit(anExit);
            messageAreaPlayers(nullptr, anExit->getDirectionString() + " you see " + description + " disappear into thin air.", oldLocation);
            location->addExit(anExit);
            messageAreaPlayers(nullptr, anExit->getDirectionString() + " you see " + description + " appear out of nowhere.", location);
            anExit->setLocation(location);
            message = "The location of the exit is now " + location->getName() + ".";
            success = true;
        } else if (attribute.compare("connecting area") == 0){
            connectArea = getAreaParameter(aPlayer, "connecting area");
            anExit->setConnectArea(connectArea);
            message = "The connecting area of the exit is now " + connectArea->getName() + ".";
            success = true;
        } else if (attribute.compare("conditional") == 0){
            isConditional = getBoolParameter(aPlayer, "conditional");
            anExit->setConditional(isConditional);
            message = "The conditional of the exit is now ";
            if (isConditional){
                message += "true.";
            } else {
                message += "false.";
            }
            success = true;
        } else if (attribute.compare("conditional item type") == 0){
            // if conditional is false, then don't need conditional item type
            if (anExit->isConditional()){
                anItemType = getItemTypeParameter(aPlayer, "conditional item type");
                anExit->setConditionItem(anItemType);
                message = "The conditional item type of the exit is now " + anItemType->getName() + ".";
                success = true;
            } else {
                message = "You must set conditional to true before you can edit the conditional item type.";
            }
        } else if (attribute.compare("alternate description") == 0){
            // if conditional is false, then don't need alt description
            if (anExit->isConditional()){
                altDescription = getStringParameter(aPlayer, "alternate description");
                anExit->setAltDescription(altDescription);
                message = "The alternate description of the exit is now " + altDescription + ".";
                success = true;
            } else {
                message = "You must set conditional to true before you can edit the alternate description.";
            }
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfFeature(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    std::string name;
    std::string placement;
    Area *location = nullptr;
    bool isConditional;
    ItemType *anItemType = nullptr;
    std::string description;
    std::string altDescription;
    Feature *aFeature = nullptr;
    Area *oldLocation = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::FEATURE)){
        aFeature = dynamic_cast<Feature*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Feature Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aFeature->setName(name);
            message = "The name of the feature is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aFeature->setDescription(description);
            message = "The description of the feature is now " + description + ".";
            success = true;
        } else if (attribute.compare("placement") == 0){
            placement = getStringParameter(aPlayer, "placement");
            aFeature->setPlacement(placement);
            message = "The placement of the feature is now " + placement + ".";
            success = true;
        } else if (attribute.compare("location") == 0){
            location = getAreaParameter(aPlayer, "location");
            oldLocation = aFeature->getLocation();
            oldLocation->removeFeature(aFeature);
            messageAreaPlayers(nullptr, "The " + aFeature->getName() + aFeature->getPlacement() + " disappears into thin air.", oldLocation);
            location->addFeature(aFeature);
            messageAreaPlayers(nullptr, "You see a " + aFeature->getName() + " appear out of nowhere " + aFeature->getPlacement() + ".", location);
            aFeature->setLocation(location);
            message = "The location of the feature is now " + location->getName() + ".";
            success = true;
        } else if (attribute.compare("conditional") == 0){
            isConditional = getBoolParameter(aPlayer, "conditional");
            aFeature->setConditional(isConditional);
            message = "The conditional of the feature is now ";
            if (isConditional){
                message += "true.";
            } else {
                message += "false.";
            }
            success = true;
        } else if (attribute.compare("conditional item type") == 0){
            // if conditional is false, then don't need conditional item type or alt description
            if (aFeature->isConditional()){
                anItemType = getItemTypeParameter(aPlayer, "conditional item type");
                aFeature->setConditionItem(anItemType);
                message = "The conditional item type of the feature is now " + anItemType->getName() + ".";
                success = true;
            } else {
                message = "You must set conditional to true before you can edit the conditional item type.";
            }
        } else if (attribute.compare("alternate description") == 0){
            if (aFeature->isConditional()){
                altDescription = getStringParameter(aPlayer, "alternate description");
                aFeature->setAltDescription(altDescription);
                message = "The alternate description of the feature is now " + altDescription + ".";
                success = true;
            } else {
                message = "You must set conditional to true before you can edit the alternate description.";
            }
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfItem(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    //InteractiveNoun* location = nullptr;
    //ItemPosition position;
    std::string name;
    Item *anItem = nullptr;
    ItemType *type = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::ITEM)){
        anItem = dynamic_cast<Item*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Item Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            anItem->setName(name);
            message = "The name of the item is now " + name + ".";
            success = true;
        } else if (attribute.compare("type") == 0){
            type = getItemTypeParameter(aPlayer, "type");
            anItem->setType(type);
            message = "The type of the item is now " + type->getName() + ".";
            success = true;
        } else if (attribute.compare("location") == 0){
            // disallow editing location for the time being
            message = "You can't edit the location of the item.";
            /*location = getInteractiveNounParameter(aPlayer, "location");
            oldLocation = anItem->getLocation();
            // remove from old location and add to new location
            anItem->setLocation(location);
            message = "The location of the item is now " + location->getName() + ".";
            success = true;*/
        } else if (attribute.compare("position") == 0){
            // disallow editing position for the time being
            message = "You can't edit the position of the item.";
            /*// make sure that the given combo of location and position make sense
            anObjectType = anItem->getLocation()->getObjectType();
            if (anObjectType == ObjectType::AREA){
                // position can only be ground (can't be changed)
                message = "You can't change the position of this item before changing its location.";
            } else if (anObjectType == ObjectType::CONTAINER){
                // position should be IN, ON, or UNDER
                position = getItemPositionParameter(aPlayer, "position", 0);
                anItem->setPosition(position);
                message = "The position of the item is now " + itemPositionToString(position) + ".";
                success = true;
            } else {
                // position should be either INVENTORY or EQUIPPED
                position = getItemPositionParameter(aPlayer, "position", 1);
                anItem->setPosition(position);
                message = "The position of the item is now " + itemPositionToString(position) + ".";
                success = true;
            }*/
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfItemType(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;
    ItemType *anItemType = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::ITEM_TYPE)){
        anItemType = dynamic_cast<ItemType*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Item Type Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            anItemType->setName(name);
            message = "The name of the item type is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            anItemType->setDescription(description);
            message = "The description of the item type is now " + description + ".";
            success = true;
        } else if (attribute.compare("weight") == 0){
            weight = getIntParameter(aPlayer, "weight");
            anItemType->setWeight(weight);
            message = "The weight of the item type is now " + std::to_string(weight) + ".";
            success = true;
        } else if (attribute.compare("rarity") == 0){
            rarity = getItemRarityParameter(aPlayer, "rarity");
            anItemType->setRarity(rarity);
            message = "The rarity of the item type is now " + itemRarityToString(rarity) + ".";
            success = true;
        } else if (attribute.compare("cost") == 0){
            cost = getIntParameter(aPlayer, "cost");
            anItemType->setCost(cost);
            message = "The cost of the item type is now " + std::to_string(cost) + ".";
            success = true;
        } else if (attribute.compare("slot type") == 0){
            slotType = getEquimentSlotParameter(aPlayer, "slot type");
            anItemType->setSlotType(slotType);
            message = "The slot type of the item type is now " + equipmentSlotToString(slotType) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfNonCombatant(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    Quest *aQuest = nullptr;
    std::string name;
    std::string description;
    int money;
    Area *aLocation = nullptr;
    int maxInventoryWeight;
    NonCombatant *aNonCombatant = nullptr;
    Area *oldLocation = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::NON_COMBATANT)){
        aNonCombatant = dynamic_cast<NonCombatant*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting NonCombatant Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aNonCombatant->setName(name);
            message = "The name of the non-combatant is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aNonCombatant->setDescription(description);
            message = "The description of the non-combatant is now " + description + ".";
            success = true;
        } else if (attribute.compare("money") == 0){
            money = getIntParameter(aPlayer, "money");
            aNonCombatant->setMoney(money);
            message = "The money of the non-combatant is now " + std::to_string(money) + ".";
            success = true;
        } else if (attribute.compare("location") == 0){
            aLocation = getAreaParameter(aPlayer, "location");
            oldLocation = aNonCombatant->getLocation();
            oldLocation->removeCharacter(aNonCombatant);
            messageAreaPlayers(nullptr, "A person named " + aNonCombatant->getName() + " disappears into thin air.", oldLocation);
            aLocation->addCharacter(aNonCombatant);
            messageAreaPlayers(nullptr, "A person named " + aNonCombatant->getName() + " appears out of nowhere.", aLocation);
            aNonCombatant->setLocation(aLocation);
            message = "The location of the non-combatant is now " + aLocation->getName() + ".";
            success = true;
        } else if (attribute.compare("maximum inventory weight") == 0){
            maxInventoryWeight = getIntParameter(aPlayer, "maximum inventory weight");
            aNonCombatant->setMaxInventoryWeight(maxInventoryWeight);
            message = "The maximum inventory weight of the non-combatant is now " + std::to_string(maxInventoryWeight) + ".";
            success = true;
        } else if (attribute.compare("quest") == 0){
            aQuest = getQuestParameter(aPlayer, "quest", true);
            aNonCombatant->setQuest(aQuest);
            message = "The quest of the non-combatant is now ";
            if (aQuest != nullptr){
                message += aQuest->getName() + ".";
            } else {
                message += "none.";
            }
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfPlayer(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    CharacterSize size;
    PlayerClass *aClass;
    Area *spawnLocation;
    std::string name;
    std::string description;
    int money;
    int maxInventoryWeight;
    Player *thePlayer = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::PLAYER)){
        thePlayer = dynamic_cast<Player*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Player Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            thePlayer->setName(name);
            message = "The name of the player is now " + name + ".";
            success = true;
            messagePlayer(thePlayer, "Your name is now " + name + ".");
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            thePlayer->setDescription(description);
            message = "The description of the player is now " + description + ".";
            success = true;
            messagePlayer(thePlayer, "Your description is now " + description + ".");
        } else if (attribute.compare("size") == 0){
            size = getCharacterSizeParameter(aPlayer, "size");
            thePlayer->setSize(size);
            message = "The size of the player is now " + characterSizeToString(size) + ".";
            success = true;
            messagePlayer(thePlayer, "Your size is now " + characterSizeToString(size) + ".");
        } else if (attribute.compare("player class") == 0){
            aClass = getPlayerClassParameter(aPlayer, "player class");
            thePlayer->setPlayerClass(aClass);
            message = "The player class of the player is now " + aClass->getName() + ".";
            success = true;
            messagePlayer(thePlayer, "Your player class is now " + aClass->getName() + ".");
        } else if (attribute.compare("spawn location") == 0){
            spawnLocation = getAreaParameter(aPlayer, "spawn location");
            thePlayer->setSpawnLocation(spawnLocation);
            message = "The spawn location of the player is now " + spawnLocation->getName() + ".";
            success = true;
        } else if (attribute.compare("money") == 0){
            money = getIntParameter(aPlayer, "money");
            thePlayer->setMoney(money);
            message = "The money of the player is now " + std::to_string(money) + ".";
            success = true;
            messagePlayer(thePlayer, "You now have " + std::to_string(money) + " money.");
        } else if (attribute.compare("maximum inventory weight") == 0){
            maxInventoryWeight = getIntParameter(aPlayer, "maximum inventory weight");
            thePlayer->setMaxInventoryWeight(maxInventoryWeight);
            message = "The maximum inventory weight of the player is now " + std::to_string(maxInventoryWeight) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfPlayerClass(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int primaryStat;
    std::string name;
    SpecialSkill* skill = nullptr;
    int attackBonus;
    int armorBonus;
    DamageType resistantTo;
    DamageType weakTo;
    float healPoints;
    PlayerClass *aPlayerClass = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::PLAYER_CLASS)){
        aPlayerClass = dynamic_cast<PlayerClass*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting PLayer Class Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aPlayerClass->setName(name);
            message = "The name of the player class is now " + name + ".";
            success = true;
        } else if (attribute.compare("primary stat") == 0){
            primaryStat = getIntParameter(aPlayer, "primary stat (enter [0] for dexterity, [1] for intelligence, or [2] for strength)", 2);
            aPlayerClass->setPrimaryStat(primaryStat);
            message = "The primary stat of the player class is now ";
            if (primaryStat == 0){
                message += "dexterity.";
            } else if (primaryStat == 1){
                message += "intelligence.";
            } else {
                message += "strength.";
            }
            success = true;
        } else if (attribute.compare("special skill") == 0){
            skill = getSpecialSkillParameter(aPlayer, "special skill");
            aPlayerClass->setSpecialSkill(skill);
            message = "The special skill of the player class is now " + skill->getName() + ".";
            success = true;
        } else if (attribute.compare("attack bonus") == 0){
            attackBonus = getIntParameter(aPlayer, "attack bonus");
            aPlayerClass->setAttackBonus(attackBonus);
            message = "The attack bonus of the player class is now " + std::to_string(attackBonus) + ".";
            success = true;
        } else if (attribute.compare("armor bonus") == 0){
            armorBonus = getIntParameter(aPlayer, "armor bonus");
            aPlayerClass->setArmorBonus(armorBonus);
            message = "The armor bonus of the player class is now " + std::to_string(armorBonus) + ".";
            success = true;
        } else if (attribute.compare("resistance type") == 0){
            resistantTo = getDamageTypeParameter(aPlayer, "resistance type");
            aPlayerClass->setResistantTo(resistantTo);
            message = "The resistance type of the player class is now " + damageTypeToString(resistantTo) + ".";
            success = true;
        } else if (attribute.compare("weakness type") == 0){
            weakTo = getDamageTypeParameter(aPlayer, "weakness type");
            aPlayerClass->setWeakTo(weakTo);
            message = "The weakness type of the player class is now " + damageTypeToString(weakTo) + ".";
            success = true;
        } else if (attribute.compare("healing point rate") == 0){
            healPoints = getFloatParameter(aPlayer, "healing point rate");
            aPlayerClass->setHealPoints(healPoints);
            message = "The healing point rate of the player class is now " + std::to_string(healPoints) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfQuest(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    std::string name;
    std::string description;
    int rewardMoney;
    Item *rewardItem = nullptr;
    Quest *aQuest = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::QUEST)){
        aQuest = dynamic_cast<Quest*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Quest Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aQuest->setName(name);
            message = "The name of the quest is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aQuest->setDescription(description);
            message = "The description of the quest is now " + description + ".";
            success = true;
        } else if (attribute.compare("reward money") == 0){
            rewardMoney = getIntParameter(aPlayer, "reward money");
            aQuest->setRewardMoney(rewardMoney);
            message = "The reward money of the quest is now " + std::to_string(rewardMoney) + ".";
            success = true;
        } else if (attribute.compare("reward item") == 0){
            rewardItem = getItemParameter(aPlayer, "reward item", true); // may want to change this so they can't choose ANY item ANYWHERE in the game
            aQuest->setRewardItem(rewardItem);
            message = "The reward item of the quest is now ";
            if (rewardItem == nullptr){
                message += "none.";
            } else {
                message += rewardItem->getName() + ".";
            }
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfQuestStep(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int ordinalNumber;
    std::string description;
    ItemType *anItemType = nullptr;
    NonCombatant *giver = nullptr;
    NonCombatant *receiver = nullptr;
    std::string completionText;
    QuestStep *aQuestStep = nullptr;
    Quest *aQuest = nullptr;
    NonCombatant *oldGiver = nullptr;
    NonCombatant *oldReceiver = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::QUEST_STEP)){
        aQuestStep = dynamic_cast<QuestStep*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Quest Step Attribute Editing Wizard...");

        if (attribute.compare("ordinal number") == 0){
            ordinalNumber = getIntParameter(aPlayer, "ordinal number");
            aQuestStep->setOrdinalNumber(ordinalNumber);
            message = "The ordinal number of the quest step is now " + std::to_string(ordinalNumber) + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aQuestStep->setDescription(description);
            message = "The description of the quest step is now " + description + ".";
            success = true;
        } else if (attribute.compare("fetch item type") == 0){
            anItemType = getItemTypeParameter(aPlayer, "fetch item type");
            aQuestStep->setFetchItem(anItemType);
            message = "The fetch item type of the quest step is now " + anItemType->getName() + ".";
            success = true;
        } else if (attribute.compare("giver") == 0){
            giver = getNonCombatantParameter(aPlayer, "giver", aQuest);
            oldGiver = aQuestStep->getGiver();
            aQuest = oldGiver->getQuest();
            oldGiver->setQuest(nullptr);
            giver->setQuest(aQuest);
            aQuestStep->setGiver(giver);
            message = "The giver of the quest step is now " + giver->getName() + ".";
            success = true;
        } else if (attribute.compare("receiver") == 0){
            receiver = getNonCombatantParameter(aPlayer, "receiver", aQuest);
            oldReceiver = aQuestStep->getReceiver();
            aQuest = oldReceiver->getQuest();
            oldReceiver->setQuest(nullptr);
            receiver->setQuest(aQuest);
            aQuestStep->setReceiver(receiver);
            message = "The receiver of the quest step is now " + receiver->getName() + ".";
            success = true;
        } else if (attribute.compare("completion text") == 0){
            completionText = getStringParameter(aPlayer, "completion text");
            aQuestStep->setCompletionText(completionText);
            message = "The completion text of the quest step is now " + completionText + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfSpecialSkill(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    std::string name;
    int damage;
    DamageType type;
    int cost;
    time_t cooldown;
    SpecialSkill *aSpecialSkill = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::SPECIAL_SKILL)){
        aSpecialSkill = dynamic_cast<SpecialSkill*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Special Skill Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aSpecialSkill->setName(name);
            message = "The name of the special skill is now " + name + ".";
            success = true;
        } else if (attribute.compare("damage") == 0){
            damage = getIntParameter(aPlayer, "damage");
            aSpecialSkill->setDamage(damage);
            message = "The damage of the special skill is now " + std::to_string(damage) + ".";
            success = true;
        } else if (attribute.compare("damage type") == 0){
            type = getDamageTypeParameter(aPlayer, "damage type");
            aSpecialSkill->setDamageType(type);
            message = "The damage type of the special skill is now " + damageTypeToString(type) + ".";
            success = true;
        } else if (attribute.compare("cost") == 0){
            cost = getIntParameter(aPlayer, "cost");
            aSpecialSkill->setCost(cost);
            message = "The cost of the special skill is now " + std::to_string(cost) + ".";
            success = true;
        } else if (attribute.compare("cooldown (in seconds)") == 0){
            cooldown = getIntParameter(aPlayer, "cooldown (in seconds)");
            aSpecialSkill->setCooldown(cooldown);
            message = "The cooldown of the special skill is now " + std::to_string(cooldown) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


// would be better without dynamic casts
bool GameLogic::editAttributeOfWeaponType(Player *aPlayer, InteractiveNoun *objectToEdit, std::string attribute){
    std::string message = "";
    bool success = false;
    int damage;
    DamageType type;
    AreaSize range;
    int critMultiplier;
    int weight;
    ItemRarity rarity;
    std::string description;
    std::string name;
    int cost;
    EquipmentSlot slotType;
    WeaponType *aWeaponType = nullptr;

    if ((objectToEdit != nullptr) && (objectToEdit->getObjectType() == ObjectType::WEAPON_TYPE)){
        aWeaponType = dynamic_cast<WeaponType*>(objectToEdit);

        addPlayerMessageQueue(aPlayer);
        messagePlayer(aPlayer, "\015\012Starting Weapon Type Attribute Editing Wizard...");

        if (attribute.compare("name") == 0){
            name = getStringParameter(aPlayer, "name");
            aWeaponType->setName(name);
            message = "The name of the weapon type is now " + name + ".";
            success = true;
        } else if (attribute.compare("description") == 0){
            description = getStringParameter(aPlayer, "description");
            aWeaponType->setDescription(description);
            message = "The description of the weapon type is now " + description + ".";
            success = true;
        } else if (attribute.compare("damage") == 0){
            damage = getIntParameter(aPlayer, "damage");
            aWeaponType->setDamage(damage);
            message = "The damage of the weapon type is now " + std::to_string(damage) + ".";
            success = true;
        } else if (attribute.compare("damage type") == 0){
            type = getDamageTypeParameter(aPlayer, "damage type");
            aWeaponType->setDamageType(type);
            message = "The damage type of the weapon type is now " + damageTypeToString(type) + ".";
            success = true;
        } else if (attribute.compare("range") == 0){
            range = getAreaSizeParameter(aPlayer, "range");
            aWeaponType->setRange(range);
            message = "The range of the weapon type is now " + areaSizeToString(range) + ".";
            success = true;
        } else if (attribute.compare("crit multiplier") == 0){
            critMultiplier = getIntParameter(aPlayer, "crit multiplier");
            aWeaponType->setCritMultiplier(critMultiplier);
            message = "The crit multiplier of the weapon type is now " + std::to_string(critMultiplier) + ".";
            success = true;
        } else if (attribute.compare("weight") == 0){
            weight = getIntParameter(aPlayer, "weight");
            aWeaponType->setWeight(weight);
            message = "The weight of the weapon type is now " + std::to_string(weight) + ".";
            success = true;
        } else if (attribute.compare("rarity") == 0){
            rarity = getItemRarityParameter(aPlayer, "rarity");
            aWeaponType->setRarity(rarity);
            message = "The rarity of the weapon type is now " + itemRarityToString(rarity) + ".";
            success = true;
        } else if (attribute.compare("cost") == 0){
            cost = getIntParameter(aPlayer, "cost");
            aWeaponType->setCost(cost);
            message = "The cost of the weapon type is now " + std::to_string(cost) + ".";
            success = true;
        } else if (attribute.compare("slot type") == 0){
            slotType = getEquimentSlotParameter(aPlayer, "slot type");
            aWeaponType->setSlotType(slotType);
            message = "The slot type of the weapon type is now " + equipmentSlotToString(slotType) + ".";
            success = true;
        } else if (attribute.compare("action") == 0){
            // add, edit, or remove action
            message = editActionAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else if (attribute.compare("alias") == 0){
            // add or remove aliases
            message = editAliasAttribute(aPlayer, objectToEdit);
            if (!message.empty()){
                success = true;
            }
        } else {
            message = "I don't understand which attribute you want to edit.";
        }

        removePlayerMessageQueue(aPlayer);
    } else {
        message = "I don't understand what you want to edit.";
    }

    messagePlayer(aPlayer, message);

    return success;
}


int GameLogic::getIntParameter(Player *aPlayer, std::string paramName, int maxNum){
    std::string response = "";
    int intParam = -1;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Please enter a positive integer value no greater than " + std::to_string(maxNum) + ".";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    intParam = validateStringNumber(response, 0, maxNum);
    while (intParam == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter a positive integer value no greater than " + std::to_string(maxNum) + ".");
        response = blockingGetMsg(aPlayer);
        intParam = validateStringNumber(response, 0, maxNum);
    }

    return intParam;
}


bool GameLogic::getBoolParameter(Player *aPlayer, std::string paramName){
    bool boolParam = false;
    std::string response = "";
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Please enter TRUE or FALSE.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);
    while ((response.compare("true") != 0) && (response.compare("false") != 0)){
        messagePlayer(aPlayer, "Invalid input. Please enter TRUE or FALSE.");
        response = blockingGetMsg(aPlayer);
        std::transform(response.begin(), response.end(), response.begin(), ::tolower);
    }

    if (response.compare("true") == 0){
        boolParam = true;
    } else if (response.compare("false") == 0){
        boolParam = false;
    }

    return boolParam;
}


float GameLogic::getFloatParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    float floatParam = -1.0;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Please enter a floating point value greater than zero.";

    messagePlayer(aPlayer, message);
    response = blockingGetMsg(aPlayer);
    floatParam = std::atof(response.c_str());
    while (floatParam <= 0.0){
        messagePlayer(aPlayer, "Invalid input. Please enter a floating point value greater than zero.");
        response = blockingGetMsg(aPlayer);
        floatParam = std::atof(response.c_str());
    }

    return floatParam;
}


std::string GameLogic::getStringParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Please enter a string.";

    messagePlayer(aPlayer, message);
    response = blockingGetMsg(aPlayer);
    while (response == ""){
        messagePlayer(aPlayer, "Invalid input. Please enter a string.");
        response = blockingGetMsg(aPlayer);
    }

    return response;
}


EffectType GameLogic::getEffectTypeParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    EffectType effectTypeParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NONE, [2] DAMAGE, [3] FALL, [4] LONG FALL, [5] LOST ITEM, [6] DROP ALL ITEMS, [7] GAIN MONEY, [8] LOSE MONEY, [9] HEAL, or [10] GET SPECIAL POINTS. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 10);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 10);
    }

    switch (choice){
        case 1:
            effectTypeParam = EffectType::NONE;
            break;
        case 2:
            effectTypeParam = EffectType::DAMAGE;
            break;
        case 3:
            effectTypeParam = EffectType::FALL;
            break;
        case 4:
            effectTypeParam = EffectType::LONG_FALL;
            break;
        case 5:
            effectTypeParam = EffectType::LOST_ITEM;
            break;
        case 6:
            effectTypeParam = EffectType::DROP_ALL_ITEMS;
            break;
        case 7:
            effectTypeParam = EffectType::GAIN_MONEY;
            break;
        case 8:
            effectTypeParam = EffectType::LOSE_MONEY;
            break;
        case 9:
            effectTypeParam = EffectType::HEAL;
            break;
        case 10:
            effectTypeParam = EffectType::GET_SPECIAL_POINTS;
            break;
        default:
            effectTypeParam = EffectType::NONE;
            break;
    }

    return effectTypeParam;
}


AreaSize GameLogic::getAreaSizeParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    AreaSize areaSizeParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] SMALL, [2] MEDIUM, or [3] LARGE. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 3);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 3);
    }

    switch (choice){
        case 1:
            areaSizeParam = AreaSize::SMALL;
            break;
        case 2:
            areaSizeParam = AreaSize::MEDIUM;
            break;
        case 3:
            areaSizeParam = AreaSize::LARGE;
            break;
        default:
            areaSizeParam = AreaSize::SMALL;
            break;
    }

    return areaSizeParam;
}


parser::Grammar::Support GameLogic::getSupportParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    parser::Grammar::Support supportParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NO, [2] YES, or [3] TEXT. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 3);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 3);
    }

    switch (choice){
        case 1:
            supportParam = parser::Grammar::Support::NO;
            break;
        case 2:
            supportParam = parser::Grammar::Support::YES;
            break;
        case 3:
            supportParam = parser::Grammar::Support::TEXT;
            break;
        default:
            supportParam = parser::Grammar::Support::NO;
            break;
    }

    return supportParam;
}


parser::PrepositionType GameLogic::getPrepositionTypeParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    parser::PrepositionType prepTypeParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NONE, [2] TO, [3] WITH, [4] OF, [5] FROM, [6] ON, [7] UNDER, [8] IN, or [9] ALL. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 9);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 9);
    }

    switch (choice){
        case 1:
            prepTypeParam = parser::PrepositionType::NONE;
            break;
        case 2:
            prepTypeParam = parser::PrepositionType::TO;
            break;
        case 3:
            prepTypeParam = parser::PrepositionType::WITH;
            break;
        case 4:
            prepTypeParam = parser::PrepositionType::OF;
            break;
        case 5:
            prepTypeParam = parser::PrepositionType::FROM;
            break;
        case 6:
            prepTypeParam = parser::PrepositionType::ON;
            break;
        case 7:
            prepTypeParam = parser::PrepositionType::UNDER;
            break;
        case 8:
            prepTypeParam = parser::PrepositionType::IN;
            break;
        case 9:
            prepTypeParam = parser::PrepositionType::ALL;
            break;
        default:
            prepTypeParam = parser::PrepositionType::NONE;
            break;
    }

    return prepTypeParam;
}


DamageType GameLogic::getDamageTypeParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    DamageType damageTypeParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NONE, [2] CRUSHING, [3] PIERCING, [4] ELECTRIC, [5] FIRE, [6] WATER, [7] WIND, [8] EARTH, or [9] HEAL. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 9);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 9);
    }

    switch (choice){
        case 1:
            damageTypeParam = DamageType::NONE;
            break;
        case 2:
            damageTypeParam = DamageType::CRUSHING;
            break;
        case 3:
            damageTypeParam = DamageType::PIERCING;
            break;
        case 4:
            damageTypeParam = DamageType::ELECTRIC;
            break;
        case 5:
            damageTypeParam = DamageType::FIRE;
            break;
        case 6:
            damageTypeParam = DamageType::WATER;
            break;
        case 7:
            damageTypeParam = DamageType::WIND;
            break;
        case 8:
            damageTypeParam = DamageType::EARTH;
            break;
        case 9:
            damageTypeParam = DamageType::HEAL;
            break;
        default:
            damageTypeParam = DamageType::NONE;
            break;
    }

    return damageTypeParam;
}


ItemRarity GameLogic::getItemRarityParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    ItemRarity itemRarityParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] COMMON, [2] UNCOMMON, [3] RARE, [4] LEGENDARY, or [5] QUEST. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 5);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 5);
    }

    switch (choice){
        case 1:
            itemRarityParam = ItemRarity::COMMON;
            break;
        case 2:
            itemRarityParam = ItemRarity::UNCOMMON;
            break;
        case 3:
            itemRarityParam = ItemRarity::RARE;
            break;
        case 4:
            itemRarityParam = ItemRarity::LEGENDARY;
            break;
        case 5:
            itemRarityParam = ItemRarity::QUEST;
            break;
        default:
            itemRarityParam = ItemRarity::COMMON;
            break;
    }

    return itemRarityParam;
}


CommandEnum GameLogic::getCommandEnumParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    CommandEnum commandEnumParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] LOOK, [2] LISTEN, [3] TAKE, [4] PUT, [5] DROP, [6] EQUIP, [7] UNEQUIP, [8] TRANSFER, [9] GO, [10] MOVE, [11] ATTACK, [12] TALK, [13] SHOP, [14] BUY, [15] SELL, [16] SEARCH, [17] USE_SKILL, [18] READ, [19] BREAK, [20] CLIMB, [21] TURN, [22] PUSH, [23] PULL, [24] EAT, or [25] DRINK. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 25);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 25);
    }

std::cout << "choice = " << std::to_string(choice) << "\n";

    switch (choice){
        case 1:
            commandEnumParam = CommandEnum::LOOK;
            break;
        case 2:
            commandEnumParam = CommandEnum::LISTEN;
            break;
        case 3:
            commandEnumParam = CommandEnum::TAKE;
            break;
        case 4:
            commandEnumParam = CommandEnum::PUT;
            break;
        case 5:
            commandEnumParam = CommandEnum::DROP;
            break;
        case 6:
            commandEnumParam = CommandEnum::EQUIP;
            break;
        case 7:
            commandEnumParam = CommandEnum::UNEQUIP;
            break;
        case 8:
            commandEnumParam = CommandEnum::TRANSFER;
            break;
        case 9:
            commandEnumParam = CommandEnum::GO;
            break;
        case 10:
            commandEnumParam = CommandEnum::MOVE;
            break;
        case 11:
            commandEnumParam = CommandEnum::ATTACK;
            break;
        case 12:
            commandEnumParam = CommandEnum::TALK;
            break;
        case 13:
            commandEnumParam = CommandEnum::SHOP;
            break;
        case 14:
            commandEnumParam = CommandEnum::BUY;
            break;
        case 15:
            commandEnumParam = CommandEnum::SELL;
            break;
        case 16:
            commandEnumParam = CommandEnum::SEARCH;
            break;
        case 17:
            commandEnumParam = CommandEnum::USE_SKILL;
            break;
        case 18:
            commandEnumParam = CommandEnum::READ;
            break;
        case 19:
            commandEnumParam = CommandEnum::BREAK;
            break;
        case 20:
            commandEnumParam = CommandEnum::CLIMB;
            break;
        case 21:
            commandEnumParam = CommandEnum::TURN;
            break;
        case 22:
            commandEnumParam = CommandEnum::PUSH;
            break;
        case 23:
            commandEnumParam = CommandEnum::PULL;
            break;
        case 24:
            commandEnumParam = CommandEnum::EAT;
            break;
        case 25:
            commandEnumParam = CommandEnum::DRINK;
            break;
        default:
            commandEnumParam = CommandEnum::INVALID;
            break;
    }

std::cout << "commandEnumParam = " << commandEnumToString(commandEnumParam) << "\n";

    return commandEnumParam;
}


EquipmentSlot GameLogic::getEquimentSlotParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    EquipmentSlot equipmentSlotParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NONE, [2] HEAD, [3] SHOULDERS, [4] NECK, [5] TORSO, [6] BELT, [7] LEGS, [8] ARMS, [9] HANDS, [10] RIGHT HAND, [11] LEFT HAND, [12] FEET, [13] RIGHT RING, or [14] LEFT RING. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 14);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 14);
    }

    switch (choice){
        case 1:
            equipmentSlotParam = EquipmentSlot::NONE;
            break;
        case 2:
            equipmentSlotParam = EquipmentSlot::HEAD;
            break;
        case 3:
            equipmentSlotParam = EquipmentSlot::SHOULDERS;
            break;
        case 4:
            equipmentSlotParam = EquipmentSlot::NECK;
            break;
        case 5:
            equipmentSlotParam = EquipmentSlot::TORSO;
            break;
        case 6:
            equipmentSlotParam = EquipmentSlot::BELT;
            break;
        case 7:
            equipmentSlotParam = EquipmentSlot::LEGS;
            break;
        case 8:
            equipmentSlotParam = EquipmentSlot::ARMS;
            break;
        case 9:
            equipmentSlotParam = EquipmentSlot::HANDS;
            break;
        case 10:
            equipmentSlotParam = EquipmentSlot::RIGHT_HAND;
            break;
        case 11:
            equipmentSlotParam = EquipmentSlot::LEFT_HAND;
            break;
        case 12:
            equipmentSlotParam = EquipmentSlot::FEET;
            break;
        case 13:
            equipmentSlotParam = EquipmentSlot::RIGHT_RING;
            break;
        case 14:
            equipmentSlotParam = EquipmentSlot::LEFT_RING;
            break;
        default:
            equipmentSlotParam = EquipmentSlot::NONE;
            break;
    }

    return equipmentSlotParam;
}


ItemPosition GameLogic::getItemPositionParameter(Player *aPlayer, std::string paramName, int option){
    std::string response = "";
    int choice = -1;
    int numOptions = 0;
    ItemPosition itemPositionParam;
    std::string message = "What would you like the " + paramName + " to be?";

    if (option == 0){
        message += " Your choices are: [1] IN, [2] ON, or [3] UNDER. Please enter the number that corresponds to your choice."; 
        numOptions = 3;
    } else if (option == 1){
        message += " Your choices are: [1] INVENTORY or [2] EQUIPPED. Please enter the number that corresponds to your choice."; 
        numOptions = 2;
    } else {
        message += " Your choices are: [1] GROUND, [2] INVENTORY, [3] EQUIPPED, [4] IN, [5] ON, or [6] UNDER. Please enter the number that corresponds to your choice."; 
        numOptions = 6;
    }

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, numOptions);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, numOptions);
    }

    switch (choice){
        case 1:
            if (option == 0){
                itemPositionParam = ItemPosition::IN;
            } else if (option == 1){
                itemPositionParam = ItemPosition::INVENTORY;
            } else {
                itemPositionParam = ItemPosition::GROUND;
            }
            break;
        case 2:
            if (option == 0){
                itemPositionParam = ItemPosition::ON;
            } else if (option == 1){
                itemPositionParam = ItemPosition::EQUIPPED;
            } else {
                itemPositionParam = ItemPosition::INVENTORY;
            }
            break;
        case 3:
            if (option == 0){
                itemPositionParam = ItemPosition::UNDER;
            } else {
                itemPositionParam = ItemPosition::EQUIPPED;
            }
            break;
        case 4:
            itemPositionParam = ItemPosition::IN;
            break;
        case 5:
            itemPositionParam = ItemPosition::ON;
            break;
        case 6:
            itemPositionParam = ItemPosition::UNDER;
            break;
        default:
            itemPositionParam = ItemPosition::NONE;
            break;
    }

    return itemPositionParam;
}


CharacterSize GameLogic::getCharacterSizeParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    CharacterSize characterSizeParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] TINY, [2] SMALL, [3] MEDIUM, [4] LARGE, or [5] HUGE. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 5);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 5);
    }

    switch (choice){
        case 1:
            characterSizeParam = CharacterSize::TINY;
            break;
        case 2:
            characterSizeParam = CharacterSize::SMALL;
            break;
        case 3:
            characterSizeParam = CharacterSize::MEDIUM;
            break;
        case 4:
            characterSizeParam = CharacterSize::LARGE;
            break;
        case 5:
            characterSizeParam = CharacterSize::HUGE;
            break;
        default:
            characterSizeParam = CharacterSize::TINY;
            break;
    }

    return characterSizeParam;
}


ExitDirection GameLogic::getExitDirectionParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    ExitDirection exitDirectionParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] NORTH, [2] SOUTH, [3] EAST, [4] WEST, [5] NORTHEAST, [6] NORTHWEST, [7] SOUTHEAST, [8] SOUTHWEST, [9] UP, or [10] DOWN. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 10);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 10);
    }

    switch (choice){
        case 1:
            exitDirectionParam = ExitDirection::NORTH;
            break;
        case 2:
            exitDirectionParam = ExitDirection::SOUTH;
            break;
        case 3:
            exitDirectionParam = ExitDirection::EAST;
            break;
        case 4:
            exitDirectionParam = ExitDirection::WEST;
            break;
        case 5:
            exitDirectionParam = ExitDirection::NORTHEAST;
            break;
        case 6:
            exitDirectionParam = ExitDirection::NORTHWEST;
            break;
        case 7:
            exitDirectionParam = ExitDirection::SOUTHEAST;
            break;
        case 8:
            exitDirectionParam = ExitDirection::SOUTHWEST;
            break;
        case 9:
            exitDirectionParam = ExitDirection::UP;
            break;
        case 10:
            exitDirectionParam = ExitDirection::DOWN;
            break;
        default:
            exitDirectionParam = ExitDirection::NORTH;
            break;
    }

    return exitDirectionParam;
}


XPTier GameLogic::getXPTierParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    int choice = -1;
    XPTier xpTierParam;
    std::string message = "What would you like the " + paramName + " to be?";
    message += " Your choices are: [1] TRIVIAL, [2] EASY, [3] NORMAL, [4] HARD, or [5] LEGENDARY. Please enter the number that corresponds to your choice.";

    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, 5);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, 5);
    }

    switch (choice){
        case 1:
            xpTierParam = XPTier::TRIVIAL;
            break;
        case 2:
            xpTierParam = XPTier::EASY;
            break;
        case 3:
            xpTierParam = XPTier::NORMAL;
            break;
        case 4:
            xpTierParam = XPTier::HARD;
            break;
        case 5:
            xpTierParam = XPTier::LEGENDARY;
            break;
        default:
            xpTierParam = XPTier::TRIVIAL;
            break;
    }

    return xpTierParam;
}


template <class aType>
int GameLogic::getPointerParameter(Player *aPlayer, std::string paramName, std::vector<aType> possibleVals, bool canBeNull, std::string question){
    std::string response = "";
    int choice = -1;
    int optionsSize = possibleVals.size();
    std::string message;

    if (question == ""){
        message = "What would you like the " + paramName + " to be?";
    } else {
        message = question;
    }
    
    message += " Your choices are: ";
    for (size_t i = 0; i < optionsSize; i++){
        message += "[" + std::to_string(i + 1) + "] " + possibleVals[i]->getName();
        if (i == (optionsSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
    }

    if (canBeNull){
        message += " Enter [0] if you would like to leave this attribute blank.";
    }

    if ((optionsSize != 0) || (canBeNull)){
        messagePlayer(aPlayer, message); 
        response = blockingGetMsg(aPlayer);
        if ((canBeNull) && (response.compare("0") == 0)){
            choice = 0;
        } else {
            choice = validateStringNumber(response, 1, optionsSize);
        }
        while (choice == -1){
            messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
            response = blockingGetMsg(aPlayer);
            if ((canBeNull) && (response.compare("0") == 0)){
                choice = 0;
            } else {
                choice = validateStringNumber(response, 1, optionsSize);
            }
        }
    }

    return choice;
}


Area* GameLogic::getAreaParameter(Player *aPlayer, std::string paramName){
    Area *areaParam = nullptr;
    std::vector<Area*> allAreas;
    int choice;
    
    allAreas = manager->getGameAreas();
    choice = getPointerParameter<Area*>(aPlayer, paramName, allAreas);
    if (choice != -1){
        areaParam = allAreas[choice - 1];
    } 

    return areaParam;
}


SpecialSkill* GameLogic::getSpecialSkillParameter(Player *aPlayer, std::string paramName){
    SpecialSkill *specialSkillParam = nullptr;
    std::vector<SpecialSkill*> allSpecialSkills;
    int choice;
    
    allSpecialSkills = manager->getGameSkills();
    choice = getPointerParameter<SpecialSkill*>(aPlayer, paramName, allSpecialSkills);
    if (choice != -1){
        specialSkillParam = allSpecialSkills[choice - 1];
    } 

    return specialSkillParam;
}


ItemType* GameLogic::getItemTypeParameter(Player *aPlayer, std::string paramName){
    ItemType *itemTypeParam = nullptr;
    std::vector<ItemType*> allItemTypes;
    int choice;
    
    allItemTypes = manager->getGameItemTypes();
    choice = getPointerParameter<ItemType*>(aPlayer, paramName, allItemTypes);
    if (choice != -1){
        itemTypeParam = allItemTypes[choice - 1];
    } 

    return itemTypeParam;
}


InteractiveNoun* GameLogic::getInteractiveNounParameter(Player *aPlayer, std::string paramName){
    std::string response = "";
    std::vector<Creature*> allCreatures;
    std::vector<Player*> allPlayers;
    std::vector<Area*> allAreas;
    std::vector<NonCombatant*> allNPCs;
    std::vector<Container*> allContainers;
    size_t totalSize, index, playerOffset, creatureOffset, npcOffset, containerOffset, unsignChoice;
    std::string message = "What would you like the " + paramName + " to be?";
    InteractiveNoun *param = nullptr;
    int choice;

    allAreas = manager->getGameAreas();
    allPlayers = manager->getGamePlayers();
    allCreatures = manager->getCreatures();
    allNPCs = manager->getGameNPCs();
    allContainers = manager->getGameContainers();
    totalSize = allAreas.size() + allPlayers.size() + allCreatures.size() + allNPCs.size() + allContainers.size();
    playerOffset = allAreas.size();
    creatureOffset = playerOffset + allPlayers.size();
    npcOffset = creatureOffset + allCreatures.size();
    containerOffset = npcOffset + allNPCs.size();
    index = 0;

    message += " Your choices are: ";
    for (size_t i = 0; i < allAreas.size(); i++){
        message += "[" + std::to_string(index + 1) + "] " + allAreas[i]->getName();
        if (i == (totalSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
        index++;
    }
    for (size_t i = 0; i < allPlayers.size(); i++){
        message += "[" + std::to_string(index + 1) + "] " + allPlayers[i]->getName();
        if (i == (totalSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
        index++;
    }
    for (size_t i = 0; i < allCreatures.size(); i++){
        message += "[" + std::to_string(index + 1) + "] " + allCreatures[i]->getName();
        if (i == (totalSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
        index++;
    }
    for (size_t i = 0; i < allNPCs.size(); i++){
        message += "[" + std::to_string(index + 1) + "] " + allNPCs[i]->getName();
        if (i == (totalSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
        index++;
    }
    for (size_t i = 0; i < allContainers.size(); i++){
        message += "[" + std::to_string(index + 1) + "] " + allContainers[i]->getName();
        if (i == (totalSize - 1)){
            message += ". Please enter the number that corresponds to your choice.";
        } else {
            message += ", ";
        }
        index++;
    }
    
    messagePlayer(aPlayer, message); 
    response = blockingGetMsg(aPlayer);
    choice = validateStringNumber(response, 1, index);
    while (choice == -1){
        messagePlayer(aPlayer, "Invalid input. Please enter the number that corresponds to your choice.");
        response = blockingGetMsg(aPlayer);
        choice = validateStringNumber(response, 1, index);
    }
    unsignChoice = choice;

    if (unsignChoice <= allAreas.size()){
        param = allAreas[unsignChoice - 1];
    } else if (unsignChoice <= (playerOffset + allPlayers.size())){
        param = allPlayers[unsignChoice - playerOffset - 1];
    } else if (unsignChoice <= (creatureOffset + allCreatures.size())){
        param = allPlayers[unsignChoice - creatureOffset - 1];
    } else if (unsignChoice <= (npcOffset + allNPCs.size())){
        param = allNPCs[unsignChoice -  npcOffset - 1];
    } else if (unsignChoice <= (containerOffset + allContainers.size())){
        param = allContainers[unsignChoice - containerOffset - 1];
    } else {
        param = nullptr;
    }

    return param;
}


CreatureType* GameLogic::getCreatureTypeParameter(Player *aPlayer, std::string paramName){
    CreatureType *creatureTypeParam = nullptr;
    std::vector<CreatureType*> allCreatureTypes;
    int choice;
    
    allCreatureTypes = manager->getGameCreatureTypes();
    choice = getPointerParameter<CreatureType*>(aPlayer, paramName, allCreatureTypes);
    if (choice != -1){
        creatureTypeParam = allCreatureTypes[choice - 1];
    } 

    return creatureTypeParam;
}


Quest* GameLogic::getQuestParameter(Player *aPlayer, std::string paramName, bool canBeNull){
    Quest *questParam = nullptr;
    std::vector<Quest*> allQuests;
    int choice;
    
    allQuests = manager->getGameQuests();
    choice = getPointerParameter<Quest*>(aPlayer, paramName, allQuests, canBeNull);
    if (choice == 0){
        questParam = nullptr;
    } else if (choice != -1){
        questParam = allQuests[choice - 1];
    } 

    return questParam;
}


PlayerClass* GameLogic::getPlayerClassParameter(Player *aPlayer, std::string paramName){
    PlayerClass *playerClassParam = nullptr;
    std::vector<PlayerClass*> allPlayerClasses;
    int choice;
    
    allPlayerClasses = manager->getPlayerClasses();
    choice = getPointerParameter<PlayerClass*>(aPlayer, paramName, allPlayerClasses);
    if (choice != -1){
        playerClassParam = allPlayerClasses[choice - 1];
    } 

    return playerClassParam;
}


Item* GameLogic::getItemParameter(Player *aPlayer, std::string paramName, bool canBeNull){
    Item *itemParam = nullptr;
    std::vector<Item*> allItems;
    int choice;
    
    allItems = manager->getGameItems();
    choice = getPointerParameter<Item*>(aPlayer, paramName, allItems, canBeNull);
    if (choice == 0){
        itemParam = nullptr;
    } else if (choice != -1){
        itemParam = allItems[choice - 1];
    } 

    return itemParam;
}


NonCombatant* GameLogic::getNonCombatantParameter(Player *aPlayer, std::string paramName, Quest *aQuest){
    NonCombatant *nonCombatantParam = nullptr;
    std::vector<NonCombatant*> allNonCombatants, availableNPCs;
    int choice;
    
    allNonCombatants = manager->getGameNPCs();

    if (aQuest != nullptr){
        for (auto NPC : allNonCombatants){
            if (NPC->getQuest() == aQuest){
                if (aQuest->isGiverOrReceiver(NPC) == nullptr){
                    availableNPCs.push_back(NPC);
                }
            } else if (NPC->getQuest() == nullptr){
                availableNPCs.push_back(NPC);
            }
        }
        choice = getPointerParameter<NonCombatant*>(aPlayer, paramName, availableNPCs);
    } else {
        choice = getPointerParameter<NonCombatant*>(aPlayer, paramName, allNonCombatants);
    }
    if (choice != -1){
        if (aQuest != nullptr){
            nonCombatantParam = availableNPCs[choice - 1];
        } else {
            nonCombatantParam = allNonCombatants[choice - 1];
        }
    } 

    return nonCombatantParam;
}


ObjectType GameLogic::getObjectType(std::string input){
    ObjectType type = ObjectType::NONE;

    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input.compare("area") == 0){
        type = ObjectType::AREA;
    } else if ((input.compare("armor type")  == 0) || (input.compare("armor_type") == 0)){
        type = ObjectType::ARMOR_TYPE;  
    } else if (input.compare("container") == 0){
        type = ObjectType::CONTAINER;  
    } else if (input.compare("creature") == 0) {
        type = ObjectType::CREATURE;   
    } else if ((input.compare("creature type")  == 0) || (input.compare("creature_type") == 0)){
        type = ObjectType::CREATURE_TYPE;  
    } else if (input.compare("exit") == 0){
        type = ObjectType::EXIT;   
    } else if (input.compare("feature") == 0){
        type = ObjectType::FEATURE;   
    } else if (input.compare("item") == 0){
        type = ObjectType::ITEM;   
    } else if ((input.compare("item type")  == 0) || (input.compare("item_type") == 0)){
        type = ObjectType::ITEM_TYPE;  
    } else if ((input.compare("non combatant")  == 0) || (input.compare("non_combatant") == 0) || (input.compare("noncombatant") == 0)){
        type = ObjectType::NON_COMBATANT;  
    } else if (input.compare("player") == 0){
        type = ObjectType::PLAYER;   
    } else if ((input.compare("player class")  == 0) || (input.compare("player_class") == 0)){
        type = ObjectType::PLAYER_CLASS; 
    } else if (input.compare("quest") == 0) {
        type = ObjectType::QUEST;   
    } else if ((input.compare("quest step")  == 0) || (input.compare("quest_step") == 0)){
        type = ObjectType::QUEST_STEP;  
    } else if ((input.compare("special skill")  == 0) || (input.compare("special_skill") == 0)){
        type = ObjectType::SPECIAL_SKILL;  
    } else if ((input.compare("weapon type")  == 0) || (input.compare("weapon_type") == 0)){
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
    std::cout << "\nindirect = ";
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
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    // Wait for saving to complete before executing any commands
    if (!waitForSaveOrTimeout()) {
        messagePlayer(aPlayer, "Timed out while waiting for game to save.");
        return false;
    }

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
            directObj = clarifyDirect(aPlayer, result);
            // World builder command. Edits the specified attribute of the specified object (or the current area if not specified).
            success = editAttributeCommand(aPlayer, directObj, result.indirectAlias);    
            break;
        case CommandEnum::EDIT_WIZARD:    
            // clarify direct
            directObj = clarifyDirect(aPlayer, result);
            // World builder command. Starts the edit wizard for the specified object.
            success = editWizardCommand(aPlayer, directObj);    
            break;
        case CommandEnum::SAVE:           
            // World builder command. Saves the game to the specified file (or the default file if not specified).
            success = saveCommand(aPlayer, result.directAlias);    
            break;
        case CommandEnum::LOAD:           
            // World builder command. Loads the game from the specified file (or the default file if not specified).
            success = loadCommand(aPlayer, result.directAlias);    
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

    if (result.direct.size() == 1){
        directObj = result.direct[0];
    } else if (result.direct.size() > 0){
        directObj = clarifyChoice(aPlayer, result.direct);
    }

    return directObj;
}


InteractiveNoun* GameLogic::clarifyIndirect(Player *aPlayer, parser::ParseResult result){
    InteractiveNoun *indirectObj = nullptr;

    if (result.indirect.size() == 1){
        indirectObj = result.indirect[0];
    } else if (result.indirect.size() > 0){
        indirectObj = clarifyChoice(aPlayer, result.indirect);
    }

    return indirectObj;
}


bool GameLogic::helpCommand(Player *aPlayer){
    std::string message;

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
        message = aPlayer->getLocation()->getFullDescription(aPlayer);
    } else {
        // command is look at
        message = param->look(aPlayer, &effects);
    }
    
    message += " ";
    message += handleEffects(aPlayer, effects);

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::listenCommand(Player *aPlayer){
    std::vector<EffectType> effects;
    std::string message = aPlayer->getLocation()->listen(&effects);

    if (message.compare("") == 0){
        message = "You don't hear anything.";
    }

    message += " ";
    message += handleEffects(aPlayer, effects);

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    bool success = false;
    std::vector<EffectType> effects;
    std::string message, resultMessage;

    if (directObj != nullptr){
        message = "You pick up the " + directObj->getName() + ". ";
        if ((indirectObj != nullptr) && (indirectObj->getObjectType() == ObjectType::CONTAINER) && ((directObj->getObjectType() == ObjectType::ITEM) || (directObj->getObjectType() == ObjectType::CONTAINER))){
            // command is of the form: take ___ from ___
            resultMessage = directObj->take(aPlayer, nullptr, indirectObj, nullptr, &effects);
        } else {
            resultMessage = directObj->take(aPlayer, nullptr, nullptr, nullptr, &effects);
        }
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't pick that up.";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }
    
    return success;
}


bool GameLogic::putCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, ItemPosition aPosition){
    bool success = false;
    std::vector<EffectType> effects;
    std::string message, resultMessage;

    if ((directObj != nullptr) && (indirectObj != nullptr)){
        message = "You put the " + directObj->getName();
        if (aPosition == ItemPosition::IN){
            message += " in the ";
        } else if (aPosition == ItemPosition::ON){
            message += " on the ";
        } else if (aPosition == ItemPosition::UNDER){
            message += " under the ";
        } else {
            return false;
        }
        message += indirectObj->getName() + ".";

        resultMessage = directObj->put(aPlayer, nullptr, indirectObj, aPosition, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You don't have access to the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::dropCommand(Player *aPlayer, InteractiveNoun *directObj){
    bool success = false;
    std::vector<EffectType> effects;
    std::string message, resultMessage;

    if (directObj != nullptr){
        message = "You drop the " + directObj->getName() + " on the ground.";

        resultMessage = directObj->drop(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You don't have access to the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::inventoryCommand(Player *aPlayer){
    std::string message;
    std::vector<Item*> inventory = aPlayer->getItemsInventory();
    std::vector<std::pair<EquipmentSlot, Item*>> equipment = aPlayer->getEquipped();

    message = "Your Inventory:\015\012";
    for (auto item : inventory){
        if (aPlayer->isEditMode()){
            message += item->getName() + " [" + std::to_string(item->getID()) + "] \015\012";
        } else {
            message += item->getName() + "\015\012";
        }
    }
    message += std::to_string(aPlayer->getMoney()) + " money\015\012";
    message += "Equipped Items:\015\012";
    for (auto equip : equipment){
        switch (equip.first){
            case EquipmentSlot::HEAD:
                message += "Head: ";
                break;
            case EquipmentSlot::SHOULDERS:
                message += "Shoulders: ";
                break;
            case EquipmentSlot::NECK:
                message += "Neck: ";
                break;
            case EquipmentSlot::TORSO:
                message += "Torso: ";
                break;
            case EquipmentSlot::BELT:
                message += "Belt: ";
                break;
            case EquipmentSlot::LEGS:
                message += "Legs: ";
                break;
            case EquipmentSlot::ARMS:
                message += "Arms: ";
                break;
            case EquipmentSlot::HANDS:
                message += "Hands: ";
                break;
            case EquipmentSlot::RIGHT_HAND:
                message += "Right hand: ";
                break;
            case EquipmentSlot::LEFT_HAND:
                message += "Left hand: ";
                break;
            case EquipmentSlot::FEET:
                message += "Feet: ";
                break;
            case EquipmentSlot::RIGHT_RING:
                message += "Right ring: ";
                break;
            case EquipmentSlot::LEFT_RING:
                message += "Left ring: ";
                break;
            default:
                message += "";
        }
        message += equip.second->getName() + "\015\012";
    }

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::moreCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::string message = "";

    if (directObj != nullptr){
        message = directObj->more(aPlayer);
        if (message.compare("false") == 0){
            message = "There isn't any more information available about that.";
        }
        messagePlayer(aPlayer, message);
        return true;
    }

    return false;
}


bool GameLogic::equipmentCommand(Player *aPlayer){
    std::string message;
    std::vector<std::pair<EquipmentSlot, Item*>> equipment = aPlayer->getEquipped();

    message += "Equipped Items:\015\012";
    for (auto equip : equipment){
        switch (equip.first){
            case EquipmentSlot::HEAD:
                message += "Head: ";
                break;
            case EquipmentSlot::SHOULDERS:
                message += "Shoulders: ";
                break;
            case EquipmentSlot::NECK:
                message += "Neck: ";
                break;
            case EquipmentSlot::TORSO:
                message += "Torso: ";
                break;
            case EquipmentSlot::BELT:
                message += "Belt: ";
                break;
            case EquipmentSlot::LEGS:
                message += "Legs: ";
                break;
            case EquipmentSlot::ARMS:
                message += "Arms: ";
                break;
            case EquipmentSlot::HANDS:
                message += "Hands: ";
                break;
            case EquipmentSlot::RIGHT_HAND:
                message += "Right hand: ";
                break;
            case EquipmentSlot::LEFT_HAND:
                message += "Left hand: ";
                break;
            case EquipmentSlot::FEET:
                message += "Feet: ";
                break;
            case EquipmentSlot::RIGHT_RING:
                message += "Right ring: ";
                break;
            case EquipmentSlot::LEFT_RING:
                message += "Left ring: ";
                break;
            default:
                message += "";
        }
        message += equip.second->getName() + "\015\012";
    }
    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::equipCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You equip the " + directObj->getName() + ".";
        resultMessage = directObj->equip(aPlayer, nullptr, nullptr, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't equip the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::unequipCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You unequip the " + directObj->getName() + ".";
        resultMessage = directObj->unequip(aPlayer, nullptr, nullptr, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't unequip the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


// would be better to get rid of dynamic casts *******************************************************************
bool GameLogic::transferCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;
    Player *otherPlayer = nullptr;

    if ((directObj != nullptr) && (indirectObj != nullptr)){
        message = "You give the " + directObj->getName() + " to " + indirectObj->getName() + ".";
        resultMessage = directObj->transfer(aPlayer, nullptr, nullptr, indirectObj, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't give the " + directObj->getName() + " to " + indirectObj->getName() + ".";
    } else {
        message += "\015\012" + resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);

        if (indirectObj->getObjectType() == ObjectType::PLAYER){
            otherPlayer = dynamic_cast<Player*>(indirectObj);
            if (otherPlayer != nullptr){
                message = "You receive a " + directObj->getName() + " from " + aPlayer->getName() + ".";
                messagePlayer(otherPlayer, message);
            }
        }  
    }

    return success;
}


bool GameLogic::speakCommand(Player *aPlayer, const std::string &stringParam){
    std::string message = aPlayer->getName() + " says \"" + stringParam + "\"";

    messageAreaPlayers(aPlayer, message, aPlayer->getLocation());
    return true;
}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){
    std::vector<Area*> areas;
    Area *anotherArea = nullptr;
    bool found = false;
    int i = 2;
    std::string message = aPlayer->getName() + " shouts \"" + stringParam + "\"";

    areas.push_back(aPlayer->getLocation());

    while (i > 0){
        for (auto anArea : areas){
            for (auto exit : anArea->getExits()){
                anotherArea = exit->getConnectArea();
                if (anotherArea != nullptr){
                    found = false;
                    for (auto area : areas){
                        if (area == anotherArea){
                            found = true;
                        }
                    }
                    if (!found){
                        areas.push_back(anotherArea);
                    }
                }
            }
        }
        i--;
    }

    for (auto area : areas){
        messageAreaPlayers(aPlayer, message, area);
    }

    return true;
}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *indirectObj, const std::string &stringParam){
    Player *otherPlayer = nullptr;
    std::string message = aPlayer->getName() + " whispers \"" + stringParam + "\" to you."; 

    if ((indirectObj != nullptr) && (indirectObj->getObjectType() == ObjectType::PLAYER)){
        otherPlayer = dynamic_cast<Player*>(indirectObj);
        if (otherPlayer != nullptr){
            messagePlayer(otherPlayer, message);
        }
    }
    return true;
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
    std:: string message;
    std::vector<EffectType> effects;
    bool success = false;
    Area *newArea = nullptr;
    Area *currLocation = aPlayer->getLocation();

    if (param != nullptr){
        message = param->go(aPlayer, nullptr, nullptr, &effects);
        success = true;
    }
    
    if (message.compare("false") == 0){
        message = "You can't go that way.";
    } else {
        newArea = aPlayer->getLocation();
        message += newArea->getFullDescription(aPlayer);
        messageAreaPlayers(aPlayer, "A player named " + aPlayer->getName() + " leaves the area.", currLocation);
        messageAreaPlayers(aPlayer, "You see a player named " + aPlayer->getName() + " enter the area.", newArea);
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }
    return success;
}


bool GameLogic::moveCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You move the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->move(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't move the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}

// add more Player class info?******************************************************************
bool GameLogic::statsCommand(Player *aPlayer){
    std::string message = "";

    message += "Level: " + std::to_string(aPlayer->getLevel()) + "\015\012";
    message += "Experience Points: " + std::to_string(aPlayer->getExperiencePoints()) + "\015\012";
    message += "Health: " + std::to_string(aPlayer->getCurrentHealth()) + "/" + std::to_string(aPlayer->getMaxHealth()) + "\015\012";
    message += "Special Points: " + std::to_string(aPlayer->getCurrentSpecialPts()) + "/" + std::to_string(aPlayer->getMaxSpecialPts()) + "\015\012";
    message += "Strength: " + std::to_string(aPlayer->getStrength()) + "\015\012";
    message += "Dexterity: " + std::to_string(aPlayer->getDexterity()) + "\015\012";
    message += "Intelligence: " + std::to_string(aPlayer->getIntelligence()) + "\015\012";
    message += "Attack Bonus: " + std::to_string(aPlayer->getPlayerClass()->getAttackBonus()) + "\015\012";
    message += "Armor Bonus: " + std::to_string(aPlayer->getPlayerClass()->getArmorBonus()) + "\015\012";

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::questsCommand(Player *aPlayer){
    std::string message;
    std::map<Quest*, std::pair<int, bool>> qList = aPlayer->getQuestList();

    if (qList.size() == 0){
        message = "You don't have any quests.";
    } else {
        message = "Quests:\015\012";
        for (auto quest : qList){
            message += quest.first->getName() + ": step ";
            message += std::to_string(quest.second.first);
            if (quest.second.second){
                message += " complete";
            }
            message += "\015\012";
        }
    }
    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::skillsCommand(Player *aPlayer){
    std::string message;
    SpecialSkill *aSkill = aPlayer->getPlayerClass()->getSpecialSkill();

    if (aSkill != nullptr){
        message = "Class Special Skill: \015\012";
        message += aSkill->getName() + "\015\012";
        message += "Damage: " + std::to_string(aSkill->getDamage());
        switch(aSkill->getDamageType()){
            case DamageType::NONE:
                message += " neutral damage\015\012";
                break;
            case DamageType::CRUSHING:
                message += " crushing damage\015\012";
                break;
            case DamageType::PIERCING:
                message += " piercing damage\015\012";
                break;
            case DamageType::ELECTRIC:
                message += " electric damage\015\012";
                break;
            case DamageType::FIRE:
                message += " fire damage\015\012";
                break;
            case DamageType::WATER:
                message += " water damage\015\012";
                break;
            case DamageType::WIND:
                message += " wind damage\015\012";
                break;
            case DamageType::EARTH:
                message += " earth damage\015\012";
                break;
            case DamageType::HEAL:
                message += " healing\015\012";
                break;
        }
        message += "Cost: " + std::to_string(aSkill->getCost()) + "\015\012";
        message += "Cooldown " + std::to_string(aSkill->getCooldown()) + "\015\012";
    } else {
        message = "You don't have any skills.\015\012";
    }
    messagePlayer(aPlayer, message);
    return true;
}


bool GameLogic::attackCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    // if player is in combat
        // if player is in combat with specified character and player cooldown == 0 and combat queue is empty
            // if indirectObj is not null
                // if indirectObj is a skill
                    // get damage from skill
                    // get damage type from skill
                    // get cost from skill
                    // get cooldown from skill

                    // check if player has enough special points to execute the skill

                    // check attack bonus of player

                    // check armor bonus, resistance, weakness, etc of character
                    // roll for attack success
                    // roll for attack damage + crit if relevant

                    // subtract damage from creature health
                    // subtract cost from player special points
                    // add skill cooldown to cooldown
                    // send message to player
                // if indirectObj is a weapon
                    // get damage from weapon
                    // get damage type from weapon

                    // check if weapon range is <= area size

                    // check attack bonus of player

                    // check armor bonus, resistance, weakness, etc of character
                    // roll for attack success
                    // roll for attack damage + crit if relevant

                    // subtract damage from creature health
                    // add weapon cooldown to cooldown
                    // send message to player
            // else
                // get default attack damage

                // check attack bonus of player

                // check armor bonus, resistance, weakness, etc of character
                // roll for attack success
                // roll for attack damage + crit if relevant

                // subtract damage from creature health
                // add default cooldown to cooldown
                // send message to player
        // if player is in combat with specified character, but player cooldown > 0 or combat queue is not empty
            // add command to combat queue  
        // else player is in combat with another character
            // send message to player (can't attack someone else while in combat)
    // else player is not yet in combat
        // start combat with specified attack

    // check if combat is over

    return false;
}


bool GameLogic::talkCommand(Player *aPlayer, InteractiveNoun *param){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (param != nullptr){
        message = "You greet " + param->getName() + ".\015\012";
        resultMessage = param->talk(aPlayer, nullptr, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message += param->getName() + " looks at you blankly and says nothing.";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::shopCommand(Player *aPlayer){
    std::string message = "";
    NonCombatant *aNPC = aPlayer->getInConversation();
    std::vector<Item*> inventory;
    ItemType *anItemType;

    if (aNPC != nullptr){
        inventory = aNPC->getItemsInventory();
        message = aNPC->getName() + "\'s Available Items:\015\012";
        if (inventory.size() == 0){
            message += "Nothing available for purchase right now.";
        }
        for (auto item : inventory){
            // should some items not be available for sale? ***********************************************************
            anItemType = item->getType();
            message += item->getName() + ", ";
            switch (anItemType->getRarity()){
                case ItemRarity::COMMON:
                    message += "common, ";
                    break;
                case ItemRarity::UNCOMMON:
                    message += "uncommon, ";
                    break;
                case ItemRarity::RARE:
                    message += "rare, ";
                    break;
                case ItemRarity::LEGENDARY:
                    message += "legendary, ";
                    break;
                case ItemRarity::QUEST:
                    message += "quest, ";
                    break;
            }
            message += std::to_string(anItemType->getCost()) + " money\015\012";
        }
    } else {
        message = "You need to talk to someone before you can shop.";
    }

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::buyCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::string message, resultMessage;
    std::vector<EffectType> effects;
    bool success = false;

    if (directObj != nullptr){
        resultMessage = directObj->buy(aPlayer, nullptr, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't buy the " + directObj->getName() + ".";
    } else {
        message = resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::sellCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::string message, resultMessage;
    std::vector<EffectType> effects;
    bool success = false;

    if (directObj != nullptr){
        resultMessage = directObj->sell(aPlayer, nullptr, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't sell the " + directObj->getName() + ".";
    } else {
        message = resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::searchCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::string message, resultMessage;
    std::vector<EffectType> effects;
    bool success = false;

    if (directObj != nullptr){
        resultMessage = directObj->search(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't search the " + directObj->getName() + ".";
    } else {
        message = resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


// send message to affected other player? *********************************************************************
bool GameLogic::useSkillCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    std::string message, resultMessage;
    std::vector<EffectType> effects;
    bool success = false;

    if (directObj != nullptr){
        if (indirectObj != nullptr){
            resultMessage = directObj->useSkill(aPlayer, nullptr, indirectObj, nullptr, &effects);
        } else {
            resultMessage = directObj->useSkill(aPlayer, nullptr, aPlayer, nullptr, &effects);
        }
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't use the " + directObj->getName() + " skill.";
    } else {
        message = resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::readCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You read the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->read(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't read the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::breakCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You break the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->breakIt(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't break the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::climbCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You climb the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->climb(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't climb the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::turnCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You turn the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->turn(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't turn the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::pushCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You push the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->push(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't push the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::pullCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You pull the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->pull(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't pull the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::eatCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std:: string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You eat the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->eat(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't eat the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::drinkCommand(Player *aPlayer, InteractiveNoun *directObj){
    std::vector<EffectType> effects;
    std::string message, resultMessage;
    bool success = false;

    if (directObj != nullptr){
        message = "You drink the " + directObj->getName() + ".\015\012";
        resultMessage = directObj->drink(aPlayer, &effects);
        success = true;
    }

    if (resultMessage.compare("false") == 0){
        message = "You can't drink the " + directObj->getName() + ".";
    } else {
        message += resultMessage;
    }

    if (success){ 
        message += " ";
        message += handleEffects(aPlayer, effects);
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::editModeCommand(Player *aPlayer){
std::cout << "username = " << aPlayer->getUser() << "\n";
    if (accountManager->verifyAdmin(aPlayer->getUser())){
        if (aPlayer->isEditMode()){
            aPlayer->setEditMode(false);
            messagePlayer(aPlayer, "Leaving edit mode...");
        } else {
            aPlayer->setEditMode(true);
            messagePlayer(aPlayer, "Entering edit mode...");
        }
    } else {
        messagePlayer(aPlayer, "You need to be an administrator to enter edit mode.");
    }

    return true;
} 


bool GameLogic::warpCommand(Player *aPlayer, InteractiveNoun *param){
    std::string message = "";
    Area *newArea = nullptr;
    Area *currLocation = aPlayer->getLocation();

    if ((aPlayer->isEditMode()) && (param != nullptr)){
        message = param->warp(aPlayer, nullptr);
    
        if (message.compare("false") == 0){
            message = "You can't warp there.";
        } else {
            newArea = aPlayer->getLocation();
            message += newArea->getFullDescription(aPlayer);
            messageAreaPlayers(aPlayer, "A player named " + aPlayer->getName() + " leaves the area.", currLocation);
            messageAreaPlayers(aPlayer, "You see a player named " + aPlayer->getName() + " enter the area.", newArea);
        }
    } else {
        message = "You must be in editmode to warp.";
    }

    messagePlayer(aPlayer, message);

    return true;
}


bool GameLogic::copyCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::createCommand(Player *aPlayer, const std::string &stringParam){
    std::string message = "";
    bool sendMessage = false;
    ObjectType anObjectType = getObjectType(stringParam);
    bool success = false;

    if (aPlayer->isEditMode()){
        if (anObjectType != ObjectType::NONE){
            success = createObject(aPlayer, anObjectType);
        } else {
            message = "I don't understand what you're trying to create.";
            sendMessage = true;
        }
    } else {
        message = "You must be in editmode to create.";
        sendMessage = true;
    }

    if (sendMessage){
        messagePlayer(aPlayer, message);
    }

    return success;
}


bool GameLogic::editAttributeCommand(Player *aPlayer, InteractiveNoun *directObj, std::string stringParam){
    bool success = false;
    ObjectType anObjectType;

    std::transform(stringParam.begin(), stringParam.end(), stringParam.begin(), ::tolower);

    if ((directObj != nullptr) && (aPlayer->isEditMode())){
        anObjectType = directObj->getObjectType();
        switch (anObjectType){
            case ObjectType::NONE:
                messagePlayer(aPlayer, "I don't know what you mean by that.");
                break;
            case ObjectType::AREA:
                success = editAttributeOfArea(aPlayer, directObj, stringParam);
                break;
            case ObjectType::ARMOR_TYPE:
                success = editAttributeOfArmorType(aPlayer, directObj, stringParam);
                break;
            case ObjectType::CONTAINER:
                success = editAttributeOfContainer(aPlayer, directObj, stringParam);
                break;
            case ObjectType::CREATURE:
                success = editAttributeOfCreature(aPlayer, directObj, stringParam);
                break;
            case ObjectType::CREATURE_TYPE:
                success = editAttributeOfCreatureType(aPlayer, directObj, stringParam);
                break;
            case ObjectType::EXIT:
                success = editAttributeOfExit(aPlayer, directObj, stringParam);
                break;
            case ObjectType::FEATURE:
                success = editAttributeOfFeature(aPlayer, directObj, stringParam);
                break;
            case ObjectType::ITEM:
                success = editAttributeOfItem(aPlayer, directObj, stringParam);
                break;
            case ObjectType::ITEM_TYPE:
                success = editAttributeOfItemType(aPlayer, directObj, stringParam);
                break;
            case ObjectType::NON_COMBATANT:
                success = editAttributeOfNonCombatant(aPlayer, directObj, stringParam);
                break;
            case ObjectType::PLAYER:
                success = editAttributeOfPlayer(aPlayer, directObj, stringParam);
                break;
            case ObjectType::PLAYER_CLASS:
                success = editAttributeOfPlayerClass(aPlayer, directObj, stringParam);
                break;
            case ObjectType::QUEST:
                success = editAttributeOfQuest(aPlayer, directObj, stringParam);
                break;
            case ObjectType::QUEST_STEP:
                success = editAttributeOfQuestStep(aPlayer, directObj, stringParam);
                break;
            case ObjectType::SPECIAL_SKILL:
                success = editAttributeOfSpecialSkill(aPlayer, directObj, stringParam);
                break;
            case ObjectType::WEAPON_TYPE:
                success = editAttributeOfWeaponType(aPlayer, directObj, stringParam);
                break;
        }
    } else {
        messagePlayer(aPlayer, "You must be in editmode to edit that.");
    }

    return success;
}


bool GameLogic::editWizardCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::saveCommand(Player *aPlayer, const std::string &stringParam){
    bool success = false;

    if (aPlayer->isEditMode()) {
        saving.store(true);

        gamedata::DataManager dm;

        if (stringParam.empty()) {
            if (!currentFilename.empty()) {
                // use current fileName
                std::cout << "Saving " << currentFilename << std::endl;
                success = dm.saveGame(currentFilename, manager, startArea->getID());
                if (success) accountManager->saveToDisk();
            }
            else {
                messagePlayer(aPlayer, "You must specify a filename");
            }
        }
        else {
            success = dm.saveGame(stringParam, manager, startArea->getID());
            if (success) {
                accountManager->setFileName(stringParam);
                accountManager->saveToDisk();
                currentFilename = stringParam;
            }
        }
        if (success) {
            messagePlayer(aPlayer, "Successfully saved " + stringParam);
        }
        else {
            messagePlayer(aPlayer, "An error occurred while saving " + stringParam);
        }

        saving.store(false);
    }
    else {
        messagePlayer(aPlayer, "You must be in edit mode to do this.");
    }

    return success;
}


bool GameLogic::loadCommand(Player *aPlayer, const std::string &stringParam){
    bool success = false;
    int startAreaId = -1;

    if (aPlayer->isEditMode()) {
        gamedata::DataManager dm;

        // Create new manager to load game data into
        GameObjectManager *gom = new GameObjectManager();
        success = dm.loadGame(stringParam, gom, startAreaId);

        // If load was successful, delete the current game world
        // and replace it with the loaded one
        if (success) {
            currentFilename = stringParam;
            messagePlayer(aPlayer, "Successfully loaded " + stringParam);
            // Tell everyone the server is shutting down
            messagePlayer(aPlayer, "Shutting down server. Please reconnect.");
            messageAllPlayers(aPlayer, "Shutting down server. Please reconnect.");
            // Get the current server information
            int port = theServer->getServerPort();
            int maxPlayers = theServer->getMaxPlayers();
            int timeout = theServer->getTimeOut();
            // Pause the server
            theServer->pause(true);
            // Disconnect all players
            for (int plr : manager->getPlayersFDs()) {
                theServer->disconnectPlayer(plr);
            }

            // Shut down the server to disconnect all players
            theServer->shutDownServer();

            // Change to the new account information
            accountManager->setFileName(stringParam + ".accounts");
            accountManager->initialize();

            // Delete the current game world and replace it with the new one
            delete manager;
            manager = gom;

            // Set the starting area
            startArea = static_cast<Area*>(manager->getPointer(startAreaId));

            // Restart the server
            theServer->initServer(port, maxPlayers, timeout, this);
        }
        else {
            messagePlayer(aPlayer, "An error occurred while loading " + stringParam);
        }
    }
    else {
        messagePlayer(aPlayer, "You must be in edit mode to do this.");
    }
    
    return success;
}


bool GameLogic::deleteCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}

}}