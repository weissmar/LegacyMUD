/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      CombatantType.hpp
 *
 * Overview:
 *     Header file for CombatantType class. Defines the members and 
 * functions that apply to all combatant types.
 ************************************************************************/

#ifndef COMBATANT_TYPE_HPP
#define COMBATANT_TYPE_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "UniqueNoun.hpp"
#include "SpecialSkill.hpp"
#include "DamageType.hpp"

namespace legacymud { namespace engine {

class CombatantType: public SerializableNoun, public UniqueNoun {
    public:
        CombatantType();
        CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);
        CombatantType(const CombatantType &otherCombatantType);
        CombatantType & operator=(const CombatantType &otherCombatantType);
        virtual ~CombatantType();
        std::string getName();
        SpecialSkill* getSpecialSkill();
        int getAttackBonus();
        int getArmorBonus();
        DamageType getResistantTo();
        DamageType getWeakTo();
        float getHealPoints();
        bool setName(std::string name);
        bool setSpecialSkill(SpecialSkill* skill);
        bool setAttackBonus(int bonus);
        bool setArmorBonus(int bonus);
        bool setResistantTo(DamageType type);
        bool setWeakTo(DamageType type);
        bool setHealPoints(float healPoints);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
    private:
        std::string name;
        SpecialSkill* specialSkill;
        int attackBonus;
        int armorBonus;
        DamageType resistantTo;
        DamageType weakTo;
        float healPoints;
};

}}

#endif