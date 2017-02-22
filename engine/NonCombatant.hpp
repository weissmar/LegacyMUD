/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
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
#include "Character.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Area;
class Quest;

/*!
 * \details     This class defines in-game NPCs that sell and buy items and 
 *              give quest steps.
 */
class NonCombatant: public Character {
    public:
        NonCombatant();
        NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        /*NonCombatant(const NonCombatant &otherNPC);
        NonCombatant & operator=(const NonCombatant &otherNPC);
        virtual ~NonCombatant();*/

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
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(); 

        /*!
         * \brief   Executes the take command on this non-combatant.
         * 
         * This function takes the specified item and places it in the inventory of 
         * this non-combatant.
         *
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[in] anItem    Specifies the item that is being taken.
         * \param[in] character Specifies the character that is taking the item.
         *
         * \note    May cause an effect on the character.
         *
         * \return  Returns a bool indicating whether or not the item was taken
         *          successfully.
         */
        virtual bool take(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter);

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
         *
         * \note    May cause an effect on the non-combatant.
         * 
         * \pre The item must be in the inventory of the specified transferring character.
         *
         * \return  Returns a bool indicating whether or not the item was transferred
         *          successfully.
         */
        virtual bool transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination);

        /*!
         * \brief   Moves this non-combatant to the specified area.
         *
         * This function moves this non-combatant to the specified area. 
         *
         * \param[in] aPlayer   Specifes the player that entered the command.
         * \param[in] anArea    Specifies the area to add the character to.  
         * \param[in] character Specifies the character to move to the specified
         *                      area.
         *
         * \return  Returns a bool indicating whether or not moving the non-combatant
         *          to this area was successful.
         */
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);

        /*!
         * \brief   Gets the response of this object to the command talk. 
         * 
         * This function gets the response of this non-combatant to the command talk. Then
         * it calls this function on the player with a pointer to this non-combatant as the
         * aNPC parameter.
         * 
         * \param[in] aPlayer   Specifies the player that is talking with the non-combatant.
         * \param[out] aNPC     Specifies the non-combatant the player is talking to.
         *
         * \return  Returns a std::string with the response to the command
         *          talk.
         */
        virtual std::string talk(Player *aPlayer, NonCombatant *aNPC); 

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
        Quest *quest;
        mutable std::mutex questMutex;
};

}}

#endif