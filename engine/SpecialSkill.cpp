/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        SpecialSkill.cpp
 *
 * \details     Implementation file for SpecialSkill class. 
 ************************************************************************/

#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

SpecialSkill::SpecialSkill(){

}


SpecialSkill::SpecialSkill(std::string name, int damage, DamageType type, int cost, int cooldown){

}


SpecialSkill::SpecialSkill(const SpecialSkill &otherSpecialSkill){

}


SpecialSkill & SpecialSkill::operator=(const SpecialSkill &otherSpecialSkill){

}


virtual SpecialSkill::~SpecialSkill(){

}


std::string SpecialSkill::getName(){

}


int SpecialSkill::getDamage(){

}


DamageType SpecialSkill::getDamageType(){

}


int SpecialSkill::getCost(){

}


int SpecialSkill::getCooldown(){

}


bool SpecialSkill::setName(std::string name){

}


bool SpecialSkill::setDamage(int damage){

}


bool SpecialSkill::setDamageType(DamageType type){

}


bool SpecialSkill::setCost(int cost){

}


bool SpecialSkill::setCooldown(int cooldown){

}


virtual ObjectType SpecialSkill::getObjectType(){

}


virtual std::string SpecialSkill::serialize(){

}


virtual bool SpecialSkill::deserialize(std::string){

}


virtual std::string SpecialSkill::more(){

} 


virtual bool SpecialSkill::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


virtual std::string SpecialSkill::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


virtual InteractiveNoun* SpecialSkill::copy(){

}


virtual bool SpecialSkill::editAttribute(Player*, std::string){

}


virtual bool SpecialSkill::editWizard(Player*){

}


static std::map<std::string, DataType> SpecialSkill::getAttributeSignature(){

}

}}