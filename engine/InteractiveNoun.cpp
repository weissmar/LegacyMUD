// Stub for InteractiveNoun class.

#include "InteractiveNoun.hpp"

namespace legacymud { namespace engine {
    InteractiveNoun::InteractiveNoun() { }
    InteractiveNoun::~InteractiveNoun() { }

    Action* InteractiveNoun::getAction(CommandEnum) {
        Action *pt;
        return pt;
    }
    std::vector<Action*> InteractiveNoun::getActions(std::string alias) {
        return actions;
    }
    std::vector<std::string> InteractiveNoun::getNounAliases() {
        return aliases;
    }
    std::vector<std::string> InteractiveNoun::getVerbAliases() {
        return aliases;
    }
    bool InteractiveNoun::checkAction(CommandEnum) {
        return false;
    }
    Action* InteractiveNoun::addAction(CommandEnum) {
        Action *pt;
        return pt;
    }
    bool InteractiveNoun::removeAction(CommandEnum) {
        return false;
    }
    bool InteractiveNoun::addAlias(std::string) {
        return false;
    }
    bool InteractiveNoun::removeAlias(std::string) {
        return false;
    }
    std::string InteractiveNoun::look() {
        return "";
    }  
    std::string InteractiveNoun::listen() {
        return "";
    } 
    bool InteractiveNoun::take(Player*, Item*, InteractiveNoun*) {
        return false;
    }
    bool InteractiveNoun::put(Player*, Item*, InteractiveNoun*, ItemPosition) {
        return false;
    }
    bool InteractiveNoun::drop() {
        return false;
    }
    std::string InteractiveNoun::more() {
        return "";
    } 
    bool InteractiveNoun::equip(Player*, Item*, InteractiveNoun*) {
        return false;
    }
    bool InteractiveNoun::unequip(Player*, Item*) {
        return false;
    }
    bool InteractiveNoun::transfer(Player*, Item*, InteractiveNoun*) {
        return false;
    }
    bool InteractiveNoun::go(Player*, Area*) {
        return false;
    }
    std::string InteractiveNoun::move() {
        return "";
    } 
    bool InteractiveNoun::attack(Player*, Item*, InteractiveNoun*) {
        return false;
    }
    std::string InteractiveNoun::talk(Player*, NonCombatant*) {
        return "";
    } 
    bool InteractiveNoun::buy(Player*, Item*) {
        return false;
    }
    bool InteractiveNoun::sell(Player*, Item*) {
        return false;
    }
    std::string InteractiveNoun::search() {
        return "";
    } 
    std::string InteractiveNoun::useSkill(SpecialSkill*, InteractiveNoun*) {
        return "";
    } 
    std::string InteractiveNoun::read(Player*) {
        return "";
    } 
    std::string InteractiveNoun::breakIt(Player*) {
        return "";
    } 
    std::string InteractiveNoun::climb(Player*) {
        return "";
    } 
    std::string InteractiveNoun::turn(Player*) {
        return "";
    } 
    std::string InteractiveNoun::push(Player*) {
        return "";
    } 
    std::string InteractiveNoun::pull(Player*) {
        return "";
    } 
    std::string InteractiveNoun::eat(Player*) {
        return "";
    } 
    std::string InteractiveNoun::drink(Player*) {
        return "";
    } 
    std::string InteractiveNoun::warp(Player*, Area*) {
        return "";
    } 
    bool InteractiveNoun::copy() {
        return false;
    }
    bool InteractiveNoun::editAttribute(Player*, std::string) {
        return false;
    }
    bool InteractiveNoun::editWizard(Player*) {
        return false;
    }
}}
