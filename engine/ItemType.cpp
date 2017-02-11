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


ItemType::~ItemType(){

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


int ItemType::getArmorBonus(){

}


DamageType ItemType::getResistantTo(){

}


int ItemType::getDamage(){

}


DamageType ItemType::getDamageType(){

}


AreaSize ItemType::getRange(){

}


int ItemType::getCritMultiplier(){

}


bool ItemType::setDamage(int damage){

}


bool ItemType::setDamageType(DamageType type){

}


bool ItemType::setRange(AreaSize){

}


bool ItemType::setCritMultiplier(int){

}


bool ItemType::setArmorBonus(int){

}


bool ItemType::setResistantTo(DamageType){

}


ObjectType ItemType::getObjectType(){

}


std::string ItemType::serialize(){

}


bool ItemType::deserialize(std::string){

}


std::string ItemType::move(Player*){

} 


std::string ItemType::read(Player*){

} 


std::string ItemType::breakIt(Player*){

} 


std::string ItemType::climb(Player*){

} 


std::string ItemType::turn(Player*){

} 


std::string ItemType::push(Player*){

} 


std::string ItemType::pull(Player*){

} 


std::string ItemType::eat(Player*){

} 


std::string ItemType::drink(Player*){

} 


InteractiveNoun* ItemType::copy(){

}


bool ItemType::editAttribute(Player*, std::string){

}


bool ItemType::editWizard(Player*){

}


std::map<std::string, DataType> ItemType::getAttributeSignature(){

}

}}