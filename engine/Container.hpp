/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
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
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::vector<Item*> under;
        std::vector<Item*> inside;
        std::vector<Item*> onTopOf;
        int insideCapacity;
};

}}

#endif