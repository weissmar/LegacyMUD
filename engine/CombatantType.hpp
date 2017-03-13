/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        CombatantType.hpp
 *
 * \details     Header file for CombatantType base class. Defines the members 
 *              and functions that apply to all combatant types. This class
 *              should not be instantiated.
 ************************************************************************/

#ifndef COMBATANT_TYPE_HPP
#define COMBATANT_TYPE_HPP

#include <string>
#include <mutex>
#include <atomic>
#include "InteractiveNoun.hpp"
#include "DamageType.hpp"

namespace legacymud { namespace engine {

class SpecialSkill;

/*!
 * \details     This class defines the inheritable characteristics that define 
 *              all in-game combatant types. This class should not be instantiated.
 */
class CombatantType: public InteractiveNoun {
    public:
        CombatantType();
        CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);
        CombatantType(std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints, int anID);
        /*CombatantType(const CombatantType &otherCombatantType);
        CombatantType & operator=(const CombatantType &otherCombatantType);
        virtual ~CombatantType();*/

        /*!
         * \brief   Gets the name of this combatant type.
         *
         * \return  Returns a std::string with the name of this combatant type.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the special skill of this combatant type.
         *
         * \return  Returns a SpecialSkill* indicating the special skill of this 
         *          combatant type.
         */
        SpecialSkill* getSpecialSkill() const;

        /*!
         * \brief   Gets the attack bonus of this combatant type.
         * 
         * \param[in] level     Specifies the level of the combatant.
         *
         * \return  Returns an int with the attack bonus of this combatant type
         *          for the given level.
         */
        int getAttackBonus(int level = 0) const;

        /*!
         * \brief   Gets the armor bonus of this combatant type.
         * 
         * \param[in] level     Specifies the level of the combatant.
         *
         * \return  Returns an int indicating the armor bonus of this combatant 
         *          type for the given level.
         */
        int getArmorBonus(int level = 0) const;

        /*!
         * \brief   Gets the damage type that this combatant type is resistant to.
         *
         * \return  Returns a DamageType indicating the damage type that this 
         *          combatant type is resistant to.
         */
        DamageType getResistantTo() const;

        /*!
         * \brief   Gets the damage type that this combatant type is weak to.
         *
         * \return  Returns a DamageType indicating the damage type that this 
         *          combatant type is weak to.
         */
        DamageType getWeakTo() const;

        /*!
         * \brief   Gets the heal points for this combatant type.
         *
         * \return  Returns a float indicating the heal points for this combatant
         *          type.
         */
        float getHealPoints() const;

        /*!
         * \brief   Sets the name of this combatant type.
         *
         * \param[in] name  Specifies the name of this combatant type.
         *
         * \return  Returns a bool indicating whether or not the name was set
         *          successfully.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the special skill for this combatant type.
         *
         * \param[in] skill     Specifies the special skill for this combatant type.
         *
         * \return  Returns a bool indicating whether or not the special skill was 
         *          set successfully.
         */
        bool setSpecialSkill(SpecialSkill* skill);

        /*!
         * \brief   Sets the attack bonus for this combatant type.
         *
         * \param[in] bonus     Specifies the attack bonus for this combatant type.
         *
         * \return  Returns a bool indicating whether or not the attack bonus was 
         *          set successfully.
         */
        bool setAttackBonus(int bonus);

        /*!
         * \brief   Sets the armor bonus for this combatant type.
         *
         * \param[in] bonus     Specifies the armor bonus for this combatant type.
         *
         * \return  Returns a bool indicating whether or not the armor bonus was 
         *          set successfully.
         */
        bool setArmorBonus(int bonus);

        /*!
         * \brief   Sets the damage type that this combatant type is resistant to.
         *
         * \param[in] type  Specifies the damage type that this combatant type is
         *                  resistant to.
         *
         * \return  Returns a bool indicating whether or not the resistance was 
         *          set successfully.
         */
        bool setResistantTo(DamageType type);

        /*!
         * \brief   Sets the damage type that this combatant type is weak to.
         *
         * \param[in] type  Specifies the damage type that this combatant type is
         *                  weak to.
         *
         * \return  Returns a bool indicating whether or not the weakness was 
         *          set successfully.
         */
        bool setWeakTo(DamageType type);

        /*!
         * \brief   Sets the heal points for this combatant type.
         *
         * \param[in] type  Specifies the heal points for this combatant type.
         *
         * \return  Returns a bool indicating whether or not the heal points were
         *          set successfully.
         */
        bool setHealPoints(float healPoints);

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();
    private:
        std::string name;
        mutable std::mutex nameMutex;
        SpecialSkill* specialSkill;
        mutable std::mutex specialSkillMutex;
        std::atomic<int> attackBonus;
        std::atomic<int> armorBonus;
        std::atomic<DamageType> resistantTo;
        std::atomic<DamageType> weakTo;
        std::atomic<float> healPoints;
};

}}

#endif