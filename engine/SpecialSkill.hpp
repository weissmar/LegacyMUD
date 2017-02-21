/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/13/2017
 * \course      CS467, Winter 2017
 * \file        SpecialSkill.hpp
 *
 * \details     Header file for SpecialSkill class. Defines the members and 
 *              functions that apply to all special skills. Special skills
 *              define in-game skills that use special points. 
 ************************************************************************/

#ifndef SPECIAL_SKILL_HPP
#define SPECIAL_SKILL_HPP

#include <string>
#include <mutex>
#include <atomic>
#include "InteractiveNoun.hpp"
#include "DamageType.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

/*!
 * \details     Special skills define in-game skills that use special points. 
 */
class SpecialSkill: public InteractiveNoun {
    public:
        SpecialSkill();
        SpecialSkill(std::string name, int damage, DamageType type, int cost, int cooldown);

        /*!
         * \brief   Gets the name of the special skill.
         *
         * \return  Returns a std::string with the name of the special skill.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the damage of the special skill.
         *
         * \return  Returns an int with the damage of the special skill.
         */
        int getDamage();

        /*!
         * \brief   Gets the damage type of the special skill.
         *
         * \return  Returns a DamageType with the damage type of the special skill.
         */
        DamageType getDamageType();

        /*!
         * \brief   Gets the cost of the special skill.
         *
         * \return  Returns an int with the cost of the special skill.
         */
        int getCost();

        /*!
         * \brief   Gets the cooldown of the special skill.
         *
         * \return  Returns an int with the cooldown of the special skill.
         */
        int getCooldown();

        /*!
         * \brief   Sets the name of the special skill.
         * 
         * \param[in] name  Specifies the name of the skill.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          name was successful.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the damage of the special skill.
         * 
         * \param[in] damage  Specifies the damage of the skill.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          damage was successful.
         */
        bool setDamage(int damage);

        /*!
         * \brief   Sets the damage type of the special skill.
         * 
         * \param[in] type  Specifies the damage type of the skill.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          damage type was successful.
         */
        bool setDamageType(DamageType type);

        /*!
         * \brief   Sets the cost of the special skill.
         * 
         * \param[in] cost  Specifies the cost of the skill.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          cost was successful.
         */
        bool setCost(int cost);

        /*!
         * \brief   Sets the cooldown of the special skill.
         * 
         * \param[in] cooldown  Specifies the cooldown of the skill.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          cooldown was successful.
         */
        bool setCooldown(int cooldown);

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
         * \brief   Gets the response of this object to the command more.
         * 
         * This function returns a string with details about this skill.
         *
         * \return  Returns a std::string with the response to the command
         *          more.
         */
        virtual std::string more(); 

        /*!
         * \brief   Executes the attack command using this skill.
         * 
         * This function executes an attack using this skill from the specified 
         * player against the specified character or, if the playerAttacker is false,
         * from the specified character against the specified player. It then calls
         * this function on the attacking player or character with a pointer to
         * this skill in the anItem parameter.
         *
         * \param[in] aPlayer           Specifies the player that is either the attacker or
         *                              the attackee, depending on the value of playerAttacker.
         * \param[in] anItem            This parameter is ignored in this case.
         * \param[out] aSkill           Specifies the skill being used in the attack.
         * \param[in] character         Specifies the character that is either the attacker
         *                              or attackee, depending on the value of playerAttacker.
         * \param[in] playerAttacker    Specifies whether or not the player is the attacker.
         * 
         * \pre The skill must be a skill available to the attacker.
         *
         * \return  Returns a bool indicating whether or not the attack was executed
         *          successfully.
         */
        virtual bool attack(Player *aPlayer, Item *anItem, SpecialSkill *aSkill, InteractiveNoun *character, bool playerAttacker);

        /*!
         * \brief   Executes this skill on the specified recipient.
         *
         * This function executes this skill and then calls this function on the character
         * with a pointer to this skill as the aSkill parameter.
         *
         * \param[in] aPlayer       Specifies the player that is using the skill.
         * \param[out] aSkill       Specifies the skill to use.
         * \param[in] character     Specifies the character using the skill, if playerSkill
         *                          is false, or the character receiving the skill.
         * \param[in] aRecipient    Specifies the recipient of the skill.
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
        std::string name;
        mutable std::mutex nameMutex;
        std::atomic<int> damage;
        std::atomic<DamageType> damageType;
        std::atomic<int> cost;
        std::atomic<int> cooldown;
};

}}

#endif