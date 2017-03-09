/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/08/2017
 * \modified    03/08/2017
 * \course      CS467, Winter 2017
 * \file        Area.cpp
 *
 * \details     Implementation file for Area class. 
 ************************************************************************/

#include "Area.hpp"
#include "Character.hpp"
#include "Exit.hpp"
#include "Item.hpp"
#include "Feature.hpp"
#include "CommandEnum.hpp"
#include "Action.hpp"
#include "Container.hpp"
#include "Player.hpp"
#include <iostream>
#include <algorithm>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <Grammar.hpp>  
#include <AreaSize_Data.hpp>    
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp> 

namespace legacymud { namespace engine {

Area::Area()
: InteractiveNoun()
, name("")
, shortDescription("")
, longDescription("")
, size(AreaSize::SMALL)
{ }


Area::Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size)
: InteractiveNoun()
, name(name)
, shortDescription(shortDescription)
, longDescription(longDescription)
, size(size)
{ 
    std::string idAlias = "area " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


Area::Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size, int anID)
: InteractiveNoun(anID)
, name(name)
, shortDescription(shortDescription)
, longDescription(longDescription)
, size(size)
{ 
    std::string idAlias = "area " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


Area::Area(const Area &otherArea) : InteractiveNoun(otherArea) {
    otherArea.nameMutex.lock();
    name = otherArea.name;
    otherArea.nameMutex.unlock();
    otherArea.shortDescMutex.lock();
    shortDescription = otherArea.shortDescription;
    otherArea.shortDescMutex.unlock();
    otherArea.longDescMutex.lock();
    longDescription = otherArea.longDescription;
    otherArea.longDescMutex.unlock();
    std::string idAlias = "area " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


Area & Area::operator=(const Area &otherArea){
    if (&otherArea == this)
        return *this;

    InteractiveNoun::operator=(otherArea);
    otherArea.nameMutex.lock();
    name = otherArea.name;
    otherArea.nameMutex.unlock();
    otherArea.shortDescMutex.lock();
    shortDescription = otherArea.shortDescription;
    otherArea.shortDescMutex.unlock();
    otherArea.longDescMutex.lock();
    longDescription = otherArea.longDescription;
    otherArea.longDescMutex.unlock();
    std::string idAlias = "area " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);

    return *this;
}

/*
Area::~Area(){

}*/


std::string Area::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
} 


std::string Area::getShortDesc() const{
    std::lock_guard<std::mutex> shortDescLock(shortDescMutex);
    return shortDescription;
}


std::string Area::getLongDesc() const{
    std::lock_guard<std::mutex> longDescLock(longDescMutex);
    return longDescription;
}


AreaSize Area::getSize() const{
    return size.load();
}


std::vector<Item*> Area::getItems() const{
    std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
    return itemContents;
}


std::vector<Character*> Area::getCharacters() const{
    std::lock_guard<std::mutex> charContentLock(charContentMutex);
    return characterContents;
}


std::vector<Feature*> Area::getFeatures() const{
    std::lock_guard<std::mutex> featContentLock(featContentMutex);
    return featureContents;
}


std::vector<Exit*> Area::getExits() const{
    std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
    return exitContents;
}


const parser::LexicalData& Area::getLexicalData() const{
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
    return contentsLexicalData;
}


std::string Area::getFullDescription(Player *aPlayer) const{
    bool editMode = aPlayer->isEditMode();
    std::string message = getLongDesc();
    std::vector<Item*> allItems = getItems();
    std::vector<Character*> allCharacters = getCharacters();
    std::vector<Feature*> allFeatures = getFeatures();
    std::vector<Exit*> allExits = getExits();
    int excludeID = aPlayer->getID();
    ItemType *condItemType = nullptr;

    if (editMode){
        message += " [area " + std::to_string(getID()) + "]";
    }

    message += "\015\012";
    
    for (auto feature : allFeatures){
        message += "You see a ";
        message += feature->getName();
        message += " ";
        message += feature->getPlacement();
        if (editMode){
            message += " [feature " + std::to_string(feature->getID()) + "]";
        }
        message += ".\015\012";
    }

    for (auto exit : allExits){
        message += exit->getDirectionString();
        message += " you see ";
        if (exit->isConditional()){
            condItemType = exit->getConditionItem();
            if ((condItemType != nullptr) && (aPlayer->hasItem(condItemType))){
                message += exit->getAltDescription();
            } else {
                message += exit->getDescription();
            }
        } else {
            message += exit->getName();
        }
        if (editMode){
            message += " [exit " + std::to_string(exit->getID()) + "]";
        }
        message += ".\015\012";
    }

    if (allItems.size() == 1){
        message += "You see a ";
        message += allItems[0]->getName();
        message += " on the ground.";
        if (editMode){
            message += " [item " + std::to_string(allItems[0]->getID()) + "]";
        }
    } else if (allItems.size() > 1){
        message += "Around you, you see a ";
        for (size_t i = 0; i < allItems.size(); i++){
            message += allItems[i]->getName();
            if (editMode){
                message += " [item " + std::to_string(allItems[i]->getID()) + "]";
            }
            if (i == (allItems.size() - 2)){
                message += " and a ";
            } else if (i == (allItems.size() - 1)){
                message += ".";
            } else {
                message += ", a ";    
            }
        }
    }
    message += "\015\012";
    

    for (size_t i = 0; i < allCharacters.size(); i++){
        if (allCharacters[i]->getID() != excludeID){
            if (allCharacters[i]->getObjectType() == ObjectType::NON_COMBATANT){
                message += "You see someone named ";
            } else if (allCharacters[i]->getObjectType() == ObjectType::CREATURE){
                message += "You see a creature named ";
            } else if (allCharacters[i]->getObjectType() == ObjectType::PLAYER){
                message += "You see a player named ";
            }
            message += allCharacters[i]->getName();
            if (editMode){
                message += " [character " + std::to_string(allCharacters[i]->getID()) + "]";
            }
            message += ".\015\012";
        }
    }

    return message;
}


bool Area::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
    this->name = name;
    return true;
}


