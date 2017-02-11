/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        GameObjectManager.hpp
 *
 * \details     Header file for GameObjectManager class. Defines the members 
 *              and functions needed to track all game objects.
 ************************************************************************/

#ifndef GAME_OBJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include <string>
#include <vector>
#include <map>

namespace legacymud { namespace engine {

class SerializableNoun;
class Creature;
class Player;

class GameObjectManager {
    public:
        GameObjectManager();
        GameObjectManager(const GameObjectManager &otherGameObjectManager);
        GameObjectManager & operator=(const GameObjectManager &otherGameObjectManager);
        ~GameObjectManager();
        int addObject(SerializableNoun *anObject, int objectType);
        int removeObject(SerializableNoun *anObject, int objectType);
        int getID(SerializableNoun *anObject);
        SerializableNoun* getPointer(int);
        std::vector<Creature*> getCreatures();
        std::vector<Player*> getPlayers();
    private:
        std::map<int, SerializableNoun*> gameObjects;
        std::vector<Creature*> gameCreatures;
        std::vector<Player*> gamePlayers;
};

}}
 
#endif