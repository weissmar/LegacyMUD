/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      PlayerClass.hpp
 *
 * Overview:
 *     Header file for PlayerClass class. Defines the members and 
 * functions that apply to all player classes.
 ************************************************************************/

#ifndef PLAYER_CLASS_HPP
#define PLAYER_CLASS_HPP

#include <string>
#include "CombatantType.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class SpecialSkill;

class PlayerClass: public CombatantType {
    public:
        PlayerClass();
        PlayerClass(int primaryStat, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);
        PlayerClass(const PlayerClass &otherPlayerClass);
        PlayerClass & operator=(const PlayerClass &otherPlayerClass);
        virtual ~PlayerClass();
        int getPrimaryStat();
        bool setPrimaryStat(int);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        int primaryStat;
};

}}

#endif