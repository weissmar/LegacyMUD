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


virtual Container::~Container(){

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


virtual ObjectType Container::getObjectType(){

}


virtual std::string Container::serialize(){

}


virtual bool Container::deserialize(std::string){

}


virtual std::string Container::look(){

}  


virtual bool Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter){

}


virtual bool Container::put(Player *aPlayer, Item *anItem, InteractiveNoun*, ItemPosition position){

}


virtual bool Container::drop(Player *aPlayer){

}


virtual std::string Container::more(){

} 


virtual bool Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun*){

}


virtual bool Container::unequip(Player *aPlayer, Item *anItem, InteractiveNoun*){

}


virtual bool Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun*, InteractiveNoun*){

}


virtual std::string Container::move(Player *aPlayer){

} 


virtual bool Container::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool playerAttacker){

}


virtual bool Container::buy(Player *aPlayer, Item *anItem){

}


virtual bool Container::sell(Player *aPlayer, Item *anItem){

}


virtual std::string Container::search(Player *aPlayer){

} 


virtual std::string Container::read(Player *aPlayer){

} 


virtual std::string Container::breakIt(Player *aPlayer){

} 


virtual std::string Container::climb(Player *aPlayer){

} 


virtual std::string Container::turn(Player *aPlayer){

} 


virtual std::string Container::push(Player *aPlayer){

} 


virtual std::string Container::pull(Player *aPlayer){

} 


virtual std::string Container::eat(Player *aPlayer){

} 


virtual std::string Container::drink(Player *aPlayer){

} 


virtual InteractiveNoun* Container::copy(){

}


virtual bool Container::editAttribute(Player *aPlayer, std::string){

}


virtual bool Container::editWizard(Player *aPlayer){

}


static std::map<std::string, DataType> Container::getAttributeSignature(){

}

}}
