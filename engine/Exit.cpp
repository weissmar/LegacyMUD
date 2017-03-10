/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)  
 * \created     02/10/2017
 * \modified    03/09/2017
 * \course      CS467, Winter 2017
 * \file        Exit.cpp
 *
 * \details     Implementation file for Exit class. 
 ************************************************************************/

#include "Exit.hpp"
#include "Area.hpp"
#include "Character.hpp"
#include "ItemType.hpp"
#include "Player.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <ExitDirection_Data.hpp>   
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

Exit::Exit()
: ConditionalElement()
, direction(ExitDirection::NORTH)
, location(nullptr)
, connectArea(nullptr)
{ }


Exit::Exit(ExitDirection direction, Area *location, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription)
: ConditionalElement(isConditional, anItemType, description, altDescription)
, direction(direction)
, location(location)
, connectArea(connectArea)
{
    std::string idAlias = "exit " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(description);
    if (altDescription != ""){
        InteractiveNoun::addNounAlias(altDescription);
    }
    addInitialDirectionalAliases(direction);
}


Exit::Exit(ExitDirection direction, Area *location, Area *connectArea, bool isConditional, ItemType *anItemType, std::string description, std::string altDescription, int anID)
: ConditionalElement(isConditional, anItemType, description, altDescription, anID)
, direction(direction)
, location(location)
, connectArea(connectArea)
{ }


/*Exit::Exit(const Exit &otherExit){

}


Exit & Exit::operator=(const Exit &otherExit){

}


Exit::~Exit(){

}*/

void Exit::addInitialDirectionalAliases(ExitDirection direction){
    switch (direction){
        case ExitDirection::NORTH:
            InteractiveNoun::addNounAlias("north");
            InteractiveNoun::addNounAlias("n");
            break;
        case ExitDirection::SOUTH:
            InteractiveNoun::addNounAlias("south");
            InteractiveNoun::addNounAlias("s");
            break;
        case ExitDirection::EAST:
            InteractiveNoun::addNounAlias("east");
            InteractiveNoun::addNounAlias("e");
            break;
        case ExitDirection::WEST:
            InteractiveNoun::addNounAlias("west");
            InteractiveNoun::addNounAlias("w");
            break;
        case ExitDirection::NORTHEAST:
            InteractiveNoun::addNounAlias("northeast");
            InteractiveNoun::addNounAlias("ne");
            break;
        case ExitDirection::NORTHWEST:
            InteractiveNoun::addNounAlias("northwest");
            InteractiveNoun::addNounAlias("nw");
            break;
        case ExitDirection::SOUTHEAST:
            InteractiveNoun::addNounAlias("southeast");
            InteractiveNoun::addNounAlias("se");
            break;
        case ExitDirection::SOUTHWEST:
            InteractiveNoun::addNounAlias("southwest");
            InteractiveNoun::addNounAlias("sw");
            break;
        case ExitDirection::UP:
            InteractiveNoun::addNounAlias("up");
            InteractiveNoun::addNounAlias("u");
            break;
        case ExitDirection::DOWN:
            InteractiveNoun::addNounAlias("down");
            InteractiveNoun::addNounAlias("d");
            break;
    }
}


void Exit::addDirectionalAliases(ExitDirection direction){
    switch (direction){
        case ExitDirection::NORTH:
            addNounAlias("north");
            addNounAlias("n");
            break;
        case ExitDirection::SOUTH:
            addNounAlias("south");
            addNounAlias("s");
            break;
        case ExitDirection::EAST:
            addNounAlias("east");
            addNounAlias("e");
            break;
        case ExitDirection::WEST:
            addNounAlias("west");
            addNounAlias("w");
            break;
        case ExitDirection::NORTHEAST:
            addNounAlias("northeast");
            addNounAlias("ne");
            break;
        case ExitDirection::NORTHWEST:
            addNounAlias("northwest");
            addNounAlias("nw");
            break;
        case ExitDirection::SOUTHEAST:
            addNounAlias("southeast");
            addNounAlias("se");
            break;
        case ExitDirection::SOUTHWEST:
            addNounAlias("southwest");
            addNounAlias("sw");
            break;
        case ExitDirection::UP:
            addNounAlias("up");
            addNounAlias("u");
            break;
        case ExitDirection::DOWN:
            addNounAlias("down");
            addNounAlias("d");
            break;
    }
}


