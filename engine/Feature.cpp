/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/10/2017
 * \modified    03/17/2017
 * \course      CS467, Winter 2017
 * \file        Feature.cpp
 *
 * \details     Implementation file for Feature class. 
 ************************************************************************/

#include "Feature.hpp"
#include "Area.hpp"
#include "ItemType.hpp"
#include "Player.hpp"
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

Feature::Feature()
: ConditionalElement()
, name("")
, placement("")
, location(nullptr)
{ }


Feature::Feature(std::string name, std::string placement, Area *location, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, name(name)
, placement(placement)
, location(location)
{
    std::string idAlias = "feature " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
}


Feature::Feature(std::string name, std::string placement, Area *location, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID)
: ConditionalElement(isConditional, anItemType, description, altDescription, anID)
, name(name)
, placement(placement)
, location(location)
{ }


std::string Feature::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


std::string Feature::getPlacement() const{
    std::lock_guard<std::mutex> placementLock(placementMutex);
    return placement;
}


Area* Feature::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


InteractiveNoun* Feature::getObjectLocation() const{
    return getLocation();
}


bool Feature::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
    this->name = name;

    return true;
}


bool Feature::setPlacement(std::string placement){
    std::lock_guard<std::mutex> placementLock(placementMutex);
    this->placement = placement;

    return true;
}


bool Feature::setLocation(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> locationLock(locationMutex);
        location = anArea;
        return true;
    }

    return false;
}


bool Feature::addNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        success = InteractiveNoun::addNounAlias(alias);
        if (success){
            anArea->registerAlias(false, alias, this);
        }
    }

    return success;
}


bool Feature::removeNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        success = InteractiveNoun::removeNounAlias(alias);
        if (success){
            anArea->unregisterAlias(false, alias, this);
        }
    }

    return success;
}


bool Feature::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
        if (success){
            anArea->registerAlias(true, alias, this);
        }
    }

    return success;
}


bool Feature::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
        if (success){
            anArea->unregisterAlias(true, alias, this);
        }
    }

    return success;
}


ObjectType Feature::getObjectType() const{
    return ObjectType::FEATURE;
}


std::string Feature::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("FEATURE");  // class of this object

    // This is all data in the Feature class that is not inherited from other classes.
    writer.String("name");
    writer.String(this->getName().c_str());
    writer.String("placement");
    writer.String(this->getPlacement().c_str());
    writer.String("location_id");
    writer.Int(this->getLocation()->getID());
    
    // This is all data inherited from the ConditionalElement class.
    writer.String("is_conditional");
    writer.Bool(this->isConditional());
    if (this->isConditional()) {
        writer.String("condition_itemtype_id");
        writer.Int(this->getConditionItem()->getID());
    }
    writer.String("description");
    writer.String(this->getDescription().c_str());
    writer.String("altdescription");
    writer.String(this->getAltDescription().c_str());
    
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


Feature* Feature::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Check if this is a conditional feature
    ItemType* conditionalItemType;
    if (objectDoc["is_conditional"].GetBool()) 
            conditionalItemType = static_cast<ItemType*>(gom->getPointer(objectDoc["condition_itemtype_id"].GetInt()));
    else
        conditionalItemType = nullptr;
    
    // Construct a new Feature object, getting all the data needed to do so from the objectDoc. 
    Feature *newFeature = new Feature(objectDoc["name"].GetString(),
                                      objectDoc["placement"].GetString(),
                                      static_cast<Area*>(gom->getPointer(objectDoc["location_id"].GetInt())),
                                      objectDoc["is_conditional"].GetBool(),
                                      conditionalItemType,
                                      objectDoc["description"].GetString(),
                                      objectDoc["altdescription"].GetString(),
                                      objectDoc["interactive_noun_data"]["id"].GetInt() );
   
    // Rebuild the new Feature noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())           
        newFeature->InteractiveNoun::addNounAlias(noun.GetString() );    
   
    // Rebuild the new Feature action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Feature          
        newFeature->addAction(command, 
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
            newFeature->InteractiveNoun::addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newFeature; 
}


std::string Feature::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg = "";
    ItemType *condItemType = nullptr;
    EffectType anEffect = EffectType::NONE;

    message += "You see ";
    if (isConditional()){
        condItemType = getConditionItem();
        if ((condItemType != nullptr) && (aPlayer->hasItem(condItemType))){
            message += getAltDescription();
        } else {
            message += getDescription();
        }
    } else {
        message += getDescription();
    }
    message += " ";
    message += getPlacement();
    if (aPlayer->isEditMode()){
        message += " [feature " + std::to_string(getID()) + "]";
    }
    message += ".";

    // get results of look for this object
    resultMsg = getTextAndEffect(CommandEnum::LOOK, anEffect);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
}  


std::string Feature::listen(std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;

    // get results of look for this object
    resultMsg = getTextAndEffect(CommandEnum::LISTEN, anEffect);
    if (resultMsg.compare("false") != 0){
        message = resultMsg;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


InteractiveNoun* Feature::copy(){
    return nullptr;
}


/*bool Feature::editAttribute(Player*, std::string){
    return false;
}*/


bool Feature::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Feature::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["placement"] = DataType::STRING_TYPE;
    signature["is conditional"] = DataType::BOOL_TYPE;
    signature["condition item"] = DataType::ITEM_TYPE_PTR;
    signature["description"] = DataType::STRING_TYPE;
    signature["alternate description"] = DataType::STRING_TYPE;

    return signature;
}
  
}}