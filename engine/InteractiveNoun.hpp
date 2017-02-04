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
        ~InteractiveNoun();
        Action* getAction(CommandEnum);
        std::vector<std::string> getNounAliases();
        std::map<std::string, CommandEnum> getVerbAliasMap();
        bool checkAction(CommandEnum);
        Action* addAction(CommandEnum);
        bool removeAction(CommandEnum);
        bool addAlias(std::string);
        bool removeAlias(std::string);
    private:
        std::vector<Action*> actions;
        std::vector<std::string> aliases;
};

}}

#endif