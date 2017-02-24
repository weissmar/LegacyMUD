/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/23/2017
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


std::string Feature::getPlacement() const{
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


ObjectType Feature::getObjectType() const{
    return ObjectType::FEATURE;
}


std::string Feature::serialize(){
    return "";
}


bool Feature::deserialize(std::string){
    return false;
}


std::string Feature::look(std::vector<EffectType> *effects){
    return "";
}  


std::string Feature::listen(std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::move(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::read(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::breakIt(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::climb(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::turn(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::push(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::pull(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::eat(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Feature::drink(Player*, std::vector<EffectType> *effects){
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