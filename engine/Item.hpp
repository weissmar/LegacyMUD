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
        virtual std::string look();  
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool drop();
        virtual std::string more(); 
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual std::string move(); 
        virtual bool attack(Player*, Item*, InteractiveNoun*);
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        InteractiveNoun* location;
        ItemPosition position;
        std::string name;
        ItemType *type;
};

}}

#endif