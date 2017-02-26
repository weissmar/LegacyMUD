/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/26/2017
 * \course      CS467, Winter 2017
 * \file        Item.cpp
 *
 * \details     Header file for Item class. Defines the members and 
 *              functions that apply to all items. Item defines generic
 *              in-game items.
 ************************************************************************/

#include <iostream>
#include "Item.hpp"
#include "ItemType.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

Item::Item()
: InteractiveNoun()
, location(nullptr)
, position(ItemPosition::NONE)
, name("")
, type(nullptr)
{ }


Item::Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type)
: InteractiveNoun()
, location(location)
, position(position)
, name(name)
, type(type)
{
    InteractiveNoun::addNounAlias(name);
}


/*Item::Item(const Item &otherItem){

}


Item & Item::operator=(const Item &otherItem){

}


Item::~Item(){

}*/


InteractiveNoun* Item::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


ItemPosition Item::getPosition() const{
    return position.load();
}


std::string Item::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


ItemType* Item::getType() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    return type;
}


bool Item::setLocation(InteractiveNoun* containingNoun){
    std::lock_guard<std::mutex> locationLock(locationMutex);
    if (containingNoun != nullptr){
        location = containingNoun;
        return true;
    }

    return false;
}


bool Item::setPosition(ItemPosition position){
    this->position.store(position);

    return true;
}


bool Item::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;

    return true;
}


bool Item::setType(ItemType *type){
    std::lock_guard<std::mutex> typeLock(typeMutex);
    if (type != nullptr){
        this->type = type;
        return true;
    }

    return false;
}


bool Item::addNounAlias(std::string alias){
    bool success = false;
    Container *aContainer = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    while (location != nullptr){
        if (aPosition == ItemPosition::GROUND){
            // location is an area
            success = location->registerAlias(false, alias, this);
            location = nullptr;
        } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
            if (location->getObjectType() == ObjectType::CONTAINER){
                // location is a container
                aContainer = dynamic_cast<Container*>(location);
                if (aContainer != nullptr){
                    location = aContainer->getLocation();
                    aPosition = aContainer->getPosition();
                } else {
                    location = nullptr;
                    success = false;
                }
            } else {
                location = nullptr;
                success = false;
            }
        } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
            // location is a character
            if (location->getObjectType() == ObjectType::PLAYER){
                // location is a player
                success = location->registerAlias(false, alias, this);
                location = nullptr;
            } else {
                // location is a creature or NPC
                // don't add alias to area, since any items in a creature or NPC inventory is unavailable
                location = nullptr;
                success = true;
            }
        } else {
            // position is NONE, something has gone wrong
            location = nullptr;
            success = false;
        }
    }

    if (success){
        success = InteractiveNoun::addNounAlias(alias);
    }
    
    return success;
}


bool Item::removeNounAlias(std::string alias){
    bool success = false;
    Container *aContainer = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    while (location != nullptr){
        if (aPosition == ItemPosition::GROUND){
            // location is an area
            success = location->unregisterAlias(false, alias, this);
            location = nullptr;
        } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
            if (location->getObjectType() == ObjectType::CONTAINER){
                // location is a container
                aContainer = dynamic_cast<Container*>(location);
                if (aContainer != nullptr){
                    location = aContainer->getLocation();
                    aPosition = aContainer->getPosition();
                } else {
                    location = nullptr;
                    success = false;
                }
            } else {
                location = nullptr;
                success = false;
            }
        } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
            // location is a character
            if (location->getObjectType() == ObjectType::PLAYER){
                // location is a player
                success = location->unregisterAlias(false, alias, this);
                location = nullptr;
            } else {
                // location is a creature or NPC
                // don't add alias to area, since any items in a creature or NPC inventory is unavailable
                location = nullptr;
                success = true;
            }
        } else {
            // position is NONE, something has gone wrong
            location = nullptr;
            success = false;
        }
    }

    if (success){
        success = InteractiveNoun::removeNounAlias(alias);
    }
    
    return success;
}


