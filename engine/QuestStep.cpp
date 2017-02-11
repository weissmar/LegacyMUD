/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        QuestStep.cpp
 *
 * \details     Implementation file for QuestStep class. 
 ************************************************************************/

#include "QuestStep.hpp"
#include "ItemType.hpp"
#include "NonCombatant.hpp"

namespace legacymud { namespace engine {

QuestStep::QuestStep(){

}


QuestStep::QuestStep(int ordinalNumber, std::string description, ItemType *anItemType, NonCombatant *giver, NonCombatant *receiver, std::string completionText){

}


QuestStep::QuestStep(const QuestStep &otherQuestStep){

}


QuestStep & QuestStep::operator=(const QuestStep &otherQuestStep){

}


virtual QuestStep::~QuestStep(){

}


int QuestStep::getOrdinalNumber(){

}


std::string QuestStep::getDescription(){

}


ItemType* QuestStep::getFetchItem(){

}


NonCombatant* QuestStep::getGiver(){

}


NonCombatant* QuestStep::getReceiver(){

}


std::string QuestStep::getCompletionText(){

}


bool QuestStep::setOrdinalNumber(int number){

}


bool QuestStep::setDescription(std::string description){

}


bool QuestStep::setFetchItem(ItemType *anItemType){

}


bool QuestStep::setGiver(NonCombatant *giver){

}


bool QuestStep::setReceiver(NonCombatant *receiver){

}


bool QuestStep::setCompletionText(std::string completionText){

} 


virtual ObjectType QuestStep::getObjectType(){

}


virtual std::string QuestStep::serialize(){

}


virtual bool QuestStep::deserialize(std::string){

}


virtual InteractiveNoun* QuestStep::copy(){

}


virtual bool QuestStep::editAttribute(Player*, std::string){

}


virtual bool QuestStep::editWizard(Player*){

}


static std::map<std::string, DataType> QuestStep::getAttributeSignature(){

}

}}