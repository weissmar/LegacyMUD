/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
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
    gamePlayerClasses.clear();
}


bool GameObjectManager::addObject(InteractiveNoun *anObject, int FD){
    int anID;
    ObjectType aType;
    bool success = false;
    Creature *aCreature = nullptr;
    Player *aPlayer = nullptr;
    PlayerClass *aPlayerClass = nullptr;

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


InteractiveNoun* GameObjectManager::getPointer(int ID){
    std::lock_guard<std::mutex> gameObjectsLock(gameObjectsMutex);
    int result = gameObjects.count(ID);

    if (result == 1){
        return gameObjects.at(ID);
    } else {
        return nullptr;
    }
}


std::vector<Creature*> GameObjectManager::getCreatures(){
    std::lock_guard<std::mutex> gameCreaturesLock(gameCreaturesMutex);
    std::vector<Creature*> creatureVector;

    for (auto creature : gameCreatures){
        creatureVector.push_back(creature.second);
    }
    return creatureVector;
}


std::vector<Player*> GameObjectManager::getPlayersPtrs(){
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    std::vector<Player*> playerVector;

    for (auto player : activeGamePlayers){
        playerVector.push_back(player.second);
    }
    return playerVector;
}


std::vector<int> GameObjectManager::getPlayersFDs(){
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    std::vector<int> fdVector;

    for (auto player : activeGamePlayers){
        fdVector.push_back(player.first);
    }
    return fdVector;
}


Player* GameObjectManager::getPlayerByFD(int fileDescriptor){
    std::lock_guard<std::mutex> activeGamePlayersLock(activeGamePlayersMutex);
    int result = activeGamePlayers.count(fileDescriptor);

    if (result == 1){
        return activeGamePlayers.at(fileDescriptor);
    } else {
        return nullptr;
    }
}


Player* GameObjectManager::getPlayerByUsername(std::string username){
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

std::vector<PlayerClass*> GameObjectManager::getPlayerClasses(){
    std::lock_guard<std::mutex> gamePlayerClassesLock(gamePlayerClassesMutex);
    return gamePlayerClasses;
}

}}