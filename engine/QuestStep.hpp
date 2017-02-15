/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    02/15/2017
 * \course      CS467, Winter 2017
 * \file        QuestStep.hpp
 *
 * \details     Header file for QuestStep class. Defines the members and 
 *              functions that apply to all quest steps. QuestStep defines
 *              the steps that make up an in-game quest.
 ************************************************************************/

#ifndef QUEST_STEP_HPP
#define QUEST_STEP_HPP

#include <string>
#include "InteractiveNoun.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class ItemType;
class NonCombatant;

/*!
 * \details     QuestStep defines the steps that make up an in-game quest.
 */
class QuestStep: public InteractiveNoun {
    public:
        QuestStep();
        QuestStep(int ordinalNumber, std::string description, ItemType *anItemType, NonCombatant *giver, NonCombatant *receiver, std::string completionText);
        /*QuestStep(const QuestStep &otherQuestStep);
        QuestStep & operator=(const QuestStep &otherQuestStep);
        virtual ~QuestStep();*/

        /*!
         * \brief   Gets the ordinal number of the quest step.
         *
         * \return  Returns an int with the ordinal number.
         */
        int getOrdinalNumber();

        /*!
         * \brief   Gets the description of the quest step.
         *
         * \return  Returns a std::string with the description.
         */
        std::string getDescription();

        /*!
         * \brief   Gets the fetch item for the quest step.
         *
         * \return  Returns an ItemType* with the fetch item.
         */
        ItemType* getFetchItem();

        /*!
         * \brief   Gets the giver of the quest step.
         *
         * \return  Returns a NonCombatant* with the giver.
         */
        NonCombatant* getGiver();

        /*!
         * \brief   Gets the receiver of the quest step.
         *
         * \return  Returns a NonCombatant* with the receiver.
         */
        NonCombatant* getReceiver();

        /*!
         * \brief   Gets the completion text for the quest step.
         *
         * \return  Returns a std::string with the completion text.
         */
        std::string getCompletionText();

        /*!
         * \brief   Sets the ordinal number of the quest step.
         * 
         * \param[in] number    Specifies the ordinal number for the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the ordinal number was successful.
         */
        bool setOrdinalNumber(int number);

        /*!
         * \brief   Sets the description of the quest step.
         * 
         * \param[in] description    Specifies the description of the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the description was successful.
         */
        bool setDescription(std::string description);

        /*!
         * \brief   Sets the fetch item for the quest step.
         * 
         * \param[in] anItemType    Specifies the fetch item for the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the fetch item was successful.
         */
        bool setFetchItem(ItemType *anItemType);

        /*!
         * \brief   Sets the giver of the quest step.
         * 
         * \param[in] giver    Specifies the giver for the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the giver was successful.
         */
        bool setGiver(NonCombatant *giver);

        /*!
         * \brief   Sets the receiver of the quest step.
         * 
         * \param[in] receiver    Specifies the receiver for the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the receiver was successful.
         */
        bool setReceiver(NonCombatant *receiver);

        /*!
         * \brief   Sets the completion text of the quest step.
         * 
         * \param[in] completionText    Specifies the completion text for 
         *                              the step.
         *
         * \return  Returns a bool indicating whether or not setting
         *          the completion text was successful.
         */
        bool setCompletionText(std::string completionText);   

        /*!
         * \brief   Gets the description of the step.
         *
         * \return  Returns a std::string with the description.
         */
        virtual std::string getName();

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
        int ordinalNumber;
        std::string description;
        ItemType *fetchItem;
        NonCombatant *giver;
        NonCombatant *receiver;
        std::string completionText;
};

}}

#endif