/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        NonCombatant.cpp
 *
 * \details     Implementation file for NonCombatant class. 
 ************************************************************************/

#include "NonCombatant.hpp"
#include "Area.hpp"
#include "Quest.hpp"

namespace legacymud { namespace engine {

NonCombatant::NonCombatant(){

}


NonCombatant::NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight){

}


NonCombatant::NonCombatant(const NonCombatant &otherNPC){

}


NonCombatant & NonCombatant::operator=(const NonCombatant &otherNPC){

}


NonCombatant::~NonCombatant(){

}


Quest* NonCombatant::getQuest(){

}


bool NonCombatant::setQuest(Quest *aQuest){

}


ObjectType NonCombatant::getObjectType(){

}


std::string NonCombatant::serialize(){

}


bool NonCombatant::deserialize(std::string){

}


std::string NonCombatant::look(){

}  


bool NonCombatant::take(Player*, Item*, InteractiveNoun*){

}


bool NonCombatant::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


bool NonCombatant::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


std::string NonCombatant::talk(Player*, NonCombatant*){

} 


InteractiveNoun* NonCombatant::copy(){

}


bool NonCombatant::editAttribute(Player*, std::string){

}


bool NonCombatant::editWizard(Player*){

}


std::map<std::string, DataType> NonCombatant::getAttributeSignature(){

}

}}