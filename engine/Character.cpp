/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        Character.cpp
 *
 * \details     Implentation file for Character base class. 
 ************************************************************************/

#include "Character.hpp"
#include "Area.hpp"
#include "Item.hpp"

namespace legacymud { namespace engine {

Character::Character(){

}


Character::Character(std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight){

}


Character::Character(const Character &otherCharacter){

}


Character & Character::operator=(const Character &otherCharacter){

}


virtual Character::~Character(){

}


std::string Character::getName(){

}


std::string Character::getDescription(){

}


int Character::getMoney(){

}


Area* Character::getLocation(){

}


std::vector<std::tuple<EquipmentSlot, Item*>> Character::getInventory(){

}


std::vector<Item*> Character::getItemsInventory(){

}


std::vector<std::tuple<EquipmentSlot, Item*>> Character::getEquipped(){

}


int Character::getMaxInventoryWeight(){

}


bool Character::setName(std::string name){

}


bool Character::setDescription(std::string description){

}


bool Character::setMoney(int money){

}


int Character::addMoney(int money){

}


int Character::subtractMoney(int money){

}


bool Character::setLocation(Area *aLocation){

}


bool Character::addToInventory(Item *anItem){

}


bool Character::equipItem(Item *anItem){

}


bool Character::removeFromInventory(Item *anItem){

}


bool Character::unequipItem(Item *anItem){

}


virtual std::string Character::serialize(){

}


virtual bool Character::deserialize(std::string){

}

}}