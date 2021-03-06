/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    03/17/2017
 * \course      CS467, Winter 2017
 * \file        Character.cpp
 *
 * \details     Implentation file for Character base class. 
 ************************************************************************/

#include "Character.hpp"
#include "Area.hpp"
#include "Item.hpp"
#include "GameLogic.hpp"
#include <iostream>

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
{
    std::string idAlias = "character " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
}


Character::Character(std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID)
: InteractiveNoun(anID)
, name(name)
, description(description)
, money(money)
, location(aLocation)
, maxInventoryWeight(maxInventoryWeight)
{ }


Character::Character(const Character &otherCharacter) : InteractiveNoun(otherCharacter) {
    name = otherCharacter.name;
    description = otherCharacter.description;
    money.store(otherCharacter.money.load());
    location = otherCharacter.location;
    maxInventoryWeight.store(otherCharacter.maxInventoryWeight.load());
    std::string idAlias = "character " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
}


Character & Character::operator=(const Character &otherCharacter){
    InteractiveNoun::operator=(otherCharacter);
    name = otherCharacter.name;
    description = otherCharacter.description;
    money.store(otherCharacter.money.load());
    location = otherCharacter.location;
    maxInventoryWeight.store(otherCharacter.maxInventoryWeight.load());
    std::string idAlias = "character " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);

    return *this;
}

/*
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
        if (item.first == EquipmentSlot::NONE){
            items.push_back(item.second);
        }
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


int Character::getArmorBonus() const{
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    int armorBonus = 0;

    for (auto item : inventory){
        if ((item.first != EquipmentSlot::NONE) && (item.second->getType()->getObjectType() == ObjectType::ARMOR_TYPE)){
            armorBonus += item.second->getType()->getArmorBonus();
        }
    }

    return armorBonus;
}


bool Character::hasItem(ItemType *anItemType) const{
    std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
    for (auto item : inventory){
        if (item.second->getType() == anItemType){
            return true;
        }
    }
    return false;
}


bool Character::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
    this->name = name;
    return true;
}


bool Character::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;
    return true;
}


bool Character::setMoney(int money){
    bool success = false;

    if (money >= 0){
        this->money.store(money);
        success = true;
    }
    
    return success;
}


bool Character::addMoney(int money){
    bool success = false; 

    if (money >= 0){
        this->money += money;
        success = true;
    }
    
    return success;
}


bool Character::subtractMoney(int money){
    bool success = false;

    if (money >= 0){
        this->money -= money;
        success = true;

        if (this->money < 0){
            this->money = 0;
        }
    }

    return success;
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
        size_t index = 100000000;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if (index == 100000000){
            inventory.push_back(std::make_pair(EquipmentSlot::NONE, anItem));
            return true;
        } 
    }
    return false;
}


std::string Character::equipItem(Item *anItem){
    std::string success = "false";
    EquipmentSlot slot = anItem->getType()->getSlotType();

    if ((anItem != nullptr) && (slot != EquipmentSlot::NONE)){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        size_t index = 100000000;
        size_t currItemInSlot = 100000000;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
            if (inventory[i].first == slot){
                currItemInSlot = i;
            }
        }


        if (currItemInSlot != 100000000){
            // unequip item that was in that slot
            inventory[currItemInSlot].first = EquipmentSlot::NONE;
            inventory[currItemInSlot].second->setPosition(ItemPosition::INVENTORY);
            success = inventory[currItemInSlot].second->getName();
        } else {
            success = "true";
        }
        if (index != 100000000){
            // equip item
            inventory[index].first = slot;
        } else {
            inventory.push_back(std::make_pair(slot, anItem));
        }
    }
    return success;
}


bool Character::removeFromInventory(Item *anItem){
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        size_t index = 100000000;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if (index != 100000000){
            inventory.erase(inventory.begin() + index);
            return true;
        }
    }
    return false;
}


bool Character::unequipItem(Item *anItem){
    if (anItem != nullptr){
        std::lock_guard<std::mutex> inventoryLock(inventoryMutex);
        size_t index = 100000000;

        for (size_t i = 0; i < inventory.size(); i++){
            if (inventory[i].second == anItem){
                index = i;
            }
        }

        if (index != 100000000){
            if (inventory[index].first != EquipmentSlot::NONE){
                inventory[index].first = EquipmentSlot::NONE;
                return true;
            }
        }
    }
    return false;
}


bool Character::setMaxInventoryWeight(int maxWeight){
    maxInventoryWeight.store(maxWeight);

    return true;
}


std::string Character::serialize(){
    return "";
}


bool Character::removeAllFromInventory(){
    Area *location = getLocation();
    std::vector<std::pair<EquipmentSlot, Item*>> allItems = getInventory();

    for (auto item : allItems){
        if (item.second->getType()->getRarity() != ItemRarity::QUEST){
            removeFromInventory(item.second);
            item.second->setLocation(location);
            item.second->setPosition(ItemPosition::GROUND);
            location->addItem(item.second);
        }
    }

    return true;
}


bool Character::removeAll(){
    Area *location = getLocation();
    std::vector<std::pair<EquipmentSlot, Item*>> allItems = getInventory();

    for (auto item : allItems){
        removeFromInventory(item.second);
        item.second->setLocation(location);
        item.second->setPosition(ItemPosition::GROUND);
        location->addItem(item.second);
    }

    return true;
}


Item* Character::removeRandomFromInventory(){
    std::vector<std::pair<EquipmentSlot, Item*>> allItems = getInventory();
    std::vector<Item*> allNonQuestItems;
    int itemToRemove  = -1;
    Item *anItem = nullptr;
    Area *location = getLocation();

    for (auto item : allItems){
        if (item.second->getType()->getRarity() != ItemRarity::QUEST){
            allNonQuestItems.push_back(item.second);
        }
    }

    if (allNonQuestItems.size() != 0){
        itemToRemove = GameLogic::rollDice(allNonQuestItems.size(), 1);
        anItem = allNonQuestItems[itemToRemove - 1];
        removeFromInventory(anItem);
        anItem->setLocation(location);
        anItem->setPosition(ItemPosition::GROUND);
        location->addItem(anItem);
    }

    return anItem;
}


}}