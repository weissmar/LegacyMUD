/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        Feature.cpp
 *
 * \details     Implementation file for Feature class. 
 ************************************************************************/

#include "Feature.hpp"

namespace legacymud { namespace engine {

Feature::Feature(){

}


Feature::Feature(std::string name, std::string placement, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription){

}


Feature::Feature(const Feature &otherFeature){

}


Feature & Feature::operator=(const Feature &otherFeature){

}


Feature::~Feature(){

}


std::string Feature::getName(){

}


std::string Feature::getPlacement(){

}


bool Feature::setName(std::string){

}


bool Feature::setPlacement(std::string){

}


ObjectType Feature::getObjectType(){

}


std::string Feature::serialize(){

}


bool Feature::deserialize(std::string){

}


std::string Feature::look(){

}  


std::string Feature::listen(){

} 


std::string Feature::move(Player*){

} 


std::string Feature::read(Player*){

} 


std::string Feature::breakIt(Player*){

} 


std::string Feature::climb(Player*){

} 


std::string Feature::turn(Player*){

} 


std::string Feature::push(Player*){

} 


std::string Feature::pull(Player*){

} 


std::string Feature::eat(Player*){

} 


std::string Feature::drink(Player*){

} 


InteractiveNoun* Feature::copy(){

}


bool Feature::editAttribute(Player*, std::string){

}


bool Feature::editWizard(Player*){

}


std::map<std::string, DataType> Feature::getAttributeSignature(){

}
  
}}