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
#include "Player.hpp"
#include "Item.hpp"
#include "Area.hpp"
#include "NonCombatant.hpp"
#include "Combatant.hpp"

namespace legacymud { namespace engine {

std::atomic<int> InteractiveNoun::nextID {1}

InteractiveNoun::InteractiveNoun() : ID(nextID++){

}


InteractiveNoun::InteractiveNoun(const InteractiveNoun &otherNoun) : ID(nextID++){
    aliases = otherNoun.aliases;
    if (!otherNoun.actions.empty()){
        for (auto action : otherNoun.actions){
            actions.push_back(new Action(*action));
        }
    }
}


InteractiveNoun & InteractiveNoun::operator=(const InteractiveNoun &otherNoun){
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


int InteractiveNoun::getID(){
    return ID;
}


Action* InteractiveNoun::getAction(CommandEnum){
    return nullptr;
}


std::vector<Action*> InteractiveNoun::getActions(std::string alias){
    std::vector<Action*> actions;

    return actions;
}


std::vector<std::string> InteractiveNoun::getNounAliases(){
    return aliases;
}


std::vector<std::string> InteractiveNoun::getVerbAliases(){
    std::vector<std::string> verbAliases;
    std::vector<std::string> aliases;

    for (auto action : actions){
        aliases = action->getAliases();
        verbAliases.insert(verbAliases.end(), aliases.begin(), aliases.end());
    }

    return verbAliases;
}


bool InteractiveNoun::checkAction(CommandEnum){
    return false;
}


Action* InteractiveNoun::addAction(CommandEnum){
    return nullptr;
}


bool InteractiveNoun::removeAction(CommandEnum){
    return false;
}


bool InteractiveNoun::addAlias(std::string){
    return false;
}


bool InteractiveNoun::removeAlias(std::string){
    return false;
}

}}