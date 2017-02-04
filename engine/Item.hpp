/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Item.hpp
 *
 * Overview:
 *     Header file for Item class. Defines the members and 
 * functions that apply to all items.
 ************************************************************************/

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "ItemPosition.hpp"
#include "ItemType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Item: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        Item();
        Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type);
        Item(const Item &otherItem);
        Item & operator=(const Item &otherItem);
        virtual ~Item();
        InteractiveNoun* getLocation();
        ItemPosition getPosition();
        std::string getName();
        ItemType* getType();
        bool setLocation(InteractiveNoun* containingNoun);
        bool setPosition(ItemPosition position);
        bool setName(std::string name);
        bool setType(ItemType *type);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        InteractiveNoun* location;
        ItemPosition position;
        std::string name;
        ItemType *type;
};

}}

#endif