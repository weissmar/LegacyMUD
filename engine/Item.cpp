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


virtual Item::~Item(){

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


virtual ObjectType Item::getObjectType(){

}


virtual std::string Item::serialize(){

}


virtual bool Item::deserialize(std::string){

}


virtual std::string Item::look(){

}  


virtual bool Item::take(Player*, Item*, InteractiveNoun*){

}


virtual bool Item::put(Player*, Item*, InteractiveNoun*, ItemPosition){

}


virtual bool Item::drop(Player*){

}


virtual std::string Item::more(){

} 


virtual bool Item::equip(Player*, Item*, InteractiveNoun*){

}


virtual bool Item::unequip(Player*, Item*, InteractiveNoun*){

}


virtual bool Item::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


virtual std::string Item::move(Player*){

} 


virtual bool Item::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


virtual bool Item::buy(Player*, Item*){

}


virtual bool Item::sell(Player*, Item*){

}


virtual std::string Item::read(Player*){

} 


virtual std::string Item::breakIt(Player*){

} 


virtual std::string Item::climb(Player*){

} 


virtual std::string Item::turn(Player*){

} 


virtual std::string Item::push(Player*){

} 


virtual std::string Item::pull(Player*){

} 


virtual std::string Item::eat(Player*){

} 


virtual std::string Item::drink(Player*){

} 


virtual InteractiveNoun* Item::copy(){

}


virtual bool Item::editAttribute(Player*, std::string){

}


virtual bool Item::editWizard(Player*){

}


static std::map<std::string, DataType> Item::getAttributeSignature(){

}

}}