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


virtual Exit::~Exit(){

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


virtual ObjectType Exit::getObjectType(){

}


virtual std::string Exit::serialize(){

}


virtual bool Exit::deserialize(std::string){

}


virtual std::string Exit::look(){

}  


virtual std::string Exit::listen(){

} 


virtual std::string Exit::move(Player*){

} 


virtual std::string Exit::read(Player*){

} 


virtual std::string Exit::breakIt(Player*){

} 


virtual std::string Exit::climb(Player*){

} 


virtual std::string Exit::turn(Player*){

} 


virtual std::string Exit::push(Player*){

} 


virtual std::string Exit::pull(Player*){

} 


virtual std::string Exit::eat(Player*){

} 


virtual std::string Exit::drink(Player*){

} 


virtual InteractiveNoun* Exit::copy(){

}


virtual bool Exit::editAttribute(Player*, std::string){

}


virtual bool Exit::editWizard(Player*){

}


static std::map<std::string, DataType> Exit::getAttributeSignature(){

}

}}