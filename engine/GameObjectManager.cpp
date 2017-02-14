/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/13/2017
 * \course      CS467, Winter 2017
 * \file        GameObjectManager.cpp
 *
 * \details     Implementation file for GameObjectManager class. 
 ************************************************************************/

#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Creature.hpp"
#include "Player.hpp"

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
    gamePlayers.clear();
}


bool GameObjectManager::addObject(InteractiveNoun *anObject, int FD){
    int anID = anObject->getID();
    ObjectType aType = anObject->getObjectType();
    bool success = false;
    Creature *aCreature = nullptr;

    if (anID >= 0){
        gameObjects[anID] = anObject;
        if ((aType == ObjectType::PLAYER) && (FD >= 0)){
            gamePlayers[FD] = dynamic_cast<Player*>(anObject);
            if (gamePlayers[FD] != nullptr){
                success = true;
            } else {
                gamePlayers.erase(FD);
            }
        } else if (aType == ObjectType::CREATURE){
            aCreature = dynamic_cast<Creature*>(anObject);
            if (aCreature != nullptr){
                gameCreatures[anID] = aCreature;
                success = true;
            }
        } else {
            success = true;
        }
    }

    return success;
}


bool GameObjectManager::removeObject(InteractiveNoun *anObject, int FD){
    int anID = anObject->getID();
    ObjectType aType = anObject->getObjectType();
    bool success = false;
    int numRemoved;

    if (anID >= 0){
        numRemoved = gameObjects.erase(anID);
        if ((aType == ObjectType::PLAYER) && (FD >= 0)){
            numRemoved += gamePlayers.erase(FD);
            if (numRemoved == 2){
                success = true;
            }
        } else if (aType == ObjectType::CREATURE){
            numRemoved += gameCreatures.erase(anID);
            if (numRemoved == 2){
                success = true;
            }
        } else if (numRemoved == 1) {
            success = true;
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


std::vector<Player*> GameObjectManager::getPlayers(){
    std::vector<Player*> playerVector;

    for (auto player : gamePlayers){
        playerVector.push_back(player.second);
    }
    return playerVector;
}


Player* GameObjectManager::getPlayerByFD(int fileDescriptor){
        int result = gamePlayers.count(fileDescriptor);

    if (result == 1){
        return gamePlayers.at(fileDescriptor);
    } else {
        return nullptr;
    }
}

}}