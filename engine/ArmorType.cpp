/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/09/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        ArmorType.cpp
 *
 * \details     Implementation file for ArmorType class. 
 ************************************************************************/

#include "ArmorType.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <DamageType_Data.hpp>  
#include <EquipmentSlot_Data.hpp> 
#include <ItemRarity_Data.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

ArmorType::ArmorType()
: ItemType()
, armorBonus(0)
, resistantTo(DamageType::NONE)
{ }


ArmorType::ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType)
: ItemType(weight, rarity, description, name, cost, slotType)
, armorBonus(bonus)
, resistantTo(resistantTo)
{ }


ArmorType::ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType, int anID)
: ItemType(weight, rarity, description, name, cost, slotType, anID)
, armorBonus(bonus)
, resistantTo(resistantTo)
{ }


int ArmorType::getArmorBonus() const{
    return armorBonus.load();
}


DamageType ArmorType::getResistantTo() const{
    return resistantTo.load();
}


bool ArmorType::setArmorBonus(int bonus){
    armorBonus.store(bonus);
    return true;
}


bool ArmorType::setResistantTo(DamageType resistance){
    resistantTo.store(resistance);
    return true;
}


ObjectType ArmorType::getObjectType() const{
    return ObjectType::ARMOR_TYPE;
}


std::string ArmorType::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("ARMOR_TYPE");  // class of this object

    // This is all data in the ArmorType class that is not inherited from other classes.
    writer.String("armor_bonus");
    writer.Int(this->getArmorBonus());
    writer.String("resistant_to");
    writer.String(gamedata::DamageType_Data::enumToString(this->getResistantTo()).c_str());
    
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


ArmorType* ArmorType::deserialize(std::string jsonStr){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new ArmorType object, getting all the data needed to do so from the objectDoc.
    ArmorType *newArmorType = new ArmorType(objectDoc["armor_bonus"].GetInt(), 
                                            gamedata::DamageType_Data::stringToEnum(objectDoc["resistant_to"].GetString()),
                                            objectDoc["weight"].GetInt(), 
                                            gamedata::ItemRarity_Data::stringToEnum(objectDoc["rarity"].GetString()), 
                                            objectDoc["description"].GetString(), 
                                            objectDoc["name"].GetString(), 
                                            objectDoc["cost"].GetInt(), 
                                            gamedata::EquipmentSlot_Data::stringToEnum(objectDoc["slot_type"].GetString()) , 
                                            objectDoc["interactive_noun_data"]["id"].GetInt() );                                            
 
    // Rebuild the new ArmorType noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray()) {           
        if (objectDoc["name"].GetString() != noun.GetString() )     // note: `name` is automatically added to noun aliases
            newArmorType->addNounAlias(noun.GetString() );    
    }
   
    // Rebuild the new ArmorType action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new ArmorType          
        newArmorType->addAction(command, 
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
            newArmorType->addVerbAlias(command, 
                                          alias["verb_alias"].GetString(), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                          prepMap );
        }       
    }    
    
    return newArmorType; 
}


InteractiveNoun* ArmorType::copy(){
    return nullptr;
}


bool ArmorType::editAttribute(Player *aPlayer, std::string attribute){
    return false;
}


bool ArmorType::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> ArmorType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["armor bonus"] = DataType::INT_TYPE;
    signature["resistant to"] = DataType::DAMAGE_TYPE;
    signature["weight"] = DataType::INT_TYPE;
    signature["rarity"] = DataType::ITEM_RARITY;
    signature["description"] = DataType::STRING_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["equipment slot type"] = DataType::EQUIPMENT_SLOT;

    return signature;
}

}}
