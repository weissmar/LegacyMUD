/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/03/2017
 * \course      CS467, Winter 2017
 * \file        ConditionalElement.cpp
 *
 * \details     Implementation file for ConditionalElement base class.
 ************************************************************************/

#include "ConditionalElement.hpp"
#include "ItemType.hpp"

namespace legacymud { namespace engine {

ConditionalElement::ConditionalElement(){

}


ConditionalElement::ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription){

}


ConditionalElement::ConditionalElement(const ConditionalElement &otherElement){

}


ConditionalElement & ConditionalElement::operator=(const ConditionalElement &otherElement){

}


virtual ConditionalElement::~ConditionalElement(){

}


bool ConditionalElement::isConditional(){

}


ItemType* ConditionalElement::getConditionItem(){

}


std::string ConditionalElement::getDescription(){

}


std::string ConditionalElement::getAltDescription(){

}


bool ConditionalElement::setConditional(bool isConditional){

}


bool ConditionalElement::setConditionItem(ItemType *anItemType){

}


bool ConditionalElement::setDescription(std::string description){

}


bool ConditionalElement::setAltDescription(std::string altDescription){

}


virtual std::string ConditionalElement::serialize(){

}


virtual bool ConditionalElement::deserialize(std::string){

}

}}