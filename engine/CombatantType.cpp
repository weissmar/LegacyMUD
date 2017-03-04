/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    03/03/2017
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
: InteractiveNoun()
, name("")
, specialSkill(nullptr)
, attackBonus(0)
, armorBonus(0)
, resistantTo(DamageType::NONE)
, weakTo(DamageType::NONE)
, healPoints(0.1)
{ }


CombatantType::CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints)
: InteractiveNoun()
, name(name)
, specialSkill(skill)
, attackBonus(attackBonus)
, armorBonus(armorBonus)
, resistantTo(resistantTo)
, weakTo(weakTo)
, healPoints(healPoints)
{
    std::string idAlias = "combatant type " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


CombatantType::CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints, int anID)
: InteractiveNoun(anID)
, name(name)
, specialSkill(skill)
, attackBonus(attackBonus)
, armorBonus(armorBonus)
, resistantTo(resistantTo)
, weakTo(weakTo)
, healPoints(healPoints)
{
    std::string idAlias = "combatant type " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


/*CombatantType::CombatantType(const CombatantType &otherCombatantType){

}


CombatantType & CombatantType::operator=(const CombatantType &otherCombatantType){

}


CombatantType::~CombatantType(){

}*/


std::string CombatantType::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


SpecialSkill* CombatantType::getSpecialSkill() const{
    std::lock_guard<std::mutex> specialSkillLock(specialSkillMutex);
    return specialSkill;
}


int CombatantType::getAttackBonus() const{
    return attackBonus.load();
}


int CombatantType::getArmorBonus() const{
    return armorBonus.load();
}


DamageType CombatantType::getResistantTo() const{
    return resistantTo.load();
}


DamageType CombatantType::getWeakTo() const{
    return weakTo.load();
}


float CombatantType::getHealPoints() const{
    return healPoints.load();
}


bool CombatantType::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;
    return true;
}


bool CombatantType::setSpecialSkill(SpecialSkill* skill){
    if (skill != nullptr){
        std::lock_guard<std::mutex> specialSkillLock(specialSkillMutex);
        specialSkill = skill;
        return true;
    }
    return false;
}


bool CombatantType::setAttackBonus(int bonus){
    attackBonus.store(bonus);
    return true;
}


bool CombatantType::setArmorBonus(int bonus){
    armorBonus.store(bonus);
    return true;
}


bool CombatantType::setResistantTo(DamageType type){
    resistantTo.store(type);
    return true;
}


bool CombatantType::setWeakTo(DamageType type){
    weakTo.store(type);
    return true;
}


bool CombatantType::setHealPoints(float healPoints){
    this->healPoints.store(healPoints);
    return true;
}


std::string CombatantType::serialize(){
    return "";
}


}}