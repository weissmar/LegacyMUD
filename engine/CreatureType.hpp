/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        CreatureType.hpp
 *
 * \details     Header file for CreatureType class. Defines the members and 
 *              functions that apply to all creature types. Creature types 
 *              define in-game creature types that a creature may use as its 
 *              type. Creature types allow the world builder to define a type  
 *              once and then create many creatures of that type.
 ************************************************************************/

#ifndef CREATURE_TYPE_HPP
#define CREATURE_TYPE_HPP

#include <string>
#include <atomic>
#include "CombatantType.hpp"
#include "CharacterSize.hpp"
#include "DataType.hpp"
#include "XPTier.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

/*!
 * \details     Creature types define in-game creature types that a creature 
 *              may use as its type. Creature types allow the world builder   
 *              to define a typeonce and then create many creatures of that 
 *              type.
 */
class CreatureType: public CombatantType {
    public:
        CreatureType();
        CreatureType(CharacterSize size, XPTier difficulty, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints);

        /*!
         * \brief   Gets the size of this creature type.
         *
         * \return  Returns a CharacterSize with the size of this creature type.
         */
        CharacterSize getSize() const;

        /*!
         * \brief   Gets the difficulty of this creature type.
         *
         * \return  Returns a XPTier with the difficulty of this creature type.
         */
        XPTier getDifficulty() const;

        /*!
         * \brief   Sets the size of this creature type.
         * 
         * \param[in] size  Specifies the size of this creature type.
         *
         * \return  Returns a bool indicating whether or not setting the size 
         *          was successful.
         */
        bool setSize(CharacterSize size);

        /*!
         * \brief   Sets the difficulty of this creature type.
         *
         * \param[in] difficulty    Specifies the difficulty of this creature type.
         *
         * \return  Returns a bool indicating whether or not setting the difficulty 
         *          was successful.
         */
        bool setDifficulty(XPTier difficulty);

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
         * \brief   Deserializes this object after reading from file.
         * 
         * \param[in] string    Holds the data to be deserialized.
         *
         * \return  Returns a bool indicating whether or not deserializing
         *          the string into an Action succeeded.
         */
        virtual bool deserialize(std::string);

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
        std::atomic<CharacterSize> size;
        std::atomic<XPTier> difficulty;
};

}}

#endif