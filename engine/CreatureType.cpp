/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        CreatureType.cpp
 *
 * \details     Implementation file for CreatureType class. 
 ************************************************************************/

#include "CreatureType.hpp"

namespace legacymud { namespace engine {

CreatureType::CreatureType(){

}


CreatureType::CreatureType(CharacterSize size, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints){

}


CreatureType::CreatureType(const CreatureType &otherCreatureType){

}


CreatureType & CreatureType::operator=(const CreatureType &otherCreatureType){

}


virtual CreatureType::~CreatureType(){

}


CharacterSize CreatureType::getSize(){

}


XPTier CreatureType::getDifficulty(){

}


bool CreatureType::setSize(CharacterSize size){

}


bool CreatureType::setDifficulty(XPTier difficulty){

}


virtual ObjectType CreatureType::getObjectType(){

}


virtual std::string CreatureType::serialize(){

}


virtual bool CreatureType::deserialize(std::string){

}


virtual InteractiveNoun* CreatureType::copy(){

}


virtual bool CreatureType::editAttribute(Player*, std::string){

}


virtual bool CreatureType::editWizard(Player*){

}


static std::map<std::string, DataType> CreatureType::getAttributeSignature(){

}

}}