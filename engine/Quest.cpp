/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        Quest.cpp
 *
 * \details     Implementation file for Quest class. 
 ************************************************************************/

#include "Quest.hpp"
#include "Item.hpp"
#include "QuestStep.hpp"
#include "Player.hpp"
#include <algorithm>

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
{
    addNounAlias(name);
}


Quest::Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem, int anID)
: InteractiveNoun(anID)
, name(name)
, description(description)
, rewardMoney(rewardMoney)
, rewardItem(rewardItem)
{
    addNounAlias(name);
}


/*Quest::Quest(const Quest &otherQuest){

}


Quest & Quest::operator=(const Quest &otherQuest){

}


Quest::~Quest(){

}*/


std::string Quest::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Quest::getDescription() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


int Quest::getRewardMoney() const{
    return rewardMoney.load();
}


Item* Quest::getRewardItem() const{
    std::lock_guard<std::mutex> rewardItemLock(rewardItemMutex);
    return rewardItem;
}


std::map<int, QuestStep*> Quest::getAllSteps() const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    return steps;
}


QuestStep* Quest::getStep(int step) const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    int found = steps.count(step);

    if (found == 1){
        return steps.at(step);
    } else {
        return nullptr;
    }
}


bool Quest::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;

    return true;
}


bool Quest::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;

    return true;
}


bool Quest::setRewardMoney(int money){
    rewardMoney.store(money);

    return true;
}


bool Quest::setRewardItem(Item *rewardItem){
    std::lock_guard<std::mutex> rewardItemLock(rewardItemMutex);
    this->rewardItem = rewardItem;

    return true;
}


bool Quest::addStep(QuestStep *aStep){
    int stepNum, found;

    if(aStep != nullptr){
        std::lock_guard<std::mutex> stepsLock(stepsMutex);
        stepNum = aStep->getOrdinalNumber();
        found = steps.count(stepNum);

        if (found != 1){
            steps[stepNum] = aStep;
            return true;
        }
    }
    return false;
}


bool Quest::removeStep(QuestStep *aStep){
    if (aStep != nullptr){
        std::lock_guard<std::mutex> stepsLock(stepsMutex);
        int stepNum = aStep->getOrdinalNumber();
        int found = steps.count(stepNum);

        if (found == 1){
            steps.erase(stepNum);
            return true;
        }
    }
    return false;
}


ObjectType Quest::getObjectType() const{
    return ObjectType::QUEST;
}


std::string Quest::serialize(){
    return "";
}


InteractiveNoun* Quest::deserialize(std::string){
    return nullptr; 
}


std::string Quest::more(Player *aPlayer){
    std::string message;
    int step = aPlayer->getQuestCurrStep(this);
    int found;
    int money = getRewardMoney();
    Item *anItem = getRewardItem();

    if (step != -1){
        std::lock_guard<std::mutex> stepsLock(stepsMutex);
        found = steps.count(step);

        if (found == 1){
            message = "Quest: " + getName() + "\015\012";
            message += getDescription() + "\015\012";
            message += "Reward: ";
            if (anItem != nullptr){
                message += anItem->getName();
            }
            if ((anItem != nullptr) && (money > 0)){
                message += " and ";
            }
            if (money > 0){
                message += std::to_string(money) + " money\015\012";
            }
        }
    }

    return message;
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