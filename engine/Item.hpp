/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        Item.hpp
 *
 * \details     Header file for Item class. Defines the members and 
 *              functions that apply to all items. Item defines generic
 *              in-game items.
 ************************************************************************/

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "ItemPosition.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class ItemType;
class SpecialSkill;

/*!
 * \details     Item defines generic in-game items.
 */
class Item: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        Item();
        Item(InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type);
        /*Item(const Item &otherItem);
        Item & operator=(const Item &otherItem);
        virtual ~Item();*/

        /*!
         * \brief   Gets the location of the item.
         *
         * \return  Returns an InteractiveNoun* with the location of the item.
         */
        InteractiveNoun* getLocation();

        /*!
         * \brief   Gets the position of the item.
         *
         * \return  Returns an ItemPosition with the position of the item.
         */
        ItemPosition getPosition();

        /*!
         * \brief   Gets the name of the item.
         *
         * \return  Returns a std::string with the name of the item.
         */
        std::string getName();

        /*!
         * \brief   Gets the type of the item.
         *
         * \return  Returns an ItemType* with the type of the item.
         */
        ItemType* getType();

        /*!
         * \brief   Sets the location of the item.
         * 
         * \param[in] containingNoun    Specifies the location of the item.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          location was successful.
         */
        bool setLocation(InteractiveNoun* containingNoun);

        /*!
         * \brief   Sets the position of the item.
         * 
         * \param[in] position  Specifies the position of the item.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          position was successful.
         */
        bool setPosition(ItemPosition position);

        /*!
         * \brief   Sets the name of the item.
         * 
         * \param[in] name  Specifies the name of the item.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          name was successful.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the type of the item.
         * 
         * \param[in] type  Specifies the type of the item.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          type was successful.
         */
        bool setType(ItemType *type);

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
         * \brief   Gets the response of this object to the command look.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look();  

        /*!
         * \brief   Executes the take command on this item.
         * 
         * This function takes this item and places it in the inventory of 
         * either the specified character or, if no character is specified, the
         * player. It then calls this function on the character or player with a
         * pointer to this item in the anItem parameter.
         *
         * \param[in] aPlayer   Specifies the player that entered the command and,
         *                      if character == nullptr, this is also the player that 
         *                      is taking the object.
         * \param[out] anItem   Specifies the item that is being taken.
         * \param[in] character Optionally specifies the character that is taking the 
         *                      item, or nullptr if the player is the one taking.
         *
         * \note    May cause an effect on the player/character.
         *
         * \return  Returns a bool indicating whether or not the item was taken
         *          successfully.
         */
        virtual bool take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter);

        /*!
         * \brief   Executes the put command on this item.
         * 
         * This function puts this item into the specified containing object, 
         * in the optionally specified position. This function will place this 
         * item and then call this function on the containingNoun with a pointer to
         * this item in the anItem parameter. 
         *
         * \param[in] aPlayer       Specifies the player that is placing the item.
         * \param[in/out] anItem    Specifies the item that is being placed.
         * \param[in] container     Optionally specifies a container to place the item 
         *                          in/on/under.
         * \param[in] position      Optionally specifies the position of the item in/
         *                          /on/under the container.
         *
         * \return  Returns a bool indicating whether or not the item was placed
         *          successfully.
         */
        virtual bool put(Player *aPlayer, Item *anItem, InteractiveNoun *container, ItemPosition position);

        /*!
         * \brief   Executes the drop command on this item.
         * 
         * This function removes this item from the player's inventory and places
         * it in the area that the player is currently in.
         *
         * \param[in] aPlayer   Specifies the player that is dropping the object.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool indicating whether or not the item was dropped
         *          successfully.
         */
        virtual bool drop(Player *aPlayer);

        /*!
         * \brief   Gets the response of this object to the command more.
         * 
         * This function returns a string with details about this item.
         *
         * \return  Returns a std::string with the response to the command
         *          more.
         */
        virtual std::string more(); 

        /*!
         * \brief   Executes the equip command on this item.
         * 
         * This function equips this item on the specified character or, if 
         * the character parameter == nullptr, player. It then calls this function 
         * on the character or player with a pointer to this item in the anItem 
         * parameter.
         *
         * \param[in] aPlayer   Specifies the player that entered the command and,
         *                      if character == nullptr, this is also the player that 
         *                      is equipping the item.
         * \param[out] anItem   Specifies the item being equipped.
         * \param[in] character Optionally specifies the character that is equipping the 
         *                      item, or nullptr if the player is the one equipping.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool indicating whether or not the item was equipped
         *          successfully.
         */
        virtual bool equip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Executes the unequip command on this item.
         * 
         * This function unequips this item from the specified character or, if 
         * the character parameter == nullptr, player. It then calls this function 
         * on the character or player with a pointer to this item in the anItem 
         * parameter.
         *
         * \param[in] aPlayer   Specifies the player that entered the command and,
         *                      if character == nullptr, this is also the player that 
         *                      is unequipping the item.
         * \param[out] anItem   Specifies the item being unequipped.
         * \param[in] character Optionally specifies the character that is unequipping  
         *                      the item, or nullptr if the player is the one unequipping.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be equipped on the specified character or player.
         *
         * \return  Returns a bool indicating whether or not the item was unequipped
         *          successfully.
         */
        virtual bool unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Executes the transfer command on this item.
         * 
         * This function transfers this item from the specified character or, if 
         * the character parameter == nullptr, player, to the specified destination. It 
         * then calls this function on the destination and the character/player with a 
         * pointer to this item in the anItem parameter.
         *
         * \param[in] aPlayer       Specifies the player that entered the command and,
         *                          if character == nullptr, this is also the player that 
         *                          is transferring the item.
         * \param[out] anItem       Specifies the item being transferred.
         * \param[in] character     Optionally specifies the character that is transferring  
         *                          the item, or nullptr if the player is the one transferring.
         * \param[in] destination   Specifies the character the item is being transferred to.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be in the inventory of the specified character or player.
         *
         * \return  Returns a bool indicating whether or not the item was transferred
         *          successfully.
         */
        virtual bool transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination);

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
         * \brief   Executes the attack command using this item.
         * 
         * This function executes an attack using this item from the specified 
         * player against the specified character or, if the playerAttacker is false,
         * from the specified character against the specified player. It then calls
         * this function on the attacking player or character with a pointer to
         * this item in the anItem parameter.
         *
         * \param[in] aPlayer           Specifies the player that is either the attacker or
         *                              the attackee, depending on the value of playerAttacker.
         * \param[out] anItem           Specifies the item being used in the attack.
         * \param[in] aSkill            This parameter is ignored in this case.
         * \param[in] character         Specifies the character that is either the attacker
         *                              or attackee, depending on the value of playerAttacker.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker.
         * 
         * \note If the item is in inventory, but not equipped, this function will also equip it.
         * 
         * \pre The item must be in the inventory of the specified attacking player or character.
         *
         * \return  Returns a bool indicating whether or not the attack was executed
         *          successfully.
         */
        virtual bool attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker);

        /*!
         * \brief   Executes the buy command on this item.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this item in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is buying the object.
         * \param[out] anItem   Specifies the item being purchased.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not buying the item
         *          was successful.
         */
        virtual bool buy(Player *aPlayer, Item *anItem);

        /*!
         * \brief   Executes the sell command on this item.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this item in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is selling the object.
         * \param[out] anItem   Specifies the item being sold.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not selling the item
         *          was successful.
         */
        virtual bool sell(Player *aPlayer, Item *anItem);

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
        InteractiveNoun* location;
        ItemPosition position;
        std::string name;
        ItemType *type;
};

}}

#endif