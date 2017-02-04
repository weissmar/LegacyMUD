/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Action.hpp
 *
 * Overview:
 *     Header file for Action class. Defines the members and 
 * functions that apply to all actions.
 ************************************************************************/

#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <vector>
#include <map>
#include "SerializableNoun.hpp"
#include "UniqueNoun.hpp"
#include "CommandEnum.hpp"
#include "EffectType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Action: public SerializableNoun, public UniqueNoun {
    public:
        Action();
        Action(CommandEnum command, bool valid, std::string flavorText, EffectType effect);
        Action(const Action &otherAction);
        Action & operator=(const Action &otherAction);
        virtual ~Action();
        CommandEnum getCommand();
        bool getValid();
        std::string getFlavorText();
        EffectType getEffect();
        std::vector<std::string> getAliases();
        bool setValid(bool validCommand);
        bool setFlavorText(std::string flavorText);
        bool setEffect(EffectType type);
        bool addAlias(std::string alias);
        bool removeAlias(std::string alais);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        CommandEnum command;
        bool valid;
        std::string flavorText;
        EffectType effect;
        std::vector<std::string> aliases;
};

}}

#endif