/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/24/2017
 * \course      CS467, Winter 2017
 * \file        Character.hpp
 *
 * \details     Header file for Character base class. Defines the members and 
 *              functions that apply to all characters. This class should not
 *              be instantiated.
 ************************************************************************/

#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <utility>
#include <mutex>
#include <atomic>
#include "InteractiveNoun.hpp"
#include "EquipmentSlot.hpp"

namespace legacymud { namespace engine {

class Area;
class Item;

/*!
 * \details     This class defines the inheritable characteristics that define 
 *              all in-game characters. This class should not be instantiated.
 */
class Character: public InteractiveNoun {
    public:
        Character();
        Character(std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        /*Character(const Character &otherCharacter);
        Character & operator=(const Character &otherCharacter);
        virtual ~Character();*/

        /*!
         * \brief   Gets the name of this character.
         *
         * \return  Returns a std::string with the name of this character.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the description of this character.
         *
         * \return  Returns a std::string with the description of this character.
         */
        std::string getDescription() const;

        /*!
         * \brief   Gets the amount money this character possesses.
         *
         * \return  Returns an int with the amount money this character possesses.
         */
        int getMoney() const;

        /*!
         * \brief   Gets the location of this character.
         *
         * \return  Returns an Area* with the location of this character.
         */
        Area* getLocation() const;

        /*!
         * \brief   Gets the inventory and equipment of this character.
         *
         * \note    This function returns all items in inventory, including those
         *          currently equipped, along with where they are equipped.
         *
         * \return  Returns a std::vector<std::pair<EquipmentSlot, Item*>> representing 
         *          the inventory and equipment of this character.
         */
        std::vector<std::pair<EquipmentSlot, Item*>> getInventory() const;

        /*!
         * \brief   Gets the inventory of this character.
         *
         * \note    This function returns all items in inventory not currently equipped.
         *
         * \return  Returns a std::vector<Item*> representing the inventory of this 
         *          character.
         */
        std::vector<Item*> getItemsInventory() const;

        /*!
         * \brief   Gets the equipment of this character.
         *
         * \note    This function returns only items in inventory that are currently 
         *          equipped, along with where they are equipped.
         *
         * \return  Returns a std::vector<std::pair<EquipmentSlot, Item*>> representing 
         *          the equipment of this character.
         */
        std::vector<std::pair<EquipmentSlot, Item*>> getEquipped() const;

        /*!
         * \brief   Gets the maximum allowed inventory weight for this character.
         *
         * \return  Returns an int with the maximum allowed inventory weight for
         *          this character.
         */
        int getMaxInventoryWeight() const;

        /*!
         * \brief   Sets the name of this character.
         *
         * \param[in] name  Specifies the name of this character.
         *
         * \return  Returns a bool indicating whether or not setting the name of 
         *          this character was successful.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the description of this character.
         *
         * \param[in] description  Specifies the description of this character.
         *
         * \return  Returns a bool indicating whether or not setting the description  
         *          of this character was successful.
         */
        bool setDescription(std::string description);

        /*!
         * \brief   Sets the money this character currently possesses.
         *
         * \param[in] money  Specifies the money this character currently possesses.
         *
         * \return  Returns a bool indicating whether or not setting the money 
         *          this character possesses was successful.
         */
        bool setMoney(int money);

        /*!
         * \brief   Adds the specified amount to the money this character currently 
         *          possesses.
         *
         * \param[in] money     Specifies amount to add to the money this character 
         *                      currently possesses.
         *
         * \return  Returns a bool indicating whether or not adding the money 
         *          was successful.
         */
        int addMoney(int money);

        /*!
         * \brief   Subtracts the specified amount from the money this character 
         *          currently possesses.
         *
         * \param[in] money     Specifies amount to subtract from the money this 
         *                      character currently possesses.
         *
         * \return  Returns a bool indicating whether or not subtracting the money 
         *          was successful.
         */
        int subtractMoney(int money);

        /*!
         * \brief   Sets the location of this character.
         *
         * \param[in] alocation     Specifies the location of this character.
         *
         * \return  Returns a bool indicating whether or not setting the location  
         *          of this character was successful.
         */
        bool setLocation(Area *aLocation);

        /*!
         * \brief   Adds the specified item to this character's inventory.
         *
         * \param[in] anItem    Specifies the item to add.
         *
         * \return  Returns a bool indicating whether or not adding the item was 
         *          successful.
         */
        virtual bool addToInventory(Item *anItem);

        /*!
         * \brief   Equips the specified item for this character.
         *
         * \param[in] anItem    Specifies the item to equip.
         *
         * \note    If the item is not already in inventory, this function adds the 
         *          item to inventory and equips it.
         *
         * \return  Returns a bool indicating whether or not equipping the item was 
         *          successful.
         */
        bool equipItem(Item *anItem);

        /*!
         * \brief   Removes the specified item from this character's inventory.
         *
         * \param[in] anItem    Specifies the item to remove.
         *
         * \note    If the item is equipped, this function unequips the item and
         *          removes it from inventory.
         *
         * \return  Returns a bool indicating whether or not removing the item was 
         *          successful.
         */
        virtual bool removeFromInventory(Item *anItem);

        /*!
         * \brief   Unequips the specified item from this character.
         *
         * \param[in] anItem    Specifies the item to unequip.
         *
         * \return  Returns a bool indicating whether or not unequipping the item was 
         *          successful.
         */
        bool unequipItem(Item *anItem);

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
         * \brief   Removes all items from this character's inventory.
         *
         * \return  Returns a bool indicating whether or not removing the items was 
         *          successful.
         */
        virtual bool removeAllFromInventory();

        /*!
         * \brief   Removes a random item from this character's inventory.
         *
         * \note    If the item is equipped, this function unequips the item and
         *          removes it from inventory.
         *
         * \return  Returns a pointer to the removed item.
         */
        virtual Item* removeRandomFromInventory();
    private:
        std::string name;
        mutable std::mutex nameMutex;
        std::string description;
        mutable std::mutex descriptionMutex;
        std::atomic<int> money;
        Area *location;
        mutable std::mutex locationMutex;
        std::vector<std::pair<EquipmentSlot, Item*>> inventory;
        mutable std::mutex inventoryMutex;
        std::atomic<int> maxInventoryWeight;
};

}}
 
#endif