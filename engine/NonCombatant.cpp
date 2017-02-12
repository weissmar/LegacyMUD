/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/12/2017
 * \course      CS467, Winter 2017
 * \file        NonCombatant.cpp
 *
 * \details     Implementation file for NonCombatant class. 
 ************************************************************************/

#include "NonCombatant.hpp"
#include "Area.hpp"
#include "Quest.hpp"

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


Quest* NonCombatant::getQuest(){
    return quest;
}


bool NonCombatant::setQuest(Quest *aQuest){
    if (aQuest != nullptr){
        quest = aQuest;
        return true;
    }

    return false;
}


ObjectType NonCombatant::getObjectType(){
    return ObjectType::NON_COMBATANT;
}


std::string NonCombatant::serialize(){
    return "";
}


bool NonCombatant::deserialize(std::string){
    return false;
}


std::string NonCombatant::look(){
    return "";
}  


bool NonCombatant::take(Player*, Item*, InteractiveNoun*){
    return false;
}


bool NonCombatant::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){
    return false;
}


bool NonCombatant::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){
    return false;
}


std::string NonCombatant::talk(Player*, NonCombatant*){
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