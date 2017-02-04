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
#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "NonCombatant.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class GameLogic {
    public:
        GameLogic();
        GameLogic(const GameLogic &otherGameLogic);
        GameLogic & operator=(const GameLogic &otherGameLogic);
        ~GameLogic();
        bool startGame(bool newGame);
        bool startGame(bool newGame, std::string fileName);
        bool newPlayerHandler(int fileDescriptor);
        bool loadPlayer(Player* aPlayer, int fileDescriptor);
        bool hibernatePlayer(Player* aPlayer);
        bool loadGame();
        bool loadGame(std::string fileName);
        bool saveGame();
        bool saveGame(std::string fileName);
        bool executeCommand(CommandEnum aCommand, Player* aPlayer, InteractiveNoun* firstParam, InteractiveNoun* secondParam, std::string stringParam);
        bool updateCreatures();
        bool updatePlayersInCombat();
        bool receivedMessageHandler(std::string message, int fileDescriptor);
        bool startCombat(Player* aPlayer, Creature* aCreature);
        bool endCombat(Player* aPlayer, Creature* aCreature);
        bool startConversation(Player* aPlayer, NonCombatant* aNPC);
        bool endConversation(Player* aPlayer);
        bool helpCommand(Player* aPlayer);
        bool lookCommand(Player* aPlayer);
        bool listenCommand(Player* aPlayer);
        bool lookAtCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool takeCommand(Player* aPlayer, InteractiveNoun* firstParam, InteractiveNoun* secondParam);
        bool dropCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool inventoryCommand(Player* aPlayer);
        bool moreObjectCommand(Player* aPlayer, InteractiveNoun*firstParam);
        bool equipmentCommand(Player* aPlayer);
        bool equipCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool unequipCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool transferCommand(Player* aPlayer, InteractiveNoun* firstParam, InteractiveNoun* secondParam);
        bool speakCommand(Player* aPlayer, std::string stringParam);
        bool shoutCommand(Player* aPlayer, std::string stringParam);
        bool whisperCommand(Player* aPlayer, InteractiveNoun* firstParam, std::string stringParam);
        bool quitCommand(Player* aPlayer);
        bool goCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool moveCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool statsCommand(Player* aPlayer);
        bool questsCommand(Player* aPlayer);
        bool skillsCommand(Player* aPlayer);
        bool moreSkillCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool attackCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool attackWithCommand(Player* aPlayer, InteractiveNoun* firstParam, InteractiveNoun* secondParam);
        bool talkCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool shopCommand(Player* aPlayer);
        bool buyCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool sellCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool searchCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool useSkillCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool readCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool breakCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool climbCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool turnCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool pushCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool pullCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool eatCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool drinkCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool editModeCommand(Player* aPlayer); 
        bool warpCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool copyCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool createCommand(Player* aPlayer, std::string stringParam);
        bool addCommand(Player* aPlayer, std::string stringParam);
        bool editAttributeCommand(Player* aPlayer, std::string stringParam);
        bool editAttributeCommand(Player* aPlayer, InteractiveNoun* firstParam, std::string stringParam);
        bool editWizardCommand(Player* aPlayer, InteractiveNoun* firstParam);
        bool saveCommand(Player* aPlayer);
        bool loadCommand(Player* aPlayer, std::string stringParam = "");
        bool deleteCommand(Player* aPlayer, InteractiveNoun* firstParam);
    private:
        GameObjectManager *manager;
        int rollDice(int numSides, int numDice);
        bool createObject(Player *aPlayer, ObjectType type);
        ObjectType getObjectType(std::string input);
        bool messagePlayer(Player *aPlayer, std::string message);
};

}}

#endif