void Exit::removeDirectionalAliases(ExitDirection direction){
    switch (direction){
        case ExitDirection::NORTH:
            removeNounAlias("north");
            removeNounAlias("n");
            break;
        case ExitDirection::SOUTH:
            removeNounAlias("south");
            removeNounAlias("s");
            break;
        case ExitDirection::EAST:
            removeNounAlias("east");
            removeNounAlias("e");
            break;
        case ExitDirection::WEST:
            removeNounAlias("west");
            removeNounAlias("w");
            break;
        case ExitDirection::NORTHEAST:
            removeNounAlias("northeast");
            removeNounAlias("ne");
            break;
        case ExitDirection::NORTHWEST:
            removeNounAlias("northwest");
            removeNounAlias("nw");
            break;
        case ExitDirection::SOUTHEAST:
            removeNounAlias("southeast");
            removeNounAlias("se");
            break;
        case ExitDirection::SOUTHWEST:
            removeNounAlias("southwest");
            removeNounAlias("sw");
            break;
        case ExitDirection::UP:
            removeNounAlias("up");
            removeNounAlias("u");
            break;
        case ExitDirection::DOWN:
            removeNounAlias("down");
            removeNounAlias("d");
            break;
    }
}


ExitDirection Exit::getDirection() const{
    return direction.load();
}


std::string Exit::getDirectionString() const{
    std::string message;

    switch (direction.load()) {
        case ExitDirection::NORTH:
            message = "To the north";
            break;        
        case ExitDirection::SOUTH:
            message = "To the south";
            break;        
        case ExitDirection::EAST:
            message = "To the east";
            break;        
        case ExitDirection::WEST:
            message = "To the west";
            break;        
        case ExitDirection::NORTHEAST:
            message = "To the northeast";
            break;        
        case ExitDirection::NORTHWEST:
            message = "To the northwest";
            break;        
        case ExitDirection::SOUTHEAST:
            message = "To the southeast";
            break;        
        case ExitDirection::SOUTHWEST :
            message = "To the southwest";
            break;        
        case ExitDirection::UP:
            message = "Above";
            break;        
        case ExitDirection::DOWN:
            message = "Below";
            break;
        default:
            message = "";
    }

    return message;
}


Area* Exit::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


Area* Exit::getConnectArea() const{
    std::lock_guard<std::mutex> connectAreaLock(connectAreaMutex);
    return connectArea;
}


bool Exit::setDirection(ExitDirection aDirection){
    removeDirectionalAliases(direction.load());
    addDirectionalAliases(aDirection);
    direction.store(aDirection);

    return true;
}


bool Exit::setLocation(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> locationLock(locationMutex);
        location = anArea;
        return true;
    }

    return false;
}


bool Exit::setConnectArea(Area *anArea){
    if (anArea != nullptr){
        std::lock_guard<std::mutex> connectAreaLock(connectAreaMutex);
        connectArea = anArea;
        return true;
    }

    return false;
}


bool Exit::setDescription(std::string description){
    removeNounAlias(getDescription());
    addNounAlias(description);
    return ConditionalElement::setDescription(description);
}


bool Exit::setAltDescription(std::string altDescription){
    removeNounAlias(getAltDescription());
    if (altDescription != ""){
        addNounAlias(altDescription);
    }
    return ConditionalElement::setAltDescription(altDescription);
}


std::string Exit::getName() const{
    return this->getDescription();
}


