/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/25/2017
 * \course      CS467, Winter 2017
 * \file        Player.cpp
 *
 * \details     Implementation file for Player class. 
 ************************************************************************/

#include <iostream>
#include "Player.hpp"
#include "Area.hpp"
#include "Quest.hpp"
#include "NonCombatant.hpp"
#include "PlayerClass.hpp"
#include "InteractiveNoun.hpp"
#include "SpecialSkill.hpp"
#include "Item.hpp"

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


Player::Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, std::string name, std::string description, Area *startArea)
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


int Player::getExperiencePoints() const{
    return experiencePoints.load();
}


int Player::getLevel() const{
    return level.load();
}


CharacterSize Player::getSize() const{
    return size.load();
}


std::string Player::getUser() const{
    std::lock_guard<std::mutex> usernameLock(usernameMutex);
    return username;
}  


PlayerClass* Player::getPlayerClass() const{
    std::lock_guard<std::mutex> playerClassLock(playerClassMutex);
    return playerClass;
}


NonCombatant* Player::getInConversation() const{
    std::lock_guard<std::mutex> inConversationLock(inConversationMutex);
    return inConversation;
}


bool Player::isActive() const{
    return active.load();
}


int Player::getFileDescriptor() const{
    return fileDescriptor.load();
}


bool Player::queueIsEmpty() const{
    std::lock_guard<std::mutex> combatQueueLock(combatQueueMutex);
    return combatQueue.empty();
}


bool Player::isEditMode() const{
    return editMode.load();
}


std::vector<std::pair<Quest*, int>> Player::getQuestList() const{
    std::lock_guard<std::mutex> questListLock(questListMutex);
    return questList;
}


const parser::LexicalData& Player::getLexicalData() const{
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
    return inventoryLexicalData;
}


int Player::addToExperiencePts(int gainedXP){
    int newXP = experiencePoints.load() + gainedXP;
    experiencePoints.store(newXP);

    return experiencePoints.load();
}


bool Player::levelUp(){
    return false;
}


bool Player::setSize(CharacterSize size){
    this->size.store(size);

    return true;
}


bool Player::setPlayerClass(PlayerClass *aClass){
    std::lock_guard<std::mutex> playerClassLock(playerClassMutex);
    playerClass = aClass;

    return true;
}


bool Player::setActive(bool active){
    this->active.store(active);

    return true;
} 


bool Player::setFileDescriptor(int FD){
    fileDescriptor.store(FD);

    return true;
}


bool Player::activate(int FD){
    fileDescriptor.store(FD);
    active.store(true);

    return true;
}


bool Player::setInConversation(NonCombatant *anNPC){
    std::lock_guard<std::mutex> inConversationLock(inConversationMutex);
    inConversation = anNPC;

    return true;
}


Command* Player::getNextCommand(){
    std::lock_guard<std::mutex> combatQueueLock(combatQueueMutex);
    Command *nextCommand = nullptr;
    if (!combatQueue.empty()){
        nextCommand = combatQueue.front();
        combatQueue.pop();
    }

    return nextCommand;
}


bool Player::addCommand(Command *aCommand){
    std::lock_guard<std::mutex> combatQueueLock(combatQueueMutex);
    combatQueue.push(aCommand);

    return true;
}


bool Player::setEditMode(bool editing){
    editMode.store(editing);

    return true;
}


bool Player::addQuest(Quest *aQuest, int step){
    return false;
}


bool Player::updateQuest(Quest *aQuest, int step){
    return false;
} 


bool Player::addToInventory(Item *anItem){
    bool success = false; 
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        success = Character::addToInventory(anItem);

        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            inventoryLexicalData.addNoun(noun, anItem);
        }
        for (auto verb : verbAliases){
            inventoryLexicalData.addVerb(verb, anItem);
        }

        if (anItem->getObjectType() == ObjectType::CONTAINER){
            //***************************************************** figure out adding aliases of contained items
        }
    }
    return success;
}


bool Player::removeFromInventory(Item *anItem){
    bool success = false; 
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        success = Character::removeFromInventory(anItem);

        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            inventoryLexicalData.removeNoun(noun, anItem);
        }
        for (auto verb : verbAliases){
            inventoryLexicalData.removeVerb(verb, anItem);
        }

        if (anItem->getObjectType() == ObjectType::CONTAINER){
            //***************************************************** figure out removing aliases of contained items
        }
    }
    return success;
}


bool Player::addNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    inventoryLexicalData.addNoun(alias, this);
    success = InteractiveNoun::addNounAlias(alias);

    return success;
}


bool Player::removeNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    inventoryLexicalData.removeNoun(alias, this);
    success = InteractiveNoun::removeNounAlias(alias);

    return success;
}


bool Player::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    inventoryLexicalData.addVerb(alias, this);
    success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);

    return success;
}


bool Player::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    inventoryLexicalData.removeVerb(alias, this);
    success = InteractiveNoun::removeVerbAlias(aCommand, alias);

    return success;
}


bool Player::registerAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            inventoryLexicalData.addVerb(alias, anObject);
        } else {
            inventoryLexicalData.addNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


bool Player::unregisterAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            inventoryLexicalData.removeVerb(alias, anObject);
        } else {
            inventoryLexicalData.removeNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


ObjectType Player::getObjectType() const{
    return ObjectType::PLAYER;
}


std::string Player::serialize(){
    return "";
}


bool Player::deserialize(std::string){
    return false;
}


std::string Player::look(std::vector<EffectType> *effects){
    return "";
}  


std::string Player::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacters, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;
    bool success;

std::cout << "inside Player::take\n";

    success = addToInventory(anItem);
    if (success){
        message = getTextAndEffect(CommandEnum::TAKE, anEffect);
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    } else {
        message = "false";
    }

    return message;
}


std::string Player::equip(Player*, Item*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Player::unequip(Player*, Item*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Player::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Player::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    return "";
}


std::string Player::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    return "";
}


std::string Player::talk(Player*, NonCombatant*, std::vector<EffectType> *effects){
    return "";
} 


std::string Player::buy(Player*, Item*, std::vector<EffectType> *effects){
    return "";
}


std::string Player::sell(Player*, Item*, std::vector<EffectType> *effects){
    return "";
}


std::string Player::search(Player*, std::vector<EffectType> *effects){
    return "";
} 


std::string Player::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill, std::vector<EffectType> *effects){
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