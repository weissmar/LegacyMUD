/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
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

Item::Item(){

}


Item::Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type){

}


Item::Item(const Item &otherItem){

}


Item & Item::operator=(const Item &otherItem){

}


Item::~Item(){

}


InteractiveNoun* Item::getLocation(){

}


ItemPosition Item::getPosition(){

}


std::string Item::getName(){

}


ItemType* Item::getType(){

}


bool Item::setLocation(InteractiveNoun* containingNoun){

}


bool Item::setPosition(ItemPosition position){

}


bool Item::setName(std::string name){

}


bool Item::setType(ItemType *type){

}


ObjectType Item::getObjectType(){

}


std::string Item::serialize(){

}


bool Item::deserialize(std::string){

}


std::string Item::look(){

}  


bool Item::take(Player*, Item*, InteractiveNoun*){

}


bool Item::put(Player*, Item*, InteractiveNoun*, ItemPosition){

}


bool Item::drop(Player*){

}


std::string Item::more(){

} 


bool Item::equip(Player*, Item*, InteractiveNoun*){

}


bool Item::unequip(Player*, Item*, InteractiveNoun*){

}


bool Item::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


std::string Item::move(Player*){

} 


bool Item::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


bool Item::buy(Player*, Item*){

}


bool Item::sell(Player*, Item*){

}


std::string Item::read(Player*){

} 


std::string Item::breakIt(Player*){

} 


std::string Item::climb(Player*){

} 


std::string Item::turn(Player*){

} 


std::string Item::push(Player*){

} 


std::string Item::pull(Player*){

} 


std::string Item::eat(Player*){

} 


std::string Item::drink(Player*){

} 


InteractiveNoun* Item::copy(){

}


bool Item::editAttribute(Player*, std::string){

}


bool Item::editWizard(Player*){

}


std::map<std::string, DataType> Item::getAttributeSignature(){

}

}}