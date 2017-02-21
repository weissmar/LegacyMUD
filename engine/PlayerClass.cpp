/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        PlayerClass.cpp
 *
 * \details     Implementation file for PlayerClass class. 
 ************************************************************************/

#include "PlayerClass.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

PlayerClass::PlayerClass()
: CombatantType()
, primaryStat(0)
{ }


PlayerClass::PlayerClass(int primaryStat, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints)
: CombatantType(name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints)
, primaryStat(primaryStat)
{ }


int PlayerClass::getPrimaryStat(){
    return primaryStat.load();
}


bool PlayerClass::setPrimaryStat(int primaryStat){
    if ((primaryStat >= 0) && (primaryStat <= 2)){
        this->primaryStat.store(primaryStat);
        return true;
    }
    return false;
}


ObjectType PlayerClass::getObjectType(){
    return ObjectType::PLAYER_CLASS;
}


std::string PlayerClass::serialize(){
    return "";
}


bool PlayerClass::deserialize(std::string){
    return false;
}


InteractiveNoun* PlayerClass::copy(){
    return nullptr;
}  


bool PlayerClass::editAttribute(Player*, std::string){
    return false;
}


bool PlayerClass::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> PlayerClass::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["primary stat"] = DataType::INT_TYPE;
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
