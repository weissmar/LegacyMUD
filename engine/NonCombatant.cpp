/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        NonCombatant.cpp
 *
 * \details     Implementation file for NonCombatant class. 
 ************************************************************************/

#include "NonCombatant.hpp"
#include "Area.hpp"
#include "Quest.hpp"
#include "QuestStep.hpp"
#include "Item.hpp"
#include "Container.hpp"
#include "EffectType.hpp"
#include "ItemType.hpp"
#include "Player.hpp"

namespace legacymud { namespace engine {

NonCombatant::NonCombatant()
: Character()
, quest(nullptr)
{ }


NonCombatant::NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Character(name, description, money, aLocation, maxInventoryWeight)
, quest(aQuest)
{ }


NonCombatant::NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID)
: Character(name, description, money, aLocation, maxInventoryWeight, anID)
, quest(aQuest)
{ }


/*NonCombatant::NonCombatant(const NonCombatant &otherNPC){

}


NonCombatant & NonCombatant::operator=(const NonCombatant &otherNPC){

}


NonCombatant::~NonCombatant(){

}*/


Quest* NonCombatant::getQuest() const{
    std::lock_guard<std::mutex> questLock(questMutex);
    return quest;
}


bool NonCombatant::setQuest(Quest *aQuest){
    std::lock_guard<std::mutex> questLock(questMutex);
    if (aQuest != nullptr){
        quest = aQuest;
        return true;
    }

    return false;
}


bool NonCombatant::addToInventory(Item *anItem){
    bool success = false; 
    Container *aContainer = nullptr;
    std::vector<Item*> contents;
    Area *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        success = Character::addToInventory(anItem);

        if (success){
            nounAliases = anItem->getNounAliases();
            verbAliases = anItem->getVerbAliases();

            for (auto noun : nounAliases){
                location->registerAlias(false, noun, anItem);
            }
            for (auto verb : verbAliases){
                location->registerAlias(true, verb, anItem);
            }

            if (anItem->getObjectType() == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anItem);
                if (aContainer != nullptr){
                    contents = aContainer->getAllContents();
                    for (auto content : contents){
                        nounAliases = content->getNounAliases();
                        verbAliases = content->getVerbAliases();

                        for (auto noun : nounAliases){
                            location->registerAlias(false, noun, content);
                        }
                        for (auto verb : verbAliases){
                            location->registerAlias(true, verb, content);
                        }
                    }
                }
            }
        }
    }
    return success;
}


bool NonCombatant::removeFromInventory(Item *anItem){
    bool success = false;  
    Container *aContainer = nullptr;
    std::vector<Item*> contents;
    Area *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        success = Character::removeFromInventory(anItem);

        if (success){
            nounAliases = anItem->getNounAliases();
            verbAliases = anItem->getVerbAliases();

            for (auto noun : nounAliases){
                location->unregisterAlias(false, noun, anItem);
            }
            for (auto verb : verbAliases){
                location->unregisterAlias(true, verb, anItem);
            }

            if (anItem->getObjectType() == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anItem);
                if (aContainer != nullptr){
                    contents = aContainer->getAllContents();
                    for (auto content : contents){
                        nounAliases = content->getNounAliases();
                        verbAliases = content->getVerbAliases();

                        for (auto noun : nounAliases){
                            location->unregisterAlias(false, noun, content);
                        }
                        for (auto verb : verbAliases){
                            location->unregisterAlias(true, verb, content);
                        }
                    }
                }
            }
        }
    }
    return success;
}


bool NonCombatant::addNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(false, alias, this);
        success = InteractiveNoun::addNounAlias(alias);
    }

    return success;
}


bool NonCombatant::removeNounAlias(std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(false, alias, this);
        success = InteractiveNoun::removeNounAlias(alias);
    }

    return success;
}


bool NonCombatant::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->registerAlias(true, alias, this);
        success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);
    }

    return success;
}


bool NonCombatant::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    Area *anArea = getLocation();
    if (anArea != nullptr){
        anArea->unregisterAlias(true, alias, this);
        success = InteractiveNoun::removeVerbAlias(aCommand, alias);
    }

    return success;
}


ObjectType NonCombatant::getObjectType() const{
    return ObjectType::NON_COMBATANT;
}


std::string NonCombatant::serialize(){
    return "";
}


NonCombatant* NonCombatant::deserialize(std::string){
    return nullptr; 
}


std::string NonCombatant::look(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
}  


std::string NonCombatant::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool success;

    success = addToInventory(anItem);
    if (success){
        resultMsg = getTextAndEffect(CommandEnum::TAKE, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    } else {
        message = "false";
    }

    return message;
}