bool Item::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;
    Container *aContainer = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    while (location != nullptr){
        if (aPosition == ItemPosition::GROUND){
            // location is an area
            success = location->registerAlias(true, alias, this);
            location = nullptr;
        } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
            if (location->getObjectType() == ObjectType::CONTAINER){
                // location is a container
                aContainer = dynamic_cast<Container*>(location);
                if (aContainer != nullptr){
                    location = aContainer->getLocation();
                    aPosition = aContainer->getPosition();
                } else {
                    location = nullptr;
                    success = false;
                }
            } else {
                location = nullptr;
                success = false;
            }
        } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
            // location is a character
            if (location->getObjectType() == ObjectType::PLAYER){
                // location is a player
                success = location->registerAlias(true, alias, this);
                location = nullptr;
            } else {
                // location is a creature or NPC
                // don't add alias to area, since any items in a creature or NPC inventory is unavailable
                location = nullptr;
                success = true;
            }
        } else {
            // position is NONE, something has gone wrong
            location = nullptr;
            success = false;
        }
    }

    if (success){
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
    }
    
    return success;
}


bool Item::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;
    Container *aContainer = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    while (location != nullptr){
        if (aPosition == ItemPosition::GROUND){
            // location is an area
            success = location->unregisterAlias(true, alias, this);
            location = nullptr;
        } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
            if (location->getObjectType() == ObjectType::CONTAINER){
                // location is a container
                aContainer = dynamic_cast<Container*>(location);
                if (aContainer != nullptr){
                    location = aContainer->getLocation();
                    aPosition = aContainer->getPosition();
                } else {
                    location = nullptr;
                    success = false;
                }
            } else {
                location = nullptr;
                success = false;
            }
        } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
            // location is a character
            if (location->getObjectType() == ObjectType::PLAYER){
                // location is a player
                success = location->unregisterAlias(true, alias, this);
                location = nullptr;
            } else {
                // location is a creature or NPC
                // don't add alias to area, since any items in a creature or NPC inventory is unavailable
                location = nullptr;
                success = true;
            }
        } else {
            // position is NONE, something has gone wrong
            location = nullptr;
            success = false;
        }
    }

    if (success){
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
    }
    
    return success;
}


ObjectType Item::getObjectType() const{
    return ObjectType::ITEM;
}


std::string Item::serialize(){
    return "";
}


bool Item::deserialize(std::string){
    return false;
}


std::string Item::look(std::vector<EffectType> *effects){
    std::string message;
    ItemType *aType = getType();

    message = "The " + getName() + " is ";
    message += aType->getDescription() + ".";

    return message;
}  

// would be better if got rid of dynamic cast **************************************************************
std::string Item::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message;
    Area *anArea = nullptr;
    EffectType anEffect = EffectType::NONE;

    // check if this item is contained within a container
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

    if ((position == ItemPosition::IN) || (position == ItemPosition::ON) || (position == ItemPosition::UNDER)){
        if ((location == aContainer) && (aContainer != nullptr)){
            aContainer->take(nullptr, this, aContainer, nullptr, nullptr);
        } else {
            return "false";
        }
    } else if (position == ItemPosition::GROUND) {
        // location is an area
        anArea = dynamic_cast<Area*>(location);
        if (anArea != nullptr){
            // remove item from area
            anArea->removeItem(this);
        } else {
            return "false";
        }
    } else {
        return "false";
    }

    setPosition(ItemPosition::INVENTORY);
    // get results of take for this object
    message = getTextAndEffect(CommandEnum::TAKE, anEffect);
    message += " ";
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }
    // call this function on player or character, and container
    if (aCharacter != nullptr){
        // aCharacter is doing the taking
        setLocation(aCharacter);
        message += aCharacter->take(nullptr, this, nullptr, aCharacter, effects);
    } else {
        // aPlayer is doing the taking
        setLocation(aPlayer);
        message += aPlayer->take(aPlayer, this, nullptr, nullptr, effects);
    }

    return message;
}

