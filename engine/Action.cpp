/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/11/2017
 * \course      CS467, Winter 2017
 * \file        Action.cpp
 *
 * \details     Implementation file for Action class. 
 ************************************************************************/

#include "Action.hpp"

namespace legacymud { namespace engine {

Action::Action() 
: SerializableNoun()
, UniqueNoun()
, command(CommandEnum::INVALID)
, valid(false)
, flavorText("")
, effect(EffectType::NONE)
{ }


Action::Action(CommandEnum command, bool valid, std::string flavorText, EffectType effect)
: SerializableNoun()
, UniqueNoun()
, command(command)
, valid(valid)
, flavorText(flavorText)
, effect(effect)
{ }


Action::Action(const Action &otherAction)
: SerializableNoun()
, UniqueNoun()
, command(otherAction.command)
, valid(otherAction.valid)
, flavorText(otherAction.flavorText)
, effect(otherAction.effect){
    if (!otherAction.aliases.empty()){
        for (auto alias : otherAction.aliases){
            aliases[alias.first] = new parser::Grammar(*(alias.second));
        }
    }
}


Action & Action::operator=(const Action &otherAction){
    if (this != &otherAction){
        for (auto alias : aliases){
            delete alias.second;
        }
        aliases.clear();

        if (!otherAction.aliases.empty()){
            for (auto alias : otherAction.aliases){
                aliases[alias.first] = new parser::Grammar(*(alias.second));
            }
        }
    }

    return *this;
}


Action::~Action(){
    for (auto alias : aliases){
        delete alias.second;
    }
    aliases.clear();
}


CommandEnum Action::getCommand(){
    return command;
}


bool Action::getValid(){
    return valid;
}


std::string Action::getFlavorText(){
    return flavorText;
}


EffectType Action::getEffect(){
    return effect;
}


parser::Grammar* Action::getGrammar(std::string alias){
    return nullptr;
}


std::map<std::string, parser::Grammar*> Action::getAliasesAndGrammar(){
    return aliases;
}

std::vector<std::string> Action::getAliases(){
    std::vector<std::string> justAliases;

    for (auto alias : aliases){
        justAliases.push_back(alias.first);
    }

    return justAliases;
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
    return "";
}


bool Action::deserialize(std::string){
    return false;
}


std::map<std::string, DataType> Action::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["command"] = DataType::COMMAND_ENUM;
    signature["valid"] = DataType::BOOL_TYPE;
    signature["flavor text"] = DataType::STRING_TYPE;
    signature["effect"] = DataType::EFFECT_TYPE;

    return signature;
}

}}