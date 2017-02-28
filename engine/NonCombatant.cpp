/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/27/2017
 * \course      CS467, Winter 2017
 * \file        NonCombatant.cpp
 *
 * \details     Implementation file for NonCombatant class. 
 ************************************************************************/

#include "NonCombatant.hpp"
#include "Area.hpp"
#include "Quest.hpp"
#include "Item.hpp"
#include "Container.hpp"
#include "EffectType.hpp"

namespace legacymud { namespace engine {

NonCombatant::NonCombatant()
: Character()
, quest(nullptr)
{}


NonCombatant::NonCombatant(Quest *aQuest, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Character(name, description, money, aLocation, maxInventoryWeight)
, quest(aQuest)
{}


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


bool NonCombatant::deserialize(std::string){
    return false;
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

    if (anItem != nullptr){
        if ((aCharacter != nullptr) && (aCharacter->getID() == this->getID())){
            // item is being removed from this character
            success = removeFromInventory(anItem);
        } else if ((destination != nullptr) && (destination->getID() == this->getID())){
            // item is being added to this character
            success = addToInventory(anItem);
        }
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::TRANSFER, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
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


std::string NonCombatant::talk(Player*, NonCombatant*, std::vector<EffectType> *effects){
    return "";
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