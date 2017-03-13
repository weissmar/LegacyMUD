/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/12/2017
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
#include <vector>
#include "parser.hpp"
#include "Combatant.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"
#include "EffectType.hpp"
#include "GameObjectManager.hpp"

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
        Creature(CreatureType *aType, bool ambulatory, int maxHealth, Area *spawnLocation, int maxSpecialPts, int dexterity, int strength, int intelligence, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID);
        Creature(const Creature &otherCreature);
        Creature & operator=(const Creature &otherCreature);
        //virtual ~Creature();

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

        virtual int getSizeModifier() const;
        int getXP() const;
        bool setRespawn();
        bool readyRespawn() const;

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
         * \brief   Deserializes and creates an object of this type from the
         *          specified string of serialized data.
         * 
         * \param[in] string    Holds the data to be deserialized.
         * \param[in] GameObjectManager    Provides access to other game objects in memory.
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static Creature* deserialize(std::string, GameObjectManager* gom);

        /*!
         * \brief   Gets the response of this object to the command look.
         * 
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \return  Returns a std::string with the response to the command
         *          look.
         */
        virtual std::string look(Player *aPlayer, std::vector<EffectType> *effects);  

        /*!
         * \brief   Executes the take command on this creature.
         * 
         * This function takes the specified item and places it in the inventory of 
         * this creature.
         *
         * \param[in] aPlayer       Specifies the player that entered the command.
         * \param[in] anItem        Specifies the item that is being taken.
         * \param[in] aContainer    Optionally specifies the container that held the
         *                          item.
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
         * \brief   Executes the equip command on this character.
         * 
         * This function equips the specified item on this creature.
         *
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[in] anItem    Specifies the item being equipped.
         * \param[in] character Specifies the character that is equipping the 
         *                      item.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the creature.
         *
         * \return  Returns a std::string with the response to the command
         *          equip.
         */
        virtual std::string equip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

        /*!
         * \brief   Executes the unequip command on this creature.
         * 
         * This function unequips the specified item from this creature.
         *
         * \param[in] aPlayer   Specifies the player that entered the command.
         * \param[in] anItem    Specifies the item being unequipped.
         * \param[in] character Specifies the character that is unequipping the item.
         * \param[out] effects  Specifies the effects of the action.
         *
         * \note    May cause an effect on the creature.
         * 
         * \pre The item must be equipped on this creature.
         *
         * \return  Returns a std::string with the response to the command
         *          unequip.
         */
        virtual std::string unequip(Player *aPlayer, Item *anItem, InteractiveNoun *character, std::vector<EffectType> *effects);

        /*!
         * \brief   Moves this creature to the specified area.
         *
         * This function moves this creature to the specified area. 
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
         * \param[out] effects      Specifies the effects of the action.
         *
         * \note    May cause an effect on the creature.
         * 
         * \pre The item must be in the inventory of the specified transferring character.
         *
         * \return  Returns a std::string with the response to the command
         *          transfer.
         */
        virtual std::string transfer(Player *aPlayer, Item *anItem, InteractiveNoun *character, InteractiveNoun *destination, std::vector<EffectType> *effects);
        

        /*!
         * \brief   Executes the attack command from this creature.
         * 
         * This function executes an attack using the specified skill or item from this creature
         * against the specified player.
         *
         * \param[in] aPlayer           Specifies the player that is being attacked.
         * \param[in] anItem            Optionally specifies the item being used in the attack.
         * \param[in] aSkill            Optionally specifies the skill being used in the attack.
         * \param[in] character         Specifies the character that is attacking.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker. This
         *                              parameter should be false when this function is called on
         *                              a creature.
         * \param[out] effects          Specifies the effects of the action.
         * 
         * \pre The item, if specified, must be in the inventory of this creature.
         * \pre The skill, if specified, must be the skill of this creature's creatureType.
         * \pre The playerAttacker parameter should be false when this function is called on a creature.
         *
         * \return  Returns a std::string with the response to the command
         *          attack.
         */
        virtual std::string attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker, std::vector<EffectType> *effects);

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
        int getAttackDamage(Player *aPlayer, int critMultiplier, int attackDamage, DamageType damageType, AreaSize range);
        CreatureType *type;
        mutable std::mutex typeMutex;
        std::atomic<bool> ambulatory;
        std::atomic<time_t> respawnClock;
};

}}

#endif