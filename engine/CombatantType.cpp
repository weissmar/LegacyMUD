/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
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

CombatantType::CombatantType(){

}


CombatantType::CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints){

}


CombatantType::CombatantType(const CombatantType &otherCombatantType){

}


CombatantType & CombatantType::operator=(const CombatantType &otherCombatantType){

}


virtual CombatantType::~CombatantType(){

}


std::string CombatantType::getName(){

}


SpecialSkill* CombatantType::getSpecialSkill(){

}


int CombatantType::getAttackBonus(){

}


int CombatantType::getArmorBonus(){

}


DamageType CombatantType::getResistantTo(){

}


DamageType CombatantType::getWeakTo(){

}


float CombatantType::getHealPoints(){

}


bool CombatantType::setName(std::string name){

}


bool CombatantType::setSpecialSkill(SpecialSkill* skill){

}


bool CombatantType::setAttackBonus(int bonus){

}


bool CombatantType::setArmorBonus(int bonus){

}


bool CombatantType::setResistantTo(DamageType type){

}


bool CombatantType::setWeakTo(DamageType type){

}


bool CombatantType::setHealPoints(float healPoints){

}


virtual std::string CombatantType::serialize(){

}


virtual bool CombatantType::deserialize(std::string){

}

}}