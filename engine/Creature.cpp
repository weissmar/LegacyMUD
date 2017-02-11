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


virtual Creature::~Creature(){

}


CreatureType* Creature::getType(){

}


bool Creature::getAmbulatory(){

}


bool Creature::setType(CreatureType *aType){

}


bool Creature::setAmbulatory(bool ambulatory){

}


virtual ObjectType Creature::getObjectType(){

}


virtual std::string Creature::serialize(){

}


virtual bool Creature::deserialize(std::string){

}


virtual std::string Creature::look(){

}  


virtual bool Creature::take(Player*, Item*, InteractiveNoun*){

}


virtual bool Creature::equip(Player*, Item*, InteractiveNoun*){

}


virtual bool Creature::unequip(Player*, Item*, InteractiveNoun*){

}


virtual bool Creature::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


virtual bool Creature::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


virtual bool Creature::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


virtual std::string Creature::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


virtual InteractiveNoun* Creature::copy(){

}


virtual bool Creature::editAttribute(Player*, std::string){

}


virtual bool Creature::editWizard(Player*){

}


static std::map<std::string, DataType> Creature::getAttributeSignature(){

}

}}