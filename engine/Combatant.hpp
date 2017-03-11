/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/10/2017
 * \course      CS467, Winter 2017
 * \file        Combatant.hpp
 *
 * \details     Header file for Combatant base class. Defines the members  
 *              and functions that apply to all combatants. This class 
 *              should not be instantiated.
 ************************************************************************/

#ifndef COMBATANT_HPP
#define COMBATANT_HPP

#include <ctime>
#include <utility>
#include <string>
#include <map>
#include <atomic>
#include <mutex>
#include "Character.hpp"

namespace legacymud { namespace engine {

class Area;

/*!
 * \details     This class defines the inheritable characteristics that define 
 *              all in-game combatants. This class should not be instantiated.
 */
class Combatant: public Character {
    public:
        Combatant();
        Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        Combatant(int maxHealth, Area *spawnLocation, int maxSpecialPts, int dexterity, int strength, int intelligence, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID);
        Combatant(const Combatant &otherCombatant);
        Combatant & operator=(const Combatant &otherCombatant);
        //virtual ~Combatant();

        /*!
         * \brief   Gets whether the cooldown period for this combatant is over.
         *
         * \return  Returns a bool indicating if the cooldown period is over.
         */
        bool cooldownIsZero() const;

        /*!
         * \brief   Gets the current health of this combatant.
         *
         * \return  Returns an int with the current health of this combatant.
         */
        int getCurrentHealth() const;

        /*!
         * \brief   Gets the maximum health of this combatant.
         *
         * \return  Returns an int with the maximum health of this combatant.
         */
        int getMaxHealth() const;

        /*!
         * \brief   Gets the spawn location for this combatant.
         *
         * \return  Returns an Area* with the spawn location for this combatant.
         */
        Area* getSpawnLocation() const;

        /*!
         * \brief   Gets the current number of special points for this combatant.
         *
         * \return  Returns an int with the current number of special points for 
         *          this combatant.
         */
        int getCurrentSpecialPts() const;

        /*!
         * \brief   Gets the maximum number of special points for this combatant.
         *
         * \return  Returns an int with the maximum number of special points for 
         *          this combatant.
         */
        int getMaxSpecialPts() const;

        /*!
         * \brief   Gets the dexterity of this combatant.
         *
         * \return  Returns an int with the dexterity of this combatant.
         */
        int getDexterity() const;

        /*!
         * \brief   Gets the strength of this combatant.
         *
         * \return  Returns an int with the strength of this combatant.
         */
        int getStrength() const;

        /*!
         * \brief   Gets the intelligence of this combatant.
         *
         * \return  Returns an int with the intelligence of this combatant.
         */
        int getIntelligence() const;

        /*!
         * \brief   Gets the whether this combatant is currently in combat.
         *
         * \return  Returns a Combatant* which points to the combatant this combatant
         *          is currenlty in combat with, or a nullptr if this combatant is 
         *          not currently in combat.
         */
        Combatant* getInCombat() const;

        virtual int getIntelligenceModifier() const;
        virtual int getStrengthModifier() const;
        virtual int getDexterityModifier() const;
        virtual int getSizeModifier() const { return 0; }
        std::vector<Item*> getWeapons() const;

        /*!
         * \brief   Sets the cooldown in seconds for this combatant.
         *
         * \param[in] cooldown  Specifies the number of seconds until this 
         *                      combatant can perform another action.
         *
         * \return  Returns a bool indicating whether or not the cooldown was set
         *          successfully.
         */
        bool setCooldown(time_t cooldown);

        /*!
         * \brief   Decrements the cooldown clock by one for this combatant.
         *
         * \return  Returns a bool indicating whether or not the cooldown was 
         *          decremented successfully.
         */
        //bool decrementCooldown();

        /*!
         * \brief   Sets whether or not this combatant is currently in combat.
         *
         * \param[in] aCombatant    Specifies the combatant with which this combatant
         *                          is currently in combat (or nullptr if not currently
         *                          in combat).
         *
         * \return  Returns a bool indicating whether or not setting this combatant's
         *          combat status was successful.
         */
        bool setInCombat(Combatant *aCombatant);

