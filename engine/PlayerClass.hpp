/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/28/2017
 * \course      CS467, Winter 2017
 * \file        PlayerClass.hpp
 *
 * \details     Header file for PlayerClass class. Defines the members and 
 *              functions that apply to all player classes. PlayerClass
 *              defines in-game player classes that a player may choose 
 *              as their class. PlayerClass allows the world builder to 
 *              define a class once and then create many players of that
 *              class.
 ************************************************************************/

#ifndef PLAYER_CLASS_HPP
#define PLAYER_CLASS_HPP

#include <string>
#include <atomic>
#include "CombatantType.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class SpecialSkill;

/*!
 * \details     PlayerClass defines in-game player classes that a player may  
 *              choose as their class. PlayerClass allows the world builder 
 *              to define a class once and then create many players of that
 *              class.
 */
class PlayerClass: public CombatantType {
    public:
        PlayerClass();
        PlayerClass(int primaryStat, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);

        /*!
         * \brief   Gets the primary stat for this player class.
         *
         * \return  Returns an int indicating the primary stat (0 = dexterity, 
         *          1 = intelligence, 2 = strength).
         */
        int getPrimaryStat() const;

        /*!
         * \brief   Sets the primary stat for this player class.
         * 
         * \param[in] stat  Specifies the primary stat for this player class
         *                  (0 = dexterity, 1 = intelligence, 2 = strength).
         *
         * \return  Returns a bool indicating whether or not the primary stat
         *          was set successfully.
         */
        bool setPrimaryStat(int stat);

        /*!
         * \brief   Gets the object type.
         *
         * \return  Returns an ObjectType indicating the actual class the object
         *          belongs to.
         */
        virtual ObjectType getObjectType() const;

        /*!
         * \brief   Serializes this object for writing to file.
         *
         * \return  Returns a std::string with the serialized data.
         */
        virtual std::string serialize();

        /*!
         * \brief   Deserializes and creates an object of this type from the
         *          specified string of serialized data.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static InteractiveNoun* deserialize(std::string);

        /*!
         * \brief   Creates a copy of this object.
         *
         * This function creates a new object with the same attributes as this
         * object and returns a pointer to the new object.
         *
         * \return  Returns a pointer to the newly created object or nullptr if 
         *          the copy was unsuccessful.
         */
        virtual InteractiveNoun* copy();

        /*!
         * \brief   Edits an attribute of this object.
         *
         * This function edits the specified attribute of this object. It asks 
         * the user for the new value and then sets it to that.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         * \param[in] attribute Specifies the attribute to edit.
         *
         * \return  Returns bool indicating whether or not editing the attribute
         *          was successful.
         */
        virtual bool editAttribute(Player *aPlayer, std::string attribute);

        /*!
         * \brief   Edits  this object.
         *
         * This function edits this object. It interacts with the user to determine
         * which attributes to update and what the new values should be and then
         * makes those changes.
         *
         * \param[in] aPlayer   Specifies the player that is doing the editing.
         *
         * \return  Returns bool indicating whether or not editing this object
         *          was successful.
         */
        virtual bool editWizard(Player *aPlayer);

        /*!
         * \brief   Gets the attribute signature of the class.
         *
         * This function returns a map of string to DataType with the 
         * attributes required by the Action class to instantiate a new
         * action.
         * 
         * \return  Returns a map of std::string to DataType indicating 
         *          the required attributes.
         */
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::atomic<int> primaryStat;
};

}}

#endif