/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Container.cpp
 *
 * \details     Implementation file for Container class. 
 ************************************************************************/

#include "Container.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

const int DEFAULT_CAPACITY = 10;

Container::Container()
: Item()
, insideCapacity(DEFAULT_CAPACITY)
{ }


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type)
: Item(location, position, name, type)
, insideCapacity(capacity)
{ }


/*Container::Container(const Container &otherContainer){

}


Container & Container::operator=(const Container &otherContainer){

}


Container::~Container(){

}*/


bool Container::isEmpty() const{
    std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
    std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
    std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
    std::lock(underLock, insideLock, onTopOfLock);

    return under.empty() && inside.empty() && onTopOf.empty();
}


bool Container::remove(Item *anItem){
    return false;
}


bool Container::place(Item *anItem, ItemPosition position){
    return false;
}


std::vector<Item*> Container::getInsideContents() const{
    std::lock_guard<std::mutex> insideLock(insideMutex);
    return inside;
}


std::vector<Item*> Container::getUnderContents() const{
    std::lock_guard<std::mutex> underLock(underMutex);
    return under;
}


std::vector<Item*> Container::getTopContents() const{
    std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
    return onTopOf;
}


int Container::getInsideCapacity() const{
    return insideCapacity.load();
}


bool Container::setInsideCapacity(int capacity){
    insideCapacity.store(capacity);

    return true;
}


ObjectType Container::getObjectType() const{
    return ObjectType::CONTAINER;
}


std::string Container::serialize(){
    return "";
}


bool Container::deserialize(std::string){
    return false;
}


std::string Container::look(){
    return "";
}  


bool Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter){
    return false;
}


bool Container::put(Player *aPlayer, Item *anItem, InteractiveNoun*, ItemPosition position){
    return false;
}


bool Container::drop(Player *aPlayer){
    return false;
}


std::string Container::more(){
    return "";
} 


bool Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun*){
    return false;
}


bool Container::unequip(Player *aPlayer, Item *anItem, InteractiveNoun*){
    return false;
}


bool Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun*, InteractiveNoun*){
    return false;
}


std::string Container::move(Player *aPlayer){
    return "";
} 


bool Container::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool playerAttacker){
    return false;
}


bool Container::buy(Player *aPlayer, Item *anItem){
    return false;
}


bool Container::sell(Player *aPlayer, Item *anItem){
    return false;
}


std::string Container::search(Player *aPlayer){
    return "";
} 


std::string Container::read(Player *aPlayer){
    return "";
} 


std::string Container::breakIt(Player *aPlayer){
    return "";
} 


std::string Container::climb(Player *aPlayer){
    return "";
} 


std::string Container::turn(Player *aPlayer){
    return "";
} 


std::string Container::push(Player *aPlayer){
    return "";
} 


std::string Container::pull(Player *aPlayer){
    return "";
} 


std::string Container::eat(Player *aPlayer){
    return "";
} 


std::string Container::drink(Player *aPlayer){
    return "";
} 


InteractiveNoun* Container::copy(){
    return nullptr;
}


bool Container::editAttribute(Player *aPlayer, std::string){
    return false;
}


bool Container::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Container::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["capacity"] = DataType::INT_TYPE;
    signature["location"] = DataType::INTERACTIVE_NOUN_PTR;
    signature["position"] = DataType::ITEM_POSITION;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["type"] = DataType::ITEM_TYPE_PTR;

    return signature;
}

}}