        /*!
         * \brief   Sets the maximum health for this combatant.
         *
         * \param[in] maxHealth     Specifies the maximum health for this combatant.
         *
         * \return  Returns a bool indicating whether or not the maximum health was 
         *          set successfully.
         */
        bool setMaxHealth(int maxHealth);

        /*!
         * \brief   Adds to the current health for this combatant.
         *
         * \param[in] healing   Specifies the amount to add to the current health.
         *
         * \return  Returns an int with the updated current health.
         */
        int addToCurrentHealth(int healing);

        /*!
         * \brief   Subtracts from the current health for this combatant.
         *
         * \param[in] healing   Specifies the amount to subtract from the current 
         *                      health.
         *
         * \return  Returns an int with the updated current health.
         */
        int subtractFromCurrentHealth(int damage);

        /*!
         * \brief   Sets the spawn location for this combatant.
         *
         * \param[in] spawnLocation     Specifies spawn location.
         *
         * \return  Returns a bool indicating whether or not the spawn location
         *          was set successfully.
         */
        bool setSpawnLocation(Area *spawnLocation);

        /*!
         * \brief   Sets the maximum special points for this combatant.
         *
         * \param[in] maxSpecialPts     Specifies the maximum special points.
         *
         * \return  Returns a bool indicating whether or not the maximum special
         *          points for this combatant was set successfully.
         */
        bool setMaxSpecialPts(int maxSpecialPts);

        /*!
         * \brief   Adds to the current special points for this combatant.
         *
         * \param[in] gainedPoints  Specifies amount to add to the current special
         *                          points.
         *
         * \return  Returns an int with the updated current special points.
         */
        int addToCurrentSpecialPts(int gainedPoints);

        /*!
         * \brief   Subtracts from the current special points for this combatant.
         *
         * \param[in] usedPoints    Specifies amount to subtract from the current 
         *                          special points.
         *
         * \return  Returns an int with the updated current special points.
         */
        int subtractFromCurrSpecialPts(int usedPoints);

        /*!
         * \brief   Sets the dexterity, strength, and intelligence for this
         *          combatant based on random "dice rolls".
         *
         * \return  Returns a bool indicating whether or not the stats were
         *          set successfully.
         */
        bool rollStats();

        /*!
         * \brief   Respawns this combatant after in-game death.
         *
         * \return  Returns a bool indicating whether or not this combatant
         *          was successfully respawned.
         */
        bool respawn();

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();
    protected:
        /*!
         * \brief   Adds to the current dexterity of this combatant.
         *
         * \param[in] dexPoints     Specifies amount to add to dexterity.
         *
         * \return  Returns an int with the updated dexterity.
         */
        int increaseDexterity(int dexPoints);

        /*!
         * \brief   Adds to the current strength of this combatant.
         *
         * \param[in] strengthPoints    Specifies amount to add to strength.
         *
         * \return  Returns an int with the updated strength.
         */
        int increaseStrength(int strengthPoints);

        /*!
         * \brief   Adds to the current intelligence of this combatant.
         *
         * \param[in] intPoints     Specifies amount to add to intelligence.
         *
         * \return  Returns an int with the updated intelligence.
         */
        int increaseIntelligence(int intPoints);
    private:
        std::atomic<time_t> cooldownClock;
        std::pair<int, int> health;
        mutable std::mutex healthMutex;
        Area* spawnLocation;
        mutable std::mutex spawnLocationMutex;
        std::pair<int, int> specialPoints;
        mutable std::mutex specialPointsMutex;
        std::atomic<int> dexterity;
        std::atomic<int> strength;
        std::atomic<int> intelligence;
        Combatant* inCombat;
        mutable std::mutex inCombatMutex;
        static std::map<int, int> skillModMap;
};

}}

#endif