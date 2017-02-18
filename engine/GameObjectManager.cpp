/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/16/2017
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
            gameObjects[anID] = anObject;
            if ((aType == ObjectType::PLAYER) && (FD >= 0)){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    activeGamePlayers[FD] = aPlayer;
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    inactivePlayers[aPlayer->getUser()] = aPlayer;
                    success = true;
                }
            } else if (aType == ObjectType::CREATURE){
                aCreature = dynamic_cast<Creature*>(anObject);
                if (aCreature != nullptr){
                    gameCreatures[anID] = aCreature;
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER_CLASS){
                aPlayerClass = dynamic_cast<PlayerClass*>(anObject);
                if (aPlayerClass != nullptr){
                    gamePlayerClasses.push_back(aPlayerClass);
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
            numRemoved = gameObjects.erase(anID);
            if ((aType == ObjectType::PLAYER) && (FD >= 0)){
                numRemoved += activeGamePlayers.erase(FD);
                if (numRemoved == 2){
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER){
                aPlayer = dynamic_cast<Player*>(anObject);
                if (aPlayer != nullptr){
                    numRemoved += inactivePlayers.erase(aPlayer->getUser());
                    if (numRemoved == 2){
                        success = true;
                    }
                }
            } else if (aType == ObjectType::CREATURE){
                numRemoved += gameCreatures.erase(anID);
                if (numRemoved == 2){
                    success = true;
                }
            } else if (aType == ObjectType::PLAYER_CLASS){
                aPlayerClass = dynamic_cast<PlayerClass*>(anObject);
                size = gamePlayerClasses.size();
                gamePlayerClasses.erase(std::remove(gamePlayerClasses.begin(), gamePlayerClasses.end(), aPlayerClass), gamePlayerClasses.end());
                if ((numRemoved == 1) && ((gamePlayerClasses.size() - size) == 1)){
                    success = true;
                }
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
    int result = gameObjects.count(ID);

    if (result == 1){
        return gameObjects.at(ID);
    } else {
        return nullptr;
    }
}


std::vector<Creature*> GameObjectManager::getCreatures(){
    std::vector<Creature*> creatureVector;

    for (auto creature : gameCreatures){
        creatureVector.push_back(creature.second);
    }
    return creatureVector;
}


std::vector<Player*> GameObjectManager::getPlayersPtrs(){
    std::vector<Player*> playerVector;

    for (auto player : activeGamePlayers){
        playerVector.push_back(player.second);
    }
    return playerVector;
}


std::vector<int> GameObjectManager::getPlayersFDs(){
    std::vector<int> fdVector;

    for (auto player : activeGamePlayers){
        fdVector.push_back(player.first);
    }
    return fdVector;
}


Player* GameObjectManager::getPlayerByFD(int fileDescriptor){
    int result = activeGamePlayers.count(fileDescriptor);

    if (result == 1){
        return activeGamePlayers.at(fileDescriptor);
    } else {
        return nullptr;
    }
}


Player* GameObjectManager::getPlayerByUsername(std::string username){
    int result = inactivePlayers.count(username);

    if (result == 1){
        return inactivePlayers.at(username);
    } else {
        return nullptr;
    }
}


bool GameObjectManager::loadPlayer(std::string username, int FD){
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
    return gamePlayerClasses;
}

}}