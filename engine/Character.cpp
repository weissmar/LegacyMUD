/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/22/2017
 * \course      CS467, Winter 2017
 * \file        Character.cpp
 *
 * \details     Implentation file for Character base class. 
 ************************************************************************/

#include "Character.hpp"
#include "Area.hpp"
#include "Item.hpp"
#include "ItemType.hpp"

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


std::string Character::getDescription() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


int Character::getMoney() const{
    return money.load();
}


Area* Character::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


std::vector<std::pair<EquipmentSlot, Item*>> Character::getInventory() const{
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    return inventory;
}


std::vector<Item*> Character::getItemsInventory() const{
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    std::vector<Item*> items;

    for (auto item : inventory){
        items.push_back(item.second);
    }

    return items;
}


std::vector<std::pair<EquipmentSlot, Item*>> Character::getEquipped() const{
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    std::vector<std::pair<EquipmentSlot, Item*>> equipment;

    for (auto item : inventory){
        if (item.first != EquipmentSlot::NONE){
            equipment.push_back(item);
        }
    }

    return equipment;
}


int Character::getMaxInventoryWeight() const{
    return maxInventoryWeight.load();
}


bool Character::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;
    return true;
}


bool Character::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;
    return true;
}


bool Character::setMoney(int money){
    this->money.store(money);
    return true;
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
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        inventory.push_back(std::make_pair(EquipmentSlot::NONE, anItem));
        return true;
    }
    return false;
}


bool Character::equipItem(Item *anItem){
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        EquipmentSlot slot = anItem->getType()->getSlotType();
        size_t index = -1;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if ((index != -1) && (slot != EquipmentSlot::NONE)){
            inventory[index].first = slot;
            return true;
        } else if (slot != EquipmentSlot::NONE) {
            inventory.push_back(std::make_pair(slot, anItem));
            return true;
        }
    }
    return false;
}


bool Character::removeFromInventory(Item *anItem){
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        size_t index = -1;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if (index != -1){
            inventory.erase(inventory.begin() + index);
            return true;
        }
    }
    return false;
}


bool Character::unequipItem(Item *anItem){
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        size_t index = -1;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if (index != -1){
            if (inventory[index].first != EquipmentSlot::NONE){
                inventory[index].first = EquipmentSlot::NONE;
                return true;
            }
        }
    }
    return false;
}


std::string Character::serialize(){
    return "";
}


bool Character::deserialize(std::string){
    return false;
}

}}