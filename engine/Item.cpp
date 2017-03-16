 /*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/10/2017
 * \modified    03/16/2017
 * \course      CS467, Winter 2017
 * \file        Item.cpp
 *
 * \details     Header file for Item class. Defines the members and 
 *              functions that apply to all items. Item defines generic
 *              in-game items.
 ************************************************************************/

#include <iostream>
#include "Item.hpp"
#include "ItemType.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Container.hpp"
#include "Area.hpp"
#include "EffectType.hpp"
#include "NonCombatant.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <Grammar.hpp>  
#include <ItemPosition_Data.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

Item::Item()
: InteractiveNoun()
, location(nullptr) 
, position(ItemPosition::NONE)
, name("")
, type(nullptr)
{ }


Item::Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type)
: InteractiveNoun()
, location(location)
, position(position)
, name(name)
, type(type)
{
    std::string idAlias = "item " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
}


Item::Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type, int anID)
: InteractiveNoun(anID)
, location(location)
, position(position)
, name(name)
, type(type)
{ }


Item::Item(const Item &otherItem) : InteractiveNoun(otherItem) {
    otherItem.locationMutex.lock();
    location = otherItem.location;
    otherItem.locationMutex.unlock();
    otherItem.nameMutex.lock();
    name = otherItem.name;
    otherItem.nameMutex.unlock();
    otherItem.typeMutex.lock();
    type = otherItem.type;
    otherItem.typeMutex.unlock();
    std::string idAlias = "item " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
}


Item & Item::operator=(const Item &otherItem){
    if (this == &otherItem)
        return *this;

    InteractiveNoun::operator=(otherItem);
    otherItem.locationMutex.lock();
    location = otherItem.location;
    otherItem.locationMutex.unlock();
    otherItem.nameMutex.lock();
    name = otherItem.name;
    otherItem.nameMutex.unlock();
    otherItem.typeMutex.lock();
    type = otherItem.type;
    otherItem.typeMutex.unlock();
    std::string idAlias = "item " + std::to_string(getID());
    InteractiveNoun::addNounAlias(idAlias);
    InteractiveNoun::addNounAlias(name);
    
    return *this;
}


bool Item::operator==(const Item &otherItem) const{
    bool equal = true;
    ItemType *thisType = this->getType();
    ItemType *otherType = otherItem.getType();

    if (this->getName().compare(otherItem.getName()) != 0){
        equal = false;
    } else if (this->getLocation()->getID() != otherItem.getLocation()->getID()) {
        equal = false;
    } else if (this->getPosition() != otherItem.getPosition()) {
        equal = false;
    } else if (thisType->getID() != otherType->getID()){
        equal = false;
    } else if (!compareActionsAndAliases(otherItem)){
        equal = false;
    }

    return equal;
}


/*
Item::~Item(){

}*/


bool Item::compareObjects(const InteractiveNoun &otherObject) const{
    Item const *otherItem;
    ObjectType otherObjectType;
    bool equal = false;

    otherObjectType = otherObject.getObjectType();

    if (otherObjectType == ObjectType::ITEM){
        otherItem = dynamic_cast<Item const*>(&otherObject);

        if (*this == *otherItem){
            equal = true;
        }
    }

    return equal;
}


InteractiveNoun* Item::getLocation() const{
    std::lock_guard<std::mutex> locationLock(locationMutex);
    return location;
}


InteractiveNoun* Item::getObjectLocation() const{
    return getLocation();
}


ItemPosition Item::getPosition() const{
    return position.load();
}


std::string Item::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


ItemType* Item::getType() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    return type;
}


int Item::getCooldown() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    int cooldown = 0;
    int weight = 0;
    if (type->getObjectType() == ObjectType::WEAPON_TYPE){
        weight = type->getWeight();

        if (weight < 5){
            cooldown = 1;
        } else if (weight < 10){
            cooldown = 2;
        } else if (weight < 20){
            cooldown = 3;
        } else if (weight < 40){
            cooldown = 4;
        } else if (weight < 80){
            cooldown = 5;
        } else {
            cooldown = 6;
        }
    }

    return cooldown;
}


