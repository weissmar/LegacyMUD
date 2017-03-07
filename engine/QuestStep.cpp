/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)
 * \created     02/10/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        QuestStep.cpp
 *
 * \details     Implementation file for QuestStep class. 
 ************************************************************************/

#include "QuestStep.hpp"
#include "ItemType.hpp"
#include "NonCombatant.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp>   
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

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
{
    std::string idAlias = "quest step " + std::to_string(getID());
    addNounAlias(idAlias);
    std::string alias = "step " + std::to_string(ordinalNumber);
    addNounAlias(alias);
}


QuestStep::QuestStep(int ordinalNumber, std::string description, ItemType *anItemType, NonCombatant *giver, NonCombatant *receiver, std::string completionText, int anID)
: InteractiveNoun(anID)
, ordinalNumber(ordinalNumber)
, description(description)
, fetchItem(anItemType)
, giver(giver)
, receiver(receiver)
, completionText(completionText)
{
    std::string idAlias = "quest step " + std::to_string(getID());
    addNounAlias(idAlias);
    std::string alias = "step " + std::to_string(ordinalNumber);
    addNounAlias(alias);
}


/*QuestStep::QuestStep(const QuestStep &otherQuestStep){

}


QuestStep & QuestStep::operator=(const QuestStep &otherQuestStep){

}


QuestStep::~QuestStep(){

}*/


int QuestStep::getOrdinalNumber() const{
    return ordinalNumber.load();
}


std::string QuestStep::getDescription() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


ItemType* QuestStep::getFetchItem() const{
    std::lock_guard<std::mutex> fetchItemLock(fetchItemMutex);
    return fetchItem;
}


NonCombatant* QuestStep::getGiver() const{
    std::lock_guard<std::mutex> giverLock(giverMutex);
    return giver;
}


NonCombatant* QuestStep::getReceiver() const{
    std::lock_guard<std::mutex> receiverLock(receiverMutex);
    return receiver;
}


std::string QuestStep::getCompletionText() const{
    std::lock_guard<std::mutex> completionTextLock(completionTextMutex);
    return completionText;
}


bool QuestStep::setOrdinalNumber(int number){
    ordinalNumber.store(number);

    return true; 
}


bool QuestStep::setDescription(std::string description){
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    this->description = description;

    return true;
}


bool QuestStep::setFetchItem(ItemType *anItemType){
    std::lock_guard<std::mutex> fetchItemLock(fetchItemMutex);
    if (anItemType != nullptr){
        fetchItem = anItemType;
        return true;
    }

    return false; 
}


bool QuestStep::setGiver(NonCombatant *giver){
    std::lock_guard<std::mutex> giverLock(giverMutex);
    if (giver != nullptr){
        this->giver = giver;
        return true;
    }

    return false; 
}


bool QuestStep::setReceiver(NonCombatant *receiver){
    std::lock_guard<std::mutex> receiverLock(receiverMutex);
    if (receiver != nullptr){
        this->receiver = receiver;
        return true;
    }

    return false; 
}


bool QuestStep::setCompletionText(std::string completionText){
    std::lock_guard<std::mutex> completionTextLock(completionTextMutex);
    this->completionText = completionText;

    return true;
} 


std::string QuestStep::getName() const{
    std::lock_guard<std::mutex> descriptionLock(descriptionMutex);
    return description;
}


ObjectType QuestStep::getObjectType() const{
    return ObjectType::QUEST_STEP;
}


std::string QuestStep::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("QUEST_STEP");  // class of this object

    // This is all data in the QuestStep class that is not inherited from other classes.  
    writer.String("ordinal_number");
    writer.Int(this->getOrdinalNumber());
    writer.String("description");
    writer.String(this->getDescription().c_str());
    writer.String("fetch_item_type_id");
    writer.Int(this->getFetchItem()->getID());
    writer.String("giver_id");
    writer.Int(this->getGiver()->getID());   
    writer.String("receiver_id");
    writer.Int(this->getReceiver()->getID());    
    writer.String("completion_text");
    writer.String(this->getCompletionText().c_str());
    
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


QuestStep* QuestStep::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
     
    // Construct a new QuestStep object, getting all the data needed to do so from the objectDoc. 
    QuestStep *newQuest = new QuestStep(objectDoc["ordinal_number"].GetInt(),
                                        objectDoc["description"].GetString(),
                                        static_cast<ItemType*>(gom->getPointer(objectDoc["fetch_item_type_id"].GetInt())),
                                        static_cast<NonCombatant*>(gom->getPointer(objectDoc["giver_id"].GetInt())),
                                        static_cast<NonCombatant*>(gom->getPointer(objectDoc["receiver_id"].GetInt())),
                                        objectDoc["completion_text"].GetString(),
                                        objectDoc["interactive_noun_data"]["id"].GetInt() );
    
    // Rebuild the new QuestStep noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray()) {           
        newQuest->addNounAlias(noun.GetString() );    
    }
 
    // Rebuild the new QuestStep action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new QuestStep          
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


std::string QuestStep::more(Player *aPlayer){
    std::string fetchItemName, receiverName;
    std::string message;

    if (getFetchItem() != nullptr){
        fetchItemName = getFetchItem()->getName();
    }
    if (getReceiver() != nullptr){
        receiverName = getReceiver()->getName();
    }

    message = "Step " + std::to_string(getOrdinalNumber()) + ":\015\012";
    message += getDescription() + "\015\012";
    message += "Give the " + fetchItemName + " to " + receiverName + ".\015\012";

    return message;
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