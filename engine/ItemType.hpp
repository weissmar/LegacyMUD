/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ItemType.hpp
 *
 * Overview:
 *     Header file for ItemType class. Defines the members and 
 * functions that apply to all item types.
 ************************************************************************/

#ifndef ITEM_TYPE_HPP
#define ITEM_TYPE_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "EquipmentSlot.hpp"
#include "ItemRarity.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class ItemType: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        ItemType();
        ItemType(int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);
        ItemType(const ItemType &otherItemType);
        ItemType & operator=(const ItemType &otherItemType);
        virtual ~ItemType();
        int getWeight();
        ItemRarity getRarity();
        std::string getDescription();
        std::string getName();
        int getCost();
        EquipmentSlot getSlotType();
        bool setWeight(int weight);
        bool setRarity(ItemRarity rarity);
        bool setDescription(std::string description);
        bool setName(std::string name);
        bool setCost(int cost);
        bool setSlotType(EquipmentSlot slotType);
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string move(); 
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
        int weight;
        ItemRarity rarity;
        std::string description;
        std::string name;
        int cost;
        EquipmentSlot slotType;
};

}}

#endif