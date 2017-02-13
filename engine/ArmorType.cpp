/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
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


int ArmorType::getArmorBonus(){
    return armorBonus;
}


DamageType ArmorType::getResistantTo(){
    return resistantTo;
}


bool ArmorType::setArmorBonus(int bonus){
    return false;
}


bool ArmorType::setResistantTo(DamageType resistance){
    return false;
}


ObjectType ArmorType::getObjectType(){
    return ObjectType::ARMOR_TYPE;
}


std::string ArmorType::serialize(){
    return "";
}


bool ArmorType::deserialize(std::string){
    return false;
}


std::string ArmorType::move(Player *aPlayer){
    return "";
} 


std::string ArmorType::read(Player *aPlayer){
    return "";
} 


std::string ArmorType::breakIt(Player *aPlayer){
    return "";
} 


std::string ArmorType::climb(Player *aPlayer){
    return "";
} 


std::string ArmorType::turn(Player *aPlayer){
    return "";
} 


std::string ArmorType::push(Player *aPlayer){
    return "";
} 


std::string ArmorType::pull(Player *aPlayer){
    return "";
} 


std::string ArmorType::eat(Player *aPlayer){
    return "";
} 


std::string ArmorType::drink(Player *aPlayer){
    return "";
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