bool Exit::addNounAlias(std::string alias){
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


bool Exit::removeNounAlias(std::string alias){
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


bool Exit::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
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


bool Exit::removeVerbAlias(CommandEnum aCommand, std::string alias){
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


ObjectType Exit::getObjectType() const{
    return ObjectType::EXIT;
}


std::string Exit::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("EXIT");  // class of this object

    // This is all data in the Exit class that is not inherited from other classes.  
    writer.String("exit_direction");
    writer.String(gamedata::ExitDirection_Data::enumToString(this->getDirection()).c_str());
    writer.String("location_id");
    writer.Int(this->getLocation()->getID());
    writer.String("connect_area_id");
    writer.Int(this->getConnectArea()->getID());
    
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


Exit* Exit::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Check if this is a conditional exit.
    ItemType* conditionalItemType;
    if (objectDoc["is_conditional"].GetBool()) 
        conditionalItemType = static_cast<ItemType*>(gom->getPointer(objectDoc["condition_itemtype_id"].GetInt()));     
    else 
        conditionalItemType = nullptr;       
    
    // Construct a new Exit object, getting all the data needed to do so from the objectDoc. 
    Exit *newExit = new Exit(gamedata::ExitDirection_Data::stringToEnum(objectDoc["exit_direction"].GetString()),
                             static_cast<Area*>(gom->getPointer(objectDoc["location_id"].GetInt())),
                             static_cast<Area*>(gom->getPointer(objectDoc["connect_area_id"].GetInt())),
                             objectDoc["is_conditional"].GetBool(),
                             conditionalItemType,
                             objectDoc["description"].GetString(),
                             objectDoc["altdescription"].GetString(),
                             objectDoc["interactive_noun_data"]["id"].GetInt() );
   
    // Rebuild the new Exit noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
        newExit->addNounAlias(noun.GetString() );    
   
    // Rebuild the new Exit action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Exit          
        newExit->addAction(command, 
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
            newExit->addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newExit; 
}


std::string Exit::look(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
}  


std::string Exit::listen(std::vector<EffectType> *effects){
    return "";
} 


std::string Exit::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool conditional = isConditional();
    Area *location = getLocation();
    Area *connectingArea = getConnectArea();
    ItemType *anItemType = getConditionItem();
    Character *aCharacter = nullptr;
    bool hasAccess = true;

    if (character != nullptr){
        // character is the one going
        aCharacter = dynamic_cast<Character*>(character);

    } else {
        // player is the one going
        aCharacter = aPlayer;
    }

    if ((conditional) && (anItemType != nullptr)){
        hasAccess = false;
        // check if the character has the conditional item
        if ((aCharacter != nullptr) && (aCharacter->hasItem(anItemType))){
            // character has the required item
            message = "You go towards " + getAltDescription() + ".\015\012";
            hasAccess = true;
        } else {
            message = "false";
        }
    } else {
        message = "You go towards " + getDescription() + ".\015\012";
    }

    if (hasAccess){
        location->removeCharacter(aCharacter); 
        connectingArea->addCharacter(aCharacter);

        // get message and effects of GO on this object
        resultMsg += getTextAndEffect(CommandEnum::GO, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }

        // call this function on character
        message += aCharacter->go(aPlayer, connectingArea, character, effects);
    }
    return message;
}


InteractiveNoun* Exit::copy(){
    return nullptr;
}


/*bool Exit::editAttribute(Player*, std::string){
    return false;
}*/


bool Exit::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Exit::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["direction"] = DataType::EXIT_DIRECTION;
    signature["connecting area"] = DataType::AREA_PTR;
    signature["is conditional"] = DataType::BOOL_TYPE;
    signature["condition item"] = DataType::ITEM_TYPE_PTR;
    signature["description"] = DataType::STRING_TYPE;
    signature["alternate description"] = DataType::STRING_TYPE;

    return signature;
}

}}