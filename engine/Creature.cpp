/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        Creature.cpp
 *
 * \details     Implementation file for Creature class. 
 ************************************************************************/

#include "Creature.hpp"
#include "Area.hpp"
#include "CreatureType.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

Creature::Creature(){

}


Creature::Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight){

}


Creature::Creature(const Creature &otherCreature){

}


Creature & Creature::operator=(const Creature &otherCreature){

}


Creature::~Creature(){

}


CreatureType* Creature::getType(){

}


bool Creature::getAmbulatory(){

}


bool Creature::setType(CreatureType *aType){

}


bool Creature::setAmbulatory(bool ambulatory){

}


ObjectType Creature::getObjectType(){

}


std::string Creature::serialize(){

}


bool Creature::deserialize(std::string){

}


std::string Creature::look(){

}  


bool Creature::take(Player*, Item*, InteractiveNoun*){

}


bool Creature::equip(Player*, Item*, InteractiveNoun*){

}


bool Creature::unequip(Player*, Item*, InteractiveNoun*){

}


bool Creature::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


bool Creature::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


bool Creature::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


std::string Creature::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


InteractiveNoun* Creature::copy(){

}


bool Creature::editAttribute(Player*, std::string){

}


bool Creature::editWizard(Player*){

}


std::map<std::string, DataType> Creature::getAttributeSignature(){

}

}}