// would be better if got rid of dynamic cast **************************************************************
std::string Item::put(Player *aPlayer, Item *anItem, InteractiveNoun *containingNoun, ItemPosition position, std::vector<EffectType> *effects){
    Area *anArea;
    InteractiveNoun *location;
    ItemPosition currPosition;
    std::string message = "";
    EffectType anEffect;

    location = getLocation();
    currPosition = getPosition();

    // check if this item is contained within a container
    if ((currPosition == ItemPosition::IN) || (currPosition == ItemPosition::ON) || (currPosition == ItemPosition::UNDER)){
        return "false";
    } else if (currPosition == ItemPosition::GROUND) {
        // location is an area
        anArea = dynamic_cast<Area*>(location);
        if (anArea != nullptr){
            // remove item from area
            anArea->removeItem(this);
        } else {
            return "false";
        }
    } else if ((currPosition == ItemPosition::INVENTORY) || (currPosition == ItemPosition::EQUIPPED)){
        // location is a character
        if (location->getID() == aPlayer->getID()){
            aPlayer->removeFromInventory(this);
        } else {
            return "false";
        }
    } else {
        return "false";
    }

    setPosition(position);
    // get results of put for this object
    message = getTextAndEffect(CommandEnum::PUT, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }
    // call this function on containingNoun
    if (containingNoun != nullptr){
        setLocation(containingNoun);
        message += containingNoun->put(aPlayer, this, nullptr, position, effects);
    } 

    return message;
}


std::string Item::drop(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();
    Area *anArea = aPlayer->getLocation();

    if ((position == ItemPosition::INVENTORY) || (position == ItemPosition::EQUIPPED)){
        if ((location != nullptr) && (location->getID() == aPlayer->getID())){
            setLocation(anArea);
            setPosition(ItemPosition::GROUND);
            aPlayer->removeFromInventory(this);
            anArea->addItem(this);

            // get results of put for this object
            message = getTextAndEffect(CommandEnum::DROP, anEffect);
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
        } else {
            message = "false";
        }
    } else {
        message = "false";
    }

    return message;
}


