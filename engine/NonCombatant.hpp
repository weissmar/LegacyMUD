/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/08/2017
 * \course      CS467, Winter 2017
 * \file        NonCombatant.hpp
 *
 * \details     Header file for NonCombatant class. Defines the members and 
 *              functions that apply to all NPCs. NonCombatant defines 
 *              in-game NPCs that sell and buy items and give quest steps.
 ************************************************************************/

#ifndef NON_COMBATANT_HPP
#define NON_COMBATANT_HPP

#include <string>
#include <mutex>
#include <vector>
#include "parser.hpp"
#include "Character.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"
#include "GameObjectManager.hpp" 

namespace legacymud { namespace engine {

class Area;
class Quest;
class Item;

/*!
 * \details     This class defines in-game NPCs that sell and buy items and 
 *              give quest steps.
 */
class NonCombatant: public Character {
    public:
        NonCombatant();
        NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID);

        /*!
         * \brief   Creates a copy of the NPC.
         *
         * This copies the NPC, but does not include any inventory or quests.
         */
        NonCombatant(const NonCombatant &otherNPC);

        /*!
         * \brief   Assigns a copy of the right-hand NPC to a variable.
         *
         * This copies the NPC, but does not include any inventory or quests.
         */
        NonCombatant & operator=(const NonCombatant &otherNPC);
        //virtual ~NonCombatant();

        /*!
         * \brief   Gets the quest.
         *
         * \return  Returns a Quest* with the quest of this non-combatant.
         */
        Quest* getQuest() const;

        /*!
         * \brief   Sets the quest.
         * 
         * \param[in] aQuest    Specifies the quest.
         *
         * \return  Returns a bool indicating whether or not setting the 
         *          quest was successful.
         */
        bool setQuest(Quest *aQuest);

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
         * \brief   Adds the specified item to this NPC's inventory.
         *
         * \param[in] anItem    Specifies the item to add.
         *
         * \return  Returns a bool indicating whether or not adding the item was 
         *          successful.
         */
        virtual bool addToInventory(Item *anItem);

        /*!
         * \brief   Removes the specified item from this NPC's inventory.
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
         * \param[in] GameObjectManager    Provides access to other game objects in memory.
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static NonCombatant* deserialize(std::string, GameObjectManager*);

        /*!
         * \brief   Gets the response of this object to the command look.
         * 
         * \param[in] aPlayer   Specifies the player that entered the command
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(Player *aPlayer, std::vector<EffectType> *effects); 

        /*!
         * \brief   Executes the take command on this non-combatant.
         * 
         * This function takes the specified item and places it in the inventory of 
         * this non-combatant.
         *
         * \param[in] aPlayer       Specifies the player that entered the command.
         * \param[in] anItem        Specifies the item that is being taken.
         * \param[in] aContainer    Optionally specifies the container that held
         *                          the item.
         * \param[in] aCharacter    Specifies the character that is taking the item.
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the character.
         *
         * \return  Returns a std::string with the response to the command
         *          take.
         */
        virtual std::string take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the transfer command on this non-combatant.
         * 
         * This function transfers the specified container to or from this non-combatant. If 
         * the character parameter matches this non-combatant, then this is the giver. If the
         * destination parameter matches this non-combatant, then this is the receiver.
         *
         * \param[in] aPlayer       Specifies the player that entered the command.
         * \param[in] anItem        Specifies the item being transferred.
         * \param[in] character     Specifies the character that is transferring the item.
         * \param[in] destination   Specifies the character the item is being transferred to.
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the non-combatant.
         * 
         * \pre The item must be in the inventory of the specified transferring character.
         *
         * \return  Returns a std::string with the response to the command
         *          transfer.
         */
        virtual std::string transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination, std::vector<EffectType> *effects);

        /*!
         * \brief   Moves this non-combatant to the specified area.
         *
         * This function moves this non-combatant to the specified area. 
         *
         * \param[in] aPlayer   Specifes the player that entered the command.
         * \param[in] anArea    Specifies the area to add the character to.  
         * \param[in] character Specifies the character to move to the specified
         *                      area.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          go.
         */
        virtual std::string go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects);

        /*!
         * \brief   Gets the response of this object to the command talk. 
         * 
         * This function gets the response of this non-combatant to the command talk. Then
         * it calls this function on the player with a pointer to this non-combatant as the
         * aNPC parameter.
         * 
         * \param[in] aPlayer   Specifies the player that is talking with the non-combatant.
         * \param[out] aNPC     Specifies the non-combatant the player is talking to.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command 
         *          talk.
         */
        virtual std::string talk(Player *aPlayer, NonCombatant *aNPC, std::vector<EffectType> *effects); 

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
        //virtual bool editAttribute(Player *aPlayer, std::string attribute);

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
        Quest *quest;
        mutable std::mutex questMutex;
};

}}

#endif