/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      CreatureType.hpp
 *
 * Overview:
 *     Header file for CreatureType class. Defines the members and 
 * functions that apply to all creature types.
 ************************************************************************/

#ifndef CREATURE_TYPE_HPP
#define CREATURE_TYPE_HPP

#include <string>
#include "CombatantType.hpp"
#include "CharacterSize.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class CreatureType: public CombatantType {
    public:
        CreatureType();
        CreatureType(CharacterSize size, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);
        CreatureType(const CreatureType &otherCreatureType);
        CreatureType & operator=(const CreatureType &otherCreatureType);
        virtual ~CreatureType();
        CharacterSize getSize();
        bool setSize(CharacterSize size);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        CharacterSize size;
};

}}

#endif