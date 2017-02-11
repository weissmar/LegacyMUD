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
    return *this;
}


Action::~Action(){

}


CommandEnum Action::getCommand(){
    return CommandEnum::INVALID;
}


bool Action::getValid(){
    return false;
}


std::string Action::getFlavorText(){
    return std::string();
}


EffectType Action::getEffect(){
    return EffectType::NONE;
}


parser::Grammar* Action::getGrammar(std::string alias){
    return nullptr;
}


std::map<std::string, parser::Grammar*> Action::getAliases(){
    return std::map<std::string, parser::Grammar*>();
}


bool Action::setValid(bool validCommand){
    return false;
}


bool Action::setFlavorText(std::string flavorText){
    return false;
}


bool Action::setEffect(EffectType type){
    return false;
}


bool Action::addAlias(std::string alias, parser::Grammar *grammar){
    return false;
}


bool Action::removeAlias(std::string alais){
    return false;
}


std::string Action::serialize(){
    return std::string();
}


bool Action::deserialize(std::string){
    return false;
}


std::map<std::string, DataType> Action::getAttributeSignature(){
    return std::map<std::string, DataType>();
}

}}