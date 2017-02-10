/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/08/2017
 * Course:        CS467, Winter 2017
 * Filename:      Container.hpp
 *
 * Overview:
 *     Header file for Container class. Defines the members and 
 * functions that apply to all containers.
 ************************************************************************/

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <string>
#include "Item.hpp"
#include "ItemPosition.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Container: public Item {
    public:
        Container();
        Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type);
        Container(const Container &otherContainer);
        Container & operator=(const Container &otherContainer);
        virtual ~Container();
        bool isEmpty();
        bool remove(Item *anItem);
        bool place(Item *anItem, ItemPosition position);
        std::vector<Item*> getInsideContents();
        std::vector<Item*> getUnderContents();
        std::vector<Item*> getTopContents();
        int getInsideCapacity();
        bool setInsideCapacity(int);
        virtual ObjectType getObjectType();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string look();  
        virtual bool take(Player*, Item*, InteractiveNoun*);
        virtual bool put(Player*, Item*, InteractiveNoun*, ItemPosition);
        virtual bool drop(Player *aPlayer);
        virtual std::string more(); 
        virtual bool equip(Player*, Item*, InteractiveNoun*);
        virtual bool unequip(Player*, Item*);
        virtual bool transfer(Player*, Item*, InteractiveNoun*);
        virtual std::string move(Player *aPlayer); 
        virtual bool attack(Player*, Item*, InteractiveNoun*);
        virtual bool buy(Player*, Item*);
        virtual bool sell(Player*, Item*);
        virtual std::string search(Player *aPlayer); 
        virtual std::string read(Player*); 
        virtual std::string breakIt(Player*); 
        virtual std::string climb(Player*); 
        virtual std::string turn(Player*); 
        virtual std::string push(Player*); 
        virtual std::string pull(Player*); 
        virtual std::string eat(Player*); 
        virtual std::string drink(Player*); 
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::vector<Item*> under;
        std::vector<Item*> inside;
        std::vector<Item*> onTopOf;
        int insideCapacity;
};

}}

#endif