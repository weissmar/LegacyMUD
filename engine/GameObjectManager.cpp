/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/12/2017
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

GameObjectManager::GameObjectManager(){

}


GameObjectManager::GameObjectManager(const GameObjectManager &otherGameObjectManager){

}


GameObjectManager & GameObjectManager::operator=(const GameObjectManager &otherGameObjectManager){
    return *this;
}


GameObjectManager::~GameObjectManager(){

}


int GameObjectManager::addObject(InteractiveNoun *anObject, int objectType){
    return -1;
}


int GameObjectManager::removeObject(InteractiveNoun *anObject, int objectType){
    return -1;
}


InteractiveNoun* GameObjectManager::getPointer(int){
    return nullptr;
}


std::vector<Creature*> GameObjectManager::getCreatures(){
    return gameCreatures;
}


std::vector<Player*> GameObjectManager::getPlayers(){
    std::vector<Player*> playerVector;

    for (auto player : gamePlayers){
        playerVector.push_back(player.second);
    }
    return playerVector;
}


Player* GameObjectManager::getPlayerByFD(int fileDescriptor){
    return nullptr;
}

}}