/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/12/2017
 * \course      CS467, Winter 2017
 * \file        CreatureType.cpp
 *
 * \details     Implementation file for CreatureType class. 
 ************************************************************************/

#include "CreatureType.hpp"

namespace legacymud { namespace engine {

CreatureType::CreatureType()
: CombatantType()
, size(CharacterSize::MEDIUM)
, difficulty(XPTier::NORMAL)
{ }


CreatureType::CreatureType(CharacterSize size, XPTier difficulty, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints)
: CombatantType(name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints)
, size(CharacterSize::MEDIUM)
, difficulty(XPTier::NORMAL)
{ }


CharacterSize CreatureType::getSize(){
    return size;
}


XPTier CreatureType::getDifficulty(){
    return difficulty;
}


bool CreatureType::setSize(CharacterSize size){
    this->size = size;

    return true;
}


bool CreatureType::setDifficulty(XPTier difficulty){
    this->difficulty = difficulty;

    return true;
}


ObjectType CreatureType::getObjectType(){
    return ObjectType::CREATURE_TYPE;
}


std::string CreatureType::serialize(){
    return "";
}


bool CreatureType::deserialize(std::string){
    return false;
}


InteractiveNoun* CreatureType::copy(){
    return nullptr;
}


bool CreatureType::editAttribute(Player*, std::string){
    return false;
}


bool CreatureType::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> CreatureType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["size"] = DataType::CHARACTER_SIZE;
    signature["difficulty"] = DataType::X_P_TIER;
    signature["name"] = DataType::STRING_TYPE;
    signature["skill"] = DataType::SPECIAL_SKILL_PTR;
    signature["attack bonus"] = DataType::INT_TYPE;
    signature["armor bonus"] = DataType::INT_TYPE;
    signature["resistant to"] = DataType::DAMAGE_TYPE;
    signature["weak to"] = DataType::DAMAGE_TYPE;
    signature["heal points"] = DataType::FLOAT_TYPE;

    return signature;
}

}}