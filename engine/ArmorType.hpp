/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        ArmorType.hpp
 *
 * \details     Header file for ArmorType class. Defines the members and 
 *              functions that apply to all armor types. Armor types define
 *              in-game armor types that an item may use as its type. Armor
 *              types allow the world builder to define a type once and 
 *              then create many items of that type.
 ************************************************************************/

#ifndef ARMOR_TYPE_HPP
#define ARMOR_TYPE_HPP

#include <string>
#include <mutex>
#include <atomic>
#include <vector>
#include "ItemType.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

/*!
 * \details     This class defines in-game armor types that an item may use 
 *              as its type. Armor types allow the world builder to define 
 *              a type once and then create many items of that type.
 */
class ArmorType: public ItemType {
    public:
        ArmorType();
        ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);
        ArmorType(int bonus, DamageType resistantTo, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType, int anID);

        /*!
         * \brief   Gets the armor bonus for this armor type.
         *
         * \return  Returns an int with the armor bonus for this armor type.
         */
        virtual int getArmorBonus() const;

        /*!
         * \brief   Gets the damage type this armor type is resistant to.
         *
         * \return  Returns a DamageType with the damage type this armor type is 
         *          resistant to.
         */
        virtual DamageType getResistantTo() const;

        /*!
         * \brief   Sets the armor bonus for this armor type.
         *
         * \param[in] bonus     Specifes the armor bonus for this armor type.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          armor bonus was successful.
         */
        virtual bool setArmorBonus(int bonus);

        /*!
         * \brief   Sets the damage type that this armor type is resistant to.
         *
         * \param[in] resistance    Specifies the damage type this armor type
         *                          is resistant to.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          resistance was successful.
         */
        virtual bool setResistantTo(DamageType resistance);

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
        std::atomic<int> armorBonus;
        std::atomic<DamageType> resistantTo;
};

}}

#endif