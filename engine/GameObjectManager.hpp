/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/03/2017
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
#include <mutex>
#include <atomic>

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
         * \brief   Gets a map of ID to InteractiveNoun* with all of the objects in the
         *          game.
         *
         * \return  Returns a std::map<int, InteractiveNoun*> with all of the objects in
         *          the game.
         */
        std::map<int, InteractiveNoun*> getAllObjects() const;

        /*!
         * \brief   Gets a pointer to the object indicated by the specified
         *          ID.
         *
         * \param[in] ID    Specifies the ID of the object to be returned.
         *
         * \return  Returns a SerializableNoun* with the object, if found, otherwise
         *          nullptr.
         */
        InteractiveNoun* getPointer(int ID) const;

        /*!
         * \brief   Gets the list of creatures in the game.
         *
         * \return  Returns a std::vector<Creature*> with the creatures in
         *          the game.
         */
        std::vector<Creature*> getCreatures() const;

        /*!
         * \brief   Gets the list of pointers to active players in the game.
         *
         * \return  Returns a std::vector<Player*> with the active players in
         *          the game.
         */
        std::vector<Player*> getPlayersPtrs() const;

        /*!
         * \brief   Gets the list of active player file descriptors in the game.
         *
         * \return  Returns a std::vector<int> with the players' file descriptors.
         */
        std::vector<int> getPlayersFDs() const;

        /*!
         * \brief   Gets the active player associated with the fileDescriptor.
         * 
         * \param[in] fileDescriptor    Specifies the file descriptor to look up.
         *
         * \return  Returns a Player* with the player with the specified fileDescriptor, 
         *          or nullptr if the player isn't active.
         */
        Player* getPlayerByFD(int fileDescriptor) const;

        /*!
         * \brief   Gets the inactive player associated with the username.
         * 
         * \param[in] username  Specifies the username to look up.
         *
         * \return  Returns a Player* with the player with the specified username
         *          or nullptr if the player isn't inactive.
         */
        Player* getPlayerByUsername(std::string username) const;

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
        std::vector<PlayerClass*> getPlayerClasses() const;

        /*!
         * \brief   Gets list of Areas.
         *
         * \return  Returns a vector of pointers to Area.
         */
        std::vector<Area*> getGameAreas() const;

        /*!
         * \brief   Gets list of SpecialSkills.
         *
         * \return  Returns a vector of pointers to SpecialSkill.
         */
        std::vector<SpecialSkill*> getGameSkills() const;

        /*!
         * \brief   Gets list of ItemTypes.
         *
         * \return  Returns a vector of pointers to ItemType.
         */
        std::vector<ItemType*> getGameItemTypes() const;

        /*!
         * \brief   Gets list of NonCombatants.
         *
         * \return  Returns a vector of pointers to NonCombatant.
         */
        std::vector<NonCombatant*> getGameNPCs() const;

        /*!
         * \brief   Gets list of Containers.
         *
         * \return  Returns a vector of pointers to Container.
         */
        std::vector<Container*> getGameContainers() const;

        /*!
         * \brief   Gets list of CreatureTypes.
         *
         * \return  Returns a vector of pointers to CreatureType.
         */
        std::vector<CreatureType*> getGameCreatureTypes() const;

        /*!
         * \brief   Gets list of Quests.
         *
         * \return  Returns a vector of pointers to Quest.
         */
        std::vector<Quest*> getGameQuests() const;

        /*!
         * \brief   Gets list of Items.
         *
         * \return  Returns a vector of pointers to Items.
         */
        std::vector<Item*> getGameItems() const;

        /*!
         * \brief   Gets list of all Players (active and inactive).
         *
         * \return  Returns a vector of pointers to Player.
         */
        std::vector<Player*> getGamePlayers() const;
    private:
        std::map<int, InteractiveNoun*> gameObjects;
        mutable std::mutex gameObjectsMutex;
        std::map<int, Creature*> gameCreatures;
        mutable std::mutex gameCreaturesMutex;
        std::map<int, Player*> activeGamePlayers;
        mutable std::mutex activeGamePlayersMutex;
        std::map<std::string, Player*> inactivePlayers;
        mutable std::mutex inactivePlayersMutex;
        std::vector<PlayerClass*> gamePlayerClasses;
        mutable std::mutex gamePlayerClassesMutex;
        std::vector<Area*> gameAreas;
        mutable std::mutex gameAreasMutex;
        std::vector<SpecialSkill*> gameSkills;
        mutable std::mutex gameSkillsMutex;
        std::vector<ItemType*> gameItemTypes;
        mutable std::mutex gameItemTypesMutex;
        std::vector<NonCombatant*> gameNPCs;
        mutable std::mutex gameNPCsMutex;
        std::vector<Container*> gameContainers;
        mutable std::mutex gameContainersMutex;
        std::vector<CreatureType*> gameCreatureTypes;
        mutable std::mutex gameCreatureTypesMutex;
        std::vector<Quest*> gameQuests;
        mutable std::mutex gameQuestsMutex;
        std::vector<Item*> gameItems;
        mutable std::mutex gameItemsMutex;
};

}}
 
#endif