bool Item::setLocation(InteractiveNoun* containingNoun){
    std::lock_guard<std::mutex> locationLock(locationMutex);
    if (containingNoun != nullptr){
        location = containingNoun;
        return true;
    }

    return false;
}


bool Item::setPosition(ItemPosition position){
    this->position.store(position);

    return true;
}


bool Item::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
    this->name = name;

    return true;
}


bool Item::setType(ItemType *type){
    std::lock_guard<std::mutex> typeLock(typeMutex);
    if (type != nullptr){
        this->type = type;
        return true;
    }

    return false;
}


bool Item::addNounAlias(std::string alias){
    bool success = false;
    Container *aContainer = nullptr;
    Character *aCharacter = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    success = InteractiveNoun::addNounAlias(alias);

    if (success){
        while (location != nullptr){
            if (aPosition == ItemPosition::GROUND){
                // location is an area
                success = location->registerAlias(false, alias, this);
                location = nullptr;
            } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
                if (location->getObjectType() == ObjectType::CONTAINER){
                    // location is a container
                    aContainer = dynamic_cast<Container*>(location);
                    if (aContainer != nullptr){
                        location = aContainer->getLocation();
                        aPosition = aContainer->getPosition();
                    } else {
                        location = nullptr;
                        success = false;
                    }
                } else {
                    location = nullptr;
                    success = false;
                }
            } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
                // location is a character
                if (location->getObjectType() == ObjectType::PLAYER){
                    // location is a player
                    success = location->registerAlias(false, alias, this);
                    location = nullptr;
                } else {
                    // location is a creature or NPC
                    aCharacter = dynamic_cast<Character*>(location);
                    if (aCharacter != nullptr){
                        success = aCharacter->getLocation()->registerAlias(false, alias, this);
                        location = nullptr;
                    } else {
                        location = nullptr;
                        success = false;
                    }
                }
            } else {
                // position is NONE, something has gone wrong
                location = nullptr;
                success = false;
            }
        }
    }
    
    return success;
}


bool Item::removeNounAlias(std::string alias){
    bool success = false;
    Container *aContainer = nullptr;
    Character *aCharacter = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    success = InteractiveNoun::removeNounAlias(alias);

    if (success){
        while (location != nullptr){
            if (aPosition == ItemPosition::GROUND){
                // location is an area
                success = location->unregisterAlias(false, alias, this);
                location = nullptr;
            } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
                if (location->getObjectType() == ObjectType::CONTAINER){
                    // location is a container
                    aContainer = dynamic_cast<Container*>(location);
                    if (aContainer != nullptr){
                        location = aContainer->getLocation();
                        aPosition = aContainer->getPosition();
                    } else {
                        location = nullptr;
                        success = false;
                    }
                } else {
                    location = nullptr;
                    success = false;
                }
            } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
                // location is a character
                if (location->getObjectType() == ObjectType::PLAYER){
                    // location is a player
                    success = location->unregisterAlias(false, alias, this);
                    location = nullptr;
                } else {
                    // location is a creature or NPC
                    aCharacter = dynamic_cast<Character*>(location);
                    if (aCharacter != nullptr){
                        success = aCharacter->getLocation()->unregisterAlias(false, alias, this);
                        location = nullptr;
                    } else {
                        location = nullptr;
                        success = false;
                    }
                }
            } else {
                // position is NONE, something has gone wrong
                location = nullptr;
                success = false;
            }
        }
    }
    
    return success;
}


