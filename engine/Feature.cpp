/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/27/2017
 * \course      CS467, Winter 2017
 * \file        Feature.cpp
 *
 * \details     Implementation file for Feature class. 
 ************************************************************************/

#include "Feature.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

Feature::Feature()
: ConditionalElement()
, name("")
, placement("")
, location(nullptr)
{ }


Feature::Feature(std::string name, std::string placement, Area *location, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, name(name)
, placement(placement)
, location(location)
{
    InteractiveNoun::addNounAlias(name);
}


std::string Feature::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Feature::getPlacement() const{
    std::lock_guard<std::mutex> placementLock(placementMutex);
    return placement;
}


Area* Feature::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
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


bool Feature::setLocation(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> locationLock(locationMutex);
        location = anArea;
        return true;
    }

    return false;
}


bool Feature::addNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(false, alias, this);
        success = InteractiveNoun::addNounAlias(alias);
    }

    return success;
}


bool Feature::removeNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(false, alias, this);
        success = InteractiveNoun::removeNounAlias(alias);
    }

    return success;
}


bool Feature::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(true, alias, this);
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
    }

    return success;
}


bool Feature::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(true, alias, this);
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
    }

    return success;
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


std::string Feature::look(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
}  


std::string Feature::listen(std::vector<EffectType> *effects){
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