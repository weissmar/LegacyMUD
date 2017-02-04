/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      SpecialSkill.hpp
 *
 * Overview:
 *     Header file for SpecialSkill class. Defines the members and 
 * functions that apply to all special skills.
 ************************************************************************/

#ifndef SPECIAL_SKILL_HPP
#define SPECIAL_SKILL_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class SpecialSkill: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        SpecialSkill();
        SpecialSkill(std::string name, int damage, DamageType type, int cost, int cooldown);
        SpecialSkill(const SpecialSkill &otherSpecialSkill);
        SpecialSkill & operator=(const SpecialSkill &otherSpecialSkill);
        virtual ~SpecialSkill();
        std::string getName();
        int getDamage();
        DamageType getDamageType();
        int getCost();
        int getCooldown();
        bool setName(std::string name);
        bool setDamage(int damage);
        bool setDamageType(DamageType type);
        bool setCost(int cost);
        bool setCooldown(int cooldown);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        int damage;
        DamageType damageType;
        int cost;
        int cooldown;
};

}}

#endif