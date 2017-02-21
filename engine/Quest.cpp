/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/20/2017
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


std::string Quest::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Quest::getDescription(){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


int Quest::getRewardMoney(){
    return rewardMoney.load();
}


Item* Quest::getRewardItem(){
    std::lock_guard<std::mutex> rewardItemLock(rewardItemMutex);
    return rewardItem;
}


std::vector<std::pair<int, QuestStep*>> Quest::getSteps(){
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    return steps;
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


bool Quest::addStep(int order, QuestStep *aStep){
    if(aStep != nullptr){
        std::lock_guard<std::mutex> stepsLock(stepsMutex);
        steps.push_back(std::make_pair(order, aStep));
        return true;
    } else {
        return false;
    }
}


bool Quest::removeStep(QuestStep *aStep){
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    int size = steps.size();
    steps.erase(std::remove(steps.begin(), steps.end(), aStep), steps.end());
    if ((steps.size() - size) == 1){
        return true;
    } else {
        return false;
    }
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