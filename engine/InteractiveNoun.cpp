/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.cpp
 *
 * \details     Implentation file for InteractiveNoun base class. 
 ************************************************************************/

#include "InteractiveNoun.hpp"
#include "Action.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

InteractiveNoun::InteractiveNoun(){

}


InteractiveNoun::InteractiveNoun(const InteractiveNoun &otherNoun){

}


InteractiveNoun & InteractiveNoun::operator=(const InteractiveNoun &otherNoun){

}


virtual InteractiveNoun::~InteractiveNoun(){

}


Action* InteractiveNoun::getAction(CommandEnum){

}


std::vector<Action*> InteractiveNoun::getActions(std::string alias){

}


std::vector<std::string> InteractiveNoun::getNounAliases(){

}


std::vector<std::string> InteractiveNoun::getVerbAliases(){

}


bool InteractiveNoun::checkAction(CommandEnum){

}


Action* InteractiveNoun::addAction(CommandEnum){

}


bool InteractiveNoun::removeAction(CommandEnum){

}


bool InteractiveNoun::addAlias(std::string){

}


bool InteractiveNoun::removeAlias(std::string){

}


virtual ObjectType InteractiveNoun::getObjectType(){

}


virtual std::string InteractiveNoun::look(){

}  


virtual std::string InteractiveNoun::listen(){

} 


virtual bool InteractiveNoun::take(Player*, Item*, InteractiveNoun*){

}


virtual bool InteractiveNoun::put(Player*, Item*, InteractiveNoun*, ItemPosition){

}


virtual bool InteractiveNoun::drop(Player*){

}


virtual std::string InteractiveNoun::more(){

} 


virtual bool InteractiveNoun::equip(Player*, Item*, InteractiveNoun*){

}


virtual bool InteractiveNoun::unequip(Player*, Item*, InteractiveNoun*){

}


virtual bool InteractiveNoun::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


virtual bool InteractiveNoun::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


virtual std::string InteractiveNoun::move(Player*){

} 


virtual bool InteractiveNoun::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


virtual std::string InteractiveNoun::talk(Player*, NonCombatant*){

} 


virtual bool InteractiveNoun::buy(Player*, Item*){

}


virtual bool InteractiveNoun::sell(Player*, Item*){

}


virtual std::string InteractiveNoun::search(Player*){

} 


virtual std::string InteractiveNoun::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


virtual std::string InteractiveNoun::read(Player*){

} 


virtual std::string InteractiveNoun::breakIt(Player*){

} 


virtual std::string InteractiveNoun::climb(Player*){

} 


virtual std::string InteractiveNoun::turn(Player*){

} 


virtual std::string InteractiveNoun::push(Player*){

} 


virtual std::string InteractiveNoun::pull(Player*){

} 


virtual std::string InteractiveNoun::eat(Player*){

} 


virtual std::string InteractiveNoun::drink(Player*){

} 


virtual std::string InteractiveNoun::warp(Player*, Area*){

} 


virtual InteractiveNoun* InteractiveNoun::copy(){

}


virtual bool InteractiveNoun::editAttribute(Player*, std::string){

}


virtual bool InteractiveNoun::editWizard(Player*){

}

}}