/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/05/2017
 * \course      CS467, Winter 2017
 * \file        GameObjectManager.cpp
 *
 * \details     Implementation file for GameObjectManager class. 
 ************************************************************************/

#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Creature.hpp"
#include "Player.hpp"
#include "PlayerClass.hpp"
#include "Area.hpp"
#include "SpecialSkill.hpp"
#include "ItemType.hpp"
#include "NonCombatant.hpp"
#include "Container.hpp"
#include "CreatureType.hpp"
#include "Quest.hpp"
#include "Item.hpp"
#include <algorithm>

namespace legacymud { namespace engine {

GameObjectManager::GameObjectManager(){ }


GameObjectManager::GameObjectManager(const GameObjectManager &otherGameObjectManager){ }


GameObjectManager & GameObjectManager::operator=(const GameObjectManager &otherGameObjectManager){
    return *this;
}


GameObjectManager::~GameObjectManager(){
    // release memory for any objects still held
    for (auto object : gameObjects){
        if (object.second != nullptr){
            delete object.second;
        }
    }

    // empty data structures
    gameObjects.clear();
    gameCreatures.clear();
    activeGamePlayers.clear();
    inactivePlayers.clear();
    gamePlayerClasses.clear();
    gameAreas.clear();
    gameSkills.clear();
    gameItemTypes.clear();
    gameNPCs.clear();
    gameContainers.clear();
    gameCreatureTypes.clear();
    gameQuests.clear();
    gameItems.clear();
}


bool GameObjectManager::addObject(InteractiveNoun *anObject, int FD){
    int anID;
    ObjectType aType;
    bool success = false;
    Creature *aCreature = nullptr;
    Player *aPlayer = nullptr;
    PlayerClass *aPlayerClass = nullptr;
    Area *anArea = nullptr;
    SpecialSkill *aSkill = nullptr;
    ItemType *anItemType = nullptr;
    NonCombatant *aNPC = nullptr;
    Container *aContainer = nullptr;
    CreatureType *aCreatureType = nullptr;
    Quest *aQuest = nullptr;
    Item *anItem = nullptr;

    if (anObject != nullptr){
        aType = anObject->getObjectType();
        anID = anObject->getID();
        if (anID >= 0){
            std::unique_lock<std::mutex> gameObjectsLock(gameObjectsMutex);
            gameObjects[anID] = anObject;
            gameObjectsLock.unlock();
            if ((aType == ObjectType::PLAYER) && (FD >= 0)){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    std::unique_lock<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
                    activeGamePlayers[FD] = aPlayer;
                    activeGamePlayersLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    std::unique_lock<std::mutex> inactivePlayersLock(inactivePlayersMutex);
                    inactivePlayers[aPlayer->getUser()] = aPlayer;
                    inactivePlayersLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::CREATURE){
                aCreature = dynamic_cast<Creature*>(anObject);
                if (aCreature != nullptr){
                    std::unique_lock<std::mutex> gameCreaturesLock(gameCreaturesMutex);
                    gameCreatures[anID] = aCreature;
                    gameCreaturesLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER_CLASS){
                aPlayerClass = dynamic_cast<PlayerClass*>(anObject);
                if (aPlayerClass != nullptr){
                    std::unique_lock<std::mutex> gamePlayerClassesLock(gamePlayerClassesMutex);
                    gamePlayerClasses.push_back(aPlayerClass);
                    gamePlayerClassesLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::AREA){
                anArea = dynamic_cast<Area*>(anObject);
                if (anArea != nullptr){
                    std::unique_lock<std::mutex> gameAreasLock(gameAreasMutex);
                    gameAreas.push_back(anArea);
                    gameAreasLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::SPECIAL_SKILL){
                aSkill = dynamic_cast<SpecialSkill*>(anObject);
                if (aSkill != nullptr){
                    std::unique_lock<std::mutex> gameSkillsLock(gameSkillsMutex);
                    gameSkills.push_back(aSkill);
                    gameSkillsLock.unlock();
                    success = true;
                }
            } else if ((aType == ObjectType::ITEM_TYPE) || (aType == ObjectType::ARMOR_TYPE) || (aType == ObjectType::WEAPON_TYPE)){
                anItemType = dynamic_cast<ItemType*>(anObject);
                if (anItemType != nullptr){
                    std::unique_lock<std::mutex> gameItemTypesLock(gameItemTypesMutex);
                    gameItemTypes.push_back(anItemType);
                    gameItemTypesLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::NON_COMBATANT){
                aNPC = dynamic_cast<NonCombatant*>(anObject);
                if (aNPC != nullptr){
                    std::unique_lock<std::mutex> gameNPCsLock(gameNPCsMutex);
                    gameNPCs.push_back(aNPC);
                    gameNPCsLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anObject);
                if (aContainer != nullptr){
                    std::unique_lock<std::mutex> gameContainersLock(gameContainersMutex);
                    gameContainers.push_back(aContainer);
                    gameContainersLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::CREATURE_TYPE){
                aCreatureType = dynamic_cast<CreatureType*>(anObject);
                if (aCreatureType != nullptr){
                    std::unique_lock<std::mutex> gameCreatureTypesLock(gameCreatureTypesMutex);
                    gameCreatureTypes.push_back(aCreatureType);
                    gameCreatureTypesLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::QUEST){
                aQuest = dynamic_cast<Quest*>(anObject);
                if (aQuest != nullptr){
                    std::unique_lock<std::mutex> gameQuestsLock(gameQuestsMutex);
                    gameQuests.push_back(aQuest);
                    gameQuestsLock.unlock();
                    success = true;
                }
            } else if (aType == ObjectType::ITEM){
                anItem = dynamic_cast<Item*>(anObject);
                if (anItem != nullptr){
                    std::unique_lock<std::mutex> gameItemsLock(gameItemsMutex);
                    gameItems.push_back(anItem);
                    gameItemsLock.unlock();
                    success = true;
                }
            } else {
                success = true;
            }
        }
    }

    return success;
}


bool GameObjectManager::removeObject(InteractiveNoun *anObject, int FD){
    int anID; 
    ObjectType aType;
    bool success = false;
    int numRemoved;
    Player *aPlayer = nullptr;
    PlayerClass *aPlayerClass = nullptr;
    Area *anArea = nullptr;
    SpecialSkill *aSkill = nullptr;
    ItemType *anItemType = nullptr;
    NonCombatant *aNPC = nullptr;
    Container *aContainer = nullptr;
    CreatureType *aCreatureType = nullptr;
    Quest *aQuest = nullptr;
    Item *anItem = nullptr;
    int size;

    if (anObject != nullptr){
        aType = anObject->getObjectType();
        anID = anObject->getID();
        if (anID >= 0){
            std::unique_lock<std::mutex> gameObjectsLock(gameObjectsMutex);
            numRemoved = gameObjects.erase(anID);
            gameObjectsLock.unlock();
            if ((aType == ObjectType::PLAYER) && (FD >= 0)){
                std::unique_lock<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
                numRemoved += activeGamePlayers.erase(FD);
                activeGamePlayersLock.unlock();
                if (numRemoved == 2){
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    std::unique_lock<std::mutex> inactivePlayersLock(inactivePlayersMutex);
                    numRemoved += inactivePlayers.erase(aPlayer->getUser());
                    inactivePlayersLock.unlock();
                    if (numRemoved == 2){
                        success = true;
                    }
                }
            } else if (aType == ObjectType::CREATURE){
                std::unique_lock<std::mutex> gameCreaturesLock(gameCreaturesMutex);
                numRemoved += gameCreatures.erase(anID);
                gameCreaturesLock.unlock();
                if (numRemoved == 2){
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER_CLASS){
                aPlayerClass = dynamic_cast<PlayerClass*>(anObject);
                std::unique_lock<std::mutex> gamePlayerClassesLock(gamePlayerClassesMutex);
                size = gamePlayerClasses.size();
                gamePlayerClasses.erase(std::remove(gamePlayerClasses.begin(), gamePlayerClasses.end(), aPlayerClass), gamePlayerClasses.end());
                if ((numRemoved == 1) && ((gamePlayerClasses.size() - size) == 1)){
                    success = true;
                }
                gamePlayerClassesLock.unlock();
            } else if (aType == ObjectType::AREA){
                anArea = dynamic_cast<Area*>(anObject);
                std::unique_lock<std::mutex> gameAreasLock(gameAreasMutex);
                size = gameAreas.size();
                gameAreas.erase(std::remove(gameAreas.begin(), gameAreas.end(), anArea), gameAreas.end());
                if ((numRemoved == 1) && ((gameAreas.size() - size) == 1)){
                    success = true;
                }
                gameAreasLock.unlock();
            } else if (aType == ObjectType::SPECIAL_SKILL){
                aSkill = dynamic_cast<SpecialSkill*>(anObject);
                std::unique_lock<std::mutex> gameSkillsLock(gameSkillsMutex);
                size = gameSkills.size();
                gameSkills.erase(std::remove(gameSkills.begin(), gameSkills.end(), aSkill), gameSkills.end());
                if ((numRemoved == 1) && ((gameSkills.size() - size) == 1)){
                    success = true;
                }
                gameSkillsLock.unlock();
            } else if (aType == ObjectType::ITEM_TYPE){
                anItemType = dynamic_cast<ItemType*>(anObject);
                std::unique_lock<std::mutex> gameItemTypesLock(gameItemTypesMutex);
                size = gameItemTypes.size();
                gameItemTypes.erase(std::remove(gameItemTypes.begin(), gameItemTypes.end(), anItemType), gameItemTypes.end());
                if ((numRemoved == 1) && ((gameItemTypes.size() - size) == 1)){
                    success = true;
                }
                gameItemTypesLock.unlock();
            } else if (aType == ObjectType::NON_COMBATANT){
                aNPC = dynamic_cast<NonCombatant*>(anObject);
                std::unique_lock<std::mutex> gameNPCsLock(gameNPCsMutex);
                size = gameNPCs.size();
                gameNPCs.erase(std::remove(gameNPCs.begin(), gameNPCs.end(), aNPC), gameNPCs.end());
                if ((numRemoved == 1) && ((gameNPCs.size() - size) == 1)){
                    success = true;
                }
                gameNPCsLock.unlock();
            } else if (aType == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anObject);
                std::unique_lock<std::mutex> gameContainersLock(gameContainersMutex);
                size = gameContainers.size();
                gameContainers.erase(std::remove(gameContainers.begin(), gameContainers.end(), aContainer), gameContainers.end());
                if ((numRemoved == 1) && ((gameContainers.size() - size) == 1)){
                    success = true;
                }
                gameContainersLock.unlock();
            } else if (aType == ObjectType::CREATURE_TYPE){
                aCreatureType = dynamic_cast<CreatureType*>(anObject);
                std::unique_lock<std::mutex> gameCreatureTypesLock(gameCreatureTypesMutex);
                size = gameCreatureTypes.size();
                gameCreatureTypes.erase(std::remove(gameCreatureTypes.begin(), gameCreatureTypes.end(), aCreatureType), gameCreatureTypes.end());
                if ((numRemoved == 1) && ((gameCreatureTypes.size() - size) == 1)){
                    success = true;
                }
                gameCreatureTypesLock.unlock();
            } else if (aType == ObjectType::QUEST){
                aQuest = dynamic_cast<Quest*>(anObject);
                std::unique_lock<std::mutex> gameQuestsLock(gameQuestsMutex);
                size = gameQuests.size();
                gameQuests.erase(std::remove(gameQuests.begin(), gameQuests.end(), aQuest), gameQuests.end());
                if ((numRemoved == 1) && ((gameQuests.size() - size) == 1)){
                    success = true;
                }
                gameQuestsLock.unlock();
            } else if (aType == ObjectType::ITEM){
                anItem = dynamic_cast<Item*>(anObject);
                std::unique_lock<std::mutex> gameItemsLock(gameItemsMutex);
                size = gameItems.size();
                gameItems.erase(std::remove(gameItems.begin(), gameItems.end(), anItem), gameItems.end());
                if ((numRemoved == 1) && ((gameItems.size() - size) == 1)){
                    success = true;
                }
                gameItemsLock.unlock();
            } else if (numRemoved == 1) {
                success = true;
            }
        }
        // if successfully removed, release memory
        if (success){
            delete anObject;
            anObject = nullptr;
        }
    }

    return success;
}


std::map<int, InteractiveNoun*> GameObjectManager::getAllObjects() const{
    std::lock_guard<std::mutex> gameObjectsLock(gameObjectsMutex);

    return gameObjects;
}


InteractiveNoun* GameObjectManager::getPointer(int ID) const{
    std::lock_guard<std::mutex> gameObjectsLock(gameObjectsMutex);
    int result = gameObjects.count(ID);

    if (result == 1){
        return gameObjects.at(ID);
    } else {
        return nullptr;
    }
}


std::vector<Creature*> GameObjectManager::getCreatures() const{
    std::lock_guard<std::mutex> gameCreaturesLock(gameCreaturesMutex);
    std::vector<Creature*> creatureVector;

    for (auto creature : gameCreatures){
        creatureVector.push_back(creature.second);
    }
    return creatureVector;
}


std::vector<Player*> GameObjectManager::getPlayersPtrs() const{
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    std::vector<Player*> playerVector;

    for (auto player : activeGamePlayers){
        playerVector.push_back(player.second);
    }
    return playerVector;
}


std::vector<int> GameObjectManager::getPlayersFDs() const{
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    std::vector<int> fdVector;

    for (auto player : activeGamePlayers){
        fdVector.push_back(player.first);
    }
    return fdVector;
}


Player* GameObjectManager::getPlayerByFD(int fileDescriptor) const{
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    int result = activeGamePlayers.count(fileDescriptor);

    if (result == 1){
        return activeGamePlayers.at(fileDescriptor);
    } else {
        return nullptr;
    }
}


Player* GameObjectManager::getPlayerByUsername(std::string username) const{
    std::lock_guard<std::mutex> inactivePlayersLock(inactivePlayersMutex);
    int result = inactivePlayers.count(username);

    if (result == 1){
        return inactivePlayers.at(username);
    } else {
        return nullptr;
    }
}


bool GameObjectManager::loadPlayer(std::string username, int FD){
    std::unique_lock<std::mutex> activeGamePlayersLock(activeGamePlayersMutex, std::defer_lock);
    std::unique_lock<std::mutex> inactivePlayersLock(inactivePlayersMutex, std::defer_lock);
    std::lock(activeGamePlayersLock, inactivePlayersLock);
    int result = inactivePlayers.count(username);

    if ((result == 1) && (FD >= 0)){
        activeGamePlayers[FD] = inactivePlayers.at(username);
        inactivePlayers.erase(username);
        return true;
    } else {
        return false;
    }
}


bool GameObjectManager::hibernatePlayer(int FD){
    std::unique_lock<std::mutex> activeGamePlayersLock(activeGamePlayersMutex, std::defer_lock);
    std::unique_lock<std::mutex> inactivePlayersLock(inactivePlayersMutex, std::defer_lock);
    std::lock(activeGamePlayersLock, inactivePlayersLock);
    int result = activeGamePlayers.count(FD);
    Player *aPlayer = nullptr;

    if (result == 1){
        aPlayer = activeGamePlayers.at(FD);
        inactivePlayers[aPlayer->getUser()] = aPlayer;
        activeGamePlayers.erase(FD);
        return true;
    } else {
        return false;
    }
}


std::vector<PlayerClass*> GameObjectManager::getPlayerClasses() const{
    std::lock_guard<std::mutex> gamePlayerClassesLock(gamePlayerClassesMutex);
    return gamePlayerClasses;
}


std::vector<Area*> GameObjectManager::getGameAreas() const{
    std::lock_guard<std::mutex> gameAreasLock(gameAreasMutex);
    return gameAreas;
}


std::vector<SpecialSkill*> GameObjectManager::getGameSkills() const{
    std::lock_guard<std::mutex> gameSkillsLock(gameSkillsMutex);
    return gameSkills;
}


std::vector<ItemType*> GameObjectManager::getGameItemTypes() const{
    std::lock_guard<std::mutex> gameItemTypesLock(gameItemTypesMutex);
    return gameItemTypes;
}


std::vector<NonCombatant*> GameObjectManager::getGameNPCs() const{
    std::lock_guard<std::mutex> gameNPCsLock(gameNPCsMutex);
    return gameNPCs;
}


std::vector<Container*> GameObjectManager::getGameContainers() const{
    std::lock_guard<std::mutex> gameContainersLock(gameContainersMutex);
    return gameContainers;
}


std::vector<CreatureType*> GameObjectManager::getGameCreatureTypes() const{
    std::lock_guard<std::mutex> gameCreatureTypesLock(gameCreatureTypesMutex);
    return gameCreatureTypes;
}


std::vector<Quest*> GameObjectManager::getGameQuests() const{
    std::lock_guard<std::mutex> gameQuestsLock(gameQuestsMutex);
    return gameQuests;
}


std::vector<Item*> GameObjectManager::getGameItems() const{
    std::lock_guard<std::mutex> gameItemsLock(gameItemsMutex);
    return gameItems;
}

std::vector<Player*> GameObjectManager::getGamePlayers() const{
    std::vector<Player*> allPlayers;
    std::unique_lock<std::mutex> activeGamePlayersLock(activeGamePlayersMutex, std::defer_lock);
    std::unique_lock<std::mutex> inactivePlayersLock(inactivePlayersMutex, std::defer_lock);
    std::lock(activeGamePlayersLock, inactivePlayersLock);

    for (auto player : activeGamePlayers){
        allPlayers.push_back(player.second);
    }
    for (auto player : inactivePlayers){
        allPlayers.push_back(player.second);
    }

    return allPlayers;
}

}}