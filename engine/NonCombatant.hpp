/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
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
#include "Area.hpp"
#include "Quest.hpp"
#include "Character.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class NonCombatant: public Character {
    public:
        NonCombatant();
        NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        NonCombatant(const NonCombatant &otherNPC);
        NonCombatant & operator=(const NonCombatant &otherNPC);
        virtual ~NonCombatant();
        Quest* getQuest();
        bool setQuest(Quest *aQuest);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        Quest *quest;
};

}}

#endif