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


SpecialSkill::~SpecialSkill(){

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


ObjectType SpecialSkill::getObjectType(){

}


std::string SpecialSkill::serialize(){

}


bool SpecialSkill::deserialize(std::string){

}


std::string SpecialSkill::more(){

} 


bool SpecialSkill::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


std::string SpecialSkill::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


InteractiveNoun* SpecialSkill::copy(){

}


bool SpecialSkill::editAttribute(Player*, std::string){

}


bool SpecialSkill::editWizard(Player*){

}


std::map<std::string, DataType> SpecialSkill::getAttributeSignature(){

}

}}