bool Item::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;
    Container *aContainer = nullptr;
    Character *aCharacter = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);

    if (success){
        while (location != nullptr){
            if (aPosition == ItemPosition::GROUND){
                // location is an area
                success = location->registerAlias(true, alias, this);
                location = nullptr;
            } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
                if (location->getObjectType() == ObjectType::CONTAINER){
                    // location is a container
                    aContainer = dynamic_cast<Container*>(location);
                    if (aContainer != nullptr){
                        location = aContainer->getLocation();
                        aPosition = aContainer->getPosition();
                    } else {
                        location = nullptr;
                        success = false;
                    }
                } else {
                    location = nullptr;
                    success = false;
                }
            } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
                // location is a character
                if (location->getObjectType() == ObjectType::PLAYER){
                    // location is a player
                    success = location->registerAlias(true, alias, this);
                    location = nullptr;
                } else {
                    // location is a creature or NPC
                    aCharacter = dynamic_cast<Character*>(location);
                    if (aCharacter != nullptr){
                        success = aCharacter->getLocation()->registerAlias(true, alias, this);
                        location = nullptr;
                    } else {
                        location = nullptr;
                        success = false;
                    }
                }
            } else {
                // position is NONE, something has gone wrong
                location = nullptr;
                success = false;
            }
        }
    }
    
    return success;
}


bool Item::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;
    Container *aContainer = nullptr;
    Character *aCharacter = nullptr;

    ItemPosition aPosition = getPosition();
    InteractiveNoun *location = getLocation();

    success = InteractiveNoun::removeVerbAlias(aCommand, alias);

    if (success){
        while (location != nullptr){
            if (aPosition == ItemPosition::GROUND){
                // location is an area
                success = location->unregisterAlias(true, alias, this);
                location = nullptr;
            } else if ((aPosition == ItemPosition::IN) || (aPosition == ItemPosition::ON) || (aPosition == ItemPosition::UNDER)){
                if (location->getObjectType() == ObjectType::CONTAINER){
                    // location is a container
                    aContainer = dynamic_cast<Container*>(location);
                    if (aContainer != nullptr){
                        location = aContainer->getLocation();
                        aPosition = aContainer->getPosition();
                    } else {
                        location = nullptr;
                        success = false;
                    }
                } else {
                    location = nullptr;
                    success = false;
                }
            } else if ((aPosition == ItemPosition::INVENTORY) || (aPosition == ItemPosition::EQUIPPED)){
                // location is a character
                if (location->getObjectType() == ObjectType::PLAYER){
                    // location is a player
                    success = location->unregisterAlias(true, alias, this);
                    location = nullptr;
                } else {
                    // location is a creature or NPC
                    aCharacter = dynamic_cast<Character*>(location);
                    if (aCharacter != nullptr){
                        success = aCharacter->getLocation()->unregisterAlias(true, alias, this);
                        location = nullptr;
                    } else {
                        location = nullptr;
                        success = false;
                    }
                }
            } else {
                // position is NONE, something has gone wrong
                location = nullptr;
                success = false;
            }
        }
    }
    
    return success;
}


ObjectType Item::getObjectType() const{
    return ObjectType::ITEM;
}


std::string Item::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("ITEM");  // class of this object

    // This is all data in the Item class that is not inherited from other classes.  
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


Item* Item::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new Item object, getting all the data needed to do so from the objectDoc.  
    Item *newItem = new Item(gom->getPointer(objectDoc["location"].GetInt()),
                             gamedata::ItemPosition_Data::stringToEnum(objectDoc["position"].GetString()), 
                             objectDoc["name"].GetString(), 
                             static_cast<ItemType*>(gom->getPointer(objectDoc["item_type_id"].GetInt())),        
                             objectDoc["interactive_noun_data"]["id"].GetInt() );
         
    // Rebuild the new Item noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
        newItem->InteractiveNoun::addNounAlias(noun.GetString() );    
   
    // Rebuild the new Item action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Item          
        newItem->addAction(command, 
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
            newItem->InteractiveNoun::addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newItem; 
}


std::string Item::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;
    ItemType *aType = getType();

    message = "The " + getName() + " is ";
    message += aType->getDescription() + ".";

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

// would be better if got rid of dynamic cast **************************************************************
std::string Item::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    Area *anArea = nullptr;
    EffectType anEffect = EffectType::NONE;

    // check if this item is contained within a container
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

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
    message += " ";
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

    return message;
}

