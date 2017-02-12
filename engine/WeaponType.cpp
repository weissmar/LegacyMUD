/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/12/2017
 * \course      CS467, Winter 2017
 * \file        WeaponType.cpp
 *
 * \details     Implementation file for WeaponType class.
 ************************************************************************/

#include "WeaponType.hpp"

namespace legacymud { namespace engine {

WeaponType::WeaponType()
: ItemType()
, damage(0)
, damageType(DamageType::NONE)
, range(AreaSize::SMALL)
, critMultiplier(2)
{ }


WeaponType::WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType)
: ItemType(weight, rarity, description, name, cost, slotType)
, damage(damage)
, damageType(type)
, range(range)
, critMultiplier(critMultiplier)
{ }


int WeaponType::getDamage(){
    return damage;
}


DamageType WeaponType::getDamageType(){
    return damageType;
}


AreaSize WeaponType::getRange(){
    return range;
}


int WeaponType::getCritMultiplier(){
    return critMultiplier;
}


bool WeaponType::setDamage(int damage){
    this->damage = damage;

    return true;
}


bool WeaponType::setDamageType(DamageType type){
    damageType = type;

    return true;
}


bool WeaponType::setRange(AreaSize range){
    this->range = range;

    return true;
}


bool WeaponType::setCritMultiplier(int multiplier){
    critMultiplier = multiplier;

    return true;
}


ObjectType WeaponType::getObjectType(){
    return ObjectType::WEAPON_TYPE;
}


std::string WeaponType::serialize(){
    return "";
}


bool WeaponType::deserialize(std::string){
    return false;
}


std::string WeaponType::move(Player*){
    return "";
} 


std::string WeaponType::read(Player*){
    return "";
} 


std::string WeaponType::breakIt(Player*){
    return "";
} 


std::string WeaponType::climb(Player*){
    return "";
} 


std::string WeaponType::turn(Player*){
    return "";
} 


std::string WeaponType::push(Player*){
    return "";
} 


std::string WeaponType::pull(Player*){
    return "";
} 


std::string WeaponType::eat(Player*){
    return "";
} 


std::string WeaponType::drink(Player*){
    return "";
} 


InteractiveNoun* WeaponType::copy(){
    return nullptr;
}


bool WeaponType::editAttribute(Player*, std::string){
    return false;
}


bool WeaponType::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> WeaponType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["damage"] = DataType::INT_TYPE;
    signature["damage type"] = DataType::DAMAGE_TYPE;
    signature["range"] = DataType::AREA_SIZE;
    signature["crit multiplier"] = DataType::INT_TYPE;
    signature["weight"] = DataType::INT_TYPE;
    signature["rarity"] = DataType::ITEM_RARITY;
    signature["description"] = DataType::STRING_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["equipment slot type"] = DataType::EQUIPMENT_SLOT;

    return signature;
}

}}