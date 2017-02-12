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
#include "SerializableNoun.hpp"
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


int GameObjectManager::addObject(SerializableNoun *anObject, int objectType){
    return -1;
}


int GameObjectManager::removeObject(SerializableNoun *anObject, int objectType){
    return -1;
}


int GameObjectManager::getID(SerializableNoun *anObject){
    return -1;
}


SerializableNoun* GameObjectManager::getPointer(int){
    return nullptr;
}


std::vector<Creature*> GameObjectManager::getCreatures(){
    return gameCreatures;
}


std::vector<Player*> GameObjectManager::getPlayers(){
    return gamePlayers;
}

}}