/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/08/2017
 * Course:        CS467, Winter 2017
 * Filename:      Quest.hpp
 *
 * Overview:
 *     Header file for Quest class. Defines the members and 
 * functions that apply to all quests.
 ************************************************************************/

#ifndef QUEST_HPP
#define QUEST_HPP

#include <string>
#include <vector>
#include <tuple>
#include "SerializableNoun.hpp"
#include "InteractiveNoun.hpp"
#include "UniqueNoun.hpp"
#include "DataType.hpp"
#include "ObjectType.hpp"

namespace legacymud { namespace engine {

class Item;
class QuestStep;

class Quest: public SerializableNoun, public InteractiveNoun, public UniqueNoun {
    public:
        Quest();
        Quest(std::string name, std::string description, int rewardMoney, Item *rewardItem);
        Quest(const Quest &otherQuest);
        Quest & operator=(const Quest &otherQuest);
        virtual ~Quest();
        std::string getName();
        std::string getDescription();
        int getRewardMoney();
        Item* getRewardItem();
        std::vector<std::tuple<int, QuestStep*>> getSteps();
        bool setName(std::string name);
        bool setDescription(std::string description);
        bool setRewardMoney(int money);
        bool setRewardItem(Item *rewardItem);
        bool addStep(int, QuestStep *aStep);
        bool removeStep(QuestStep *aStep);
        virtual ObjectType getObjectType();
        virtual std::string serialize();
        virtual bool deserialize(std::string);
        virtual InteractiveNoun* copy();
        virtual bool editAttribute(Player*, std::string);
        virtual bool editWizard(Player*);
        static std::map<std::string, DataType> getAttributeSignature();
    private:
        std::string name;
        std::string description;
        int rewardMoney;
        Item *rewardItem;
        std::vector<std::tuple<int, QuestStep*>> steps;
};

}}

#endif