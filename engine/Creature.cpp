/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions)  
 * \created     02/09/2017
 * \modified    03/12/2017
 * \course      CS467, Winter 2017
 * \file        Creature.cpp
 *
 * \details     Implementation file for Creature class. 
 ************************************************************************/

#include <iostream>
#include "Creature.hpp"
#include "Area.hpp"
#include "CreatureType.hpp"
#include "SpecialSkill.hpp"
#include "CharacterSize.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "GameLogic.hpp"
#include "PlayerClass.hpp"
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

const int RESPAWN_TIME = 60;

Creature::Creature()
: Combatant()
, type(nullptr)
, ambulatory (false)
, respawnClock(0)
{ }


Creature::Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, name, description, money, aLocation, maxInventoryWeight)
, type(aType)
, ambulatory (ambulatory)
, respawnClock(0)
{ }


Creature::Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, int dexterity, int strength, int intelligence, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, anID)
, type(aType)
, ambulatory (ambulatory)
, respawnClock(0)
{ }


Creature::Creature(const Creature &otherCreature) : Combatant(otherCreature) {
    otherCreature.typeMutex.lock();
    type = otherCreature.type;
    otherCreature.typeMutex.unlock();
    ambulatory.store(false);
    respawnClock.store(0);
}


Creature & Creature::operator=(const Creature &otherCreature){
    if (this == &otherCreature)
        return *this;

    otherCreature.typeMutex.lock();
    type = otherCreature.type;
    otherCreature.typeMutex.unlock();
    ambulatory.store(false);
    respawnClock.store(0);

    return *this;
} 

/*
Creature::~Creature(){

}*/


CreatureType* Creature::getType() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    return type;
}


bool Creature::getAmbulatory() const{
    return ambulatory.load();
}


int Creature::getSizeModifier() const{
    int modifier = 0;
    CharacterSize aSize = getType()->getSize();

    switch (aSize){
        case CharacterSize::TINY:
            modifier = 2;
            break;
        case CharacterSize::SMALL:
            modifier = 1;
            break;
        case CharacterSize::MEDIUM:
            modifier = 0;
            break;
        case CharacterSize::LARGE:
            modifier = -1;
            break;
        case CharacterSize::HUGE:
            modifier = -2;
            break;
    }

    return modifier;
}


int Creature::getXP() const{
    XPTier xpTier = getType()->getDifficulty();
    int xp = 0;

    switch(xpTier){
        case XPTier::TRIVIAL:
            xp = 100;
            break;
        case XPTier::EASY:
            xp = 200;
            break;
        case XPTier::NORMAL:
            xp = 400;
            break;
        case XPTier::HARD:
            xp = 800;
            break;
        case XPTier::LEGENDARY:
            xp = 1600;
            break;
    }

    return xp;
}


bool Creature::setRespawn(){
    respawnClock.store(std::time(nullptr) + RESPAWN_TIME);
    return true;
}


bool Creature::readyRespawn() const{
    return respawnClock.load() <= std::time(nullptr);
}


bool Creature::setType(CreatureType *aType){
    if (aType != nullptr){
        std::lock_guard<std::mutex> typeLock(typeMutex);
        type = aType;
        return true;
    }
    
    return false;
}


bool Creature::setAmbulatory(bool ambulatory){
    this->ambulatory.store(ambulatory);
    return true;
}