bool Area::setShortDesc(std::string shortDescription){
    std::lock_guard<std::mutex> shortDescLock(shortDescMutex);
    this->shortDescription = shortDescription;
    return true;
}


bool Area::setLongDesc(std::string longDescription){
    std::lock_guard<std::mutex> longDescLock(longDescMutex);
    this->longDescription = longDescription;
    return true;
}


bool Area::setSize(AreaSize size){
    this->size.store(size);
    return true;
}

// would be best to remove dynamic_cast ********************************************************
bool Area::addItem(Item *anItem){
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
        itemContents.push_back(anItem);
        addAllLexicalData(anItem);

        if (anItem->getObjectType() == ObjectType::CONTAINER){
            aContainer = dynamic_cast<Container*>(anItem);
            if (aContainer != nullptr){
                contents = aContainer->getAllContents();
                for (auto content : contents){
                    addAllLexicalData(content);
                }
            }
        }

        return true;
    }
    return false;
}


bool Area::addCharacter(Character *aCharacter){
    std::vector<std::pair<EquipmentSlot, Item*>> inventory;

    if (aCharacter != nullptr){
        std::lock_guard<std::mutex> charContentLock(charContentMutex);
        characterContents.push_back(aCharacter);
        addAllLexicalData(aCharacter);

        if (aCharacter->getObjectType() != ObjectType::PLAYER){
            inventory = aCharacter->getInventory();
            for (auto item : inventory){
                addAllLexicalData(item.second);
            }
        }

        return true;
    }
    return false;
}


bool Area::addFeature(Feature *aFeature){
    if (aFeature != nullptr){
        std::lock_guard<std::mutex> featContentLock(featContentMutex);
        featureContents.push_back(aFeature);
        addAllLexicalData(aFeature);
        return true;
    }
    return false;
}


bool Area::addExit(Exit *anExit){
    if (anExit != nullptr){
        std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
        exitContents.push_back(anExit);
        addAllLexicalData(anExit);
        return true;
    }
    return false;
}


void Area::addAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            contentsLexicalData.addNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            contentsLexicalData.addVerb(verb, anObject);
        }
    }
    
}


void Area::removeAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            contentsLexicalData.removeNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            contentsLexicalData.removeVerb(verb, anObject);
        }
    }
}


bool Area::removeItem(Item *anItem){
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
        itemContents.erase(std::remove(itemContents.begin(), itemContents.end(), anItem), itemContents.end());
        removeAllLexicalData(anItem);

        if (anItem->getObjectType() == ObjectType::CONTAINER){
            aContainer = dynamic_cast<Container*>(anItem);
            if (aContainer != nullptr){
                contents = aContainer->getAllContents();
                for (auto content : contents){
                    removeAllLexicalData(content);
                }
            }
        }
        
        return true;
    }
    return false;
}


bool Area::removeCharacter(Character *aCharacter){
    std::vector<std::pair<EquipmentSlot, Item*>> inventory;

    if (aCharacter != nullptr){
        std::lock_guard<std::mutex> charContentLock(charContentMutex);
        characterContents.erase(std::remove(characterContents.begin(), characterContents.end(), aCharacter), characterContents.end());
        removeAllLexicalData(aCharacter);

        if (aCharacter->getObjectType() != ObjectType::PLAYER){
            inventory = aCharacter->getInventory();
            for (auto item : inventory){
                removeAllLexicalData(item.second);
            }
        }

        return true;
    }
    return false;
}


