/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/25/2017
 * \course      CS467, Winter 2017
 * \file        Container.hpp
 *
 * \details     Header file for Container class. Defines the members and 
 *              functions that apply to all containers. Container defines
 *              in-game items that can hold other items in, on, and under
 *              themselves.
 ************************************************************************/

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include "parser.hpp"
#include "Item.hpp"
#include "ItemPosition.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

class SpecialSkill;

/*!
 * \details     This class defines in-game items that can hold other items in, 
 *              on, and under themselves.
 */
class Container: public Item {
    public:
        Container();
        Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type);
        /*Container(const Container &otherContainer);
        Container & operator=(const Container &otherContainer);
        virtual ~Container();*/

        /*!
         * \brief   Gets whether or not this container is empty
         *
         * \return  Returns a bool indicating whether or not this container
         *          is empty.
         */
        bool isEmpty() const;

        /*!
         * \brief   Checks if the specified item is contained in/on/under
         *          this container.
         *
         * \param[in] anItem    Specifes the item to check.
         *
         * \return  Returns a bool indicating whether or not the item is
         *          in/on/under this container.
         */
        bool isContained(Item *anItem) const;

        /*!
         * \brief   Removes the speficied item from this container.
         *
         * \param[in] anItem    Specifes the item to remove.
         *
         * \return  Returns a bool indicating whether or not removing the 
         *          item was successful.
         */
        bool remove(Item *anItem);

        /*!
         * \brief   Removes the speficied item in the specified position 
         *          in/on/under this container.
         *
         * \param[in] anItem    Specifes the item to place.
         * \param[in] position  Specifies the position to place the item in.
         *
         * \return  Returns a bool indicating whether or not placing the 
         *          item was successful.
         */
        bool place(Item *anItem, ItemPosition position);

        /*!
         * \brief   Gets the inside contents of this container.
         *
         * \return  Returns a std::vector<Item*> with the inside contents of
         *          this container.
         */
        std::vector<Item*> getInsideContents() const;

        /*!
         * \brief   Gets the under contents of this container.
         *
         * \return  Returns a std::vector<Item*> with the under contents of
         *          this container.
         */
        std::vector<Item*> getUnderContents() const;

        /*!
         * \brief   Gets the top contents of this container.
         *
         * \return  Returns a std::vector<Item*> with the top contents of
         *          this container.
         */
        std::vector<Item*> getTopContents() const;

        /*!
         * \brief   Gets all of the contents of this container.
         *
         * \return  Returns a std::vector<Item*> with all of the contents of
         *          this container.
         */
        std::vector<Item*> getAllContents() const;

        /*!
         * \brief   Gets the inside capacity of this container.
         *
         * \return  Returns an int with the inside capacity of this container.
         */
        int getInsideCapacity() const;

        /*!
         * \brief   Sets the inside capacity of this container.
         *
         * \param[in] capacity  Specifes the inside capacity of this container.
         *
         * \return  Returns a bool indicating whether or not setting the inside
         *          capacity was successful.
         */
        bool setInsideCapacity(int capacity);

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
         * \brief   Gets the response of this object to the command look.
         * 
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(std::vector<EffectType> *effects);  

        /*!
         * \brief   Executes the take command on this container.
         * 
         * If aContainer matches this container, then this function removes the
         * specified item from this container. Otherwise, this function takes this 
         * container and places it in the inventory of either the specified character 
         * or, if no character is specified, the player. It then calls this function 
         * on the character or player with a pointer to this container in the anItem 
         * parameter. It also calls this function on the container, if specified, also
         * with a pointer to this container in the anItem parameter.
         *
         * \param[in] aPlayer       Specifies the player that entered the command and,
         *                          if character == nullptr, this is also the player that 
         *                          is taking the object.
         * \param[out] anItem       Specifies the item that is being taken.
         * \param[in] aContainer    Optionally specifies the container that currently 
         *                          holds the item ot be taken.
         * \param[in] aCharacter    Optionally specifies the character that is taking the 
         *                          item, or nullptr if the player is the one taking.
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the player/character.
         *
         * \return  Returns a std::string with the response to the command
         *          take.
         */
        virtual std::string take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the put command on this container.
         * 
         * This function puts the specified item into the specified containing object, 
         * in the optionally specified position. If anItem == nullptr, then this is
         * the item being placed, otherwise the item is being placed in/on/under this
         * container. If this is the item being placed, this function will place this 
         * item and then call this function on the containingNoun with a pointer to
         * this container in the anItem parameter. If this is the containing noun, 
         * this function will add the item to this container in the specified position.
         *
         * \param[in] aPlayer       Specifies the player that is placing the item.
         * \param[in/out] anItem    Specifies the item that is being placed.
         * \param[in] container     Optionally specifies a container to place the item 
         *                          in/on/under.
         * \param[in] position      Optionally specifies the position of the item in/
         *                          /on/under the container.
         * \param[out] effects      Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          put.
         */
        virtual std::string put(Player *aPlayer, Item *anItem, InteractiveNoun *containingNoun, ItemPosition position, std::vector<EffectType> *effects);

        /*!
         * \brief   Gets the response of this object to the command more.
         * 
         * This function returns a string with details about this container.
         * 
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          more.
         */
        virtual std::string more(std::vector<EffectType> *effects); 

        /*!
         * \brief   Executes the equip command on this container.
         * 
         * This function equips this container on the specified character or, if 
         * the character parameter == nullptr, player. It then calls this function 
         * on the character or player with a pointer to this container in the anItem 
         * parameter.
         *
         * \param[in] aPlayer   Specifies the player that entered the command and,
         *                      if character == nullptr, this is also the player that 
         *                      is equipping the container.
         * \param[out] anItem   Specifies the item being equipped.
         * \param[in] character Optionally specifies the character that is equipping the 
         *                      item, or nullptr if the player is the one equipping.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          equip.
         */
        virtual std::string equip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the unequip command on this container.
         * 
         * This function unequips this container from the specified character or, if 
         * the character parameter == nullptr, player. It then calls this function 
         * on the character or player with a pointer to this container in the anItem 
         * parameter.
         *
         * \param[in] aPlayer   Specifies the player that entered the command and,
         *                      if character == nullptr, this is also the player that 
         *                      is unequipping the container.
         * \param[out] anItem   Specifies the item being unequipped.
         * \param[in] character Optionally specifies the character that is unequipping  
         *                      the item, or nullptr if the player is the one unequipping.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be equipped on the specified character or player.
         *
         * \return  Returns a std::string with the response to the command
         *          unequip.
         */
        virtual std::string unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the transfer command on this container.
         * 
         * This function transfers this container from the specified character or, if 
         * the character parameter == nullptr, player, to the specified destination. It 
         * then calls this function on the destination and the character/player with a 
         * pointer to this container in the anItem parameter.
         *
         * \param[in] aPlayer       Specifies the player that entered the command and,
         *                          if character == nullptr, this is also the player that 
         *                          is transferring the container.
         * \param[out] anItem       Specifies the item being transferred.
         * \param[in] character     Optionally specifies the character that is transferring  
         *                          the item, or nullptr if the player is the one transferring.
         * \param[in] destination   Specifies the character the item is being transferred to.
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be in the inventory of the specified character or player.
         *
         * \return  Returns a std::string with the response to the command
         *          transfer.
         */
        virtual std::string transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination, std::vector<EffectType> *effects);

        /*!
         * \brief   Gets the response of this object to the command move.
         *
         * \param[in] aPlayer   Specifies the player that is moving the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          move.
         */
        virtual std::string move(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Executes the attack command using this container.
         * 
         * This function executes an attack using this container from the specified 
         * player against the specified character or, if the playerAttacker is false,
         * from the specified character against the specified player. It then calls
         * this function on the attacking player or character with a pointer to
         * this container in the anItem parameter.
         *
         * \param[in] aPlayer           Specifies the player that is either the attacker or
         *                              the attackee, depending on the value of playerAttacker.
         * \param[out] anItem           Specifies the item being used in the attack.
         * \param[in] aSkill            This parameter is ignored in this case.
         * \param[in] character         Specifies the character that is either the attacker
         *                              or attackee, depending on the value of playerAttacker.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker.
         * \param[out] effects          Specifies the effects of the action.
         * 
         * \note If the item is in inventory, but not equipped, this function will also equip it.
         * 
         * \pre The item must be in the inventory of the specified attacking player or character.
         *
         * \return  Returns a std::string with the response to the command
         *          attack.
         */
        virtual std::string attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the buy command on this container.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this container in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is buying the object.
         * \param[out] anItem   Specifies the item being purchased.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          buy.
         */
        virtual std::string buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the sell command on this container.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this container in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is selling the object.
         * \param[out] anItem   Specifies the item being sold.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          sell.
         */
        virtual std::string sell(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects);

        /*!
         * \brief   Gets the response of this object to the command search.
         *
         * \param[in] aPlayer   Specifies the player that is searching the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          search.
         */
        virtual std::string search(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command read.
         *
         * \param[in] aPlayer   Specifies the player that is reading the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          read.
         */
        virtual std::string read(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command break.
         *
         * \param[in] aPlayer   Specifies the player that is breaking the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          break.
         */
        virtual std::string breakIt(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command climb.
         *
         * \param[in] aPlayer   Specifies the player that is climbing the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          climb.
         */
        virtual std::string climb(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command turn.
         *
         * \param[in] aPlayer   Specifies the player that is turning the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          turn.
         */
        virtual std::string turn(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command push.
         *
         * \param[in] aPlayer   Specifies the player that is pushing the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          push.
         */
        virtual std::string push(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command pull.
         *
         * \param[in] aPlayer   Specifies the player that is pulling the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          pull.
         */
        virtual std::string pull(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command eat.
         *
         * \param[in] aPlayer   Specifies the player that is eating the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          eat.
         */
        virtual std::string eat(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Gets the response of this object to the command drink.
         *
         * \param[in] aPlayer   Specifies the player that is drinking the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          drink.
         */
        virtual std::string drink(Player *aPlayer, std::vector<EffectType> *effects); 

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
        std::vector<Item*> under;
        mutable std::mutex underMutex;
        std::vector<Item*> inside;
        mutable std::mutex insideMutex;
        std::vector<Item*> onTopOf;
        mutable std::mutex onTopOfMutex;
        std::atomic<int> insideCapacity;
};

}}

#endif