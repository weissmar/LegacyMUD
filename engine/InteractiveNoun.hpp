/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.hpp
 *
 * \details     Header file for InteractiveNoun base class. Defines necessary
 *              members for aliasing nouns and commands and interface for defining
 *              command actions.
 ************************************************************************/

#ifndef INTERACTIVE_NOUN_HPP
#define INTERACTIVE_NOUN_HPP

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <Grammar.h>
#include "CommandEnum.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Action;
class SpecialSkill;

class InteractiveNoun {
    public:
        InteractiveNoun();
        InteractiveNoun(const InteractiveNoun &otherNoun);
        InteractiveNoun & operator=(const InteractiveNoun &otherNoun);
        virtual ~InteractiveNoun();
        Action* getAction(CommandEnum);
        std::vector<Action*> getActions(std::string alias);
        std::vector<std::string> getNounAliases();
        std::vector<std::string> getVerbAliases();
        bool checkAction(CommandEnum);
        Action* addAction(CommandEnum);
        bool removeAction(CommandEnum);
        bool addAlias(std::string);
        bool removeAlias(std::string);
        virtual ObjectType getObjectType();
        virtual std::string look();  
        virtual std::string listen(); 
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool put(Player*, Item*, InteractiveNoun*, ItemPosition);
        virtual bool drop(Player*);
        virtual std::string more(); 
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*, InteractiveNoun*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*);
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);
        virtual std::string move(Player*); 
        virtual bool attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool);
        virtual std::string talk(Player*, NonCombatant*); 
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string search(Player*); 
        virtual std::string useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill); 
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual std::string warp(Player*, Area*); 
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
    private:
        std::vector<Action*> actions;
        std::vector<std::string> aliases;
};

}}

#endif