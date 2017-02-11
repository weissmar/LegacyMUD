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


virtual Feature::~Feature(){

}


std::string Feature::getName(){

}


std::string Feature::getPlacement(){

}


bool Feature::setName(std::string){

}


bool Feature::setPlacement(std::string){

}


virtual ObjectType Feature::getObjectType(){

}


virtual std::string Feature::serialize(){

}


virtual bool Feature::deserialize(std::string){

}


virtual std::string Feature::look(){

}  


virtual std::string Feature::listen(){

} 


virtual std::string Feature::move(Player*){

} 


virtual std::string Feature::read(Player*){

} 


virtual std::string Feature::breakIt(Player*){

} 


virtual std::string Feature::climb(Player*){

} 


virtual std::string Feature::turn(Player*){

} 


virtual std::string Feature::push(Player*){

} 


virtual std::string Feature::pull(Player*){

} 


virtual std::string Feature::eat(Player*){

} 


virtual std::string Feature::drink(Player*){

} 


virtual InteractiveNoun* Feature::copy(){

}


virtual bool Feature::editAttribute(Player*, std::string){

}


virtual bool Feature::editWizard(Player*){

}


static std::map<std::string, DataType> Feature::getAttributeSignature(){

}
  
}}