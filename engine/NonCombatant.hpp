/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/08/2017
 * Course:        CS467, Winter 2017
 * Filename:      NonCombatant.hpp
 *
 * Overview:
 *     Header file for NonCombatant class. Defines the members and 
 * functions that apply to all NPCs.
 ************************************************************************/

#ifndef NON_COMBATANT_HPP
#define NON_COMBATANT_HPP

#include <string>
#include "Character.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Area;
class Quest;

class NonCombatant: public Character {
    public:
        NonCombatant();
        NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        NonCombatant(const NonCombatant &otherNPC);
        NonCombatant & operator=(const NonCombatant &otherNPC);
        virtual ~NonCombatant();
        Quest* getQuest();
        bool setQuest(Quest *aQuest);
        virtual ObjectType getObjectType();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual std::string talk(Player*, NonCombatant*); 
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        Quest *quest;
};

}}

#endif