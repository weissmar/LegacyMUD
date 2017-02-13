/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/13/2017
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


bool ConditionalElement::isConditional(){
    return conditionSet;
}


ItemType* ConditionalElement::getConditionItem(){
    return conditionItem;
}


std::string ConditionalElement::getDescription(){
    return description;
}


std::string ConditionalElement::getAltDescription(){
    return altDescription;
}


bool ConditionalElement::setConditional(bool isConditional){
    return false;
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