/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/27/2017
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


int WeaponType::getDamage() const{
    return damage.load();
}


DamageType WeaponType::getDamageType() const{
    return damageType.load();
}


AreaSize WeaponType::getRange() const{
    return range.load();
}


int WeaponType::getCritMultiplier() const{
    return critMultiplier.load();
}


bool WeaponType::setDamage(int damage){
    this->damage.store(damage);

    return true;
}


bool WeaponType::setDamageType(DamageType type){
    damageType.store(type);

    return true;
}


bool WeaponType::setRange(AreaSize range){
    this->range.store(range);

    return true;
}


bool WeaponType::setCritMultiplier(int multiplier){
    critMultiplier.store(multiplier);

    return true;
}


ObjectType WeaponType::getObjectType() const{
    return ObjectType::WEAPON_TYPE;
}


std::string WeaponType::serialize(){
    return "";
}


bool WeaponType::deserialize(std::string){
    return false;
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