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


virtual WeaponType::~WeaponType(){

}


virtual int WeaponType::getDamage(){

}


virtual DamageType WeaponType::getDamageType(){

}


virtual AreaSize WeaponType::getRange(){

}


virtual int WeaponType::getCritMultiplier(){

}


virtual bool WeaponType::setDamage(int damage){

}


virtual bool WeaponType::setDamageType(DamageType type){

}


virtual bool WeaponType::setRange(AreaSize){

}


virtual bool WeaponType::setCritMultiplier(int){

}


virtual ObjectType WeaponType::getObjectType(){

}


virtual std::string WeaponType::serialize(){

}


virtual bool WeaponType::deserialize(std::string){

}


virtual std::string WeaponType::move(Player*){

} 


virtual std::string WeaponType::read(Player*){

} 


virtual std::string WeaponType::breakIt(Player*){

} 


virtual std::string WeaponType::climb(Player*){

} 


virtual std::string WeaponType::turn(Player*){

} 


virtual std::string WeaponType::push(Player*){

} 


virtual std::string WeaponType::pull(Player*){

} 


virtual std::string WeaponType::eat(Player*){

} 


virtual std::string WeaponType::drink(Player*){

} 


virtual InteractiveNoun* WeaponType::copy(){

}


virtual bool WeaponType::editAttribute(Player*, std::string){

}


virtual bool WeaponType::editWizard(Player*){

}


static std::map<std::string, DataType> WeaponType::getAttributeSignature(){

}

}}