std::string NonCombatant::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    bool success = false;
    EffectType anEffect = EffectType::NONE;
    Quest *aQuest = getQuest();
    QuestStep *aStep, *nextStep;
    ItemType *questItemType;
    std::pair<int, bool> playerStepProgress;
    int rewardMoney;
    Item *rewardItem = nullptr;
    NonCombatant *aNPC = nullptr;

    if (anItem != nullptr){
        if ((aCharacter != nullptr) && (aCharacter->getID() == this->getID())){
            // item is being removed from this character
            success = removeFromInventory(anItem);
        } else if ((destination != nullptr) && (destination->getID() == this->getID())){
            // item is being added to this character
            success = addToInventory(anItem);

            if (success){
                // check if this is a quest item 
                if (aQuest != nullptr){
                    aStep = aQuest->isGiverOrReceiver(this);
                    if (aStep != nullptr){
                        questItemType = aStep->getFetchItem();
                        if ((questItemType != nullptr) && (anItem->getType() == questItemType)){
                            // this is a quest item
                            playerStepProgress = aPlayer->getQuestCurrStep(aQuest);
                            if ((playerStepProgress.first != -1) && (!playerStepProgress.second) && (aQuest->getStep(playerStepProgress.first) == aStep)){
                                // player is on this step and hasn't completed it yet
                                message = getName() + " says\"";
                                message += "Thanks for the " + questItemType->getName() + ". I really needed that.";

                                // update the quest for the player
                                aPlayer->addOrUpdateQuest(aQuest, aStep->getOrdinalNumber(), true);

                                // if this is the last step in the quest
                                if (aQuest->isLastStep(aStep->getOrdinalNumber())){
                                    // give congrats, reward, completion message
                                    message += " Hey - that means you completed the quest! Here is your reward!\"\015\012";
                                    rewardMoney = aQuest->getRewardMoney();
                                    rewardItem = aQuest->getUniqueRewardItem();
                                    aPlayer->addMoney(rewardMoney);
                                    message += "You received " + std::to_string(rewardMoney) + " money.\015\012";
                                    if (rewardItem != nullptr){
                                        aPlayer->addToInventory(rewardItem);
                                        rewardItem->setPosition(ItemPosition::INVENTORY);
                                        rewardItem->setLocation(aPlayer);
                                        message += "You received the " + rewardItem->getName() + ".\015\012";
                                    }
                                    message += aStep->getCompletionText();
                                } else {
                                    message += aStep->getCompletionText();
                                    nextStep = aQuest->getNextStep(aStep->getOrdinalNumber());
                                    if (nextStep != nullptr){
                                        aNPC = nextStep->getGiver();
                                        if (aNPC != nullptr){
                                            message += "To get the next part of the quest, go talk to " + aNPC->getName();
                                            message += " in the " + aNPC->getLocation()->getName() + ".\015\012";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                resultMsg = getTextAndEffect(CommandEnum::TRANSFER, anEffect);
                if (resultMsg.compare("false") != 0){
                    message += resultMsg;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }
            }
        }
    }

    return message;
}


std::string NonCombatant::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;

    if (anArea != nullptr){
        setLocation(anArea);

        resultMsg += getTextAndEffect(CommandEnum::GO, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string NonCombatant::talk(Player *aPlayer, NonCombatant *aNPC, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    Quest *aQuest = getQuest();
    QuestStep *aStep, playerStep;
    ItemType *anItemType;
    NonCombatant *otherNPC;
    std::pair<int, bool> playerStepProgress;
    bool giveStep = false;
    bool receiveFetch = false;

    resultMsg += getTextAndEffect(CommandEnum::TALK, anEffect);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    if (aQuest != nullptr){
        aStep = aQuest->isGiverOrReceiver(this);
        if (aStep != nullptr){
            // check if the player is on this step
            playerStepProgress = aPlayer->getQuestCurrStep(aQuest);
            if (playerStepProgress.first == -1){
                // player hasn't started the quest yet
                if (aQuest->isFirstStep(aStep->getOrdinalNumber())){
                    // this step is the first step of the quest
                    // if this is the giver, then the quest is given
                    giveStep = true;
                }
            } else if (!playerStepProgress.second) {
                // player hasn't completed the step they're on
                if (aQuest->getStep(playerStepProgress.first) == aStep){
                    // the step the player is on matches this step
                    // if this is the receiver, check if the player has the fetch item
                    receiveFetch = true;
                }
            } else {
                // player has completed the step they're on
                if (aQuest->getNextStep(playerStepProgress.first) == aStep){
                    // the step after the step the player is on matches this step
                    // if this is the giver, give the step/prompt
                    giveStep = true;
                }   
            }

            // if a message could be given
            if (giveStep || receiveFetch){
                anItemType = aStep->getFetchItem();
                if ((anItemType != nullptr) && (giveStep) && (aStep->getGiver() == this)){
                    // if this is the giver, give step
                    otherNPC = aStep->getReceiver();
                    if ((otherNPC != nullptr) && (otherNPC->getLocation() != nullptr)){
                        message = getName() + " says \"";
                        message += "Acquire a " + anItemType->getName() + " and give it to " + otherNPC->getName();
                        message += " in the " + otherNPC->getLocation()->getName() + ".\"";

                        // add or update quest for the player
                        aPlayer->addOrUpdateQuest(aQuest, aStep->getOrdinalNumber(), false);
                    }
                } else if ((anItemType != nullptr) && (receiveFetch) && (aStep->getReceiver() == this)){
                    // if this is the receiver, check for fetch item
                    if (aPlayer->hasItem(anItemType)){
                        // player has fetch item
                        message = getName() + " says \"";
                        message += "Ah, it looks like you have the " + anItemType->getName() + " I've been looking for.\"";
                    } else {
                        // player doesn't have fetch item
                        message = getName() + " says \"";
                        message += "Why are you bothering me without bringing the " + anItemType->getName() + ".\"";
                    }
                }
            }
        } 
    }

    resultMsg = aPlayer->talk(aPlayer, this, effects);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }

    return message;
} 


InteractiveNoun* NonCombatant::copy(){
    return nullptr;
}


bool NonCombatant::editAttribute(Player*, std::string){
    return false;
}


bool NonCombatant::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> NonCombatant::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["quest"] = DataType::QUEST_PTR;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["money"] = DataType::INT_TYPE;
    signature["location"] = DataType::AREA_PTR;
    signature["maximum inventory weight"] = DataType::INT_TYPE;

    return signature;
}

}}