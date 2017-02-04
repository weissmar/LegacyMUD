/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Player.hpp
 *
 * Overview:
 *     Header file for Player class and Command struct. Defines the 
 * members and functions that apply to all players.
 ************************************************************************/

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <queue>
#include <vector>
#include <tuple>
#include "Area.hpp"
#include "Combatant.hpp"
#include "Quest.hpp"
#include "CharacterSize.hpp"
#include "PlayerClass.hpp"
#include "NonCombatant.hpp"
#include "InteractiveNoun.hpp"
#include "CommandEnum.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

struct Command {
    CommandEnum commandE;
    InteractiveNoun *firstParam;
    InteractiveNoun *secondParam;
};

class Player: public Combatant {
    public:
        Player();
        Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Player(const Player &otherPlayer);
        Player & operator=(const Player &otherPlayer);
        virtual ~Player();
        int getExperiencePoints();
        int getLevel();
        CharacterSize getSize();
        std::string getUser();
        PlayerClass* getPlayerClass();
        NonCombatant* getInConversation();
        bool isActive();
        int getFileDescriptor();
        bool queueIsEmpty();
        bool isEditMode();
        std::vector<std::tuple<Quest*, int>> getQuestList();
        int addToExperiencePts(int gainedXP);
        bool levelUp();
        bool setSize(CharacterSize size);
        bool setPlayerClass(PlayerClass *aClass);
        bool setActive(bool active); 
        bool setFileDescriptor(int FD);
        bool setInConversation(NonCombatant *anNPC);
        Command* getNextCommand();
        bool addCommand(Command *aCommand);
        bool setEditMode(bool editing);
        bool addQuest(Quest *aQuest, int step);
        bool updateQuest(Quest *aQuest, int step);    
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        int experiencePoints;
        int level;
        CharacterSize size;
        PlayerClass *playerClass;
        NonCombatant *inConversation;
        std::string username;
        bool active;
        int fileDescriptor;
        std::queue<Command*> combatQueue;
        bool editMode;
        std::vector<std::tuple<Quest*, int>> questList;
};

}}

#endif