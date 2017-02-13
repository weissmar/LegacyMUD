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


InteractiveNoun::~InteractiveNoun(){

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

}}

