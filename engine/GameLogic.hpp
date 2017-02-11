/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.hpp
 *
 * \details     Header file for GameLogic class. Defines the members and 
 *              functions that control the game logic.
 ************************************************************************/

#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <string>
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class GameObjectManager;
class InteractiveNoun;
class Player;
class Creature;
class NonCombatant;

class GameLogic {
    public:
        GameLogic();
        GameLogic(const GameLogic &otherGameLogic);
        GameLogic & operator=(const GameLogic &otherGameLogic);
        ~GameLogic();

        /*!
         * \brief   Starts a game.
         * 
         * This function starts a new game, if newGame is true, or loads the
         * game from the default file.
         * 
         * \param[in] newGame   Specifies whether or not to start a new game.
         *
         * \return  Returns a bool indicating whether or not starting the game
         *          was successful.
         */
        bool startGame(bool newGame);

        /*!
         * \brief   Starts a game.
         * 
         * This function starts a new game, if newGame is true, or loads the
         * game from the passed-in file.
         * 
         * \param[in] newGame   Specifies whether or not to start a new game.
         * \param[in] fileName  Specifies the file to load the game from, if
         *                      newGame is false.
         *
         * \return  Returns a bool indicating whether or not starting the game
         *          was successful.
         */
        bool startGame(bool newGame, const std::string &fileName);

        /*!
         * \brief   Loads a new player into the game.
         * 
         * This function starts the set-up process for a new player, if they
         * haven't played before, and loads existing players into the game.
         * 
         * \param[in] fileDescriptor    Specifies the player identifier to use when 
         *                              communicating with the server.
         *
         * \return  Returns a bool indicating whether or not loading the player
         *          was successful.
         */
        bool newPlayerHandler(int fileDescriptor);

        /*!
         * \brief   Processes messages from the message queue.
         * 
         * \param[in] numToProcess  Specifies how many messages to process at a
         *                          time.
         *
         * \return  Returns a bool indicating whether or not processing messages
         *          was successful.
         */
        bool processInput(int numToProcess);

        /*!
         * \brief   Adds a new message into the message queue.
         * 
         * This function adds a new message from the server into the message
         * queue for later processing.
         * 
         * \param[in] message           Specifies the message to add.
         * \param[in] fileDescriptor    Specifies the player that sent the message.
         *
         * \return  Returns a bool indicating whether or not adding the message
         *          was successful.
         */
        bool receivedMessageHandler(std::string message, int fileDescriptor);

        /*!
         * \brief   Updates creatures' movements and attacks.
         * 
         * This function updates the position of ambulatory creatures, and the
         * attacks of all creatures.
         *
         * \return  Returns a bool indicating whether or not updating the creatures
         *          was successful.
         */
        bool updateCreatures();

        /*!
         * \brief   Updates players that are in combat.
         * 
         * This function updates all players that are currently in combat, processing
         * actions off of their combat queues or making default attacks.
         *
         * \return  Returns a bool indicating whether or not updating the players
         *          was successful.
         */
        bool updatePlayersInCombat();
    private:
        bool loadPlayer(Player *aPlayer, int fileDescriptor);
        bool hibernatePlayer(Player *aPlayer);
        int rollDice(int numSides, int numDice);
        bool createObject(Player *aPlayer, ObjectType type);
        ObjectType getObjectType(const std::string &input);
        bool messagePlayer(Player *aPlayer, std::string message);
        bool startCombat(Player* aPlayer, Creature *aCreature);
        bool endCombat(Player *aPlayer, Creature *aCreature);
        bool startConversation(Player *aPlayer, NonCombatant *aNPC);
        bool endConversation(Player *aPlayer);
        bool executeCommand(CommandEnum aCommand, Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam, const std::string &stringParam, ItemPosition aPosition = none);
        bool helpCommand(Player *aPlayer);
        bool lookCommand(Player *aPlayer);
        bool listenCommand(Player *aPlayer);
        bool lookAtCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool takeCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam);
        bool putCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam, ItemPosition aPosition);
        bool dropCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool inventoryCommand(Player *aPlayer);
        bool moreCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool equipmentCommand(Player *aPlayer);
        bool equipCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam);
        bool unequipCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam);
        bool transferCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam);
        bool speakCommand(Player *aPlayer, const std::string &stringParam);
        bool shoutCommand(Player *aPlayer, const std::string &stringParam);
        bool whisperCommand(Player *aPlayer, InteractiveNoun *firstParam, const std::string &stringParam);
        bool quitCommand(Player *aPlayer);
        bool goCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool moveCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool statsCommand(Player *aPlayer);
        bool questsCommand(Player *aPlayer);
        bool skillsCommand(Player *aPlayer);
        bool attackCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam);
        bool talkCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool shopCommand(Player *aPlayer);
        bool buyCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool sellCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool searchCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool useSkillCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool readCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool breakCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool climbCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool turnCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool pushCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool pullCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool eatCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool drinkCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool editModeCommand(Player *aPlayer); 
        bool warpCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool copyCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool createCommand(Player *aPlayer, const std::string &stringParam);
        bool editAttributeCommand(Player *aPlayer, const std::string &stringParam);
        bool editAttributeCommand(Player *aPlayer, InteractiveNoun *firstParam, const std::string &stringParam);
        bool editWizardCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool saveCommand(Player *aPlayer);
        bool saveCommand(Player *aPlayer, const std::string &stringParam);
        bool loadCommand(Player *aPlayer);
        bool loadCommand(Player *aPlayer, const std::string &stringParam);
        bool deleteCommand(Player *aPlayer, InteractiveNoun *firstParam);
        GameObjectManager *manager;
        std::queue<std::tuple<std::string, int>> messageQueue;
};

}}

#endif