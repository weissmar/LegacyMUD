/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/20/2017
 * \course      CS467, Winter 2017
 * \file        Creature.hpp
 *
 * \details     Header file for Creature class. Defines the members and 
 *              functions that apply to all creatures. Creature defines
 *              in-game creatures that engage players in combat and can
 *              be defeated for experience points.
 ************************************************************************/

#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <string>
#include <mutex>
#include <atomic>
#include "Combatant.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Area;
class CreatureType;
class SpecialSkill;

/*!
 * \details     Creature defines in-game creatures that engage players in 
 *              combat and can be defeated for experience points.
 */
class Creature: public Combatant {
    public:
        Creature();
        Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight);
        /*Creature(const Creature &otherCreature);
        Creature & operator=(const Creature &otherCreature);
        virtual ~Creature();*/

        /*!
         * \brief   Gets the type of this creature.
         *
         * \return  Returns a CreatureType* with the type of this creature.
         */
        CreatureType* getType() const;

        /*!
         * \brief   Gets whether or not this creature moves autonomously
         *          around the game.
         *
         * \return  Returns a bool indicating whether or not this creature 
         *          moves autonomously around the game.
         */
        bool getAmbulatory() const;

        /*!
         * \brief   Sets the type of this creature.
         * 
         * \param[in] aType     Specifies the type of this creature.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          creature type was successful.
         */
        bool setType(CreatureType *aType);

        /*!
         * \brief   Sets whether or not this creature moves around the 
         *          game autonomously.
         * 
         * \param[in] ambulatory    Specifies whether or not this creature
         *                          moves around the game autonomously.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          ambularoty nature of the creature was successful.
         */
        bool setAmbulatory(bool ambulatory);

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
         * \brief   Executes the take command on this creature.
         * 
         * This function takes the specified item and places it in the inventory of 
         * this creature.
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
         * \brief   Executes the equip command on this character.
         * 
         * This function equips the specified item on this creature.
         *
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[in] anItem   Specifies the item being equipped.
         * \param[in] character Specifies the character that is equipping the 
         *                      item.
         *
         * \note    May cause an effect on the creature.
         *
         * \return  Returns a bool indicating whether or not the item was equipped
         *          successfully.
         */
        virtual bool equip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Executes the unequip command on this creature.
         * 
         * This function unequips the specified item from this creature.
         *
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[in] anItem   Specifies the item being unequipped.
         * \param[in] character Specifies the character that is unequipping the item.
         *
         * \note    May cause an effect on the creature.
         * 
         * \pre The item must be equipped on this creature.
         *
         * \return  Returns a bool indicating whether or not the item was unequipped
         *          successfully.
         */
        virtual bool unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character);

        /*!
         * \brief   Moves this creature to the specified area.
         *
         * This function moves this creature to the specified area. 
         *
         * \param[in] aPlayer   Specifes the player that entered the command.
         * \param[in] anArea   Specifies the area to add the character to.  
         * \param[in] character Specifies the character to move to the specified
         *                      area.
         *
         * \return  Returns a bool indicating whether or not moving the creature
         *          to this area was successful.
         */
        virtual bool go(Player *aPlayer, Area *anArea, InteractiveNoun *character);

        /*!
         * \brief   Executes the transfer command on this creature.
         * 
         * This function transfers the specified container to or from this creature. If 
         * the character parameter matches this creature, then this is the giver. If the
         * destination parameter matches this creature, then this is the receiver.
         *
         * \param[in] aPlayer       Specifies the player that entered the command.
         * \param[in] anItem        Specifies the item being transferred.
         * \param[in] character     Specifies the character that is transferring the item.
         * \param[in] destination   Specifies the character the item is being transferred to.
         *
         * \note    May cause an effect on the creature.
         * 
         * \pre The item must be in the inventory of the specified transferring character.
         *
         * \return  Returns a bool indicating whether or not the item was transferred
         *          successfully.
         */
        virtual bool transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination);
        

        /*!
         * \brief   Executes the attack command from this creature.
         * 
         * This function executes an attack using the specified skill or item from this creature
         * against the specified player.
         *
         * \param[in] aPlayer           Specifies the player that is being attacked.
         * \param[in] anItem           Optionally specifies the item being used in the attack.
         * \param[in] aSkill            Optionally specifies the skill being used in the attack.
         * \param[in] character         Specifies the character that is attacking.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker. This
         *                              parameter should be false when this function is called on
         *                              a creature.
         * 
         * \pre The item, if specified, must be in the inventory of this creature.
         * \pre The skill, if specified, must be the skill of this creature's creatureType.
         * \pre The playerAttacker parameter should be false when this function is called on a creature.
         *
         * \return  Returns a bool indicating whether or not the attack was executed 
         *          successfully.
         */
        virtual bool attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker);

        /*!
         * \brief   Executes the specified skill on either this creature or the player,
         *          based on the value of playerSkill.
         *
         * This function executes the specified skill from this creature if playerSkill 
         * is false. Otherwise, it calls this function on the player with a pointer to
         * this creature as the aRecipient parameter.
         *
         * \param[in] aPlayer       Specifies the player that entered the command and, if 
         *                          playerSkill is true, the player that is using the skill.
         * \param[in] aSkill        Specifies the skill to use.
         * \param[in] character     Specifies the character to use the skill on, if playerSkill
         *                          is true, or the character that is using the skill otherwise.
         * \param[out] aRecipient   Specifies the recipient of the skill, if playerSkill is true.  
         * \param[in] playerSkill   Specifies whether or not the player is using the skill.  
         *
         * \return  Returns a std::string with the results of the skill.
         */
        virtual std::string useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill); 

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
        CreatureType *type;
        mutable std::mutex typeMutex;
        std::atomic<bool> ambulatory;
};

}}

#endif