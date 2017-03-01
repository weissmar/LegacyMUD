/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/28/2017
 * \course      CS467, Winter 2017
 * \file        ArmorType.cpp
 *
 * \details     Implementation file for ArmorType class. 
 ************************************************************************/

#include "ArmorType.hpp"

namespace legacymud { namespace engine {

ArmorType::ArmorType()
: ItemType()
, armorBonus(0)
, resistantTo(DamageType::NONE)
{ }


ArmorType::ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType)
: ItemType(weight, rarity, description, name, cost, slotType)
, armorBonus(bonus)
, resistantTo(resistantTo)
{ }


int ArmorType::getArmorBonus() const{
    return armorBonus.load();
}


DamageType ArmorType::getResistantTo() const{
    return resistantTo.load();
}


bool ArmorType::setArmorBonus(int bonus){
    armorBonus.store(bonus);
    return true;
}


bool ArmorType::setResistantTo(DamageType resistance){
    resistantTo.store(resistance);
    return true;
}


ObjectType ArmorType::getObjectType() const{
    return ObjectType::ARMOR_TYPE;
}


std::string ArmorType::serialize(){
    return "";
}


InteractiveNoun* ArmorType::deserialize(std::string){
    return nullptr; 
}


InteractiveNoun* ArmorType::copy(){
    return nullptr;
}


bool ArmorType::editAttribute(Player *aPlayer, std::string attribute){
    return false;
}


bool ArmorType::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> ArmorType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["armor bonus"] = DataType::INT_TYPE;
    signature["resistant to"] = DataType::DAMAGE_TYPE;
    signature["weight"] = DataType::INT_TYPE;
    signature["rarity"] = DataType::ITEM_RARITY;
    signature["description"] = DataType::STRING_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["equipment slot type"] = DataType::EQUIPMENT_SLOT;

    return signature;
}

}}