// would be better if got rid of dynamic cast **************************************************************
std::string Item::put(Player *aPlayer, Item *anItem, InteractiveNoun *containingNoun, ItemPosition position, std::vector<EffectType> *effects){
    Area *anArea;
    InteractiveNoun *location;
    ItemPosition currPosition;
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect;

    location = getLocation();
    currPosition = getPosition();

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
    // call this function on containingNoun
    if (containingNoun != nullptr){
        setLocation(containingNoun);
        message += containingNoun->put(aPlayer, this, nullptr, position, effects);
    } 

    return message;
}


std::string Item::drop(Player *aPlayer, std::vector<EffectType> *effects){
std::cout << "inside item::drop \n";
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();
    Area *anArea = aPlayer->getLocation();

    if ((position == ItemPosition::INVENTORY) || (position == ItemPosition::EQUIPPED)){
        if ((location != nullptr) && (location->getID() == aPlayer->getID())){
std::cout << "inside both if statements\n";
            setLocation(anArea);
            setPosition(ItemPosition::GROUND);
            aPlayer->removeFromInventory(this);
            anArea->addItem(this);

            // get results of put for this object
            resultMsg = getTextAndEffect(CommandEnum::DROP, anEffect);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
            }
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
        } else {
            message = "false";
        }
    } else {
        message = "false";
    }

    return message;
}


std::string Item::more(Player *aPlayer){
    std::string message = "Item: " + getName() + "\015\012";
    ItemType *aType = getType();
    int armorBonus = aType->getArmorBonus();
    int damage = aType->getDamage();
    int critMult = aType->getCritMultiplier();
    DamageType resistantTo = aType->getResistantTo();
    DamageType damageType = aType->getDamageType();
    AreaSize range = aType->getRange();

    message += "Type: " + aType->getName() + "\015\012";
    message += "Description: " + aType->getDescription() + "\015\012";
    message += "Weight: " + std::to_string(aType->getWeight()) + "\015\012";
    message += "Cost: " + std::to_string(aType->getCost()) + "\015\012";
    message += "Rarity: ";
    switch(aType->getRarity()){
        case ItemRarity::COMMON:
            message += "common \015\012";
            break;
        case ItemRarity::UNCOMMON:
            message += "uncommon \015\012";
            break;
        case ItemRarity::RARE:
            message += "rare \015\012";
            break;
        case ItemRarity::LEGENDARY:
            message += "legendary \015\012";
            break;
        case ItemRarity::QUEST:
            message += "quest \015\012";
            break;
    }
    switch(aType->getSlotType()){
        case EquipmentSlot::NONE:
            message += "";
            break;
        case EquipmentSlot::HEAD:
            message += "Equipment Slot Type: head \015\012";
            break;
        case EquipmentSlot::SHOULDERS:
            message += "Equipment Slot Type: shoulder \015\012";
            break;
        case EquipmentSlot::NECK:
            message += "Equipment Slot Type: neck \015\012";
            break;
        case EquipmentSlot::TORSO:
            message += "Equipment Slot Type: torso \015\012";
            break;
        case EquipmentSlot::BELT:
            message += "Equipment Slot Type: belt \015\012";
            break;
        case EquipmentSlot::LEGS:
            message += "Equipment Slot Type: legs \015\012";
            break;
        case EquipmentSlot::ARMS:
            message += "Equipment Slot Type: arms \015\012";
            break;
        case EquipmentSlot::HANDS:
            message += "Equipment Slot Type: hands \015\012";
            break;
        case EquipmentSlot::RIGHT_HAND:
            message += "Equipment Slot Type: right hand \015\012";
            break;
        case EquipmentSlot::LEFT_HAND:
            message += "Equipment Slot Type: left hand \015\012";
            break;
        case EquipmentSlot::FEET:
            message += "Equipment Slot Type: feet \015\012";
            break;
        case EquipmentSlot::RIGHT_RING:
            message += "Equipment Slot Type: right ring \015\012";
            break;
        case EquipmentSlot::LEFT_RING:
            message += "Equipment Slot Type: left ring \015\012";
            break;
    }
    if (armorBonus != -1){
        message += "Armor Bonus: " + std::to_string(armorBonus) + "\015\012";
    }
    if (damage != -1){
        message += "Damage: " + std::to_string(damage) + "\015\012";
    }
    if (critMult != -1){
        message += "Crit Multiplier: " + std::to_string(critMult) + "\015\012";
    }
    switch(resistantTo){
        case DamageType::NONE:
            message += "";
            break;
        case DamageType::CRUSHING:
            message += "Resistant to crushing damage\015\012";
            break;
        case DamageType::PIERCING:
            message += "Resistant to piercing damage\015\012";
            break;
        case DamageType::ELECTRIC:
            message += "Resistant to electric damage\015\012";
            break;
        case DamageType::FIRE:
            message += "Resistant to fire damage\015\012";
            break;
        case DamageType::WATER:
            message += "Resistant to water damage\015\012";
            break;
        case DamageType::WIND:
            message += "Resistant to wind damage\015\012";
            break;
        case DamageType::EARTH:
            message += "Resistant to earth damage\015\012";
            break;
        case DamageType::HEAL:
            message += "Resistant to healing\015\012";
            break;
    }
    switch(damageType){
        case DamageType::NONE:
            message += "";
            break;
        case DamageType::CRUSHING:
            message += "Damage Type: crushing damage\015\012";
            break;
        case DamageType::PIERCING:
            message += "Damage Type: piercing damage\015\012";
            break;
        case DamageType::ELECTRIC:
            message += "Damage Type: electric damage\015\012";
            break;
        case DamageType::FIRE:
            message += "Damage Type: fire damage\015\012";
            break;
        case DamageType::WATER:
            message += "Damage Type: water damage\015\012";
            break;
        case DamageType::WIND:
            message += "Damage Type: wind damage\015\012";
            break;
        case DamageType::EARTH:
            message += "Damage Type: earth damage\015\012";
            break;
        case DamageType::HEAL:
            message += "Damage Type: healing\015\012";
            break;
    }
    switch(range){
        case AreaSize::NONE:
            message += "";
            break;
        case AreaSize::SMALL:
            message += "Short Range Weapon \015\012";
            break;
        case AreaSize::MEDIUM:
            message += "Medium Range Weapon\015\012";
            break;
        case AreaSize::LARGE:
            message += "Long Range Weapon\015\012";
            break;
    }

    return message;
} 


