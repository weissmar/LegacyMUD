/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/08/2017
 * \course      CS467, Winter 2017
 * \file        Action.cpp
 *
 * \details     Implementation file for Action class. 
 ************************************************************************/

#include "Action.hpp"

namespace legacymud { namespace engine {

Action::Action(){

}


Action::Action(CommandEnum command, bool valid, std::string flavorText, EffectType effect){

}


Action::Action(const Action &otherAction){

}


Action & Action::operator=(const Action &otherAction){

}


Action::~Action(){

}


CommandEnum Action::getCommand(){

}


bool Action::getValid(){

}


std::string Action::getFlavorText(){

}


EffectType Action::getEffect(){

}


parser::Grammar* Action::getGrammar(std::string alias){

}


std::map<std::string, parser::Grammar*> Action::getAliases(){

}


bool Action::setValid(bool validCommand){

}


bool Action::setFlavorText(std::string flavorText){

}


bool Action::setEffect(EffectType type){

}


bool Action::addAlias(std::string alias, parser::Grammar *grammar){

}


bool Action::removeAlias(std::string alais){

}


std::string Action::serialize(){

}


bool Action::deserialize(std::string){

}


std::map<std::string, DataType> Action::getAttributeSignature(){

}

}}