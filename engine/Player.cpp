/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/14/2017
 * \course      CS467, Winter 2017
 * \file        Player.cpp
 *
 * \details     Implementation file for Player class. 
 ************************************************************************/

#include "Player.hpp"
#include "Area.hpp"
#include "Quest.hpp"
#include "NonCombatant.hpp"
#include "PlayerClass.hpp"
#include "InteractiveNoun.hpp"
#include "SpecialSkill.hpp"

namespace legacymud { namespace engine {

const int START_HEALTH = 10;
const int START_SPECIAL_PTS = 10;
const int START_MONEY = 10;
const int MAX_INVENTORY_WEIGHT = 30;

Player::Player()
: Combatant()
, experiencePoints(0)
, level(1)
, size(CharacterSize::MEDIUM)
, playerClass(nullptr)
, inConversation(nullptr)
, username("")
, active(false)
, fileDescriptor(-1)
, editMode(false)
{ }


Player::Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, std::string name, std::string description)
: Combatant(START_HEALTH, nullptr, START_SPECIAL_PTS, name, description, START_MONEY, nullptr, MAX_INVENTORY_WEIGHT)
, experiencePoints(0)
, level(1)
, size(size)
, playerClass(aClass)
, inConversation(nullptr)
, username(username)
, active(true)
, fileDescriptor(FD)
, editMode(false)
{ }


Player::Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, name, description, money, aLocation, maxInventoryWeight)
, experiencePoints(0)
, level(1)
, size(size)
, playerClass(aClass)
, inConversation(nullptr)
, username(username)
, active(true)
, fileDescriptor(FD)
, editMode(false)
{ }


/*Player::Player(const Player &otherPlayer){

}


Player & Player::operator=(const Player &otherPlayer){

}


Player::~Player(){

}*/


int Player::getExperiencePoints(){
    return experiencePoints;
}


int Player::getLevel(){
    return level;
}


CharacterSize Player::getSize(){
    return size;
}


std::string Player::getUser(){
    return username;
}  


PlayerClass* Player::getPlayerClass(){
    return playerClass;
}


NonCombatant* Player::getInConversation(){
    return inConversation;
}


bool Player::isActive(){
    return active;
}


int Player::getFileDescriptor(){
    return fileDescriptor;
}


bool Player::queueIsEmpty(){
    return combatQueue.empty();
}


bool Player::isEditMode(){
    return editMode;
}


std::vector<std::pair<Quest*, int>> Player::getQuestList(){
    return questList;
}


parser::LexicalData& Player::getLexicalData(){
    return inventoryLexicalData;
}


int Player::addToExperiencePts(int gainedXP){
    experiencePoints += gainedXP;

    return experiencePoints;
}


bool Player::levelUp(){
    return false;
}


bool Player::setSize(CharacterSize size){
    this->size = size;

    return true;
}


bool Player::setPlayerClass(PlayerClass *aClass){
    playerClass = aClass;

    return true;
}


bool Player::setActive(bool active){
    this->active = active;

    return true;
} 


bool Player::setFileDescriptor(int FD){
    fileDescriptor = FD;

    return true;
}


bool Player::setInConversation(NonCombatant *anNPC){
    inConversation = anNPC;

    return true;
}


Command* Player::getNextCommand(){
    Command *nextCommand = nullptr;
    if (!combatQueue.empty()){
        nextCommand = combatQueue.front();
        combatQueue.pop();
    }

    return nextCommand;
}


bool Player::addCommand(Command *aCommand){
    combatQueue.push(aCommand);

    return true;
}


bool Player::setEditMode(bool editing){
    editMode = editing;

    return true;
}


bool Player::addQuest(Quest *aQuest, int step){
    return false;
}


bool Player::updateQuest(Quest *aQuest, int step){
    return false;
} 


bool Player::addToInventory(Item *anItem){
    return false;
}


bool Player::removeFromInventory(Item *anItem){
    return false;
}


ObjectType Player::getObjectType(){
    return ObjectType::PLAYER;
}


std::string Player::serialize(){
    return "";
}


bool Player::deserialize(std::string){
    return false;
}


std::string Player::look(){
    return "";
}  


bool Player::take(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Player::equip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Player::unequip(Player*, Item*, InteractiveNoun*){
    return false;
}


bool Player::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){
    return false;
}


bool Player::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){
    return false;
}


bool Player::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){
    return false;
}


std::string Player::talk(Player*, NonCombatant*){
    return "";
} 


bool Player::buy(Player*, Item*){
    return false;
}


bool Player::sell(Player*, Item*){
    return false;
}


std::string Player::search(Player*){
    return "";
} 


std::string Player::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){
    return "";
} 


std::string Player::warp(Player*, Area*){
    return "";
} 


InteractiveNoun* Player::copy(){
    return nullptr;
}


bool Player::editAttribute(Player*, std::string){
    return false;
}


bool Player::editWizard(Player*){
    return false;
}


std::map<std::string, DataType> Player::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["size"] = DataType::CHARACTER_SIZE;
    signature["player class"] = DataType::PLAYER_CLASS_PTR;
    signature["username"] = DataType::STRING_TYPE;
    signature["file descriptor"] = DataType::INT_TYPE;
    signature["maximum health"] = DataType::INT_TYPE;
    signature["spawn location"] = DataType::AREA_PTR;
    signature["maximum special points"] = DataType::INT_TYPE;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["money"] = DataType::INT_TYPE;
    signature["current location"] = DataType::AREA_PTR;
    signature["maximum inventory weight"] = DataType::INT_TYPE;

    return signature;
}

}}