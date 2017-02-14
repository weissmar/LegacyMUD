/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/13/2017
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

class InteractiveNoun;
class Creature;
class Player;

/*!
 * \details     This class tracks all game objects. It should only be 
 *              instantiated once per instance of the game.
 */
class GameObjectManager {
    public:
        GameObjectManager();
        GameObjectManager(const GameObjectManager &otherGameObjectManager);
        GameObjectManager & operator=(const GameObjectManager &otherGameObjectManager);
        ~GameObjectManager();

        /*!
         * \brief   Adds the specified object to the game manager.
         *
         * \param[in] anObject      Specifies the object to be added.
         * \param[in] objectType    Specifies the type of the object.
         *
         * \return  Returns a int with the ID of the added object, if adding the 
         *          object was successful. Otherwise, returns -1.
         */
        int addObject(InteractiveNoun *anObject, int objectType);

        /*!
         * \brief   Removes the specified object from the game manager.
         *
         * \param[in] anObject      Specifies the object to be removed.
         * \param[in] objectType    Specifies the type of the object.
         *
         * \return  Returns a int with the ID of the removed object, if removing the 
         *          object was successful. Otherwise, returns -1.
         */
        int removeObject(InteractiveNoun *anObject, int objectType);

        /*!
         * \brief   Gets a pointer to the object indicated by the specified
         *          ID.
         *
         * \param[in] ID    Specifies the ID of the object to be returned.
         *
         * \return  Returns a SerializableNoun* with the object, if found, otherwise
         *          nullptr.
         */
        InteractiveNoun* getPointer(int ID);

        /*!
         * \brief   Gets the list of creatures in the game.
         *
         * \return  Returns a std::vector<Creature*> with the creatures in
         *          the game.
         */
        std::vector<Creature*> getCreatures();

        /*!
         * \brief   Gets the list of players in the game.
         *
         * \return  Returns a std::vector<Player*> with the players in
         *          the game.
         */
        std::vector<Player*> getPlayers();

        /*!
         * \brief   Gets the player associated with the fileDescriptor.
         * 
         * \param[in] fileDescriptor    Specifies the file descriptor to look up.
         *
         * \return  Returns a Player* with the player with the specified fileDescriptor.
         */
        Player* getPlayerByFD(int fileDescriptor);
    private:
        std::map<int, InteractiveNoun*> gameObjects;
        std::vector<Creature*> gameCreatures;
        std::map<int, Player*> gamePlayers;
};

}}
 
#endif