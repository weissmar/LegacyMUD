/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Character.hpp
 *
 * Overview:
 *     Header file for Character base class. Defines the members and 
 * functions that apply to all characters.
 ************************************************************************/

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <tuple>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "Area.hpp"
#include "Item.hpp"
#include "EquipmentSlot.hpp"

namespace legacymud { namespace engine {

class Character: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        Character();
        Character(std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Character(const Character &otherCharacter);
        Character & operator=(const Character &otherCharacter);
        virtual ~Character();
        std::string getName();
        std::string getDescription();
        int getMoney();
        Area* getLocation();
        std::vector<std::tuple<EquipmentSlot, Item*>> getInventory();
        int getMaxInventoryWeight();
        bool setName(std::string name);
        bool setDescription(std::string description);
        bool setMoney(int money);
        int addMoney(int money);
        int subtractMoney(int money);
        bool setLocation(Area *aLocation);
        bool addToInventory(Item *anItem);
        bool equipItem(Item *anItem);
        bool removeFromInventory(Item *anItem);
        bool unequipItem(Item *anItem);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
    private:
        std::string name;
        std::string description;
        int money;
        Area *location;
        std::vector<std::tuple<EquipmentSlot, Item*>> inventory;
        int maxInventoryWeight;
};

}}
 
#endif