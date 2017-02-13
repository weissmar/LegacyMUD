/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        WeaponType.cpp
 *
 * \details     Implementation file for WeaponType class.
 ************************************************************************/

#include "WeaponType.hpp"

namespace legacymud { namespace engine {

WeaponType::WeaponType(){

}


WeaponType::WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType){

}


WeaponType::WeaponType(const WeaponType &otherWeaponType){

}


WeaponType & WeaponType::operator=(const WeaponType &otherWeaponType){

}


WeaponType::~WeaponType(){

}


int WeaponType::getDamage(){

}


DamageType WeaponType::getDamageType(){

}


AreaSize WeaponType::getRange(){

}


int WeaponType::getCritMultiplier(){

}


bool WeaponType::setDamage(int damage){

}


bool WeaponType::setDamageType(DamageType type){

}


bool WeaponType::setRange(AreaSize){

}


bool WeaponType::setCritMultiplier(int){

}


ObjectType WeaponType::getObjectType(){

}


std::string WeaponType::serialize(){

}


bool WeaponType::deserialize(std::string){

}


std::string WeaponType::move(Player*){

} 


std::string WeaponType::read(Player*){

} 


std::string WeaponType::breakIt(Player*){

} 


std::string WeaponType::climb(Player*){

} 


std::string WeaponType::turn(Player*){

} 


std::string WeaponType::push(Player*){

} 


std::string WeaponType::pull(Player*){

} 


std::string WeaponType::eat(Player*){

} 


std::string WeaponType::drink(Player*){

} 


InteractiveNoun* WeaponType::copy(){

}


bool WeaponType::editAttribute(Player*, std::string){

}


bool WeaponType::editWizard(Player*){

}


std::map<std::string, DataType> WeaponType::getAttributeSignature(){

}

}}