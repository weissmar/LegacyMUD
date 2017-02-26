/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/25/2017
 * \course      CS467, Winter 2017
 * \file        SpecialSkill.cpp
 *
 * \details     Implementation file for SpecialSkill class. 
 ************************************************************************/

#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

SpecialSkill::SpecialSkill()
: InteractiveNoun()
, name("")
, damage(0)
, damageType(DamageType::NONE)
, cost(0)
, cooldown(0)
{ }


SpecialSkill::SpecialSkill(std::string name, int damage, DamageType type, int cost, int cooldown)
: InteractiveNoun()
, name(name)
, damage(damage)
, damageType(type)
, cost(cost)
, cooldown(cooldown)
{
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


int SpecialSkill::getCooldown() const{
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


bool SpecialSkill::setCooldown(int cooldown){
    this->cooldown.store(cooldown);

    return true;
}


ObjectType SpecialSkill::getObjectType() const{
    return ObjectType::SPECIAL_SKILL;
}


std::string SpecialSkill::serialize(){
    return "";
}


bool SpecialSkill::deserialize(std::string){
    return false;
}


std::string SpecialSkill::more(std::vector<EffectType> *effects){
    return "";
} 


std::string SpecialSkill::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    return "";
}


std::string SpecialSkill::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill, std::vector<EffectType> *effects){
    return "";
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