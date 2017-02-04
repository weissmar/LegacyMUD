/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      Combatant.hpp
 *
 * Overview:
 *     Header file for Combatant base class. Defines the members and 
 * functions that apply to all combatants.
 ************************************************************************/

#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include <tuple>
#include <string>
#include "Character.hpp"
#include "Area.hpp"

namespace legacymud { namespace engine {

class Combatant: public Character {
    public:
        Combatant();
        Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Combatant(const Combatant &otherCombatant);
        Combatant & operator=(const Combatant &otherCombatant);
        virtual ~Combatant();
        bool cooldownIsZero();
        int getCurrentHealth();
        int getMaxHealth();
        Area* getSpawnLocation();
        int getCurrentSpecialPts();
        int getMaxSpecialPts();
        int getDexterity();
        int getStrength();
        int getIntelligence();
        Combatant* getInCombat();
        bool setCooldown(int cooldown);
        bool decrementCooldown();
        bool setInCombat(Combatant *aCombatant);
        bool setMaxHealth(int maxHealth);
        int addToCurrentHealth(int healing);
        int subtractFromCurrentHealth(int damage);
        bool setSpawnLocation(Area *spawnLocation);
        bool setMaxSpecialPts(int maxSpecialPts);
        int addToCurrentSpecialPts(int gainedPoints);
        int subtractFromCurrSpecialPts(int usedPoints);
        bool rollStats();
        bool respawn();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
    protected:
        int increaseDexterity(int dexPoints);
        int increaseStrength(int strengthPoints);
        int increaseIntelligence(int intPoints);
    private:
        int cooldownClock;
        std::tuple<int, int> health;
        Area* spawnLocation;
        std::tuple<int, int> specialPoints;
        int dexterity;
        int strength;
        int intelligence;
        Combatant* inCombat;
};

}}

#endif