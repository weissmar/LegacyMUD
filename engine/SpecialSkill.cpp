/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    03/03/2017
 * \course      CS467, Winter 2017
 * \file        SpecialSkill.cpp
 *
 * \details     Implementation file for SpecialSkill class. 
 ************************************************************************/

#include "SpecialSkill.hpp"
#include "EffectType.hpp"
#include "CommandEnum.hpp"

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
    return "";
}


SpecialSkill* SpecialSkill::deserialize(std::string){
    return nullptr; 
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