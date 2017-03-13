/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/09/2017
 * \modified    03/13/2017
 * \course      CS467, Winter 2017
 * \file        Container.cpp
 *
 * \details     Implementation file for Container class. 
 ************************************************************************/

#include "Container.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Area.hpp"
#include "ItemType.hpp"
#include <algorithm>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp>   
#include <ItemPosition_Data.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

const int DEFAULT_CAPACITY = 10;

Container::Container()
: Item()
, insideCapacity(DEFAULT_CAPACITY)
{ }


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type)
: Item(location, position, name, type)
, insideCapacity(capacity)
{ }


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type, int anID)
: Item(location, position, name, type, anID)
, insideCapacity(capacity)
{ }


Container::Container(const Container &otherContainer) : Item(otherContainer) { 
    insideCapacity.store(otherContainer.getInsideCapacity());
}

/*
Container & Container::operator=(const Container &otherContainer){

}


Container::~Container(){

}*/


bool Container::isEmpty() const{
    std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
    std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
    std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
    std::lock(underLock, insideLock, onTopOfLock);

    return under.empty() && inside.empty() && onTopOf.empty();
}

bool Container::isContained(Item *anItem) const{
    if (anItem != nullptr){
        std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
        std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
        std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
        std::lock(underLock, insideLock, onTopOfLock);

        for (auto item : under){
            if (item == anItem){
                return true;
            }
        }
        for (auto item : inside){
            if (item == anItem){
                return true;
            }
        }
        for (auto item : onTopOf){
            if (item == anItem){
                return true;
            }
        }
    }
    return false;
}

bool Container::remove(Item *anItem){
    bool success = false; 
    Container *aContainer;
    InteractiveNoun *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;
    ItemPosition position;

    if (anItem != nullptr){
        position = anItem->getPosition();
        if (position == ItemPosition::IN){
            std::lock_guard<std::mutex> insideLock(insideMutex);
            inside.erase(std::remove(inside.begin(), inside.end(), anItem), inside.end());
            success = true;
        } else if (position == ItemPosition::ON){
            std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
            onTopOf.erase(std::remove(onTopOf.begin(), onTopOf.end(), anItem), onTopOf.end());
            success = true;
        } else if (position == ItemPosition::UNDER){
            std::lock_guard<std::mutex> underLock(underMutex);
            under.erase(std::remove(under.begin(), under.end(), anItem), under.end());
            success = true;
        }
    }

    if (success){
        while ((location->getObjectType() != ObjectType::PLAYER) && (location->getObjectType() != ObjectType::AREA)){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                location = aContainer->getLocation();
            } else {
                return false;
            }
        }

        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            location->unregisterAlias(false, noun, anItem);
        }
        for (auto verb : verbAliases){
            location->unregisterAlias(true, verb, anItem);
        }
    }

    return success;
}

// would be best to remove dynamic casts ***********************************************************
// should check capacity ****************************************
bool Container::place(Item *anItem, ItemPosition position){
    bool success = false; 
    Container *aContainer;
    InteractiveNoun *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        if (position == ItemPosition::IN){
            std::lock_guard<std::mutex> insideLock(insideMutex);
            inside.push_back(anItem);
            success = true;
        } else if (position == ItemPosition::ON){
            std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
            onTopOf.push_back(anItem);
            success = true;
        } else if (position == ItemPosition::UNDER){
            std::lock_guard<std::mutex> underLock(underMutex);
            under.push_back(anItem);
            success = true;
        }
    }

    if (success){
        while ((location->getObjectType() != ObjectType::PLAYER) && (location->getObjectType() != ObjectType::AREA)){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                location = aContainer->getLocation();
            } else {
                return false;
            }
        }

        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            location->registerAlias(false, noun, anItem);
        }
        for (auto verb : verbAliases){
            location->registerAlias(true, verb, anItem);
        }
    }

    return success;
}


std::vector<Item*> Container::getInsideContents() const{
    std::lock_guard<std::mutex> insideLock(insideMutex);
    return inside;
}


std::vector<Item*> Container::getUnderContents() const{
    std::lock_guard<std::mutex> underLock(underMutex);
    return under;
}


std::vector<Item*> Container::getTopContents() const{
    std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
    return onTopOf;
}

std::vector<Item*> Container::getAllContents() const{
    std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
    std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
    std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
    std::lock(onTopOfLock, underLock, insideLock);

    std::vector<Item*> allContents;
    allContents.insert(allContents.end(), inside.begin(), inside.end());
    allContents.insert(allContents.end(), under.begin(), under.end());
    allContents.insert(allContents.end(), onTopOf.begin(), onTopOf.end());

    return allContents;
}


int Container::getInsideCapacity() const{
    return insideCapacity.load();
}


bool Container::setInsideCapacity(int capacity){
    insideCapacity.store(capacity);

    return true;
}


ObjectType Container::getObjectType() const{
    return ObjectType::CONTAINER;
}


