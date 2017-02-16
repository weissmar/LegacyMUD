/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/13/2017
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


InteractiveNoun* Item::getLocation(){
    return location;
}


ItemPosition Item::getPosition(){
    return position;
}


std::string Item::getName() const{
    return name;
}


ItemType* Item::getType(){
    return type;
}


bool Item::setLocation(InteractiveNoun* containingNoun){
    if (containingNoun != nullptr){
        location = containingNoun;
        return true;
    }

    return false;
}


bool Item::setPosition(ItemPosition position){
    this->position = position;

    return true;
}


bool Item::setName(std::string name){
    this->name = name;

    return true;
}


bool Item::setType(ItemType *type){
    if (type != nullptr){
        this->type = type;
        return true;
    }

    return false;
}


ObjectType Item::getObjectType(){
    return ObjectType::ITEM;
}


std::string Item::serialize(){
    return "";
}


bool Item::deserialize(std::string){
    return false;
}


std::string Item::look(){
    return "";
}  


bool Item::take(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Item::put(Player*, Item*, InteractiveNoun*, ItemPosition){
    return false;
}


bool Item::drop(Player*){
    return false;
}


std::string Item::more(){
    return "";
} 


bool Item::equip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Item::unequip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Item::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){
    return false;
}


std::string Item::move(Player*){
    return "";
} 


bool Item::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){
    return false;
}


bool Item::buy(Player*, Item*){
    return false;
}


bool Item::sell(Player*, Item*){
    return false;
}


std::string Item::read(Player*){
    return "";
} 


std::string Item::breakIt(Player*){
    return "";
} 


std::string Item::climb(Player*){
    return "";
} 


std::string Item::turn(Player*){
    return "";
} 


std::string Item::push(Player*){
    return "";
} 


std::string Item::pull(Player*){
    return "";
} 


std::string Item::eat(Player*){
    return "";
} 


std::string Item::drink(Player*){
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