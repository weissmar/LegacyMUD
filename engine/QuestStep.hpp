/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
 * Course:        CS467, Winter 2017
 * Filename:      QuestStep.hpp
 *
 * Overview:
 *     Header file for QuestStep class. Defines the members and 
 * functions that apply to all quest steps.
 ************************************************************************/

#ifndef QUEST_STEP_HPP
#define QUEST_STEP_HPP

#include <string>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "ItemType.hpp"
#include "NonCombatant.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class QuestStep: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        QuestStep();
        QuestStep(int ordinalNumber, std::string description, ItemType *anItemType, NonCombatant *giver, NonCombatant *receiver, std::string completionText);
        QuestStep(const QuestStep &otherQuestStep);
        QuestStep & operator=(const QuestStep &otherQuestStep);
        virtual ~QuestStep();
        int getOrdinalNumber();
        std::string getDescription();
        ItemType* getFetchItem();
        NonCombatant* getGiver();
        NonCombatant* getReceiver();
        std::string getCompletionText();
        bool setOrdinalNumber(int number);
        bool setDescription(std::string description);
        bool setFetchItem(ItemType *anItemType);
        bool setGiver(NonCombatant *giver);
        bool setReceiveer(NonCombatant *receiver);
        bool setCompletionText(std::string completionText);        
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual bool copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
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