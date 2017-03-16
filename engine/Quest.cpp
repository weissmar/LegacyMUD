/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)  
 * \created     02/10/2017
 * \modified    03/09/2017
 * \course      CS467, Winter 2017
 * \file        Quest.cpp
 *
 * \details     Implementation file for Quest class. 
 ************************************************************************/

#include "Quest.hpp"
#include "Item.hpp"
#include "QuestStep.hpp"
#include "Player.hpp"
#include "NonCombatant.hpp"
#include <algorithm>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <vector>
#include <map>
#include <Grammar.hpp>  
#include <ItemRarity_Data.hpp>  
#include <EquipmentSlot_Data.hpp>  
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp> 

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
    std::string idAlias = "quest " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


Quest::Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem, int anID)
: InteractiveNoun(anID)
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


Item* Quest::getUniqueRewardItem() const{
    std::lock_guard<std::mutex> rewardItemLock(rewardItemMutex);
    Item *anItem = new Item(*rewardItem);
    manager->addObject(anItem, -1);

    return anItem;
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


QuestStep* Quest::getNextStep(int step) const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    std::map<int, QuestStep*>::const_iterator it;
    int found = steps.count(step);

    if (found == 1){
        it = steps.find(step);
        ++it;
        if (it != steps.end()){
            return it->second;
        }
    }
    return nullptr;
}


bool Quest::isFirstStep(int step) const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    std::map<int, QuestStep*>::const_iterator it = steps.find(step);

    if (it == steps.begin()){
        return true;
    }
    return false;
}


bool Quest::isLastStep(int step) const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);
    std::map<int, QuestStep*>::const_iterator it = steps.find(step);

    if (it != steps.end()){
        ++it;
        if (it == steps.end()){
            return true;
        }
    }
    return false;
}


QuestStep* Quest::isGiverOrReceiver(NonCombatant *aNPC) const{
    std::lock_guard<std::mutex> stepsLock(stepsMutex);

    for (auto step : steps){
        if ((step.second->getGiver() == aNPC) || (step.second->getReceiver() == aNPC)){
            return step.second;
        }
    }
    return nullptr;
}


bool Quest::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
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


void Quest::registerObjectManager(GameObjectManager *manager){
    this->manager = manager;
}


ObjectType Quest::getObjectType() const{
    return ObjectType::QUEST;
}


std::string Quest::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("QUEST");  // class of this object

    // This is all data in the Quest class that is not inherited from other classes.  
    writer.String("name");
    writer.String(this->getName().c_str()); 
    writer.String("description");
    writer.String(this->getDescription().c_str());
    writer.String("reward_money");
    writer.Int(this->getRewardMoney());
    writer.String("reward_item_id");
    if(this->getRewardItem() == nullptr) 
        writer.Int(-1);    // indicates there is no reward item
    else  
        writer.Int(this->getRewardItem()->getID()); // there is a reward
    
    // capture the quest steps
    writer.String("quest_steps");    
    std::map<int, QuestStep*> questSteps = this->getAllSteps();
    writer.StartArray(); 
    for (auto step = questSteps.begin(); step != questSteps.end(); step++ ) {
        writer.StartObject();
        writer.String("step_count");
        writer.Int(step->first);
        writer.String("quest_step_id");  
        writer.Int(step->second->getID());  
        writer.EndObject();
    }
    writer.EndArray(); 
        
    writer.EndObject(); // ends this object     
    writer.EndObject(); // outer object wrapper    
    
    // Convert to a document so that data from inherited classes can be added.
    rapidjson::Document objectDoc;
    objectDoc.Parse(buffer.GetString());    
    
    // Get the data inherited from the InteractiveNoun class and add it to the object document.
    rapidjson::Document interactiveNounDataDoc(&objectDoc.GetAllocator());
    interactiveNounDataDoc.Parse(this->serializeJustInteractiveNoun().c_str());
    objectDoc["object"].AddMember("interactive_noun_data",interactiveNounDataDoc, objectDoc.GetAllocator() );
    
    // Stringify the object doc for output
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> outWriter(buffer);
    objectDoc.Accept(outWriter);
    
    return buffer.GetString();
}


Quest* Quest::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Check if there is a reward item
    Item* rewardItem;
    if (objectDoc["reward_item_id"].GetInt() == -1)
        rewardItem = nullptr;
    else
        rewardItem = static_cast<Item*>(gom->getPointer(objectDoc["reward_item_id"].GetInt()));
    
    // Construct a new Quest object, getting all the data needed to do so from the objectDoc.  
    Quest *newQuest = new Quest(objectDoc["name"].GetString(),
                                objectDoc["description"].GetString(),
                                objectDoc["reward_money"].GetInt(),
                                rewardItem,
                                objectDoc["interactive_noun_data"]["id"].GetInt() ); 

    // Rebuild the quest steps          
    for (auto& step : objectDoc["quest_steps"].GetArray()) {              
        newQuest->addStep(static_cast<QuestStep*>(gom->getPointer(step["quest_step_id"].GetInt())));          
    }     
                               
    // Rebuild the new Quest noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())           
        newQuest->addNounAlias(noun.GetString() );    
   
    // Rebuild the new Quest action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Quest          
        newQuest->addAction(command, 
                           action["valid"].GetBool(), 
                           action["flavor_text"].GetString(), 
                           gamedata::EffectType_Data::stringToEnum(action["effect"].GetString()) );
 
        // Rebuild the verb alias list for this action command.
        for (auto& alias : action["aliases"].GetArray()) {              

            // rebuild the preposition map
            std::map<std::string, parser::PrepositionType> prepMap;           
            for (auto& prep : alias["grammar"]["preposition_map"].GetArray()) {              
                parser::PrepositionType prepType = gamedata::PrepositionType_Data::stringToEnum(prep["preposition_type"].GetString() );               
                prepMap[prep["preposition"].GetString()] = prepType;
            }           
            
            // add the verb alias
            newQuest->addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newQuest; 
}


std::string Quest::more(Player *aPlayer){
    std::string message;
    int step = aPlayer->getQuestCurrStep(this).first;
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


/*bool Quest::editAttribute(Player*, std::string){
    return false;
}*/


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