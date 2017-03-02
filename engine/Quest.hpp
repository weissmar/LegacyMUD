/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/01/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        Quest.hpp
 *
 * \details     Header file for Quest class. Defines the members and 
 *              functions that apply to all quests. Quests define in-game
 *              quests with steps that players may complete for rewards.
 ************************************************************************/

#ifndef QUEST_HPP
#define QUEST_HPP

#include <string>
#include <map>
#include <utility>
#include <mutex>
#include <atomic>
#include "InteractiveNoun.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Item;
class QuestStep;
class NonCombatant;

/*!
 * \details     Quests define in-game quests with steps that players may 
 *              complete for rewards.
 */
class Quest: public InteractiveNoun {
    public:
        Quest();
        Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem);
        Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem, int anID);
        /*Quest(const Quest &otherQuest);
        Quest & operator=(const Quest &otherQuest);
        virtual ~Quest();*/

        /*!
         * \brief   Gets the name of this quest.
         *
         * \return  Returns a std::string with the name.
         */
        std::string getName() const;

        /*!
         * \brief   Gets the description of this quest.
         *
         * \return  Returns a std::string with the description.
         */
        std::string getDescription() const;

        /*!
         * \brief   Gets the reward money for this quest.
         *
         * \return  Returns an int with the reward money.
         */
        int getRewardMoney() const;

        /*!
         * \brief   Gets the reward item for this quest.
         *
         * \return  Returns an Item* with the reward item.
         */
        Item* getRewardItem() const;

        /*!
         * \brief   Gets the steps of this quest.
         *
         * \return  Returns a std::vector<std::pair<int, QuestStep*>> 
         *          with the steps.
         */
        std::map<int, QuestStep*> getAllSteps() const;

        /*!
         * \brief   Gets the specified step of this quest.
         * 
         * \param[in] step  Specifies the step to get.
         *
         * \return  Returns a QuestStep* with the specified step.
         */
        QuestStep* getStep(int step) const;

        /*!
         * \brief   Gets the step after the specified step of this quest.
         * 
         * \param[in] step  Specifies the step before the step to get.
         *
         * \return  Returns a QuestStep* with the next step.
         */
        QuestStep* getNextStep(int step) const;

        /*!
         * \brief   Gets whether or not the specified step is the first
         *          step of this quest.
         * 
         * \param[in] step  Specifies the step to look up.
         *
         * \return  Returns true if the specified step is the first step of
         *          this quest; false otherwise.
         */
        bool isFirstStep(int step) const;

        /*!
         * \brief   Gets the step that the specified NPC is involved in
         *          if applicable.
         * 
         * \param[in] aNPC  Specified the NPC to look up.
         *
         * \return  Returns a QuestStep* with the specified step or nullptr
         *          if the NPC is not involved in the quest.
         */
        QuestStep* isGiverOrReceiver(NonCombatant *aNPC) const;

        /*!
         * \brief   Sets the name of this quest.
         * 
         * \param[in] name  Specifies the name of the quest.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          name succeeded.
         */
        bool setName(std::string name);

        /*!
         * \brief   Sets the description of this quest.
         * 
         * \param[in] description   Specifies the description of the quest.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          description succeeded.
         */
        bool setDescription(std::string description);

        /*!
         * \brief   Sets the reward money for this quest.
         * 
         * \param[in] money     Specifies the reward money for the quest.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          reward money succeeded.
         */
        bool setRewardMoney(int money);

        /*!
         * \brief   Sets the reward item for this quest.
         * 
         * \param[in] rewardItem    Specifies the reward item for the quest.
         *
         * \return  Returns a bool indicating whether or not setting the
         *          reward item succeeded.
         */
        bool setRewardItem(Item *rewardItem);

        /*!
         * \brief   Adds the specified step to this quest.
         * 
         * \param[in] aStep     Specifies the step to add.
         *
         * \return  Returns a bool indicating whether or not adding the
         *          step succeeded.
         */
        bool addStep(QuestStep *aStep);

        /*!
         * \brief   Removes the specified step from this quest.
         * 
         * \param[in] aStep     Specifies the step to remove.
         *
         * \return  Returns a bool indicating whether or not removing the
         *          step succeeded.
         */
        bool removeStep(QuestStep *aStep);

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
         *
         * \return  Returns an InteractiveNoun* with the newly created object.
         */
        static Quest* deserialize(std::string);

        /*!
         * \brief   Gets the response of this object to the command more.
         * 
         * This function returns a string with details about this skill.
         * 
         * \param[in] aPlayer   Specifies the player that entered the command.
         *
         * \return  Returns a std::string with the response to the command
         *          more.
         */
        virtual std::string more(Player *aPlayer); 

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
        std::string description;
        mutable std::mutex descriptionMutex;
        std::atomic<int> rewardMoney;
        Item *rewardItem;
        mutable std::mutex rewardItemMutex;
        std::map<int, QuestStep*> steps;
        mutable std::mutex stepsMutex;
};

}}

#endif