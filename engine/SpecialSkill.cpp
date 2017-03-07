/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)  
 * \created     02/10/2017
 * \modified    03/07/2017
 * \course      CS467, Winter 2017
 * \file        SpecialSkill.cpp
 *
 * \details     Implementation file for SpecialSkill class. 
 ************************************************************************/

#include "SpecialSkill.hpp"
#include "EffectType.hpp"
#include "CommandEnum.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <DamageType_Data.hpp>  
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

namespace legacymud { namespace engine {

SpecialSkill::SpecialSkill()
: InteractiveNoun()
, name("")
, damage(0)
, damageType(DamageType::NONE)
, cost(0)
, cooldown(0)
{ }


SpecialSkill::SpecialSkill(std::string name, int damage, DamageType type, int cost, time_t cooldown)
: InteractiveNoun()
, name(name)
, damage(damage)
, damageType(type)
, cost(cost)
, cooldown(cooldown)
{
    std::string idAlias = "skill " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


SpecialSkill::SpecialSkill(std::string name, int damage, DamageType type, int cost, time_t cooldown, int anID)
: InteractiveNoun(anID)
, name(name)
, damage(damage)
, damageType(type)
, cost(cost)
, cooldown(cooldown)
{
    std::string idAlias = "skill " + std::to_string(getID());
    addNounAlias(idAlias);
    addNounAlias(name);
}


std::string SpecialSkill::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
}


int SpecialSkill::getDamage() const{
    return damage.load();
}


DamageType SpecialSkill::getDamageType() const{
    return damageType.load();
}


int SpecialSkill::getCost() const{
    return cost.load();
}


time_t SpecialSkill::getCooldown() const{
    return cooldown.load();
}


bool SpecialSkill::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    this->name = name;

    return true;
}


bool SpecialSkill::setDamage(int damage){
    this->damage.store(damage);

    return true;
}


bool SpecialSkill::setDamageType(DamageType type){
    damageType.store(type);

    return true; 
}


bool SpecialSkill::setCost(int cost){
    this->cost.store(cost);

    return true;
}


bool SpecialSkill::setCooldown(time_t cooldown){
    this->cooldown.store(cooldown);

    return true;
}


ObjectType SpecialSkill::getObjectType() const{
    return ObjectType::SPECIAL_SKILL;
}


std::string SpecialSkill::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("SPECIAL_SKILL");  // class of this object

    // This is all data in the SpecialSkill class that is not inherited from other classes.  
    writer.String("name");
    writer.String(this->getName().c_str());
    writer.String("damage");
    writer.Int(this->getDamage());
    writer.String("damage_type");
    writer.String(gamedata::DamageType_Data::enumToString(this->getDamageType()).c_str());
    writer.String("cost");
    writer.Int(this->getCost());
    writer.String("cooldown");
    writer.Int(this->getCooldown());
    
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


SpecialSkill* SpecialSkill::deserialize(std::string jsonStr){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
 
    // Construct a new SpecialSkill object, getting all the data needed to do so from the objectDoc. 
    SpecialSkill *newSpecialSkill = new SpecialSkill(objectDoc["name"].GetString(),
                                                     objectDoc["damage"].GetInt(),
                                                     gamedata::DamageType_Data::stringToEnum(objectDoc["damage_type"].GetString()),
                                                     objectDoc["cost"].GetInt(),
                                                     objectDoc["cooldown"].GetInt(),
                                                     objectDoc["interactive_noun_data"]["id"].GetInt() );
 
    // Rebuild the new SpecialSkill noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray()) {           
        if (objectDoc["name"].GetString() != noun.GetString() )     // note: `name` is automatically added to noun aliases
            newSpecialSkill->addNounAlias(noun.GetString() );    
    }
   
    // Rebuild the new SpecialSkill action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new SpecialSkill          
        newSpecialSkill->addAction(command, 
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
            newSpecialSkill->addVerbAlias(command, 
                                          alias["verb_alias"].GetString(), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                          gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                          prepMap );
        }       
    }    
    
    return newSpecialSkill; 
}


std::string SpecialSkill::more(Player *aPlayer){
    std::string message = "Special Skill: " + getName() + "\015\012";
    message += "Effect: " + std::to_string(getDamage()) + " ";
    switch(getDamageType()){
        case DamageType::NONE:
            message += "neutral damage\015\012";
            break;
        case DamageType::CRUSHING:
            message += "crushing damage\015\012";
            break;
        case DamageType::PIERCING:
            message += "piercing damage\015\012";
            break;
        case DamageType::ELECTRIC:
            message += "electric damage\015\012";
            break;
        case DamageType::FIRE:
            message += "fire damage\015\012";
            break;
        case DamageType::WATER:
            message += "water damage\015\012";
            break;
        case DamageType::WIND:
            message += "wind damage\015\012";
            break;
        case DamageType::EARTH:
            message += "earth damage\015\012";
            break;
        case DamageType::HEAL:
            message += "healing\015\012";
            break;
    }
    message += "Cost: " + std::to_string(getCost()) + "\015\012";
    message += "Cooldown: " + std::to_string(getCooldown()) + "\015\012";

    return message;
} 


std::string SpecialSkill::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    return "";
}


std::string SpecialSkill::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Player *aRecipient, std::vector<EffectType> *effects){
    std::string message, resultMessage;
    EffectType anEffect = EffectType::NONE;

    if (getDamageType() == DamageType::HEAL){
        // call this function on character
        resultMessage = character->useSkill(aPlayer, this, nullptr, nullptr, effects);
        if (resultMessage.compare("false") != 0){
            if (resultMessage.front() == 'f'){
                // skill can't be used
                resultMessage.erase(resultMessage.begin());
                message = resultMessage;
            } else {
                message += resultMessage;

                // get results of useSkill for this skill
                resultMessage = getTextAndEffect(CommandEnum::USE_SKILL, anEffect);
                if (resultMessage.compare("false") != 0){
                    message += resultMessage;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }
            }
        } else {
            message = "You can't use the " + getName() + " skill on " + character->getName() + ".";
        }
    } else {
        message = getName() + " is an attacking skill. You can't \"use\" it, but rather need to \"attack with\" it.";
    }

    return message;
} 


InteractiveNoun* SpecialSkill::copy(){
    return nullptr;
}


bool SpecialSkill::editAttribute(Player*, std::string){
    return false;
}


bool SpecialSkill::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> SpecialSkill::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["damage"] = DataType::INT_TYPE;
    signature["damage type"] = DataType::DAMAGE_TYPE;
    signature["cost"] = DataType::INT_TYPE;
    signature["cooldown"] = DataType::INT_TYPE;

    return signature;
}

}}