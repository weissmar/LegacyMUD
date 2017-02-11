/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        Container.cpp
 *
 * \details     Implementation file for Container class. 
 ************************************************************************/

#include "Container.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

Container::Container(){

}


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type){

}


Container::Container(const Container &otherContainer){

}


Container & Container::operator=(const Container &otherContainer){

}


Container::~Container(){

}


bool Container::isEmpty(){

}


bool Container::remove(Item *anItem){

}


bool Container::place(Item *anItem, ItemPosition position){

}


std::vector<Item*> Container::getInsideContents(){

}


std::vector<Item*> Container::getUnderContents(){

}


std::vector<Item*> Container::getTopContents(){

}


int Container::getInsideCapacity(){

}


bool Container::setInsideCapacity(int capacity){

}


ObjectType Container::getObjectType(){

}


std::string Container::serialize(){

}


bool Container::deserialize(std::string){

}


std::string Container::look(){

}  


bool Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter){

}


bool Container::put(Player *aPlayer, Item *anItem, InteractiveNoun*, ItemPosition position){

}


bool Container::drop(Player *aPlayer){

}


std::string Container::more(){

} 


bool Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun*){

}


bool Container::unequip(Player *aPlayer, Item *anItem, InteractiveNoun*){

}


bool Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun*, InteractiveNoun*){

}


std::string Container::move(Player *aPlayer){

} 


bool Container::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool playerAttacker){

}


bool Container::buy(Player *aPlayer, Item *anItem){

}


bool Container::sell(Player *aPlayer, Item *anItem){

}


std::string Container::search(Player *aPlayer){

} 


std::string Container::read(Player *aPlayer){

} 


std::string Container::breakIt(Player *aPlayer){

} 


std::string Container::climb(Player *aPlayer){

} 


std::string Container::turn(Player *aPlayer){

} 


std::string Container::push(Player *aPlayer){

} 


std::string Container::pull(Player *aPlayer){

} 


std::string Container::eat(Player *aPlayer){

} 


std::string Container::drink(Player *aPlayer){

} 


InteractiveNoun* Container::copy(){

}


bool Container::editAttribute(Player *aPlayer, std::string){

}


bool Container::editWizard(Player *aPlayer){

}


std::map<std::string, DataType> Container::getAttributeSignature(){

}

}}
