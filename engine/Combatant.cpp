/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/11/2017
 * \course      CS467, Winter 2017
 * \file        Combatant.cpp
 *
 * \details     Implementation file for Combatant base class. 
 ************************************************************************/

#include <cstdlib>
#include "Combatant.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

const int BEGIN_MAX_HEALTH = 10;
const int BEGIN_MAX_SPECIAL = 10;

Combatant::Combatant()
: Character()
, cooldownClock(0)
, health(std::make_pair(BEGIN_MAX_HEALTH, BEGIN_MAX_HEALTH))
, spawnLocation(nullptr)
, specialPoints(std::make_pair(BEGIN_MAX_SPECIAL, BEGIN_MAX_SPECIAL))
, inCombat(nullptr) {
    rollStats();
}


Combatant::Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Character(name, description, money, aLocation, maxInventoryWeight)
, cooldownClock(0)
, health(std::make_pair(maxHealth, maxHealth))
, spawnLocation(spawnLocation)
, specialPoints(std::make_pair(maxSpecialPts, maxSpecialPts))
, inCombat(nullptr) {
    rollStats();
}


/*Combatant::Combatant(const Combatant &otherCombatant){

}


Combatant & Combatant::operator=(const Combatant &otherCombatant){

}


Combatant::~Combatant(){

}*/


bool Combatant::cooldownIsZero(){
    return cooldownClock == 0;
}


int Combatant::getCurrentHealth(){
    return health.first;
}


int Combatant::getMaxHealth(){
    return health.second;
}


Area* Combatant::getSpawnLocation(){
    return spawnLocation;
}


int Combatant::getCurrentSpecialPts(){
    return specialPoints.first;
}


int Combatant::getMaxSpecialPts(){
    return specialPoints.second;
}


int Combatant::getDexterity(){
    return dexterity;
}


int Combatant::getStrength(){
    return strength;
}


int Combatant::getIntelligence(){
    return intelligence;
}


Combatant* Combatant::getInCombat(){
    return inCombat;
}


bool Combatant::setCooldown(int cooldown){
    return false;
}


bool Combatant::decrementCooldown(){
    return false;
}


bool Combatant::setInCombat(Combatant *aCombatant){
    return false;
}


bool Combatant::setMaxHealth(int maxHealth){
    return false;
}


int Combatant::addToCurrentHealth(int healing){
    return health.first;
}


int Combatant::subtractFromCurrentHealth(int damage){
    return health.first;
}


bool Combatant::setSpawnLocation(Area *spawnLocation){
    return false;
}


bool Combatant::setMaxSpecialPts(int maxSpecialPts){
    return false;
}


int Combatant::addToCurrentSpecialPts(int gainedPoints){
    return specialPoints.first;
}


int Combatant::subtractFromCurrSpecialPts(int usedPoints){
    return specialPoints.first;
}


bool Combatant::rollStats(){
    dexterity = 0;
    strength = 0;
    intelligence = 0;

    while ((dexterity + strength + intelligence) != 36){
        dexterity = (std::rand() % 18) + 1;
        strength = (std::rand() % 18) + 1;
        intelligence = (std::rand() % 18) + 1;
    }

    return true;
}


bool Combatant::respawn(){
    return false;
}


std::string Combatant::serialize(){
    return "";
}


bool Combatant::deserialize(std::string){
    return false;
}


int Combatant::increaseDexterity(int dexPoints){
    dexterity += dexPoints;

    return dexterity;
}


int Combatant::increaseStrength(int strengthPoints){
    strength += strengthPoints;

    return strength;
}


int Combatant::increaseIntelligence(int intPoints){
    intelligence += intPoints;

    return intelligence;
}

}}