std::string Item::equip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;
    InteractiveNoun *location = getLocation();

    if (aCharacter != nullptr){
        // character is the one equipping the item 
        if ((location != nullptr) && (location->getID() != aCharacter->getID())){
            // item is not in the Character's inventory
            return "false";
        } else if ((location != nullptr) && (getType()->getSlotType() != EquipmentSlot::NONE)){
            // item is in the Character's inventory
            setPosition(ItemPosition::EQUIPPED);

            // get results of equip for this object
            resultMsg = getTextAndEffect(CommandEnum::EQUIP, anEffect);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
            }
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
            // call this function on aCharacter
            message += aCharacter->equip(nullptr, this, aCharacter, effects);
        } else {
            return "false";
        }
    } else {
        // player is the one equipping the item
        if ((location != nullptr) && (location->getID() != aPlayer->getID())){
            // item is not in the player's inventory
            return "false";
        } else if ((location != nullptr) && (getType()->getSlotType() != EquipmentSlot::NONE)){
            // item is in the player's inventory
            setPosition(ItemPosition::EQUIPPED);

            // get results of equip for this object
            resultMsg = getTextAndEffect(CommandEnum::EQUIP, anEffect);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
            }
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
            // call this function on aPlayer
            message += aPlayer->equip(aPlayer, this, nullptr, effects);
        } else {
            return "false";
        }
    }

    return message;
}


