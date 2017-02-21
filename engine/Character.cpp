/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Character.cpp
 *
 * \details     Implentation file for Character base class. 
 ************************************************************************/

#include "Character.hpp"
#include "Area.hpp"
#include "Item.hpp"

namespace legacymud { namespace engine {

const int DEFAULT_MAX_INV = 10;

Character::Character() 
: InteractiveNoun()
, name("")
, description("")
, money(0)
, location(nullptr)
, maxInventoryWeight(DEFAULT_MAX_INV) 
{ }


Character::Character(std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: InteractiveNoun()
, name(name)
, description(description)
, money(money)
, location(aLocation)
, maxInventoryWeight(maxInventoryWeight)
{ }


/*Character::Character(const Character &otherCharacter){

}


Character & Character::operator=(const Character &otherCharacter){

}


Character::~Character(){

}*/


std::string Character::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Character::getDescription(){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


int Character::getMoney(){
    return money.load();
}


Area* Character::getLocation(){
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


std::vector<std::pair<EquipmentSlot, Item*>> Character::getInventory(){
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    return inventory;
}


std::vector<Item*> Character::getItemsInventory(){
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    std::vector<Item*> items;

    for (auto item : inventory){
        items.push_back(item.second);
    }

    return items;
}


std::vector<std::pair<EquipmentSlot, Item*>> Character::getEquipped(){
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    std::vector<std::pair<EquipmentSlot, Item*>> equipment;

    for (auto item : inventory){
        if (item.first != EquipmentSlot::NONE){
            equipment.push_back(item);
        }
    }

    return equipment;
}


int Character::getMaxInventoryWeight(){
    return maxInventoryWeight.load();
}


bool Character::setName(std::string name){
    return false;
}


bool Character::setDescription(std::string description){
    return false;
}


bool Character::setMoney(int money){
    return false;
}


int Character::addMoney(int money){
    this->money += money;

    return money;
}


int Character::subtractMoney(int money){
    this->money += money;

    return money;
}


bool Character::setLocation(Area *aLocation){
    if (aLocation != nullptr){
        std::lock_guard<std::mutex> locationLock(locationMutex);
        location = aLocation;
        return true;
    }
    return false;
}


bool Character::addToInventory(Item *anItem){
    return false;
}


bool Character::equipItem(Item *anItem){
    return false;
}


bool Character::removeFromInventory(Item *anItem){
    return false;
}


bool Character::unequipItem(Item *anItem){
    return false;
}


std::string Character::serialize(){
    return "";
}


bool Character::deserialize(std::string){
    return false;
}

}}