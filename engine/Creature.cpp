/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/26/2017
 * \course      CS467, Winter 2017
 * \file        Creature.cpp
 *
 * \details     Implementation file for Creature class. 
 ************************************************************************/

#include "Creature.hpp"
#include "Area.hpp"
#include "CreatureType.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

Creature::Creature()
: Combatant()
, type(nullptr)
, ambulatory (false)
{ }


Creature::Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, name, description, money, aLocation, maxInventoryWeight)
, type(aType)
, ambulatory (ambulatory)
{ }


/*Creature::Creature(const Creature &otherCreature){

}


Creature & Creature::operator=(const Creature &otherCreature){

} 


Creature::~Creature(){

}*/


CreatureType* Creature::getType() const{
    std::lock_guard<std::mutex> typeLock(typeMutex);
    return type;
}


bool Creature::getAmbulatory() const{
    return ambulatory.load();
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
        anArea->registerAlias(false, alias, this);
        success = InteractiveNoun::addNounAlias(alias);
    }

    return success;
}


bool Creature::removeNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(false, alias, this);
        success = InteractiveNoun::removeNounAlias(alias);
    }

    return success;
}


bool Creature::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(true, alias, this);
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
    }

    return success;
}


bool Creature::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(true, alias, this);
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
    }

    return success;
}


ObjectType Creature::getObjectType() const{
    return ObjectType::CREATURE;
}


std::string Creature::serialize(){
    return "";
}


bool Creature::deserialize(std::string){
    return false;
}


std::string Creature::look(std::vector<EffectType> *effects){
    return "";
}  


std::string Creature::take(Player* aPlayer, Item* anItem, InteractiveNoun* aContainer, InteractiveNoun* aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;
    bool success;

    success = addToInventory(anItem);
    if (success){
        message = getTextAndEffect(CommandEnum::TAKE, anEffect);
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
        message += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::unequip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;
    bool success = false;

    if (anItem != nullptr){
        success = unequipItem(anItem);
    }

    if (success){
        message += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Creature::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    return "";
}


std::string Creature::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Creature::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    return "";
}


std::string Creature::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill, std::vector<EffectType> *effects){
    return "";
} 


InteractiveNoun* Creature::copy(){
    return nullptr;
}


bool Creature::editAttribute(Player*, std::string){
    return false;
}


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