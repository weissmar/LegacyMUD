/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/11/2017
 * \course      CS467, Winter 2017
 * \file        CombatantType.cpp
 *
 * \details     Header file for CombatantType base class. Defines the members 
 *              and functions that apply to all combatant types. This class
 *              should not be instantiated.
 ************************************************************************/

#include "SpecialSkill.hpp"
#include "CombatantType.hpp"

namespace legacymud { namespace engine {

CombatantType::CombatantType()
: SerializableNoun()
, InteractiveNoun()
, UniqueNoun()
, name("")
, specialSkill(nullptr)
, attackBonus(0)
, armorBonus(0)
, resistantTo(DamageType::NONE)
, weakTo(DamageType::NONE)
, healPoints(0.1)
{ }


CombatantType::CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints)
: SerializableNoun()
, InteractiveNoun()
, UniqueNoun()
, name(name)
, specialSkill(skill)
, attackBonus(attackBonus)
, armorBonus(armorBonus)
, resistantTo(resistantTo)
, weakTo(weakTo)
, healPoints(healPoints)
{ }


/*CombatantType::CombatantType(const CombatantType &otherCombatantType){

}


CombatantType & CombatantType::operator=(const CombatantType &otherCombatantType){

}


CombatantType::~CombatantType(){

}*/


std::string CombatantType::getName(){
    return name;
}


SpecialSkill* CombatantType::getSpecialSkill(){
    return specialSkill;
}


int CombatantType::getAttackBonus(){
    return attackBonus;
}


int CombatantType::getArmorBonus(){
    return armorBonus;
}


DamageType CombatantType::getResistantTo(){
    return resistantTo;
}


DamageType CombatantType::getWeakTo(){
    return weakTo;
}


float CombatantType::getHealPoints(){
    return healPoints;
}


bool CombatantType::setName(std::string name){
    return false;
}


bool CombatantType::setSpecialSkill(SpecialSkill* skill){
    return false;
}


bool CombatantType::setAttackBonus(int bonus){
    return false;
}


bool CombatantType::setArmorBonus(int bonus){
    return false;
}


bool CombatantType::setResistantTo(DamageType type){
    return false;
}


bool CombatantType::setWeakTo(DamageType type){
    return false;
}


bool CombatantType::setHealPoints(float healPoints){
    return false;
}


std::string CombatantType::serialize(){
    return "";
}


bool CombatantType::deserialize(std::string){
    return false;
}

}}