/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    03/03/2017
 * \course      CS467, Winter 2017
 * \file        ItemType.cpp
 *
 * \details     Implementation file for ItemType class.
 ************************************************************************/

#include "ItemType.hpp"
#include "EffectType.hpp"

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
{
    std::string idAlias = "item type " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


ItemType::ItemType(int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType, int anID)
: InteractiveNoun(anID)
, weight(weight)
, rarity(rarity)
, description(description)
, name(name)
, cost(cost)
, slotType(slotType)
{
    std::string idAlias = "item type " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


int ItemType::getWeight() const{
    return weight.load();
}


ItemRarity ItemType::getRarity() const{
    return rarity.load();
}


std::string ItemType::getDescription() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


std::string ItemType::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


int ItemType::getCost() const{
    return cost.load();
}


EquipmentSlot ItemType::getSlotType() const{
    return slotType.load();
}


bool ItemType::setWeight(int weight){
    this->weight.store(weight);

    return true;
}


bool ItemType::setRarity(ItemRarity rarity){
    this->rarity.store(rarity);

    return true;
}


bool ItemType::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;

    return true;
}


bool ItemType::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;

    return true;
}


bool ItemType::setCost(int cost){
    this->cost.store(cost);

    return true;
}


bool ItemType::setSlotType(EquipmentSlot slotType){
    this->slotType.store(slotType);

    return true;
}


int ItemType::getArmorBonus() const{
    return -1;
}


DamageType ItemType::getResistantTo() const{
    return DamageType::NONE;
}


int ItemType::getDamage() const{
    return -1;
}


DamageType ItemType::getDamageType() const{
    return DamageType::NONE;
}


AreaSize ItemType::getRange() const{
    return AreaSize::NONE;
}


int ItemType::getCritMultiplier() const{
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


ObjectType ItemType::getObjectType() const{
    return ObjectType::ITEM_TYPE;
}


std::string ItemType::serialize(){
    return "";
}


ItemType* ItemType::deserialize(std::string){
    return nullptr; 
}


std::string ItemType::move(Player*, std::vector<EffectType> *effects){ 
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of move for this object
    message = getTextAndEffect(CommandEnum::MOVE, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::read(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of read for this object
    message = getTextAndEffect(CommandEnum::READ, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::breakIt(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of break for this object
    message = getTextAndEffect(CommandEnum::BREAK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::climb(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of climb for this object
    message = getTextAndEffect(CommandEnum::CLIMB, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::turn(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of turn for this object
    message = getTextAndEffect(CommandEnum::TURN, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::push(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of push for this object
    message = getTextAndEffect(CommandEnum::PUSH, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::pull(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of pull for this object
    message = getTextAndEffect(CommandEnum::PULL, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::eat(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of eat for this object
    message = getTextAndEffect(CommandEnum::EAT, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ItemType::drink(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of drink for this object
    message = getTextAndEffect(CommandEnum::DRINK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
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