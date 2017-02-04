/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      ArmorType.hpp
 *
 * Overview:
 *     Header file for ArmorType class. Defines the members and 
 * functions that apply to all armor types.
 ************************************************************************/

#ifndef ARMOR_TYPE_HPP
#define ARMOR_TYPE_HPP

#include <string>
#include "ItemType.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class ArmorType: public ItemType {
    public:
        ArmorType();
        ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);
        ArmorType(const ArmorType &otherArmorType);
        ArmorType & operator=(const ArmorType &otherArmorType);
        virtual ~ArmorType();
        int getArmorBonus();
        DamageType getResistantTo();
        bool setArmorBonus(int);
        bool setResistantTo(DamageType);
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
        int armorBonus;
        DamageType resistantTo;
};

}}

#endif