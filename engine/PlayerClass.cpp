/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        PlayerClass.cpp
 *
 * \details     Implementation file for PlayerClass class. 
 ************************************************************************/

#include "PlayerClass.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

PlayerClass::PlayerClass(){

}


PlayerClass::PlayerClass(int primaryStat, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints){

}


PlayerClass::PlayerClass(const PlayerClass &otherPlayerClass){

}


PlayerClass & PlayerClass::operator=(const PlayerClass &otherPlayerClass){

}


PlayerClass::~PlayerClass(){

}


int PlayerClass::getPrimaryStat(){

}


bool PlayerClass::setPrimaryStat(int){

}


ObjectType PlayerClass::getObjectType(){

}


std::string PlayerClass::serialize(){

}


bool PlayerClass::deserialize(std::string){

}


InteractiveNoun* PlayerClass::copy(){

}


bool PlayerClass::editAttribute(Player*, std::string){

}


bool PlayerClass::editWizard(Player*){

}


std::map<std::string, DataType> PlayerClass::getAttributeSignature(){

}

}}
