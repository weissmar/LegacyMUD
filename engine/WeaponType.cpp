/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)  
 * \created     02/01/2017
 * \modified    03/09/2017
 * \course      CS467, Winter 2017
 * \file        WeaponType.cpp
 *
 * \details     Implementation file for WeaponType class.
 ************************************************************************/

#include "WeaponType.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <DamageType_Data.hpp>  
#include <EquipmentSlot_Data.hpp>
#include <AreaSize_Data.hpp>  
#include <ItemRarity_Data.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

WeaponType::WeaponType()
: ItemType()
, damage(0)
, damageType(DamageType::NONE)
, range(AreaSize::SMALL)
, critMultiplier(2)
{ }


WeaponType::WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType)
: ItemType(weight, rarity, description, name, cost, slotType)
, damage(damage)
, damageType(type)
, range(range)
, critMultiplier(critMultiplier)
{ }


WeaponType::WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType, int anID)
: ItemType(weight, rarity, description, name, cost, slotType, anID)
, damage(damage)
, damageType(type)
, range(range)
, critMultiplier(critMultiplier)
{ }


int WeaponType::getDamage() const{
    return damage.load();
}


DamageType WeaponType::getDamageType() const{
    return damageType.load();
}


AreaSize WeaponType::getRange() const{
    return range.load();
}


int WeaponType::getCritMultiplier() const{
    return critMultiplier.load();
}


bool WeaponType::setDamage(int damage){
    this->damage.store(damage);

    return true;
}


bool WeaponType::setDamageType(DamageType type){
    damageType.store(type);

    return true;
}


bool WeaponType::setRange(AreaSize range){
    this->range.store(range);

    return true;
}


bool WeaponType::setCritMultiplier(int multiplier){
    critMultiplier.store(multiplier);

    return true;
}


ObjectType WeaponType::getObjectType() const{
    return ObjectType::WEAPON_TYPE;
}


std::string WeaponType::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("WEAPON_TYPE");  // class of this object

    // This is all data in the WeaponType class that is not inherited from other classes.
    writer.String("damage");
    writer.Int(this->getDamage());
    writer.String("damage_type");
    writer.String(gamedata::DamageType_Data::enumToString(this->getDamageType()).c_str());
    writer.String("range");
    writer.String(gamedata::AreaSize_Data::enumToString(this->getRange()).c_str());
    writer.String("crit_multiplier");
    writer.Int(this->getCritMultiplier());
        
    // This is all data inherited from the ItemType class.
    writer.String("name");                          
    writer.String(this->getName().c_str());    
    writer.String("weight");                        
    writer.Int(this->getWeight());    
    writer.String("rarity");                       
    writer.String(gamedata::ItemRarity_Data::enumToString(this->getRarity()).c_str()); 
    writer.String("description");                   
    writer.String(this->getDescription().c_str());     
    writer.String("cost");                          
    writer.Int(this->getCost());      
    writer.String("slot_type");                     
    writer.String(gamedata::EquipmentSlot_Data::enumToString(this->getSlotType()).c_str());  

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


WeaponType* WeaponType::deserialize(std::string jsonStr){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new WeaponType object, getting all the data needed to do so from the objectDoc.    
    WeaponType *newWeaponType = new WeaponType(objectDoc["damage"].GetInt(), 
                                               gamedata::DamageType_Data::stringToEnum(objectDoc["damage_type"].GetString()),
                                               gamedata::AreaSize_Data::stringToEnum(objectDoc["range"].GetString()),
                                               objectDoc["crit_multiplier"].GetInt(),
                                               objectDoc["weight"].GetInt(), 
                                               gamedata::ItemRarity_Data::stringToEnum(objectDoc["rarity"].GetString()), 
                                               objectDoc["description"].GetString(), 
                                               objectDoc["name"].GetString(), 
                                               objectDoc["cost"].GetInt(), 
                                               gamedata::EquipmentSlot_Data::stringToEnum(objectDoc["slot_type"].GetString()) , 
                                               objectDoc["interactive_noun_data"]["id"].GetInt() );   

    // Rebuild the new WeaponType noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())           
        newWeaponType->addNounAlias(noun.GetString() );    
   
    // Rebuild the new WeaponType action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new WeaponType          
        newWeaponType->addAction(command, 
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
            newWeaponType->addVerbAlias(command, 
                                          alias["verb_alias"].GetString(), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                          prepMap );
        }       
    }    
    
    return newWeaponType; 
}


InteractiveNoun* WeaponType::copy(){
    return nullptr;
}


bool WeaponType::editAttribute(Player*, std::string){
    return false;
}


bool WeaponType::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> WeaponType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["damage"] = DataType::INT_TYPE;
    signature["damage type"] = DataType::DAMAGE_TYPE;
    signature["range"] = DataType::AREA_SIZE;
    signature["crit multiplier"] = DataType::INT_TYPE;
    signature["weight"] = DataType::INT_TYPE;
    signature["rarity"] = DataType::ITEM_RARITY;
    signature["description"] = DataType::STRING_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["equipment slot type"] = DataType::EQUIPMENT_SLOT;

    return signature;
}

}}