bool Area::removeFeature(Feature *aFeature){
    if (aFeature != nullptr){
        std::lock_guard<std::mutex> featContentLock(featContentMutex);
        featureContents.erase(std::remove(featureContents.begin(), featureContents.end(), aFeature), featureContents.end());
        removeAllLexicalData(aFeature);
        return true;
    }
    return false;
}


bool Area::removeExit(Exit *anExit){
    if (anExit != nullptr){
        std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
        exitContents.erase(std::remove(exitContents.begin(), exitContents.end(), anExit), exitContents.end());
        removeAllLexicalData(anExit);
        return true;
    }
    return false;
}


bool Area::addNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.addNoun(alias, this);
    success = InteractiveNoun::addNounAlias(alias);

    return success;
}


bool Area::removeNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.removeNoun(alias, this);
    success = InteractiveNoun::removeNounAlias(alias);

    return success;
}


bool Area::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.addVerb(alias, this);
    success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);

    return success;
}


bool Area::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.removeVerb(alias, this);
    success = InteractiveNoun::removeVerbAlias(aCommand, alias);

    return success;
}


bool Area::registerAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            contentsLexicalData.addVerb(alias, anObject);
        } else {
            contentsLexicalData.addNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


bool Area::unregisterAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            contentsLexicalData.removeVerb(alias, anObject);
        } else {
            contentsLexicalData.removeNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


ObjectType Area::getObjectType() const{
    return ObjectType::AREA;
}


std::string Area::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("AREA");  // class of this object

    // This is all data in the Area class that is not inherited from other classes.       
    
    writer.String("name");                          
    writer.String(this->getName().c_str());   
    writer.String("short_description");             
    writer.String(this->getShortDesc().c_str());    
    writer.String("long_description");              
    writer.String(this->getLongDesc().c_str());
    writer.String("area_size");                    
    writer.String(gamedata::AreaSize_Data::enumToString(this->getSize()).c_str()); 
     
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


Area* Area::deserialize(std::string jsonStr){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str()); 
 
    // Construct a new Area object, getting all the data needed to do so from the objectDoc.         
    Area *newArea = new Area(objectDoc["name"].GetString(), 
                             objectDoc["short_description"].GetString(), 
                             objectDoc["long_description"].GetString(), 
                             gamedata::AreaSize_Data::stringToEnum(objectDoc["area_size"].GetString()) , 
                             objectDoc["interactive_noun_data"]["id"].GetInt() );
    
    // Rebuild the new Area noun allias list.  
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray()) {     
        if (objectDoc["name"].GetString() != noun.GetString() )     // note: `name` is automatically added to noun aliases
            newArea->addNounAlias(noun.GetString() );    
    }
   
    // Rebuild the new Area action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Area          
        newArea->addAction(command, 
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
            newArea->addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
     
    return newArea; 
}


std::string Area::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = getFullDescription(aPlayer);
    std::string resultMsg = "";
    EffectType anEffect = EffectType::NONE;

    message += " ";

    resultMsg = getTextAndEffect(CommandEnum::LOOK, anEffect);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }

    if ((anEffect != EffectType::NONE) && (effects != nullptr)){
        effects->push_back(anEffect);
    }

    return message;
}  


std::string Area::listen(std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect;

    resultMsg = getTextAndEffect(CommandEnum::LISTEN, anEffect);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }

    if ((anEffect != EffectType::NONE) && (effects != nullptr)){
        effects->push_back(anEffect);
    }

    std::unique_lock<std::mutex> featContentLock(featContentMutex, std::defer_lock);
    std::unique_lock<std::mutex> exitContentLock(exitContentMutex, std::defer_lock);
    std::lock(featContentLock, exitContentLock);

    for (auto feature : featureContents){
        message += feature->listen(effects);
        message += " ";
    }
    for (auto exit : exitContents){
        message += exit->listen(effects);
        message += " ";
    }

    return message;
} 


/*std::string Area::search(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} */


std::string Area::warp(Player *aPlayer, Area *anArea){
    std::string message = "";

    // remove player from current area and add to this area
    aPlayer->getLocation()->removeCharacter(aPlayer);
    addCharacter(aPlayer);

    // call this function on player
    message += aPlayer->warp(aPlayer, this);
    
    return message;
} 


InteractiveNoun* Area::copy(){
    Area *anArea = new Area(*this);

    return anArea;
}


/*bool Area::editAttribute(Player *aPlayer, std::string attribute){
    return false;
}*/


bool Area::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Area::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["short description"] = DataType::STRING_TYPE;
    signature["long description"] = DataType::STRING_TYPE;
    signature["size"] = DataType::AREA_SIZE;

    return signature;
}

}}