std::string Item::unequip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

    if (aCharacter != nullptr){
        // character is the one unequipping the item 
        if ((location != nullptr) && (location->getID() != aCharacter->getID())){
            // item is not in the Character's inventory/equipment
            return "false";
        } else if ((location != nullptr) && (position == ItemPosition::EQUIPPED)){
            // item is in the Character's equipment
            setPosition(ItemPosition::INVENTORY);

            // get results of unequip for this object
            resultMsg = getTextAndEffect(CommandEnum::UNEQUIP, anEffect);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
            }
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
            // call this function on aCharacter
            message += aCharacter->unequip(nullptr, this, aCharacter, effects);
        } else {
            return "false";
        }
    } else {
        // player is the one unequipping the item
        if ((location != nullptr) && (location->getID() != aPlayer->getID())){
            // item is not in the player's inventory/equipment
            return "false";
        } else if ((location != nullptr) && (position == ItemPosition::EQUIPPED)){
            // item is in the player's equipment
            setPosition(ItemPosition::INVENTORY);

            // get results of unequip for this object
            resultMsg = getTextAndEffect(CommandEnum::UNEQUIP, anEffect);
            if (resultMsg.compare("false") != 0){
                message += resultMsg;
            }
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }
            // call this function on aPlayer
            message += aPlayer->unequip(aPlayer, this, nullptr, effects);
        } else {
            return "false";
        }
    }

    return message;
}


std::string Item::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    InteractiveNoun *location = getLocation();

    if (destination != nullptr){
        if (aCharacter != nullptr){
            // character is the one transferring the item
            if ((location != nullptr) && (location->getID() == aCharacter->getID())){
                // item is in character's inventory
                setLocation(destination);
                setPosition(ItemPosition::INVENTORY);

                // get results of transfer for this object
                resultMsg = getTextAndEffect(CommandEnum::TRANSFER, anEffect);
                if (resultMsg.compare("false") != 0){
                    message += resultMsg;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }

                // call this function on character and destination
                message += aCharacter->transfer(aPlayer, this, aCharacter, nullptr, effects);
                message += destination->transfer(aPlayer, this, nullptr, destination, effects);
            } else {
                return "false";
            }
        } else {
            // player is the one transferring the item
            if ((location != nullptr) && (location->getID() == aPlayer->getID())){
                // item is in player's inventory
                setLocation(destination);
                setPosition(ItemPosition::INVENTORY);

                // get results of transfer for this object
                resultMsg = getTextAndEffect(CommandEnum::TRANSFER, anEffect);
                if (resultMsg.compare("false") != 0){
                    message += resultMsg;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }

                // call this function on player and destination
                message += aPlayer->transfer(aPlayer, this, nullptr, nullptr, effects);
                message += destination->transfer(aPlayer, this, nullptr, destination, effects);
            } else {
                return "false";
            }
        }
    }

    return message;
}


