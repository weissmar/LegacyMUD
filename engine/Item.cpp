/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/24/2017
 * \course      CS467, Winter 2017
 * \file        Item.cpp
 *
 * \details     Header file for Item class. Defines the members and 
 *              functions that apply to all items. Item defines generic
 *              in-game items.
 ************************************************************************/

#include "Item.hpp"
#include "ItemType.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"

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
{ }


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
    return "";
}  


std::string Item::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message;
    EffectType anEffect = EffectType::NONE;

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
    if (aContainer != nullptr){
        aContainer->take(nullptr, this, aContainer, nullptr, nullptr);
    }

    return message;
}


std::string Item::put(Player*, Item*, InteractiveNoun*, ItemPosition, std::vector<EffectType> *effects){
    return "";
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