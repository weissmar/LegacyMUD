/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        ItemType.hpp
 *
 * \details     Header file for ItemType class. Defines the members and 
 *              functions that apply to all item types. Item types 
 *              define in-game item types that an item may use as its 
 *              type. Item types allow the world builder to define a type  
 *              once and then create many items of that type.
 ************************************************************************/

#ifndef ITEM_TYPE_HPP
#define ITEM_TYPE_HPP

#include <string>
#include <atomic>
#include <mutex>
#include "InteractiveNoun.hpp"
#include "EquipmentSlot.hpp"
#include "ItemRarity.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "DamageType.hpp"
#include "AreaSize.hpp"

namespace legacymud { namespace engine {

/*!
 * \details     This class defines in-game item types that an item may use as  
 *              its type. Item types allow the world builder to define a type  
 *              once and then create many items of that type.
 */
class ItemType: public InteractiveNoun {
    public:
        ItemType();
        ItemType(int weight, ItemRarity rarity, std::string description, std::string name, int cost, EquipmentSlot slotType);

        /*!
         * \brief   Gets the weight of this item type.
         *
         * \return  Returns an int with the weight of this item type.
         */
        int getWeight();

        /*!
         * \brief   Gets the rarity of this item type.
         *
         * \return  Returns an ItemRarity with the rarity of this item type.
         */
        ItemRarity getRarity();

        /*!
         * \brief   Gets the description of this item type.
         *
         * \return  Returns a std::string with the description of this item type.
         */
        std::string getDescription();

        /*!
         * \brief   Gets the name of this item type.
         *
         * \return  Returns a std::string with the name of this item type.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the cost of this item type.
         *
         * \return  Returns an int with the cost of this item type.
         */
        int getCost();

        /*!
         * \brief   Gets the slot type of this item type.
         *
         * \return  Returns an EquipmentSlot with the slot type of this item type.
         */
        EquipmentSlot getSlotType();

        /*!
         * \brief   Sets the weight of this item type.
         * 
         * \param[in] weight    Specifies the weight of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          weight was successful.
         */
        bool setWeight(int weight);

        /*!
         * \brief   Sets the rarity of this item type.
         * 
         * \param[in] rarity    Specifies the rarity of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          rarity was successful.
         */
        bool setRarity(ItemRarity rarity);

        /*!
         * \brief   Sets the description of this item type.
         * 
         * \param[in] description   Specifies the description of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          description was successful.
         */
        bool setDescription(std::string description);

        /*!
         * \brief   Sets the name of this item type.
         * 
         * \param[in] name  Specifies the name of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          name was successful.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the cost of this item type.
         * 
         * \param[in] cost  Specifies the cost of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          cost was successful.
         */
        bool setCost(int cost);

        /*!
         * \brief   Sets the slot type of this item type.
         * 
         * \param[in] slotType  Specifies the slot type of this item type.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          slot type was successful.
         */
        bool setSlotType(EquipmentSlot slotType);

        /*!
         * \brief   Gets the armor bonus if this item type has an armor bonus.
         *
         * \return  Returns an int with the armor bonus for this item type.
         */
        virtual int getArmorBonus();

        /*!
         * \brief   Gets the damage type this item type is resistant to.
         *
         * \return  Returns a DamageType with the damage type this item type is 
         *          resistant to.
         */
        virtual DamageType getResistantTo();

        /*!
         * \brief   Gets the damage if this item type has a damage.
         *
         * \return  Returns an int with the damage for this item type.
         */
        virtual int getDamage();

        /*!
         * \brief   Gets the damage type if this item type has a damage type.
         *
         * \return  Returns a DamageType with the damage type for this item type.
         */
        virtual DamageType getDamageType();

        /*!
         * \brief   Gets the range if this item type has a range.
         *
         * \return  Returns an AreaSize with the range for this item type.
         */
        virtual AreaSize getRange();

        /*!
         * \brief   Gets the crit multiplier if this item type has an crit multiplier.
         *
         * \return  Returns an int with the crit multiplier for this item type.
         */
        virtual int getCritMultiplier();

        /*!
         * \brief   Sets the damage if this item type has a damage.
         *
         * \param[in] damage    Specifies the damage.
         *
         * \return  Returns a bool indicating whether or not setting the damage was
         *          sucessful.
         */
        virtual bool setDamage(int damage);

        /*!
         * \brief   Sets the damage type if this item type has a damage type.
         *
         * \param[in] type  Specifies the damage type.
         *
         * \return  Returns a bool indicating whether or not setting the damage type was
         *          sucessful.
         */
        virtual bool setDamageType(DamageType type);

        /*!
         * \brief   Sets the range if this item type has a range.
         *
         * \param[in] range    Specifies the range.
         *
         * \return  Returns a bool indicating whether or not setting the range was
         *          sucessful.
         */
        virtual bool setRange(AreaSize range);

        /*!
         * \brief   Sets the crit multiplier if this item type has a crit multiplier.
         *
         * \param[in] multiplier    Specifies the crit multiplier.
         *
         * \return  Returns a bool indicating whether or not setting the crit multiplier 
         *          was sucessful.
         */
        virtual bool setCritMultiplier(int multiplier);

        /*!
         * \brief   Sets the armor bonus for this item type, if it has an armor bonus.
         *
         * \param[in] bonus     Specifes the armor bonus for this item type.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          armor bonus was successful.
         */
        virtual bool setArmorBonus(int bonus);

        /*!
         * \brief   Sets the damage type that this item type is resistant to.
         *
         * \param[in] resistance    Specifies the damage type this item type
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
        virtual ObjectType getObjectType();

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
         * \brief   Gets the response of this object to the command move.
         *
         * \param[in] aPlayer   Specifies the player that is moving the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          move.
         */
        virtual std::string move(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command read.
         *
         * \param[in] aPlayer   Specifies the player that is reading the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          read.
         */
        virtual std::string read(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command break.
         *
         * \param[in] aPlayer   Specifies the player that is breaking the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          break.
         */
        virtual std::string breakIt(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command climb.
         *
         * \param[in] aPlayer   Specifies the player that is climbing the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          climb.
         */
        virtual std::string climb(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command turn.
         *
         * \param[in] aPlayer   Specifies the player that is turning the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          turn.
         */
        virtual std::string turn(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command push.
         *
         * \param[in] aPlayer   Specifies the player that is pushing the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          push.
         */
        virtual std::string push(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command pull.
         *
         * \param[in] aPlayer   Specifies the player that is pulling the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          pull.
         */
        virtual std::string pull(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command eat.
         *
         * \param[in] aPlayer   Specifies the player that is eating the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          eat.
         */
        virtual std::string eat(Player *aPlayer); 

        /*!
         * \brief   Gets the response of this object to the command drink.
         *
         * \param[in] aPlayer   Specifies the player that is drinking the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          drink.
         */
        virtual std::string drink(Player *aPlayer); 

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
        std::atomic<int> weight;
        std::atomic<ItemRarity> rarity;
        std::string description;
        mutable std::mutex descriptionMutex;
        std::string name;
        mutable std::mutex nameMutex;
        std::atomic<int> cost;
        std::atomic<EquipmentSlot> slotType;
};

}}

#endif