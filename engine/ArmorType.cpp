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

ArmorType::ArmorType(){

}


ArmorType::ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType){

}


ArmorType::ArmorType(const ArmorType &otherArmorType){

}


ArmorType & ArmorType::operator=(const ArmorType &otherArmorType){

}


ArmorType::~ArmorType(){

}


int ArmorType::getArmorBonus(){

}


DamageType ArmorType::getResistantTo(){

}


bool ArmorType::setArmorBonus(int bonus){

}


bool ArmorType::setResistantTo(DamageType resistance){

}


ObjectType ArmorType::getObjectType(){

}


std::string ArmorType::serialize(){

}


bool ArmorType::deserialize(std::string){

}


std::string ArmorType::move(Player *aPlayer){

} 


std::string ArmorType::read(Player *aPlayer){

} 


std::string ArmorType::breakIt(Player *aPlayer){

} 


std::string ArmorType::climb(Player *aPlayer){

} 


std::string ArmorType::turn(Player *aPlayer){

} 


std::string ArmorType::push(Player *aPlayer){

} 


std::string ArmorType::pull(Player *aPlayer){

} 


std::string ArmorType::eat(Player *aPlayer){

} 


std::string ArmorType::drink(Player *aPlayer){

} 


InteractiveNoun* ArmorType::copy(){

}


bool ArmorType::editAttribute(Player *aPlayer, std::string attribute){

}


bool ArmorType::editWizard(Player *aPlayer){

}


std::map<std::string, DataType> ArmorType::getAttributeSignature(){

}

}}
