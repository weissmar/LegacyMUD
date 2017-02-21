/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        ConditionalElement.cpp
 *
 * \details     Implementation file for ConditionalElement base class.
 ************************************************************************/

#include "ConditionalElement.hpp"
#include "ItemType.hpp"

namespace legacymud { namespace engine {

ConditionalElement::ConditionalElement()
: InteractiveNoun()
, conditionSet(false)
, conditionItem(nullptr)
, description("")
, altDescription("")
{ }


ConditionalElement::ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: InteractiveNoun()
, conditionSet(isConditional)
, conditionItem(anItemType)
, description(description)
, altDescription(altDescription)
{ }


/*ConditionalElement::ConditionalElement(const ConditionalElement &otherElement){

}


ConditionalElement & ConditionalElement::operator=(const ConditionalElement &otherElement){

}


ConditionalElement::~ConditionalElement(){

}*/


bool ConditionalElement::isConditional() const{
    return conditionSet.load();
}


ItemType* ConditionalElement::getConditionItem() const{
    std::lock_guard<std::mutex> conditionItemLock(conditionItemMutex);
    return conditionItem;
}


std::string ConditionalElement::getDescription() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


std::string ConditionalElement::getAltDescription() const{
    std::lock_guard<std::mutex> altDescriptionLock(altDescriptionMutex);
    return altDescription;
}


bool ConditionalElement::setConditional(bool isConditional){
    conditionSet.store(isConditional);
    return true;
}


bool ConditionalElement::setConditionItem(ItemType *anItemType){
    return false;
}


bool ConditionalElement::setDescription(std::string description){
    return false;
}


bool ConditionalElement::setAltDescription(std::string altDescription){
    return false;
}


std::string ConditionalElement::serialize(){
    return "";
}


bool ConditionalElement::deserialize(std::string){
    return false;
}

}}