std::string Container::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("CONTAINER");  // class of this object

    // This is all data in the Container class that is not inherited from other classes.
    writer.String("capacity");
    writer.Int(this->getInsideCapacity());

    // This is all data inherited from the Item class.    
    writer.String("name");
    writer.String(this->getName().c_str()); 
    writer.String("location");
    writer.Int(this->getLocation()->getID());                 
    writer.String("position");
    writer.String(gamedata::ItemPosition_Data::enumToString(this->getPosition()).c_str());
    writer.String("item_type_id");
    writer.Int(this->getType()->getID()); 
   
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


Container* Container::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Confirm that the location of this container has already been added to the gom.
    if (gom->getPointer(objectDoc["location"].GetInt()) == nullptr)
        return nullptr;   // this can happen if the location of this container is another container that hasn't been loaded yet.
    
    // Confirm that this container has not already been added to the gom.
    if (gom->getPointer(objectDoc["interactive_noun_data"]["id"].GetInt()) != nullptr) 
        return nullptr;
    else {
        // Construct a new Container object, getting all the data needed to do so from the objectDoc. 
        Container *newContainer = new Container(objectDoc["capacity"].GetInt(),
                                                gom->getPointer(objectDoc["location"].GetInt()),
                                                gamedata::ItemPosition_Data::stringToEnum(objectDoc["position"].GetString()), 
                                                objectDoc["name"].GetString(), 
                                                static_cast<ItemType*>(gom->getPointer(objectDoc["item_type_id"].GetInt())),        
                                                objectDoc["interactive_noun_data"]["id"].GetInt() );    
    
        // Rebuild the new Container noun alias list. 
        for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
            newContainer->InteractiveNoun::addNounAlias(noun.GetString() );    
       
        // Rebuild the new Container action list.
        for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
            
            //  get the command from the object doc
            engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                    
            // add the action to the new Container          
            newContainer->addAction(command, 
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
                newContainer->InteractiveNoun::addVerbAlias(command, 
                                      alias["verb_alias"].GetString(), 
                                      gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                      gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                      prepMap );
            }       
        }            
        return newContainer; 
    }
}


std::string Container::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    ItemType *aType = getType();
    EffectType anEffect = EffectType::NONE;

    message = "The " + getName() + " is ";
    message += aType->getDescription() + ".";
    if (!isEmpty()){
        message += " It looks like it might contain something.";
    }

    if (aPlayer->isEditMode()){
        message += " [item " + std::to_string(getID()) + "].";
    }

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

// would be best to remove dynamic casts ***********************************************************
std::string Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    Area *anArea = nullptr;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

    if ((aContainer != nullptr) && (this->getID() == aContainer->getID())){
        // this is the containing object
        if (isContained(anItem)){
            remove(anItem);
        }
    } else {
        // this is the item being taken

        // check if this item is contained within a container
        if ((position == ItemPosition::IN) || (position == ItemPosition::ON) || (position == ItemPosition::UNDER)){
            if ((location == aContainer) && (aContainer != nullptr)){
                aContainer->take(nullptr, this, aContainer, nullptr, nullptr);
            } else {
                return "false";
            }
        } else if (position == ItemPosition::GROUND) {
            // location is an area
            anArea = dynamic_cast<Area*>(location);
            if (anArea != nullptr){
                // remove item from area
                anArea->removeItem(this);
            } else {
                return "false";
            }
        } else {
            return "false";
        }

        setPosition(ItemPosition::INVENTORY);
        // get results of take for this object
        resultMsg = getTextAndEffect(CommandEnum::TAKE, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }

        // call this function on player or character, and container
        if (aCharacter != nullptr){
            // aCharacter is doing the taking
            setLocation(aCharacter);
            message += aCharacter->take(nullptr, this, nullptr, aCharacter, effects);
        } else {
            // aPlayer is doing the taking
            setLocation(aPlayer);
            message += aPlayer->take(aPlayer, this, nullptr, nullptr, effects);
        }
    }

    return message;
}

// would be best to remove dynamic casts ***********************************************************
std::string Container::put(Player *aPlayer, Item *anItem, InteractiveNoun *containingNoun, ItemPosition position, std::vector<EffectType> *effects){
    Area *anArea;
    InteractiveNoun *location;
    ItemPosition currPosition;
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect;
    Container *aContainer = nullptr;

    if (anItem != nullptr){
        // this is the containing item
        location = getLocation();
        while (location->getObjectType() == ObjectType::CONTAINER){
            if (location->getID() == anItem->getID()){
                return "false";
            }
            aContainer = static_cast<Container*>(location);
            location = aContainer->getLocation();
        }
        place(anItem, position);
    } else {
        // this is the item being placed
        location = getLocation();
        currPosition = getPosition();

        // call this function on containingNoun
        if (containingNoun != nullptr){
            resultMsg = containingNoun->put(aPlayer, this, nullptr, position, effects);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
                setLocation(containingNoun);

                // check if this item is contained within a container
                if ((currPosition == ItemPosition::IN) || (currPosition == ItemPosition::ON) || (currPosition == ItemPosition::UNDER)){
                    return "false";
                } else if (currPosition == ItemPosition::GROUND) {
                    // location is an area
                    anArea = dynamic_cast<Area*>(location);
                    if (anArea != nullptr){
                        // remove item from area
                        anArea->removeItem(this);
                    } else {
                        return "false";
                    }
                } else if ((currPosition == ItemPosition::INVENTORY) || (currPosition == ItemPosition::EQUIPPED)){
                    // location is a character
                    if (location->getID() == aPlayer->getID()){
                        aPlayer->removeFromInventory(this);
                    } else {
                        return "false";
                    }
                } else {
                    return "false";
                }

                setPosition(position);
                // get results of put for this object
                resultMsg = getTextAndEffect(CommandEnum::PUT, anEffect);
                if (resultMsg.compare("false") != 0){
                    message += resultMsg;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }
            } else {
                return "false";
            }
        } else {
            return "false";
        }
    }

    return message;
}


