/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        ConditionalElement.cpp
 *
 * \details     Implementation file for ConditionalElement base class.
 ************************************************************************/

#include "ConditionalElement.hpp"
#include "ItemType.hpp"
#include "EffectType.hpp"

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


ConditionalElement::ConditionalElement(bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID)
: InteractiveNoun(anID)
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
    if (anItemType != nullptr){
        std::lock_guard<std::mutex> conditionItemLock(conditionItemMutex);
        conditionItem = anItemType;
        return true;
    }
    return false; 
}


bool ConditionalElement::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;
    return true;
}


bool ConditionalElement::setAltDescription(std::string altDescription){
    std::lock_guard<std::mutex> altDescriptionLock(altDescriptionMutex);
    this->altDescription = altDescription;
    return true;
}


std::string ConditionalElement::move(Player*, std::vector<EffectType> *effects){ 
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of move for this object
    message = getTextAndEffect(CommandEnum::MOVE, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::read(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of read for this object
    message = getTextAndEffect(CommandEnum::READ, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::breakIt(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of break for this object
    message = getTextAndEffect(CommandEnum::BREAK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::climb(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of climb for this object
    message = getTextAndEffect(CommandEnum::CLIMB, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::turn(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of turn for this object
    message = getTextAndEffect(CommandEnum::TURN, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::push(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of push for this object
    message = getTextAndEffect(CommandEnum::PUSH, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::pull(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of pull for this object
    message = getTextAndEffect(CommandEnum::PULL, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::eat(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of eat for this object
    message = getTextAndEffect(CommandEnum::EAT, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::drink(Player*, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;

    // get results of drink for this object
    message = getTextAndEffect(CommandEnum::DRINK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


std::string ConditionalElement::serialize(){
    return "";
}


}}