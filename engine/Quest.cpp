/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        Quest.cpp
 *
 * \details     Implementation file for Quest class. 
 ************************************************************************/

#include "Quest.hpp"
#include "Item.hpp"
#include "QuestStep.hpp"

namespace legacymud { namespace engine {

Quest::Quest(){

}


Quest::Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem){

}


Quest::Quest(const Quest &otherQuest){

}


Quest & Quest::operator=(const Quest &otherQuest){

}


Quest::~Quest(){

}


std::string Quest::getName(){

}


std::string Quest::getDescription(){

}


int Quest::getRewardMoney(){

}


Item* Quest::getRewardItem(){

}


std::vector<std::tuple<int, QuestStep*>> Quest::getSteps(){

}


bool Quest::setName(std::string name){

}


bool Quest::setDescription(std::string description){

}


bool Quest::setRewardMoney(int money){

}


bool Quest::setRewardItem(Item *rewardItem){

}


bool Quest::addStep(int, QuestStep *aStep){

}


bool Quest::removeStep(QuestStep *aStep){

}


ObjectType Quest::getObjectType(){

}


std::string Quest::serialize(){

}


bool Quest::deserialize(std::string){

}


InteractiveNoun* Quest::copy(){

}


bool Quest::editAttribute(Player*, std::string){

}


bool Quest::editWizard(Player*){

}


std::map<std::string, DataType> Quest::getAttributeSignature(){

}

}}