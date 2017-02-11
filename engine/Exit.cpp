/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        Exit.cpp
 *
 * \details     Implementation file for Exit class. 
 ************************************************************************/

#include "Exit.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

Exit::Exit(){

}


Exit::Exit(ExitDirection direction, EffectType effect, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription){

}


Exit::Exit(const Exit &otherExit){

}


Exit & Exit::operator=(const Exit &otherExit){

}


Exit::~Exit(){

}


ExitDirection Exit::getDirection(){

}


EffectType Exit::getEffect(){

}


Area* Exit::getConnectArea(){

}


bool Exit::setDirection(ExitDirection aDirection){

}


bool Exit::setEffect(EffectType anEffect){

}


bool Exit::setConnectArea(Area *anArea){

}


ObjectType Exit::getObjectType(){

}


std::string Exit::serialize(){

}


bool Exit::deserialize(std::string){

}


std::string Exit::look(){

}  


std::string Exit::listen(){

} 


std::string Exit::move(Player*){

} 


std::string Exit::read(Player*){

} 


std::string Exit::breakIt(Player*){

} 


std::string Exit::climb(Player*){

} 


std::string Exit::turn(Player*){

} 


std::string Exit::push(Player*){

} 


std::string Exit::pull(Player*){

} 


std::string Exit::eat(Player*){

} 


std::string Exit::drink(Player*){

} 


InteractiveNoun* Exit::copy(){

}


bool Exit::editAttribute(Player*, std::string){

}


bool Exit::editWizard(Player*){

}


std::map<std::string, DataType> Exit::getAttributeSignature(){

}

}}