/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/10/2017
 * \modified    03/07/2017
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
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>

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


NonCombatant::NonCombatant(const NonCombatant &otherNPC) : Character(otherNPC) {

}

NonCombatant & NonCombatant::operator=(const NonCombatant &otherNPC){
    return *this;
}

/*
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
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("NON_COMBATANT");  // class of this object

    // This is all data in the NonCombatant class that is not inherited from other classes.
    writer.String("quest_id");
    if (this->getQuest() == nullptr)
        writer.Int(-1);   
    else
        writer.Int(this->getQuest()->getID());
    
    // This is all data inherited from the Character class. 
    writer.String("name");
    writer.String(this->getName().c_str());
    writer.String("description");
    writer.String(this->getDescription().c_str());
    writer.String("money");
    writer.Int(this->getMoney());
    writer.String("location_area_id");
    writer.Int(this->getLocation()->getID());
    writer.String("max_inventory_weight");
    writer.Int(this->getMaxInventoryWeight());
    
    writer.EndObject(); // ends this object     
    writer.EndObject(); // outer object wrapper
          
    // Convert to a document so that inherited data from InteractiveNoun can be added.
    rapidjson::Document objectDoc;
    objectDoc.Parse(buffer.GetString());    
    
    // Get the data inherited from the InteractiveNoun class and add it to the object document.
    rapidjson::Document interactiveNounDataDoc(&objectDoc.GetAllocator());
    interactiveNounDataDoc.Parse(this->serializeJustInteractiveNoun().c_str());
    objectDoc["object"].AddMember("interactive_noun_data",interactiveNounDataDoc, objectDoc.GetAllocator() );
    
    // Write the object doc to a buffer for output.
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> outWriter(buffer);
    objectDoc.Accept(outWriter);
    
    return buffer.GetString();
}


NonCombatant* NonCombatant::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Check if there is a Quest
    Quest* quest;
    if (objectDoc["quest_id"].GetInt() == -1)
        quest = nullptr;
    else
        quest = static_cast<Quest*>(gom->getPointer(objectDoc["quest_id"].GetInt()));
    
    // Construct a new NonCombatant object, getting all the data needed to do so from the objectDoc. 
    NonCombatant *newNonCombatant = new NonCombatant(quest,
                                                     objectDoc["name"].GetString(),
                                                     objectDoc["description"].GetString(),
                                                     objectDoc["money"].GetInt(),
                                                     static_cast<Area*>(gom->getPointer(objectDoc["location_area_id"].GetInt())),
                                                     objectDoc["max_inventory_weight"].GetInt(),
                                                     objectDoc["interactive_noun_data"]["id"].GetInt() );
                                                     
    // Rebuild the new NonCombatant noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray()) {           
        if (objectDoc["name"].GetString() != noun.GetString() )     // note: `name` is automatically added to noun aliases
            newNonCombatant->addNounAlias(noun.GetString() );    
    }
   
    // Rebuild the new NonCombatant action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new NonCombatant          
        newNonCombatant->addAction(command, 
                           action["valid"].GetBool(), 
                           action["flavor_text"].GetString(), 
                           gamedata::EffectType_Data::stringToEnum(action["effect"].GetString()) );
 
        // Rebuild the verb alias list for this action command.
        for (auto& alias : action["aliases"].GetArray()) {              

            // rebuild the preposition map
            std::map<std::string, parser::PrepositionType> prepMap;           
            for (auto& prep : alias["grammar"]["preposition_map"].GetArray()) {              
                parser::PrepositionType prepType = gamedata::PrepositionType_Data::stringToEnum(prep["preposition_type"].GetString() );               
                prepMap[prep["preposition"].GetString()] = prepType;
            }           
            
            // add the verb alias
            newNonCombatant->addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newNonCombatant; 
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
                    if ((aStep != nullptr) && (aStep->getReceiver() == this)){
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