std::string Item::move(Player *aPlayer, std::vector<EffectType> *effects){ 
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of move for this object
    message = getTextAndEffect(CommandEnum::MOVE, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->move(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *aCreature, bool playerAttacker, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;
    ItemType *anItemType = getType();
    bool itemEquipped = false;

    // check that item is a weapon (and doesn't heal)
    if ((anItemType->getObjectType() == ObjectType::WEAPON_TYPE) && (anItemType->getDamageType() != DamageType::HEAL)){
        // check that the attacker has this item equipped
        if (playerAttacker){
            if ((getLocation()->getID() == aPlayer->getID()) && (getPosition() == ItemPosition::EQUIPPED)){
                itemEquipped = true;
            } else {
                message = "You must have the weapon equipped to use it in an attack.";
            }
        } else {
            if ((getLocation()->getID() == aCreature->getID()) && (getPosition() == ItemPosition::EQUIPPED)){
                itemEquipped = true;
            }
        }

        if (itemEquipped){
            // get results of attack for this item
            message = getTextAndEffect(CommandEnum::ATTACK, anEffect);
            if (anEffect != EffectType::NONE){
                effects->push_back(anEffect);
            }

            // call this function on attacker
            if (playerAttacker){
                message = aPlayer->attack(aPlayer, this, nullptr, aCreature, true, effects);
            } else {
                message = aCreature->attack(aPlayer, this, nullptr, aCreature, false, effects);
            }
        }
    } else if (playerAttacker){
        message = "You can't attack with that.";
    }

    return message;
}


std::string Item::buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMessage;
    EffectType anEffect = EffectType::NONE;

    if ((aPlayer->getInConversation() != nullptr) && (getLocation()->getID() == aPlayer->getInConversation()->getID())){
        // this item is in the inventory of the NPC the player is talking to
        resultMessage = aPlayer->buy(aPlayer, this, effects);
        if (resultMessage.front() == 'f'){
            // item can't be purchased
            resultMessage.erase(resultMessage.begin());
            return resultMessage;
        } else {
            message += resultMessage;

            // change location, position
            setLocation(aPlayer);
            setPosition(ItemPosition::INVENTORY);
        }
    } else {
        return "false";
    }

    resultMessage = getTextAndEffect(CommandEnum::BUY, anEffect);
    if (resultMessage.compare("false") != 0){
        message += resultMessage;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
}


std::string Item::sell(Player *aPlayer, Item*, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMessage;
    EffectType anEffect = EffectType::NONE;

    if ((aPlayer->getInConversation() != nullptr) && (getLocation()->getID() == aPlayer->getID())){
        // this item is in the inventory of the player and the player is in conversation with a NPC
        resultMessage = aPlayer->sell(aPlayer, this, effects);
        if (resultMessage.front() == 'f'){
            // item can't be sold
            resultMessage.erase(resultMessage.begin());
            return resultMessage;
        } else {
            message += resultMessage;

            // change location, position
            setLocation(aPlayer->getInConversation());
            setPosition(ItemPosition::INVENTORY);
        }
    } else {
        return "false";
    }

    resultMessage = getTextAndEffect(CommandEnum::SELL, anEffect);
    if (resultMessage.compare("false") != 0){
        message += resultMessage;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
}


std::string Item::read(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of read for this object
    message = getTextAndEffect(CommandEnum::READ, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->read(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::breakIt(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of breakIt for this object
    message = getTextAndEffect(CommandEnum::BREAK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->breakIt(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::climb(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of climb for this object
    message = getTextAndEffect(CommandEnum::CLIMB, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->climb(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::turn(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of turn for this object
    message = getTextAndEffect(CommandEnum::TURN, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->turn(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::push(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of push for this object
    message = getTextAndEffect(CommandEnum::PUSH, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->push(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::pull(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of pull for this object
    message = getTextAndEffect(CommandEnum::PULL, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->pull(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::eat(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of eat for this object
    message = getTextAndEffect(CommandEnum::EAT, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->eat(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


std::string Item::drink(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    // get results of drink for this object
    message = getTextAndEffect(CommandEnum::DRINK, anEffect);
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    // call this function on ItemType
    resultMessage = getType()->drink(aPlayer, effects);

    if (resultMessage.compare("false") != 0){
        if (message.compare("false") != 0){
            message += " " + resultMessage;
        } else {
            message = resultMessage;
        }
    }

    return message;
} 


InteractiveNoun* Item::copy(){
    InteractiveNoun *location = nullptr;
    Area *anArea = nullptr;
    Character *aCharacter = nullptr;
    Container *aContainer = nullptr;
    Item *anItem = new Item(*this);

    location = anItem->getLocation();
    anItem->setPosition(this->getPosition());

    if (location->getObjectType() == ObjectType::AREA){
        anArea = dynamic_cast<Area*>(location);
        if (anArea != nullptr){
            anArea->addItem(anItem);
        }
    } else if (location->getObjectType() == ObjectType::CONTAINER){
        aContainer = dynamic_cast<Container*>(location);
        if (aContainer != nullptr){
            aContainer->place(anItem, anItem->getPosition());
        }
    } else {
        aCharacter = dynamic_cast<Character*>(location);
        if (aCharacter != nullptr){
            aCharacter->addToInventory(anItem);
            anItem->setPosition(ItemPosition::INVENTORY);
        }
    }

    return anItem;
}


/*bool Item::editAttribute(Player *aPlayer, std::string){
    return false;
}*/


bool Item::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Item::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["location"] = DataType::INTERACTIVE_NOUN_PTR;
    signature["position"] = DataType::ITEM_POSITION;
    signature["name"] = DataType::STRING_TYPE;
    signature["type"] = DataType::ITEM_TYPE_PTR;

    return signature;
}

}}