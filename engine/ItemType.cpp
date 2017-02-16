/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/13/2017
 * \course      CS467, Winter 2017
 * \file        ItemType.cpp
 *
 * \details     Implementation file for ItemType class.
 ************************************************************************/

#include "ItemType.hpp"

namespace legacymud { namespace engine {

ItemType::ItemType()
: InteractiveNoun()
, weight(1)
, rarity(ItemRarity::COMMON)
, description("")
, name("")
, cost(1)
, slotType(EquipmentSlot::NONE)
{ }


ItemType::ItemType(int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType)
: InteractiveNoun()
, weight(weight)
, rarity(rarity)
, description(description)
, name(name)
, cost(cost)
, slotType(slotType)
{ }


int ItemType::getWeight(){
    return weight;
}


ItemRarity ItemType::getRarity(){
    return rarity;
}


std::string ItemType::getDescription(){
    return description;
}


std::string ItemType::getName() const{
    return name;
}


int ItemType::getCost(){
    return cost;
}


EquipmentSlot ItemType::getSlotType(){
    return slotType;
}


bool ItemType::setWeight(int weight){
    this->weight = weight;

    return true;
}


bool ItemType::setRarity(ItemRarity rarity){
    this->rarity = rarity;

    return true;
}


bool ItemType::setDescription(std::string description){
    this->description = description;

    return true;
}


bool ItemType::setName(std::string name){
    this->name = name;

    return true;
}


bool ItemType::setCost(int cost){
    this->cost = cost;

    return true;
}


bool ItemType::setSlotType(EquipmentSlot slotType){
    this->slotType = slotType;

    return true;
}


int ItemType::getArmorBonus(){
    return -1;
}


DamageType ItemType::getResistantTo(){
    return DamageType::NONE;
}


int ItemType::getDamage(){
    return -1;
}


DamageType ItemType::getDamageType(){
    return DamageType::NONE;
}


AreaSize ItemType::getRange(){
    return AreaSize::NONE;
}


int ItemType::getCritMultiplier(){
    return -1;
}


bool ItemType::setDamage(int damage){
    return false;
}


bool ItemType::setDamageType(DamageType type){
    return false;
}


bool ItemType::setRange(AreaSize){
    return false;
}


bool ItemType::setCritMultiplier(int){
    return false;
}


bool ItemType::setArmorBonus(int){
    return false;
}


bool ItemType::setResistantTo(DamageType){
    return false;
}


ObjectType ItemType::getObjectType(){
    return ObjectType::ITEM_TYPE;
}


std::string ItemType::serialize(){
    return "";
}


bool ItemType::deserialize(std::string){
    return false;
}


std::string ItemType::move(Player*){
    return "";
} 


std::string ItemType::read(Player*){
    return "";
} 


std::string ItemType::breakIt(Player*){
    return "";
} 


std::string ItemType::climb(Player*){
    return "";
} 


std::string ItemType::turn(Player*){
    return "";
} 


std::string ItemType::push(Player*){
    return "";
} 


std::string ItemType::pull(Player*){
    return "";
} 


std::string ItemType::eat(Player*){
    return "";
} 


std::string ItemType::drink(Player*){
    return "";
} 


InteractiveNoun* ItemType::copy(){
    return nullptr;
}


bool ItemType::editAttribute(Player*, std::string){
    return false;
}


bool ItemType::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> ItemType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["weight"] = DataType::INT_TYPE;
    signature["rarity"] = DataType::ITEM_RARITY;
    signature["description"] = DataType::STRING_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["equipment slot type"] = DataType::EQUIPMENT_SLOT;

    return signature;
}

}}