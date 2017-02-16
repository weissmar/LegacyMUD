/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/14/2017
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
class PlayerClass;

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
         * \param[in] anObject  Specifies the object to be added.
         * \param[in] FD        If the object is an active player, specifies their 
         *                      file descriptor
         *
         * \return  Returns a bool indicating whether or not adding the object was
         *          successful.
         */
        bool addObject(InteractiveNoun *anObject, int FD);

        /*!
         * \brief   Removes the specified object from the game manager and releases
         *          the associated memory if successfully removed.
         *
         * \param[in] anObject  Specifies the object to be removed.
         * \param[in] FD        If the object is a player, specifies their file descriptor
         *
         * \return  Returns a bool indicating whether or not removing the object was
         *          successful.
         */
        bool removeObject(InteractiveNoun *anObject, int FD);

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
         * \brief   Gets the list of pointers to active players in the game.
         *
         * \return  Returns a std::vector<Player*> with the active players in
         *          the game.
         */
        std::vector<Player*> getPlayersPtrs();

        /*!
         * \brief   Gets the list of active player file descriptors in the game.
         *
         * \return  Returns a std::vector<int> with the players' file descriptors.
         */
        std::vector<int> getPlayersFDs();

        /*!
         * \brief   Gets the active player associated with the fileDescriptor.
         * 
         * \param[in] fileDescriptor    Specifies the file descriptor to look up.
         *
         * \return  Returns a Player* with the player with the specified fileDescriptor, 
         *          or nullptr if the player isn't active.
         */
        Player* getPlayerByFD(int fileDescriptor);

        /*!
         * \brief   Gets the inactive player associated with the username.
         * 
         * \param[in] username  Specifies the username to look up.
         *
         * \return  Returns a Player* with the player with the specified username
         *          or nullptr if the player isn't inactive.
         */
        Player* getPlayerByUsername(std::string username);

        /*!
         * \brief   Loads the player associated with the username into active players
         * 
         * \param[in] username  Specifies the username to look up.
         * \param[in] FD        Specifies the player's file descriptor.
         *
         * \return  Returns a bool indicating whether or not the player was successfully
         *          loaded.
         */
        bool loadPlayer(std::string username, int FD);

        /*!
         * \brief   Hibernates the player associated with the username into inactive players
         * 
         * \param[in] FD    Specifies the player's file descriptor.
         *
         * \return  Returns a bool indicating whether or not the player was successfully
         *          hibernated.
         */
        bool hibernatePlayer(int FD);

        /*!
         * \brief   Gets list of PlayerClasses.
         *
         * \return  Returns a vector of pointers to PlayerClass.
         */
        std::vector<PlayerClass*> getPlayerClasses();
    private:
        std::map<int, InteractiveNoun*> gameObjects;
        std::map<int, Creature*> gameCreatures;
        std::map<int, Player*> activeGamePlayers;
        std::map<std::string, Player*> inactivePlayers;
        std::vector<PlayerClass*> gamePlayerClasses;
};

}}
 
#endif