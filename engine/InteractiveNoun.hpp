/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      InteractiveNoun.hpp
 *
 * Overview:
 *     Header file for InteractiveNoun base class. Defines necessary
 * members for aliasing nouns and commands and interface for defining
 * command actions.
 ************************************************************************/

#ifndef INTERACTIVE_NOUN_HPP
#define INTERACTIVE_NOUN_HPP

#include <string>
#include <vector>
#include <map>
#include "Action.hpp"
#include "CommandEnum.hpp"

namespace legacymud { namespace engine {

class InteractiveNoun {
    public:
        InteractiveNoun();
        InteractiveNoun(const InteractiveNoun &otherNoun);
        InteractiveNoun & operator=(const InteractiveNoun &otherNoun);
        virtual ~InteractiveNoun();
        Action* getAction(CommandEnum);
        std::vector<std::string> getNounAliases();
        std::map<std::string, CommandEnum> getVerbAliasMap();
        bool checkAction(CommandEnum);
        Action* addAction(CommandEnum);
        bool removeAction(CommandEnum);
        bool addAlias(std::string);
        bool removeAlias(std::string);
        virtual std::string look();  
        virtual std::string listen(); 
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool drop();
        virtual std::string more(); 
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual bool go(Player*, Area*);
        virtual std::string move(); 
        virtual bool attack(Player*, Item*, InteractiveNoun*);
        virtual std::string talk(Player*, NonCombatant*); 
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string search(); 
        virtual std::string useSkill(SpecialSkill*, InteractiveNoun*); 
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual std::string warp(Player*, Area*); 
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
    private:
        std::vector<Action*> actions;
        std::vector<std::string> aliases;
};

}}

#endif