/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/09/2017
 * Course:        CS467, Winter 2017
 * Filename:      WeaponType.hpp
 *
 * Overview:
 *     Header file for WeaponType class. Defines the members and 
 * functions that apply to all weapon types.
 ************************************************************************/

#ifndef WEAPON_TYPE_HPP
#define WEAPON_TYPE_HPP

#include <string>
#include "ItemType.hpp"
#include "ItemRarity.hpp"
#include "EquipmentSlot.hpp"
#include "DamageType.hpp"
#include "AreaSize.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class WeaponType: public ItemType {
    public:
        WeaponType();
        WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);
        WeaponType(const WeaponType &otherWeaponType);
        WeaponType & operator=(const WeaponType &otherWeaponType);
        virtual ~WeaponType();
        virtual int getDamage();
        virtual DamageType getDamageType();
        virtual AreaSize getRange();
        virtual int getCritMultiplier();
        virtual bool setDamage(int damage);
        virtual bool setDamageType(DamageType type);
        virtual bool setRange(AreaSize);
        virtual bool setCritMultiplier(int);
        virtual ObjectType getObjectType();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual std::string move(Player*); 
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
        int damage;
        DamageType damageType;
        AreaSize range;
        int critMultiplier;
};

}}

#endif