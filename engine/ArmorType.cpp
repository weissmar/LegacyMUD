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


virtual ArmorType::~ArmorType(){

}


virtual int ArmorType::getArmorBonus(){

}


virtual DamageType ArmorType::getResistantTo(){

}


virtual bool ArmorType::setArmorBonus(int bonus){

}


virtual bool ArmorType::setResistantTo(DamageType resistance){

}


virtual ObjectType ArmorType::getObjectType(){

}


virtual std::string ArmorType::serialize(){

}


virtual bool ArmorType::deserialize(std::string){

}


virtual std::string ArmorType::move(Player *aPlayer){

} 


virtual std::string ArmorType::read(Player *aPlayer){

} 


virtual std::string ArmorType::breakIt(Player *aPlayer){

} 


virtual std::string ArmorType::climb(Player *aPlayer){

} 


virtual std::string ArmorType::turn(Player *aPlayer){

} 


virtual std::string ArmorType::push(Player *aPlayer){

} 


virtual std::string ArmorType::pull(Player *aPlayer){

} 


virtual std::string ArmorType::eat(Player *aPlayer){

} 


virtual std::string ArmorType::drink(Player *aPlayer){

} 


virtual InteractiveNoun* ArmorType::copy(){

}


virtual bool ArmorType::editAttribute(Player *aPlayer, std::string attribute){

}


virtual bool ArmorType::editWizard(Player *aPlayer){

}


static std::map<std::string, DataType> ArmorType::getAttributeSignature(){

}

}}
