/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/13/2017
 * \course      CS467, Winter 2017
 * \file        Quest.cpp
 *
 * \details     Implementation file for Quest class. 
 ************************************************************************/

#include "Quest.hpp"
#include "Item.hpp"
#include "QuestStep.hpp"

namespace legacymud { namespace engine {

Quest::Quest()
: InteractiveNoun()
, name("")
, description("")
, rewardMoney(0)
, rewardItem(nullptr)
{ }


Quest::Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem)
: InteractiveNoun()
, name(name)
, description(description)
, rewardMoney(rewardMoney)
, rewardItem(rewardItem)
{ }


/*Quest::Quest(const Quest &otherQuest){

}


Quest & Quest::operator=(const Quest &otherQuest){

}


Quest::~Quest(){

}*/


std::string Quest::getName(){
    return name;
}


std::string Quest::getDescription(){
    return description;
}


int Quest::getRewardMoney(){
    return rewardMoney;
}


Item* Quest::getRewardItem(){
    return rewardItem;
}


std::vector<std::pair<int, QuestStep*>> Quest::getSteps(){
    return steps;
}


bool Quest::setName(std::string name){
    this->name = name;

    return true;
}


bool Quest::setDescription(std::string description){
    this->description = description;

    return true;
}


bool Quest::setRewardMoney(int money){
    rewardMoney = money;

    return true;
}


bool Quest::setRewardItem(Item *rewardItem){
    this->rewardItem = rewardItem;

    return true;
}


bool Quest::addStep(int, QuestStep *aStep){
    return false;
}


bool Quest::removeStep(QuestStep *aStep){
    return false;
}


ObjectType Quest::getObjectType(){
    return ObjectType::QUEST;
}


std::string Quest::serialize(){
    return "";
}


bool Quest::deserialize(std::string){
    return false;
}


InteractiveNoun* Quest::copy(){
    return nullptr;
}


bool Quest::editAttribute(Player*, std::string){
    return false;
}


bool Quest::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Quest::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["reward money"] = DataType::INT_TYPE;
    signature["reward item"] = DataType::ITEM_PTR;

    return signature;
}

}}