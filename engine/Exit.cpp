/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/15/2017
 * \course      CS467, Winter 2017
 * \file        Exit.cpp
 *
 * \details     Implementation file for Exit class. 
 ************************************************************************/

#include "Exit.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

Exit::Exit()
: ConditionalElement()
, direction(ExitDirection::NORTH)
, effect(EffectType::NONE)
, connectArea(nullptr)
{ }


Exit::Exit(ExitDirection direction, EffectType effect, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, direction(direction)
, effect(effect)
, connectArea(connectArea)
{ }


/*Exit::Exit(const Exit &otherExit){

}


Exit & Exit::operator=(const Exit &otherExit){

}


Exit::~Exit(){

}*/


ExitDirection Exit::getDirection(){
    return direction;
}


EffectType Exit::getEffect(){
    return effect;
}


Area* Exit::getConnectArea(){
    return connectArea;
}


bool Exit::setDirection(ExitDirection aDirection){
    direction = aDirection;

    return true;
}


bool Exit::setEffect(EffectType anEffect){
    effect = anEffect;

    return true;
}


bool Exit::setConnectArea(Area *anArea){
    if (anArea != nullptr){
        connectArea = anArea;
        return true;
    }

    return false;
}


std::string Exit::getName() const{
    return this->getDescription();
}


ObjectType Exit::getObjectType(){
    return ObjectType::EXIT;
}


std::string Exit::serialize(){
    return "";
}


bool Exit::deserialize(std::string){
    return false;
}


std::string Exit::look(){
    return "";
}  


std::string Exit::listen(){
    return "";
} 


std::string Exit::move(Player*){
    return "";
} 


std::string Exit::read(Player*){
    return "";
} 


std::string Exit::breakIt(Player*){
    return "";
} 


std::string Exit::climb(Player*){
    return "";
} 


std::string Exit::turn(Player*){
    return "";
} 


std::string Exit::push(Player*){
    return "";
} 


std::string Exit::pull(Player*){
    return "";
} 


std::string Exit::eat(Player*){
    return "";
} 


std::string Exit::drink(Player*){
    return "";
} 


InteractiveNoun* Exit::copy(){
    return nullptr;
}


bool Exit::editAttribute(Player*, std::string){
    return false;
}


bool Exit::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Exit::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["direction"] = DataType::EXIT_DIRECTION;
    signature["effect"] = DataType::EFFECT_TYPE;
    signature["connecting area"] = DataType::AREA_PTR;
    signature["is conditional"] = DataType::BOOL_TYPE;
    signature["condition item"] = DataType::ITEM_TYPE_PTR;
    signature["description"] = DataType::STRING_TYPE;
    signature["alternate description"] = DataType::STRING_TYPE;

    return signature;
}

}}