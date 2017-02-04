/*************************************************************************
 * Author:        Rachel Weissman-Hohler
 * Date Created:  02/01/2017
 * Last Modified: 02/03/2017
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
#include "UniqueNoun.hpp"
#include "Item.hpp"
#include "QuestStep.hpp"
#include "DataType.hpp"

namespace legacymud { namespace engine {

class Quest: public SerializableNoun, public UniqueNoun {
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
        virtual std::string serialize();
        virtual bool deserialize(std::string);
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