std::string Item::more(){
    std::string message = "Item: " + getName() + "\015\012";
    ItemType *aType = getType();
    int armorBonus = aType->getArmorBonus();
    int damage = aType->getDamage();
    int critMult = aType->getCritMultiplier();
    DamageType resistantTo = aType->getResistantTo();
    DamageType damageType = aType->getDamageType();
    AreaSize range = aType->getRange();

    message += "Type: " + aType->getName() + "\015\012";
    message += "Description: " + aType->getDescription() + "\015\012";
    message += "Weight: " + std::to_string(aType->getWeight()) + "\015\012";
    message += "Cost: " + std::to_string(aType->getCost()) + "\015\012";
    message += "Rarity: ";
    switch(aType->getRarity()){
        case ItemRarity::COMMON:
            message += "common \015\012";
            break;
        case ItemRarity::UNCOMMON:
            message += "uncommon \015\012";
            break;
        case ItemRarity::RARE:
            message += "rare \015\012";
            break;
        case ItemRarity::LEGENDARY:
            message += "legendary \015\012";
            break;
        case ItemRarity::QUEST:
            message += "quest \015\012";
            break;
    }
    switch(aType->getSlotType()){
        case EquipmentSlot::NONE:
            message += "";
            break;
        case EquipmentSlot::HEAD:
            message += "Equipment Slot Type: head \015\012";
            break;
        case EquipmentSlot::SHOULDERS:
            message += "Equipment Slot Type: shoulder \015\012";
            break;
        case EquipmentSlot::NECK:
            message += "Equipment Slot Type: neck \015\012";
            break;
        case EquipmentSlot::TORSO:
            message += "Equipment Slot Type: torso \015\012";
            break;
        case EquipmentSlot::BELT:
            message += "Equipment Slot Type: belt \015\012";
            break;
        case EquipmentSlot::LEGS:
            message += "Equipment Slot Type: legs \015\012";
            break;
        case EquipmentSlot::ARMS:
            message += "Equipment Slot Type: arms \015\012";
            break;
        case EquipmentSlot::HANDS:
            message += "Equipment Slot Type: hands \015\012";
            break;
        case EquipmentSlot::RIGHT_HAND:
            message += "Equipment Slot Type: right hand \015\012";
            break;
        case EquipmentSlot::LEFT_HAND:
            message += "Equipment Slot Type: left hand \015\012";
            break;
        case EquipmentSlot::FEET:
            message += "Equipment Slot Type: feet \015\012";
            break;
        case EquipmentSlot::RIGHT_RING:
            message += "Equipment Slot Type: right ring \015\012";
            break;
        case EquipmentSlot::LEFT_RING:
            message += "Equipment Slot Type: left ring \015\012";
            break;
    }
    if (armorBonus != -1){
        message += "Armor Bonus: " + std::to_string(armorBonus) + "\015\012";
    }
    if (damage != -1){
        message += "Damage: " + std::to_string(damage) + "\015\012";
    }
    if (critMult != -1){
        message += "Crit Multiplier: " + std::to_string(critMult) + "\015\012";
    }
    switch(resistantTo){
        case DamageType::NONE:
            message += "";
            break;
        case DamageType::CRUSHING:
            message += "Resistant to crushing damage\015\012";
            break;
        case DamageType::PIERCING:
            message += "Resistant to piercing damage\015\012";
            break;
        case DamageType::ELECTRIC:
            message += "Resistant to electric damage\015\012";
            break;
        case DamageType::FIRE:
            message += "Resistant to fire damage\015\012";
            break;
        case DamageType::WATER:
            message += "Resistant to water damage\015\012";
            break;
        case DamageType::WIND:
            message += "Resistant to wind damage\015\012";
            break;
        case DamageType::EARTH:
            message += "Resistant to earth damage\015\012";
            break;
        case DamageType::HEAL:
            message += "Resistant to healing\015\012";
            break;
    }
    switch(damageType){
        case DamageType::NONE:
            message += "";
            break;
        case DamageType::CRUSHING:
            message += "Damage Type: crushing damage\015\012";
            break;
        case DamageType::PIERCING:
            message += "Damage Type: piercing damage\015\012";
            break;
        case DamageType::ELECTRIC:
            message += "Damage Type: electric damage\015\012";
            break;
        case DamageType::FIRE:
            message += "Damage Type: fire damage\015\012";
            break;
        case DamageType::WATER:
            message += "Damage Type: water damage\015\012";
            break;
        case DamageType::WIND:
            message += "Damage Type: wind damage\015\012";
            break;
        case DamageType::EARTH:
            message += "Damage Type: earth damage\015\012";
            break;
        case DamageType::HEAL:
            message += "Damage Type: healing\015\012";
            break;
    }
    switch(range){
        case AreaSize::NONE:
            message += "";
            break;
        case AreaSize::SMALL:
            message += "Short Range Weapon \015\012";
            break;
        case AreaSize::MEDIUM:
            message += "Medium Range Weapon\015\012";
            break;
        case AreaSize::LARGE:
            message += "Long Range Weapon\015\012";
            break;
    }

    return message;
} 


std::string Item::equip(Player*, Item*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::unequip(Player*, Item*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::move(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    return "";
}


std::string Item::buy(Player*, Item*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::sell(Player*, Item*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::read(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::breakIt(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::climb(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::turn(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::push(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::pull(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::eat(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Item::drink(Player*, std::vector<EffectType> *effects){
    return "";
} 


InteractiveNoun* Item::copy(){
    return nullptr;
}


bool Item::editAttribute(Player*, std::string){
    return false;
}


bool Item::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Item::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["location"] = DataType::INTERACTIVE_NOUN_PTR;
    signature["position"] = DataType::ITEM_POSITION;
    signature["name"] = DataType::STRING_TYPE;
    signature["type"] = DataType::ITEM_TYPE_PTR;

    return signature;
}

}}