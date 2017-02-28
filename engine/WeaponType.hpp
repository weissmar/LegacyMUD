/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/27/2017
 * \course      CS467, Winter 2017
 * \file        WeaponType.hpp
 *
 * \details     Header file for WeaponType class. Defines the members and 
 *              functions that apply to all weapon types. Weapon types 
 *              define in-game weapon types that an item may use as its 
 *              type. Weapon types allow the world builder to define a type  
 *              once and then create many items of that type.
 ************************************************************************/

#ifndef WEAPON_TYPE_HPP
#define WEAPON_TYPE_HPP

#include <string>
#include <atomic>
#include <vector>
#include "ItemType.hpp"
#include "ItemRarity.hpp"
#include "EquipmentSlot.hpp"
#include "DamageType.hpp"
#include "AreaSize.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

/*!
 * \details     This class defines in-game weapon types that an item may use 
 *              as its type. Weapon types allow the world builder to define a   
 *              type once and then create many items of that type.
 */
class WeaponType: public ItemType {
    public:
        WeaponType();
        WeaponType(int damage, DamageType type, AreaSize range, int critMultiplier, int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);

        /*!
         * \brief   Gets the damage of this weapon type.
         *
         * \return  Returns an int with the damage for this weapon type.
         */
        virtual int getDamage() const;

        /*!
         * \brief   Gets the damage type of this weapon type. 
         *
         * \return  Returns a DamageType with the damage type for this weapon type.
         */
        virtual DamageType getDamageType() const;

        /*!
         * \brief   Gets the range of this weapon type.
         *
         * \return  Returns an AreaSize with the range for this weapon type.
         */
        virtual AreaSize getRange() const;

        /*!
         * \brief   Gets the crit multiplier of this weapon type.
         *
         * \return  Returns an int with the crit multiplier for this weapon type.
         */
        virtual int getCritMultiplier() const;

        /*!
         * \brief   Sets the damage of this weapon type.
         *
         * \param[in] damage    Specifies the damage.
         *
         * \return  Returns a bool indicating whether or not setting the damage was
         *          sucessful.
         */
        virtual bool setDamage(int damage);

        /*!
         * \brief   Sets the damage type of this weapon type. 
         *
         * \param[in] type  Specifies the damage type.
         *
         * \return  Returns a bool indicating whether or not setting the damage type was
         *          sucessful.
         */
        virtual bool setDamageType(DamageType type);

        /*!
         * \brief   Sets the range of this weapon type.
         *
         * \param[in] range    Specifies the range.
         *
         * \return  Returns a bool indicating whether or not setting the range was
         *          sucessful.
         */
        virtual bool setRange(AreaSize range);

        /*!
         * \brief   Sets the crit multiplier of this weapon type.
         *
         * \param[in] multiplier    Specifies the crit multiplier.
         *
         * \return  Returns a bool indicating whether or not setting the crit multiplier 
         *          was sucessful.
         */
        virtual bool setCritMultiplier(int multiplier);
        
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
        std::atomic<int> damage;
        std::atomic<DamageType> damageType;
        std::atomic<AreaSize> range;
        std::atomic<int> critMultiplier;
};

}}

#endif