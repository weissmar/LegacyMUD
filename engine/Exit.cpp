/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/23/2017
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


ExitDirection Exit::getDirection() const{
    return direction.load();
}


std::string Exit::getDirectionString() const{
    std::string message;

    switch (direction.load()) {
        case ExitDirection::NORTH:
            message = "To the north";
            break;        
        case ExitDirection::SOUTH:
            message = "To the south";
            break;        
        case ExitDirection::EAST:
            message = "To the east";
            break;        
        case ExitDirection::WEST:
            message = "To the west";
            break;        
        case ExitDirection::NORTHEAST:
            message = "To the northeast";
            break;        
        case ExitDirection::NORTHWEST:
            message = "To the northwest";
            break;        
        case ExitDirection::SOUTHEAST:
            message = "To the southeast";
            break;        
        case ExitDirection::SOUTHWEST :
            message = "To the southwest";
            break;        
        case ExitDirection::UP:
            message = "Above";
            break;        
        case ExitDirection::DOWN:
            message = "Below";
            break;
        default:
            message = "";
    }

    return message;
}


EffectType Exit::getEffect() const{
    return effect.load();
}


Area* Exit::getConnectArea() const{
    std::lock_guard<std::mutex> connectAreaLock(connectAreaMutex);
    return connectArea;
}


bool Exit::setDirection(ExitDirection aDirection){
    direction.store(aDirection);

    return true;
}


bool Exit::setEffect(EffectType anEffect){
    effect.store(anEffect);

    return true;
}


bool Exit::setConnectArea(Area *anArea){
    std::lock_guard<std::mutex> connectAreaLock(connectAreaMutex);
    if (anArea != nullptr){
        connectArea = anArea;
        return true;
    }

    return false;
}


std::string Exit::getName() const{
    return this->getDescription();
}


ObjectType Exit::getObjectType() const{
    return ObjectType::EXIT;
}


std::string Exit::serialize(){
    return "";
}


bool Exit::deserialize(std::string){
    return false;
}


std::string Exit::look(std::vector<EffectType> *effects){
    return "";
}  


std::string Exit::listen(std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::move(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::read(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::breakIt(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::climb(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::turn(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::push(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::pull(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::eat(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::drink(Player*, std::vector<EffectType> *effects){
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