bool Creature::addNounAlias(std::string alias){
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


bool Creature::removeNounAlias(std::string alias){
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


bool Creature::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
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


bool Creature::removeVerbAlias(CommandEnum aCommand, std::string alias){
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


ObjectType Creature::getObjectType() const{
    return ObjectType::CREATURE;
}


std::string Creature::serialize(){    
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("CREATURE");  // class of this object

    // This is all data in the Creature class that is not inherited from other classes.
    writer.String("creature_type_id");
    writer.Int(this->getType()->getID());
    writer.String("ambulatory");
    writer.Bool(this->getAmbulatory());
    
    // This is all data kept by the inherited Combatant class.
    writer.String("max_health");
    writer.Int(this->getMaxHealth());
    writer.String("spawn_location_id");
    writer.Int(this->getSpawnLocation()->getID());
    writer.String("max_special_points");
    writer.Int(this->getMaxSpecialPts());
    writer.String("dexterity");
    writer.Int(this->getDexterity());
    writer.String("strength");
    writer.Int(this->getStrength()); 
    writer.String("intelligence");
    writer.Int(this->getIntelligence());
   
    // This is all data kept by the inherited Character class.
    writer.String("name");
    writer.String(this->getName().c_str());
    writer.String("description");
    writer.String(this->getDescription().c_str());
    writer.String("money");
    writer.Int(this->getMoney());
    writer.String("location_area_id");
    writer.Int(this->getLocation()->getID());
    writer.String("max_inventory_weight");
    writer.Int(this->getMaxInventoryWeight());    
    
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


Creature* Creature::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new Creature object, getting all the data needed to do so from the objectDoc. 
    Creature *newCreature = new Creature(static_cast<CreatureType*>(gom->getPointer(objectDoc["creature_type_id"].GetInt())),
                                         objectDoc["ambulatory"].GetBool(),
                                         objectDoc["max_health"].GetInt(),
                                         static_cast<Area*>(gom->getPointer(objectDoc["spawn_location_id"].GetInt())),
                                         objectDoc["max_special_points"].GetInt(),
                                         objectDoc["dexterity"].GetInt(),
                                         objectDoc["strength"].GetInt(),
                                         objectDoc["intelligence"].GetInt(),
                                         objectDoc["name"].GetString(),
                                         objectDoc["description"].GetString(),
                                         objectDoc["money"].GetInt(),
                                         static_cast<Area*>(gom->getPointer(objectDoc["location_area_id"].GetInt())),
                                         objectDoc["max_inventory_weight"].GetInt(),
                                         objectDoc["interactive_noun_data"]["id"].GetInt() );
                                                     
    // Rebuild the new Creature noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
        newCreature->InteractiveNoun::addNounAlias(noun.GetString() );    
   
    // Rebuild the new Creature action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Creature          
        newCreature->addAction(command, 
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
            newCreature->InteractiveNoun::addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newCreature; 
}


std::string Creature::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg = "";
    EffectType anEffect = EffectType::NONE;

    message = getName() + " is " + getDescription();

    if (aPlayer->isEditMode()){
        message += " [character " + std::to_string(getID()) + "]";
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


std::string Creature::take(Player* aPlayer, Item* anItem, InteractiveNoun* aContainer, InteractiveNoun* aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool success;

    success = addToInventory(anItem);
    if (success){
        resultMsg = getTextAndEffect(CommandEnum::TAKE, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    } else {
        message = "false";
    }

    return message;
}


std::string Creature::equip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    std::string strSuccess;
    EffectType anEffect = EffectType::NONE;
    bool success = false;

    if (anItem != nullptr){
        strSuccess = equipItem(anItem);

        if (strSuccess.compare("true") == 0){
            success = true;
        } else if (strSuccess.compare("false") == 0){
            message = "false";
        } else {
            success = true;
            message = "Unequipped the " + strSuccess + ". ";
        }
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::unequip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool success = false;

    if (anItem != nullptr){
        success = unequipItem(anItem);
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;

    if (anArea != nullptr){
        setLocation(anArea);

        resultMsg += getTextAndEffect(CommandEnum::GO, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    bool success = false;
    EffectType anEffect = EffectType::NONE;

    if (anItem != nullptr){
        if ((aCharacter != nullptr) && (aCharacter->getID() == this->getID())){
            // item is being removed from this character
            success = removeFromInventory(anItem);
        } else if ((destination != nullptr) && (destination->getID() == this->getID())){
            // item is being added to this character
            success = addToInventory(anItem);
        }
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::TRANSFER, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *aCreature, bool playerAttacker, std::vector<EffectType> *effects){
    int attackDamage = 0;
    DamageType damageType = DamageType::NONE;
    int cost = 0;
    int cooldown = 0;
    int totalDamage = 0;
    int critMultiplier = 1;
    int currHealth = 0;
    AreaSize range = AreaSize::NONE;
    Area *location = getLocation();
    std::string message = "";

    if (aSkill != nullptr){
        // get damage, damage type, cost, and cooldown from skill
        attackDamage = aSkill->getDamage();
        damageType = aSkill->getDamageType();
        cost = aSkill->getCost();
        cooldown = aSkill->getCooldown();

        // check if creature has enough special points to execute the skill
        if (getCurrentSpecialPts() > cost){   
            // get attack damage (if attack succeeds)
            totalDamage = getAttackDamage(aPlayer, 2, attackDamage, damageType, AreaSize::NONE);

            // if attack hit
            if (totalDamage != 0){
                // subtract damage from player health
                currHealth = aPlayer->subtractFromCurrentHealth(totalDamage);

                // subtract cost from creature special points
                subtractFromCurrSpecialPts(cost);

                // add skill cooldown to cooldown
                setCooldown(cooldown);

                // message player
                message = "A creature named " + getName() + " attacks you with " + aSkill->getName() + " and does ";
                message += std::to_string(totalDamage) + " damage. You now have " + std::to_string(currHealth) + " health.";
            } else {
                message = "A creature named " + getName() + " tries to attack you, but misses.";
            }
        } else {
            message = "A creature named " + getName() + " tries to attack you, but they just don't quite have it in them right now.";
        }
    } else if (anItem != nullptr){
        // get damage, damage type, cooldown, range, crit multiplier from weapon
        attackDamage = anItem->getType()->getDamage();
        damageType = anItem->getType()->getDamageType();
        cooldown = anItem->getCooldown();
        range = anItem->getType()->getRange();
        critMultiplier = anItem->getType()->getCritMultiplier();

        // check if weapon range is <= area size
        if (location->isBiggerThanOrEqual(range)){
            // get attack damage (if attack succeeds)
            totalDamage = getAttackDamage(aPlayer, critMultiplier, attackDamage, damageType, range);

            if (totalDamage != 0){
                // subtract damage from player health
                currHealth = aPlayer->subtractFromCurrentHealth(totalDamage);

                // add weapon cooldown to cooldown
                setCooldown(cooldown);

                // message player
                message = "A creature named " + getName() + " attacks you with " + anItem->getName() + " and does ";
                message += std::to_string(totalDamage) + " damage. You now have " + std::to_string(currHealth) + " health.";
            } else {
                message = "A creature named " + getName() + " tries to attack you, but misses.";
            }
        } else {
            message = "A creature named " + getName() + " tries to attack you, but they don't have enough room to manuever.";
        }
    } else {
        // default attack (unarmed)
        totalDamage = getAttackDamage(aPlayer, 2, 3, DamageType::NONE, AreaSize::SMALL);

        if (totalDamage != 0){
            // subtract damage from player health
            currHealth = aPlayer->subtractFromCurrentHealth(totalDamage);

            // add unarmed cooldown to cooldown
            setCooldown(1);

            // message player
            message = "A creature named " + getName() + " hits you and does ";
            message += std::to_string(totalDamage) + " damage. You now have " + std::to_string(currHealth) + " health.";
        } else {
            message = "A creatue named " + getName() + " tries to hit you, but misses.";
        }
    }

    return message;
}


int Creature::getAttackDamage(Player *aPlayer, int critMultiplier, int attackDamage, DamageType damageType, AreaSize range){
    int attackBonus = 0;
    int armorClass = 0;
    int attackRoll = 0;
    bool hit = false;
    bool possibleCrit = false;
    PlayerClass *playerClass = aPlayer->getPlayerClass();
    int totalDamage = 0;

    // check attack bonus of creature 
    attackBonus = getType()->getAttackBonus(0) + getSizeModifier();
    if (range == AreaSize::NONE){
        // skill attack - add intelligence modifier to attack bonus
        attackBonus += getIntelligenceModifier();
    } else if (range == AreaSize::SMALL){
        // close-range weapon/unarmed attack - add strength modifier to attack bonus
        attackBonus += getStrengthModifier();
    } else {
        // long-range weapon attack - add dexterity modifier to attack bonus
        attackBonus += getDexterityModifier();
    }

    // check armor class of player
    armorClass = 10 + aPlayer->getArmorBonus() + aPlayer->getDexterityModifier() + aPlayer->getSizeModifier();

    // roll for attack success
    attackRoll = GameLogic::rollDice(20, 1);

    if (attackRoll == 20){
        // natural 20
        hit = true;
        possibleCrit = true;
    } else if (attackRoll == 1){
        // natural 1
        hit = false;
    } else {
        attackRoll += attackBonus;

        if (attackRoll > armorClass){
            hit = true;
        }
    }

    // roll attack again to see if crit
    if (possibleCrit){
        attackRoll = GameLogic::rollDice(20, 1) + attackBonus;
        if (attackRoll <= armorClass){
            // no crit
            critMultiplier = 1;
        }
    } else {
        critMultiplier = 1;
    }

    if (hit){
        // roll for attack damage + crit if relevant
        totalDamage = GameLogic::rollDice(attackDamage, critMultiplier);
        if (range == AreaSize::SMALL){
            // add strength modifier to damage if short-range
            totalDamage += getStrengthModifier() * critMultiplier;
        }

        // check resistance and weakness of player
        if (playerClass->getResistantTo() == damageType){
            // reduce damage by 1d3 (but not less than 1)
            totalDamage -= GameLogic::rollDice(3, 1);
        } else if (playerClass->getWeakTo() == damageType){
            // increase damage by 1d3 
            totalDamage += GameLogic::rollDice(3, 1);
        }
    }

    if (totalDamage < 1){
        totalDamage = 1;
    }

    return totalDamage;
}


InteractiveNoun* Creature::copy(){
    Area *location = getLocation();
    Creature *aCreature = new Creature(*this);

    location->addCharacter(aCreature);

    return aCreature;
}


/*bool Creature::editAttribute(Player*, std::string){
    return false;
}*/


bool Creature::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Creature::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["creature type"] = DataType::CREATURE_TYPE_PTR;
    signature["ambulatory"] = DataType::BOOL_TYPE;
    signature["maximum health"] = DataType::INT_TYPE;
    signature["spawn location"] = DataType::AREA_PTR;
    signature["maximum special points"] = DataType::INT_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["money"] = DataType::INT_TYPE;
    signature["current location"] = DataType::AREA_PTR;
    signature["maximum inventory weight"] = DataType::INT_TYPE;

    return signature;
}

}}