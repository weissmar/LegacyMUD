/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Creature.cpp
 *
 * \details     Implementation file for Creature class. 
 ************************************************************************/

#include "Creature.hpp"
#include "Area.hpp"
#include "CreatureType.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

Creature::Creature()
: Combatant()
, type(nullptr)
, ambulatory (false)
{ }


Creature::Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, name, description, money, aLocation, maxInventoryWeight)
, type(aType)
, ambulatory (ambulatory)
{ }


/*Creature::Creature(const Creature &otherCreature){

}


Creature & Creature::operator=(const Creature &otherCreature){

}


Creature::~Creature(){

}*/


CreatureType* Creature::getType() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    return type;
}


bool Creature::getAmbulatory() const{
    return ambulatory.load();
}


bool Creature::setType(CreatureType *aType){
    if (aType != nullptr){
        std::lock_guard<std::mutex> typeLock(typeMutex);
        type = aType;
        return true;
    }
    
    return false;
}


bool Creature::setAmbulatory(bool ambulatory){
    this->ambulatory.store(ambulatory);
    return true;
}


ObjectType Creature::getObjectType() const{
    return ObjectType::CREATURE;
}


std::string Creature::serialize(){
    return "";
}


bool Creature::deserialize(std::string){
    return false;
}


std::string Creature::look(){
    return "";
}  


bool Creature::take(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Creature::equip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Creature::unequip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Creature::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){
    return false;
}


bool Creature::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){
    return false;
}


bool Creature::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){
    return false;
}


std::string Creature::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){
    return "";
} 


InteractiveNoun* Creature::copy(){
    return nullptr;
}


bool Creature::editAttribute(Player*, std::string){
    return false;
}


bool Creature::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Creature::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["creature type"] = DataType::CREATURE_TYPE_PTR;
    signature["ambulatory"] = DataType::BOOL_TYPE;
    signature["maximum health"] = DataType::INT_TYPE;
    signature["spawn location"] = DataType::AREA_PTR;
    signature["maximum special points"] = DataType::INT_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["money"] = DataType::INT_TYPE;
    signature["current location"] = DataType::AREA_PTR;
    signature["maximum inventory weight"] = DataType::INT_TYPE;

    return signature;
}

}}