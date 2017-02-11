/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
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

}


GameObjectManager::~GameObjectManager(){

}


int GameObjectManager::addObject(SerializableNoun *anObject, int objectType){

}


int GameObjectManager::removeObject(SerializableNoun *anObject, int objectType){

}


int GameObjectManager::getID(SerializableNoun *anObject){

}


SerializableNoun* GameObjectManager::getPointer(int){

}


std::vector<Creature*> GameObjectManager::getCreatures(){

}


std::vector<Player*> GameObjectManager::getPlayers(){

}

}}