std::string Container::more(Player *aPlayer){
    std::string message = Item::more(aPlayer);

    message += "Inside Capacity: " + std::to_string(getInsideCapacity()) + "\015\012";
    
    return message;
} 


std::string Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = Item::equip(aPlayer, anItem, aCharacter, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = Item::transfer(aPlayer, anItem, aCharacter, destination, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool playerAttacker, std::vector<EffectType> *effects){
    return "";
}


std::string Container::buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    std::string message = Item::buy(aPlayer, anItem, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::sell(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    std::string message = Item::sell(aPlayer, anItem, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::search(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMessage;
    EffectType anEffect = EffectType::NONE;
    std::vector<Item*> in, on, under;

    if (!isEmpty()){
        in = getInsideContents();
        on = getTopContents();
        under = getUnderContents();

        // list items on top of this container
        if (on.size() > 0){
            message += "You look on top of the " + getName() + " and see a ";
            if (on.size() == 1){
                message += on[0]->getName() + ".\015\012";
            } else {
                for (size_t i = 0; i < on.size(); i++){
                    message += on[i]->getName();
                    if (i == (on.size() - 2)){
                        message += " and a ";
                    } else if (i == (on.size() - 1)){
                        message += ".\015\012";
                    } else {
                        message += ", a ";
                    }
                }
            }
        }

        // list items under this container
        if (under.size() > 0){
            message += "You look under the " + getName() + " and see a ";
            if (under.size() == 1){
                message += under[0]->getName() + ".\015\012";
            } else {
                for (size_t i = 0; i < under.size(); i++){
                    message += under[i]->getName();
                    if (i == (under.size() - 2)){
                        message += " and a ";
                    } else if (i == (under.size() - 1)){
                        message += ".\015\012";
                    } else {
                        message += ", a ";
                    }
                }
            }
        }

        // list items inside this container
        if (in.size() > 0){
            message += "You look inside the " + getName() + " and see a ";
            if (in.size() == 1){
                message += in[0]->getName() + ".\015\012";
            } else {
                for (size_t i = 0; i < in.size(); i++){
                    message += in[i]->getName();
                    if (i == (in.size() - 2)){
                        message += " and a ";
                    } else if (i == (in.size() - 1)){
                        message += ".\015\012";
                    } else {
                        message += ", a ";
                    }
                }
            }
        }
    } else {
        message = "You search the " + getName() + " thoroughly, but find nothing.\015\012";
    }

    resultMessage = getTextAndEffect(CommandEnum::SEARCH, anEffect);
    if (resultMessage.compare("false") != 0){
        message += resultMessage;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
} 


InteractiveNoun* Container::copy(){
    InteractiveNoun *location = nullptr;
    Area *anArea = nullptr;
    Character *aCharacter = nullptr;
    Container *aContainer = nullptr;
    Container *newContainer = new Container(*this);

    location = newContainer->getLocation();
    newContainer->setPosition(this->getPosition());

    if (location->getObjectType() == ObjectType::AREA){
        anArea = dynamic_cast<Area*>(location);
        if (anArea != nullptr){
            anArea->addItem(newContainer);
        }
    } else if (location->getObjectType() == ObjectType::CONTAINER){
        aContainer = dynamic_cast<Container*>(location);
        if (aContainer != nullptr){
            aContainer->place(newContainer, newContainer->getPosition());
        }
    } else {
        aCharacter = dynamic_cast<Character*>(location);
        if (aCharacter != nullptr){
            aCharacter->addToInventory(newContainer);
            newContainer->setPosition(ItemPosition::INVENTORY);
        }
    }

    return newContainer;
}


/*bool Container::editAttribute(Player *aPlayer, std::string){
    return false;
}*/


bool Container::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Container::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["capacity"] = DataType::INT_TYPE;
    signature["location"] = DataType::INTERACTIVE_NOUN_PTR;
    signature["position"] = DataType::ITEM_POSITION;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["type"] = DataType::ITEM_TYPE_PTR;

    return signature;
}

}}
