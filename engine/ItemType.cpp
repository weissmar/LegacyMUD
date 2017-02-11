/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        ItemType.cpp
 *
 * \details     Implementation file for ItemType class.
 ************************************************************************/

#include "ItemType.hpp"

namespace legacymud { namespace engine {

ItemType::ItemType(){

}


ItemType::ItemType(int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType){

}


ItemType::ItemType(const ItemType &otherItemType){

}


ItemType & ItemType::operator=(const ItemType &otherItemType){

}


virtual ItemType::~ItemType(){

}


int ItemType::getWeight(){

}


ItemRarity ItemType::getRarity(){

}


std::string ItemType::getDescription(){

}


std::string ItemType::getName(){

}


int ItemType::getCost(){

}


EquipmentSlot ItemType::getSlotType(){

}


bool ItemType::setWeight(int weight){

}


bool ItemType::setRarity(ItemRarity rarity){

}


bool ItemType::setDescription(std::string description){

}


bool ItemType::setName(std::string name){

}


bool ItemType::setCost(int cost){

}


bool ItemType::setSlotType(EquipmentSlot slotType){

}


virtual int ItemType::getArmorBonus(){

}


virtual DamageType ItemType::getResistantTo(){

}


virtual int ItemType::getDamage(){

}


virtual DamageType ItemType::getDamageType(){

}


virtual AreaSize ItemType::getRange(){

}


virtual int ItemType::getCritMultiplier(){

}


virtual bool ItemType::setDamage(int damage){

}


virtual bool ItemType::setDamageType(DamageType type){

}


virtual bool ItemType::setRange(AreaSize){

}


virtual bool ItemType::setCritMultiplier(int){

}


virtual bool ItemType::setArmorBonus(int){

}


virtual bool ItemType::setResistantTo(DamageType){

}


virtual ObjectType ItemType::getObjectType(){

}


virtual std::string ItemType::serialize(){

}


virtual bool ItemType::deserialize(std::string){

}


virtual std::string ItemType::move(Player*){

} 


virtual std::string ItemType::read(Player*){

} 


virtual std::string ItemType::breakIt(Player*){

} 


virtual std::string ItemType::climb(Player*){

} 


virtual std::string ItemType::turn(Player*){

} 


virtual std::string ItemType::push(Player*){

} 


virtual std::string ItemType::pull(Player*){

} 


virtual std::string ItemType::eat(Player*){

} 


virtual std::string ItemType::drink(Player*){

} 


virtual InteractiveNoun* ItemType::copy(){

}


virtual bool ItemType::editAttribute(Player*, std::string){

}


virtual bool ItemType::editWizard(Player*){

}


static std::map<std::string, DataType> ItemType::getAttributeSignature(){

}

}}