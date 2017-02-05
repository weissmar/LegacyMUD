/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      GameLogic.hpp
 *
 * Overview:
 *     Header file for GameLogic class. Defines the members and 
 * functions that control the game logic.
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
        bool startGame(bool newGame);
        bool startGame(bool newGame, const std::string &fileName);
        bool newPlayerHandler(int fileDescriptor);
        bool processInput(int numToProcess);
        bool receivedMessageHandler(std::string message, int fileDescriptor);
        bool updateCreatures();
        bool updatePlayersInCombat();
    private:
        GameObjectManager *manager;
        std::queue<std::tuple<std::string, int>> messageQueue;
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
        bool equipCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool unequipCommand(Player *aPlayer, InteractiveNoun *firstParam);
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
        bool addCommand(Player *aPlayer, const std::string &stringParam);
        bool editAttributeCommand(Player *aPlayer, const std::string &stringParam);
        bool editAttributeCommand(Player *aPlayer, InteractiveNoun *firstParam, const std::string &stringParam);
        bool editWizardCommand(Player *aPlayer, InteractiveNoun *firstParam);
        bool saveCommand(Player *aPlayer);
        bool saveCommand(Player *aPlayer, const std::string &stringParam);
        bool loadCommand(Player *aPlayer);
        bool loadCommand(Player *aPlayer, const std::string &stringParam);
        bool deleteCommand(Player *aPlayer, InteractiveNoun *firstParam);
};

}}

#endif