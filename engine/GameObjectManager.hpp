/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameObjectManager.hpp
 *
 * Overview:
 *     Header file for GameObjectManager base class. Defines the members and 
 * functions that apply to all GameObjectManagers.
 ************************************************************************/

#ifndef GAME_OBJECT_MANAGER_HPP
#define GAME_OBJECT_MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include "SerializableNoun.hpp"
#include "Creature.hpp"
#include "Player.hpp"

namespace legacymud { namespace engine {

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