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
#include "Combatant.hpp"
#include "CharacterSize.hpp"
#include "CommandEnum.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Area;
class Quest;
class NonCombatant;
class PlayerClass;
class InteractiveNoun;

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
        std::multimap<std::string, InteractiveNoun*> getVerbLookup();
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
        bool addVerbs(std::vector<std::string>, InteractiveNoun*);
        bool removeVerbs(InteractiveNoun*);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool put(Player*, Item*, InteractiveNoun*, ItemPosition);
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual bool go(Player*, Area*);
        virtual bool attack(Player*, Item*, InteractiveNoun*);
        virtual std::string talk(Player*, NonCombatant*); 
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string search(); 
        virtual std::string useSkill(SpecialSkill*, InteractiveNoun*); 
        virtual std::string warp(Player*, Area*); 
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
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
        std::multimap<std::string, InteractiiveNoun*> verbLookup;
};

}}

#endif