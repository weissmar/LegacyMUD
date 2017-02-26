/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/25/2017
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
#include <atomic>
#include <mutex>
#include <vector>
#include "parser.hpp"
#include "InteractiveNoun.hpp"
#include "ItemPosition.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

class ItemType;
class SpecialSkill;

/*!
 * \details     Item defines generic in-game items.
 */
class Item: public InteractiveNoun {
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
        InteractiveNoun* getLocation() const;

        /*!
         * \brief   Gets the position of the item.
         *
         * \return  Returns an ItemPosition with the position of the item.
         */
        ItemPosition getPosition() const;

        /*!
         * \brief   Gets the name of the item.
         *
         * \return  Returns a std::string with the name of the item.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the type of the item.
         *
         * \return  Returns an ItemType* with the type of the item.
         */
        ItemType* getType() const;

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
         * \brief   Adds the specified noun alias to this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to add.
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was added successfully.
         */
        virtual bool addNounAlias(std::string);

        /*!
         * \brief   Removes the specified noun alias from this interactive noun.
         *
         * \param[in] alias     Specifies the noun alias to remove
         *
         * \return  Returns a bool indicating whether or not the noun alias 
         *          was found and removed successfully.
         */
        virtual bool removeNounAlias(std::string);

        /*!
         * \brief   Adds an alias of the specified command for this interactive noun.
         *
         * This function adds an alias-grammar pair to the map of aliases
         * for the Action associated with the specified command for this interactive 
         * noun.
         * 
         * \param[in] aCommand      Specifies the command the alias is aliasing.
         * \param[in] alias         Specifies the verb alias to be added.
         * \param[in] direct        Specifies support for direct objects.
         * \param[in] indirect      Specifies support for indirect objects.
         * \param[in] prepositions  Specifies supported prepositions.
         *
         * \return  Returns a bool indicating whether or not adding the
         *          alias to the interactive noun succeeded.
         */
        virtual bool addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions);

        /*!
         * \brief   Removes the verb alias for the specified command from this 
         * interactive noun.
         *
         * This function removes the alias-grammar pair indicated by the
         * specified alias string from the Action associated with the specified 
         * command for this interactive noun.
         * 
         * \param[in] aCommand  Specifies the command the alias is aliasing.
         * \param[in] alias     Specifies the verb alias to remove.
         *
         * \return  Returns a bool indicating whether or not removing the
         *          specified alias succeeded.
         */
        virtual bool removeVerbAlias(CommandEnum aCommand, std::string alias);

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
         * \brief   Executes the take command on this item.
         * 
         * This function takes this item and places it in the inventory of 
         * either the specified character or, if no character is specified, the
         * player. It then calls this function on the character or player with a
         * pointer to this item in the anItem parameter. It also calls this function
         * on the container, if specified, with a pointer to this iten in the anItem
         * parameter.
         *
         * \param[in] aPlayer       Specifies the player that entered the command and,
         *                          if character == nullptr, this is also the player that 
         *                          is taking the object.
         * \param[out] anItem       Specifies the item that is being taken.
         * \param[in] aContainer    Optionally specifies the contianer that held the 
         *                          item.
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
         * \param[out] effects      Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          put.
         */
        virtual std::string put(Player *aPlayer, Item *anItem, InteractiveNoun *container, ItemPosition position, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the drop command on this item.
         * 
         * This function removes this item from the player's inventory and places
         * it in the area that the player is currently in.
         *
         * \param[in] aPlayer   Specifies the player that is dropping the object.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a std::string with the response to the command
         *          drop.
         */
        virtual std::string drop(Player *aPlayer, std::vector<EffectType> *effects);

        /*!
         * \brief   Gets the response of this object to the command more.
         * 
         * \param[out] effects  Specifies the effects of the action.
         * 
         * This function returns a string with details about this item.
         *
         * \return  Returns a std::string with the response to the command
         *          more.
         */
        virtual std::string more(std::vector<EffectType> *effects); 

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
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool indicating whether or not the item was equipped
         *          successfully.
         */
        virtual std::string equip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

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
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be equipped on the specified character or player.
         *
         * \return  Returns a bool indicating whether or not the item was unequipped
         *          successfully.
         */
        virtual std::string unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

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
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         * 
         * \pre The item must be in the inventory of the specified character or player.
         *
         * \return  Returns a bool indicating whether or not the item was transferred
         *          successfully.
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
         * \param[out] effects          Specifies the effects of the action.
         * 
         * \note If the item is in inventory, but not equipped, this function will also equip it.
         * 
         * \pre The item must be in the inventory of the specified attacking player or character.
         *
         * \return  Returns a bool indicating whether or not the attack was executed
         *          successfully.
         */
        virtual std::string attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the buy command on this item.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this item in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is buying the object.
         * \param[out] anItem   Specifies the item being purchased.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not buying the item
         *          was successful.
         */
        virtual std::string buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the sell command on this item.
         * 
         * This function calls this function on the specified player with a pointer 
         * to this item in the anItem parameter. 
         *
         * \param[in] aPlayer   Specifies the player that is selling the object.
         * \param[out] anItem   Specifies the item being sold.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the player.
         *
         * \return  Returns a bool that indicates whether or not selling the item
         *          was successful.
         */
        virtual std::string sell(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects);

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
        InteractiveNoun* location;
        mutable std::mutex locationMutex;
        std::atomic<ItemPosition> position;
        std::string name;
        mutable std::mutex nameMutex;
        ItemType *type;
        mutable std::mutex typeMutex;
};

}}

#endif