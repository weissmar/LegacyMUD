/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/15/2017
 * \course      CS467, Winter 2017
 * \file        QuestStep.cpp
 *
 * \details     Implementation file for QuestStep class. 
 ************************************************************************/

#include "QuestStep.hpp"
#include "ItemType.hpp"
#include "NonCombatant.hpp"

namespace legacymud { namespace engine {

QuestStep::QuestStep()
: InteractiveNoun()
, ordinalNumber(0)
, description("")
, fetchItem(nullptr)
, giver(nullptr)
, receiver(nullptr)
, completionText("")
{ }


QuestStep::QuestStep(int ordinalNumber, std::string description, ItemType *anItemType, NonCombatant *giver, NonCombatant *receiver, std::string completionText)
: InteractiveNoun()
, ordinalNumber(ordinalNumber)
, description(description)
, fetchItem(anItemType)
, giver(giver)
, receiver(receiver)
, completionText(completionText)
{ }


/*QuestStep::QuestStep(const QuestStep &otherQuestStep){

}


QuestStep & QuestStep::operator=(const QuestStep &otherQuestStep){

}


QuestStep::~QuestStep(){

}*/


int QuestStep::getOrdinalNumber(){
    return ordinalNumber;
}


std::string QuestStep::getDescription(){
    return description;
}


ItemType* QuestStep::getFetchItem(){
    return fetchItem;
}


NonCombatant* QuestStep::getGiver(){
    return giver;
}


NonCombatant* QuestStep::getReceiver(){
    return receiver;
}


std::string QuestStep::getCompletionText(){
    return completionText;
}


bool QuestStep::setOrdinalNumber(int number){
    ordinalNumber = number;

    return true; 
}


bool QuestStep::setDescription(std::string description){
    this->description = description;

    return true;
}


bool QuestStep::setFetchItem(ItemType *anItemType){
    if (anItemType != nullptr){
        fetchItem = anItemType;
        return true;
    }

    return false; 
}


bool QuestStep::setGiver(NonCombatant *giver){
    if (giver != nullptr){
        this->giver = giver;
        return true;
    }

    return false; 
}


bool QuestStep::setReceiver(NonCombatant *receiver){
    if (receiver != nullptr){
        this->receiver = receiver;
        return true;
    }

    return false; 
}


bool QuestStep::setCompletionText(std::string completionText){
    this->completionText = completionText;

    return true;
} 


std::string QuestStep::getName() const{
    return description;
}


ObjectType QuestStep::getObjectType(){
    return ObjectType::QUEST_STEP;
}


std::string QuestStep::serialize(){
    return "";
}


bool QuestStep::deserialize(std::string){
    return false;
}


InteractiveNoun* QuestStep::copy(){
    return nullptr;
}


bool QuestStep::editAttribute(Player*, std::string){
    return false;
}


bool QuestStep::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> QuestStep::getAttributeSignature(){
    std::map<std::string, DataType> signature;
   
    signature["ordinal number"] = DataType::INT_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["fetch item"] = DataType::ITEM_TYPE_PTR;
    signature["giver"] = DataType::NON_COMBATANT_PTR;
    signature["receiver"] = DataType::NON_COMBATANT_PTR;
    signature["completion text"] = DataType::STRING_TYPE;

    return signature;
}

}}