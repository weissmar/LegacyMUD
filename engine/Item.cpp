/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/25/2017
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
    addNounAlias(name);
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


std::string Item::drop(Player*, std::vector<EffectType> *effects){
    return "";
}


std::string Item::more(std::vector<EffectType> *effects){
    return "";
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