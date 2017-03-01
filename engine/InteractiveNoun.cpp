/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.cpp
 *
 * \details     Implentation file for InteractiveNoun base class. 
 ************************************************************************/

#include "InteractiveNoun.hpp"
#include "Action.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Area.hpp"
#include "NonCombatant.hpp"
#include "Combatant.hpp"

namespace legacymud { namespace engine {

std::atomic<int> InteractiveNoun::nextID {1};


int InteractiveNoun::getStaticID(){
    return nextID;
}


void InteractiveNoun::setStaticID(int num){
    nextID = num;
}


InteractiveNoun::InteractiveNoun(int anID) : ID(anID){

}


InteractiveNoun::InteractiveNoun(const InteractiveNoun &otherNoun) : ID(nextID++){
    std::unique_lock<std::mutex> aliasesLock(otherNoun.aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> actionsLock(otherNoun.actionsMutex, std::defer_lock);
    std::lock(aliasesLock, actionsLock);
    aliases = otherNoun.aliases;
    if (!otherNoun.actions.empty()){
        for (auto action : otherNoun.actions){
            actions.push_back(new Action(*action));
        }
    }
}


InteractiveNoun & InteractiveNoun::operator=(const InteractiveNoun &otherNoun){
    std::unique_lock<std::mutex> otherAliasesLock(otherNoun.aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> otherActionsLock(otherNoun.actionsMutex, std::defer_lock);
    std::unique_lock<std::mutex> aliasesLock(aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> actionsLock(actionsMutex, std::defer_lock);
    std::lock(otherAliasesLock, otherActionsLock, aliasesLock, actionsLock);
    if (this != &otherNoun){
        for (auto action : actions){
            delete action;
        }
        actions.clear();

        aliases = otherNoun.aliases;
        if (!otherNoun.actions.empty()){
            for (auto action : otherNoun.actions){
                actions.push_back(new Action(*action));
            }
        }        
    }

    return *this;
}


InteractiveNoun::~InteractiveNoun(){
    for (auto action : actions){
        delete action;
    }
    actions.clear();
}


int InteractiveNoun::getID() const{
    return ID;
}


Action* InteractiveNoun::getAction(CommandEnum aCommand) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            return actions[i];
        }
    }
    return nullptr;
}


std::vector<Action*> InteractiveNoun::getActions(std::string alias) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    std::vector<Action*> aliasActions;

    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->isAlias(alias) == true){
            aliasActions.push_back(actions[i]);
        }
    }

    return aliasActions;
}


std::vector<Action*> InteractiveNoun::getAllActions() const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);

    return actions;
}



std::vector<std::string> InteractiveNoun::getNounAliases() const{
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    return aliases;
}


std::vector<std::string> InteractiveNoun::getVerbAliases() const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    std::vector<std::string> verbAliases;
    std::vector<std::string> aliases;

    for (auto action : actions){
        aliases = action->getAliases();
        verbAliases.insert(verbAliases.end(), aliases.begin(), aliases.end());
    }

    return verbAliases;
}


bool InteractiveNoun::checkAction(CommandEnum aCommand) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            return true;
        }
    }
    return false;
}


Action* InteractiveNoun::addAction(CommandEnum aCommand){
    if (!checkAction(aCommand)){
        std::lock_guard<std::mutex> actionsLock(actionsMutex);
        Action *anAction = new Action(aCommand);
        actions.push_back(anAction);
        return anAction;
    } else {
        return getAction(aCommand);
    }
}


Action* InteractiveNoun::addAction(CommandEnum aCommand, bool valid, std::string flavorText, EffectType effect){
    if (!checkAction(aCommand)){
        std::lock_guard<std::mutex> actionsLock(actionsMutex);
        Action *anAction = new Action(aCommand, valid, flavorText, effect);
        actions.push_back(anAction);
        return anAction;
    } else {
        return getAction(aCommand);
    }
}


bool InteractiveNoun::removeAction(CommandEnum aCommand){
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    int index = -1;

    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            index = i;
        }
    }

    if (index != -1){
        actions.erase(actions.begin() + index);
        return true;
    }

    return false;
}


bool InteractiveNoun::addNounAlias(std::string anAlias){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    bool found = false;

    for (size_t i = 0; i < aliases.size(); i++){
        if (aliases[i] == anAlias)
            found = true;
    }

    if (!found){
        aliases.push_back(anAlias);
        return true;
    }
    return false;
}


bool InteractiveNoun::removeNounAlias(std::string anAlias){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    int index = -1;

    for (size_t i = 0; i < aliases.size(); i++){
        if (aliases[i] == anAlias)
            index = i;
    }

    if (index != -1){
        aliases.erase(aliases.begin() + index);
        return true;
    }

    return false;
}


bool InteractiveNoun::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    Action *anAction = getAction(aCommand);
    parser::Grammar *aGrammar;

    if (anAction != nullptr){
        if (prepositions.empty()){
            aGrammar = new parser::Grammar(direct, false, indirect);
        } else {
            aGrammar = new parser::Grammar(direct, true, indirect);
            for (auto prep : prepositions){
                aGrammar->addPreposition(prep.first, prep.second);
            }
        }
        anAction->addAlias(alias, aGrammar);
        return true;
    }

    return false;
}


bool InteractiveNoun::removeVerbAlias(CommandEnum aCommand, std::string alias){
    Action *anAction = getAction(aCommand);

    if (anAction != nullptr){
        anAction->removeAlias(alias);
        return true;
    }
    return false;
}


std::string InteractiveNoun::getTextAndEffect(CommandEnum aCommand, EffectType &anEffect) const{
    std::string message = "false";
    Action *anAction = nullptr;

    anAction = this->getAction(aCommand);
    if (anAction != nullptr){
        if (anAction->getValid()){
            message = anAction->getFlavorText();
            anEffect = anAction->getEffect();
        } 
    }

    return message;
}

}}