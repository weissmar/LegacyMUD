/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ConditionalElement.hpp
 *
 * Overview:
 *     Header file for ConditionalElement class. Defines the members and 
 * functions that apply to all conditional elements.
 ************************************************************************/

#ifndef CONDITIONAL_ELEMENT_HPP
#define CONDITIONAL_ELEMENT_HPP

#include <string>
#include "InteractiveNoun.hpp"
#include "SerializableNoun.hpp"
#include "UniqueNoun.hpp"
#include "ItemType.hpp"

namespace legacymud { namespace engine {

class ConditionalElement: public InteractiveNoun, public SerializableNoun, public UniqueNoun {
    public:
        ConditionalElement();
        ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription);
        ConditionalElement(const ConditionalElement &otherElement);
        ConditionalElement & operator=(const ConditionalElement &otherElement);
        virtual ~ConditionalElement();
        bool isConditional();
        ItemType* getConditionItem();
        std::string getDescription();
        std::string getAltDescription();
        bool setConditional(bool isConditional);
        bool setConditionItem(ItemType *anItemType);
        bool setDescription(std::string description);
        bool setAltDescription(std::string altDescription);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
    private:
        bool conditionSet;
        ItemType *conditionItem;
        std::string description;
        std::string altDescription;
};

}}

#endif