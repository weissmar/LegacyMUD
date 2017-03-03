/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    03/02/2017
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


Combatant::Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, int dexterity, int strength, int intelligence, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID)
: Character(name, description, money, aLocation, maxInventoryWeight, anID)
, cooldownClock(0)
, health(std::make_pair(maxHealth, maxHealth))
, spawnLocation(spawnLocation)
, specialPoints(std::make_pair(maxSpecialPts, maxSpecialPts))
, dexterity(dexterity)
, strength(strength)
, intelligence(intelligence)
, inCombat(nullptr) {
    rollStats();
}

/*Combatant::Combatant(const Combatant &otherCombatant){

}


Combatant & Combatant::operator=(const Combatant &otherCombatant){

}


Combatant::~Combatant(){

}*/


bool Combatant::cooldownIsZero() const{
    return cooldownClock.load() <= std::time(nullptr);
}


int Combatant::getCurrentHealth() const{
    std::lock_guard<std::mutex> healthLock(healthMutex);
    return health.first;
}


int Combatant::getMaxHealth() const{
    std::lock_guard<std::mutex> healthLock(healthMutex);
    return health.second;
}


Area* Combatant::getSpawnLocation() const{
    std::lock_guard<std::mutex> spawnLocationLock(spawnLocationMutex);
    return spawnLocation;
}


int Combatant::getCurrentSpecialPts() const{
    std::lock_guard<std::mutex> specialPointsLock(specialPointsMutex);
    return specialPoints.first;
}


int Combatant::getMaxSpecialPts() const{
    std::lock_guard<std::mutex> specialPointsLock(specialPointsMutex);
    return specialPoints.second;
}


int Combatant::getDexterity() const{
    return dexterity.load();
}


int Combatant::getStrength() const{
    return strength.load();
}


int Combatant::getIntelligence() const{
    return intelligence.load();
}


Combatant* Combatant::getInCombat() const{
    std::lock_guard<std::mutex> inCombatLock(inCombatMutex);
    return inCombat;
}


bool Combatant::setCooldown(time_t cooldown){
    cooldownClock.store(std::time(nullptr) + cooldown);
    return true;
}

/*
bool Combatant::decrementCooldown(){
    cooldownClock--;
    return true;
}*/


bool Combatant::setInCombat(Combatant *aCombatant){
    if (aCombatant != nullptr){
        std::lock_guard<std::mutex> inCombatLock(inCombatMutex);
        inCombat = aCombatant;
        return true;
    }
    return false;
}


bool Combatant::setMaxHealth(int maxHealth){
    std::lock_guard<std::mutex> healthLock(healthMutex);
    health.second = maxHealth;
    return true;
}


int Combatant::addToCurrentHealth(int healing){
    std::lock_guard<std::mutex> healthLock(healthMutex);

    health.first += healing;
    if (health.first > health.second){
        health.first = health.second;
    }
    return health.first;
}


int Combatant::subtractFromCurrentHealth(int damage){
    std::lock_guard<std::mutex> healthLock(healthMutex);
    health.first -= damage;
    if (health.first < 0){
        health.first = 0;
    }
    return health.first;
}


bool Combatant::setSpawnLocation(Area *spawnLocation){
    if (spawnLocation != nullptr){
        std::lock_guard<std::mutex> spawnLocationLock(spawnLocationMutex);
        this->spawnLocation = spawnLocation;
        return true;
    }
    return false;
}


bool Combatant::setMaxSpecialPts(int maxSpecialPts){
    std::lock_guard<std::mutex> specialPointsLock(specialPointsMutex);
    specialPoints.second = maxSpecialPts;
    return true;
}


int Combatant::addToCurrentSpecialPts(int gainedPoints){
    std::lock_guard<std::mutex> specialPointsLock(specialPointsMutex);

    specialPoints.first += gainedPoints;
    if (specialPoints.first > specialPoints.second){
        specialPoints.first = specialPoints.second;
    }
    return specialPoints.first;
}


int Combatant::subtractFromCurrSpecialPts(int usedPoints){
    std::lock_guard<std::mutex> specialPointsLock(specialPointsMutex);
    specialPoints.first -= usedPoints;
    if (specialPoints.first < 0){
        specialPoints.first = 0;
    }
    return specialPoints.first;
}


bool Combatant::rollStats(){
    dexterity.store(0);
    strength.store(0);
    intelligence.store(0);

    while ((dexterity + strength + intelligence) != 36){
        dexterity.store((std::rand() % 18) + 1);
        strength.store((std::rand() % 18) + 1);
        intelligence.store((std::rand() % 18) + 1);
    }

    return true;
}


bool Combatant::respawn(){
    return this->setLocation(spawnLocation);
}


std::string Combatant::serialize(){
    return "";
}


int Combatant::increaseDexterity(int dexPoints){
    dexterity += dexPoints;

    return dexterity.load();
}


int Combatant::increaseStrength(int strengthPoints){
    strength += strengthPoints;

    return strength.load();
}


int Combatant::increaseIntelligence(int intPoints){
    intelligence += intPoints;

    return intelligence.load();
}

}}