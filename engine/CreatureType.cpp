/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/09/2017
 * \modified    03/09/2017
 * \course      CS467, Winter 2017
 * \file        CreatureType.cpp
 *
 * \details     Implementation file for CreatureType class. 
 ************************************************************************/

#include "CreatureType.hpp"
#include "SpecialSkill.hpp"
#include "ItemType.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <CharacterSize_Data.hpp>
#include <XPTier_Data.hpp>     
#include <DamageType_Data.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

CreatureType::CreatureType()
: CombatantType()
, size(CharacterSize::MEDIUM)
, difficulty(XPTier::NORMAL)
{ }


CreatureType::CreatureType(CharacterSize size, XPTier difficulty, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints)
: CombatantType(name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints)
, size(CharacterSize::MEDIUM)
, difficulty(XPTier::NORMAL)
{ }


CreatureType::CreatureType(CharacterSize size, XPTier difficulty, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints, int anID)
: CombatantType(name, skill, attackBonus, armorBonus, resistantTo, weakTo, healPoints, anID)
, size(CharacterSize::MEDIUM)
, difficulty(XPTier::NORMAL)
{ }


CharacterSize CreatureType::getSize() const{
    return size.load();
}


XPTier CreatureType::getDifficulty() const{
    return difficulty.load();
}


bool CreatureType::setSize(CharacterSize size){
    this->size.store(size);

    return true;
}


bool CreatureType::setDifficulty(XPTier difficulty){
    this->difficulty.store(difficulty);

    return true;
}


ObjectType CreatureType::getObjectType() const{
    return ObjectType::CREATURE_TYPE;
}


std::string CreatureType::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("CREATURE_TYPE");  // class of this object

    // This is all data in the CreatureType class that is not inherited from other classes.
    writer.String("size");
    writer.String(gamedata::CharacterSize_Data::enumToString(this->getSize()).c_str());
    writer.String("difficulty");
    writer.String(gamedata::XPTier_Data::enumToString(this->getDifficulty()).c_str());
       
    // This is all data inherited from the CombatantType class.    
    writer.String("name");
    writer.String(this->getName().c_str());
    writer.String("special_skill_id");
    writer.Int(this->getSpecialSkill()->getID());
    writer.String("attack_bonus");
    writer.Int(this->getAttackBonus());
    writer.String("armor_bonus");
    writer.Int(this->getArmorBonus());
    writer.String("resistant_to");
    writer.String(gamedata::DamageType_Data::enumToString(this->getResistantTo()).c_str());
    writer.String("weak_to");
    writer.String(gamedata::DamageType_Data::enumToString(this->getWeakTo()).c_str());
    writer.String("heal_points");
    writer.Double(this->getHealPoints());   
    
    writer.EndObject(); // ends this object     
    writer.EndObject(); // outer object wrapper
          
    // Convert to a document so that inherited data from InteractiveNoun can be added.
    rapidjson::Document objectDoc;
    objectDoc.Parse(buffer.GetString());    
    
    // Get the data inherited from the InteractiveNoun class and add it to the object document.
    rapidjson::Document interactiveNounDataDoc(&objectDoc.GetAllocator());
    interactiveNounDataDoc.Parse(this->serializeJustInteractiveNoun().c_str());
    objectDoc["object"].AddMember("interactive_noun_data",interactiveNounDataDoc, objectDoc.GetAllocator() );
    
    // Write the object doc to a buffer for output.
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> outWriter(buffer);
    objectDoc.Accept(outWriter);
    
    return buffer.GetString();
}


CreatureType* CreatureType::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new CreatureType object, getting all the data needed to do so from the objectDoc. 
    CreatureType *newCreatureType = new CreatureType(gamedata::CharacterSize_Data::stringToEnum(objectDoc["size"].GetString()),
                                                     gamedata::XPTier_Data::stringToEnum(objectDoc["difficulty"].GetString()),
                                                     objectDoc["name"].GetString(),
                                                     static_cast<SpecialSkill*>(gom->getPointer(objectDoc["special_skill_id"].GetInt())),
                                                     objectDoc["attack_bonus"].GetInt(),
                                                     objectDoc["armor_bonus"].GetInt(),
                                                     gamedata::DamageType_Data::stringToEnum(objectDoc["resistant_to"].GetString()),
                                                     gamedata::DamageType_Data::stringToEnum(objectDoc["weak_to"].GetString()),
                                                     objectDoc["heal_points"].GetDouble(),
                                                     objectDoc["interactive_noun_data"]["id"].GetInt() );
       
    // Rebuild the new CreatureType noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
        newCreatureType->addNounAlias(noun.GetString() );    
   
    // Rebuild the new CreatureType action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new CreatureType          
        newCreatureType->addAction(command, 
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
            newCreatureType->addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newCreatureType; 
}


InteractiveNoun* CreatureType::copy(){
    return nullptr;
}


bool CreatureType::editAttribute(Player*, std::string){
    return false;
}


bool CreatureType::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> CreatureType::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["size"] = DataType::CHARACTER_SIZE;
    signature["difficulty"] = DataType::X_P_TIER;
    signature["name"] = DataType::STRING_TYPE;
    signature["skill"] = DataType::SPECIAL_SKILL_PTR;
    signature["attack bonus"] = DataType::INT_TYPE;
    signature["armor bonus"] = DataType::INT_TYPE;
    signature["resistant to"] = DataType::DAMAGE_TYPE;
    signature["weak to"] = DataType::DAMAGE_TYPE;
    signature["heal points"] = DataType::FLOAT_TYPE;

    return signature;
}

}}