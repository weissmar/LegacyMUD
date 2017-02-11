/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        Combatant.cpp
 *
 * \details     Implementation file for Combatant base class. 
 ************************************************************************/

#include "Combatant.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

Combatant::Combatant(){

}


Combatant::Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight){

}


Combatant::Combatant(const Combatant &otherCombatant){

}


Combatant & Combatant::operator=(const Combatant &otherCombatant){

}


Combatant::~Combatant(){

}


bool Combatant::cooldownIsZero(){

}


int Combatant::getCurrentHealth(){

}


int Combatant::getMaxHealth(){

}


Area* Combatant::getSpawnLocation(){

}


int Combatant::getCurrentSpecialPts(){

}


int Combatant::getMaxSpecialPts(){

}


int Combatant::getDexterity(){

}


int Combatant::getStrength(){

}


int Combatant::getIntelligence(){

}


Combatant* Combatant::getInCombat(){

}


bool Combatant::setCooldown(int cooldown){

}


bool Combatant::decrementCooldown(){

}


bool Combatant::setInCombat(Combatant *aCombatant){

}


bool Combatant::setMaxHealth(int maxHealth){

}


int Combatant::addToCurrentHealth(int healing){

}


int Combatant::subtractFromCurrentHealth(int damage){

}


bool Combatant::setSpawnLocation(Area *spawnLocation){

}


bool Combatant::setMaxSpecialPts(int maxSpecialPts){

}


int Combatant::addToCurrentSpecialPts(int gainedPoints){

}


int Combatant::subtractFromCurrSpecialPts(int usedPoints){

}


bool Combatant::rollStats(){

}


bool Combatant::respawn(){

}


std::string Combatant::serialize(){

}


bool Combatant::deserialize(std::string){

}


int Combatant::increaseDexterity(int dexPoints){

}


int Combatant::increaseStrength(int strengthPoints){

}


int Combatant::increaseIntelligence(int intPoints){

}

}}