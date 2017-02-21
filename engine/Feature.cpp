/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Feature.cpp
 *
 * \details     Implementation file for Feature class. 
 ************************************************************************/

#include "Feature.hpp"

namespace legacymud { namespace engine {

Feature::Feature()
: ConditionalElement()
, name("")
, placement("")
{ }


Feature::Feature(std::string name, std::string placement, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, name("")
, placement("")
{ }


std::string Feature::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Feature::getPlacement(){
    std::lock_guard<std::mutex> placementLock(placementMutex);
    return placement;
}


bool Feature::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;

    return true;
}


bool Feature::setPlacement(std::string placement){
    std::lock_guard<std::mutex> placementLock(placementMutex);
    this->placement = placement;

    return true;
}


ObjectType Feature::getObjectType(){
    return ObjectType::FEATURE;
}


std::string Feature::serialize(){
    return "";
}


bool Feature::deserialize(std::string){
    return false;
}


std::string Feature::look(){
    return "";
}  


std::string Feature::listen(){
    return "";
} 


std::string Feature::move(Player*){
    return "";
} 


std::string Feature::read(Player*){
    return "";
} 


std::string Feature::breakIt(Player*){
    return "";
} 


std::string Feature::climb(Player*){
    return "";
} 


std::string Feature::turn(Player*){
    return "";
} 


std::string Feature::push(Player*){
    return "";
} 


std::string Feature::pull(Player*){
    return "";
} 


std::string Feature::eat(Player*){
    return "";
} 


std::string Feature::drink(Player*){
    return "";
} 


InteractiveNoun* Feature::copy(){
    return nullptr;
}


bool Feature::editAttribute(Player*, std::string){
    return false;
}


bool Feature::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Feature::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["placement"] = DataType::STRING_TYPE;
    signature["is conditional"] = DataType::BOOL_TYPE;
    signature["condition item"] = DataType::ITEM_TYPE_PTR;
    signature["description"] = DataType::STRING_TYPE;
    signature["alternate description"] = DataType::STRING_TYPE;

    return signature;
}
  
}}