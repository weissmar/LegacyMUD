/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/25/2017
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
, location(nullptr)
, connectArea(nullptr)
{ }


Exit::Exit(ExitDirection direction, EffectType effect, Area *location, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, direction(direction)
, effect(effect)
, location(location)
, connectArea(connectArea)
{
    InteractiveNoun::addNounAlias(description);
    InteractiveNoun::addNounAlias(altDescription);
}


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


Area* Exit::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
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


bool Exit::setLocation(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> locationLock(locationMutex);
        location = anArea;
        return true;
    }

    return false;
}


bool Exit::setConnectArea(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> connectAreaLock(connectAreaMutex);
        connectArea = anArea;
        return true;
    }

    return false;
}


std::string Exit::getName() const{
    return this->getDescription();
}


bool Exit::addNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(false, alias, this);
        success = InteractiveNoun::addNounAlias(alias);
    }

    return success;
}


bool Exit::removeNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(false, alias, this);
        success = InteractiveNoun::removeNounAlias(alias);
    }

    return success;
}


bool Exit::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(true, alias, this);
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
    }

    return success;
}


bool Exit::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(true, alias, this);
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
    }

    return success;
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