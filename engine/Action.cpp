/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/15/2017
 * \course      CS467, Winter 2017
 * \file        Action.cpp
 *
 * \details     Implementation file for Action class. 
 ************************************************************************/

#include "Action.hpp"

namespace legacymud { namespace engine {

Action::Action() 
: command(CommandEnum::INVALID)
, valid(false)
, flavorText("")
, effect(EffectType::NONE)
{ }


Action::Action(CommandEnum command, bool valid, std::string flavorText, EffectType effect)
: command(command)
, valid(valid)
, flavorText(flavorText)
, effect(effect)
{ }


Action::Action(const Action &otherAction)
: flavorText(otherAction.getFlavorText()){
    command.store(otherAction.command.load());
    valid.store(otherAction.valid.load());
    effect.store(otherAction.effect.load());
    std::lock_guard<std::mutex> lockMap(otherAction.aliasesMutex);
    if (!otherAction.aliases.empty()){
        for (auto alias : otherAction.aliases){
            aliases[alias.first] = new parser::Grammar(*(alias.second));
        }
    }
}


Action & Action::operator=(const Action &otherAction){
    if (this != &otherAction){
        std::unique_lock<std::mutex> lockMap(aliasesMutex, std::defer_lock);
        std::unique_lock<std::mutex> lockOtherMap(otherAction.aliasesMutex, std::defer_lock);
        std::lock(lockMap, lockOtherMap);
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


CommandEnum Action::getCommand() const{
    return command.load();
}


bool Action::getValid() const{
    return valid.load();
}


std::string Action::getFlavorText() const{
    std::lock_guard<std::mutex> flavorTextLock(flavorTextMutex);
    return flavorText;
}


EffectType Action::getEffect() const{
    return effect.load();
}


parser::Grammar* Action::getGrammar(std::string alias) const{
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    int count = aliases.count(alias);

    if (count == 1){
        return aliases.at(alias);
    } else {
        return nullptr;
    }
}


std::map<std::string, parser::Grammar*> Action::getAliasesAndGrammar() const{
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    return aliases;
}

std::vector<std::string> Action::getAliases() const{
    std::vector<std::string> justAliases;
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);

    for (auto alias : aliases){
        justAliases.push_back(alias.first);
    }

    return justAliases;
}


bool Action::setValid(bool validCommand){
    valid.store(validCommand);
    return true;
}


bool Action::setFlavorText(std::string flavorText){
    std::lock_guard<std::mutex> flavorTextLock(flavorTextMutex);
    this->flavorText = flavorText;
    return true;
}


bool Action::setEffect(EffectType type){
    effect.store(type);
    return true;
}


bool Action::addAlias(std::string alias, parser::Grammar *grammar){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    aliases[alias] = grammar;
    return true;
}


bool Action::removeAlias(std::string alias){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    int count = aliases.count(alias);

    if (count == 1){
        aliases.erase(alias);
        return true;
    }
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