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


virtual NonCombatant::~NonCombatant(){

}


Quest* NonCombatant::getQuest(){

}


bool NonCombatant::setQuest(Quest *aQuest){

}


virtual ObjectType NonCombatant::getObjectType(){

}


virtual std::string NonCombatant::serialize(){

}


virtual bool NonCombatant::deserialize(std::string){

}


virtual std::string NonCombatant::look(){

}  


virtual bool NonCombatant::take(Player*, Item*, InteractiveNoun*){

}


virtual bool NonCombatant::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


virtual bool NonCombatant::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


virtual std::string NonCombatant::talk(Player*, NonCombatant*){

} 


virtual InteractiveNoun* NonCombatant::copy(){

}


virtual bool NonCombatant::editAttribute(Player*, std::string){

}


virtual bool NonCombatant::editWizard(Player*){

}


static std::map<std::string, DataType> NonCombatant::getAttributeSignature(){

}

}}