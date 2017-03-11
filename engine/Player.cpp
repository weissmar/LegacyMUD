/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serialize and deserialize functions) 
 * \created     02/10/2017
 * \modified    03/10/2017
 * \course      CS467, Winter 2017
 * \file        Player.cpp
 *
 * \details     Implementation file for Player class. 
 ************************************************************************/

#include <iostream>
#include "Player.hpp"
#include "Area.hpp"
#include "Quest.hpp"
#include "QuestStep.hpp"
#include "NonCombatant.hpp"
#include "PlayerClass.hpp"
#include "InteractiveNoun.hpp"
#include "SpecialSkill.hpp"
#include "Item.hpp"
#include "Container.hpp"
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <map>
#include <Grammar.hpp> 
#include <CharacterSize_Data.hpp>
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp>


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
: Combatant(START_HEALTH, startArea, START_SPECIAL_PTS, name, description, START_MONEY, startArea, MAX_INVENTORY_WEIGHT)
, experiencePoints(0)
, level(1)
, size(size)
, playerClass(aClass)
, inConversation(nullptr)
, username(username)
, active(true)
, fileDescriptor(FD)
, editMode(false)
{
    addAllLexicalData(aClass); 
    addAllLexicalData(aClass->getSpecialSkill());
}


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
{ 
    addAllLexicalData(aClass); 
    addAllLexicalData(aClass->getSpecialSkill());
}


Player::Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, int maxHealth, Area *spawnLocation, int maxSpecialPts, int dexterity, int strength, int intelligence, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight, int anID)
: Combatant(maxHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, anID)
, experiencePoints(0)
, level(1)
, size(size)
, playerClass(aClass)
, inConversation(nullptr)
, username(username)
, active(false)
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


std::map<Quest*, std::pair<int, bool>> Player::getQuestList() const{
    std::lock_guard<std::mutex> questListLock(questListMutex);
    return questList;
}


std::pair<int, bool> Player::getQuestCurrStep(Quest *aQuest) const{
    if (aQuest != nullptr){
        std::lock_guard<std::mutex> questListLock(questListMutex);
        int found = questList.count(aQuest);

        if (found == 1){
            return questList.at(aQuest);
        }
    }
    return std::make_pair(-1, false);
}


const parser::LexicalData& Player::getLexicalData() const{
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
    return inventoryLexicalData;
}


int Player::getSizeModifier() const{
    int modifier = 0;
    CharacterSize aSize = getSize();

    switch (aSize){
        case CharacterSize::TINY:
            modifier = 2;
            break;
        case CharacterSize::SMALL:
            modifier = 1;
            break;
        case CharacterSize::MEDIUM:
            modifier = 0;
            break;
        case CharacterSize::LARGE:
            modifier = -1;
            break;
        case CharacterSize::HUGE:
            modifier = -2;
            break;
    }

    return modifier;
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
    removeAllLexicalData(playerClass);
    removeAllLexicalData(playerClass->getSpecialSkill());
    playerClass = aClass;
    addAllLexicalData(playerClass);
    addAllLexicalData(playerClass->getSpecialSkill());

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


Command Player::getNextCommand(){
    std::lock_guard<std::mutex> combatQueueLock(combatQueueMutex);
    Command nextCommand;
    if (!combatQueue.empty()){
        nextCommand = combatQueue.front();
        combatQueue.pop();
    }

    return nextCommand;
}


bool Player::addCommand(Command aCommand){
    std::lock_guard<std::mutex> combatQueueLock(combatQueueMutex);
    combatQueue.push(aCommand);

    return true;
}


bool Player::setEditMode(bool editing){
    editMode.store(editing);

    return true;
}


bool Player::addOrUpdateQuest(Quest *aQuest, int step, bool complete){
    int found;

    if (aQuest != nullptr){
        std::lock_guard<std::mutex> questListLock(questListMutex);
        found = questList.count(aQuest);

        if (found != 1){
            // add quest
            questList[aQuest] = std::make_pair(step, complete);
            addAllLexicalData(aQuest);
            addAllLexicalData(aQuest->getStep(step));
        } else {
            // update quest
            removeAllLexicalData(aQuest->getStep(questList.at(aQuest).first));
            questList.at(aQuest) = std::make_pair(step, complete);
            addAllLexicalData(aQuest->getStep(step));
        }
        return true;
    }   
    return false;
}


bool Player::addToInventory(Item *anItem){
    bool success = false; 
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        success = Character::addToInventory(anItem);

        if (success){
            addAllLexicalData(anItem);

            if (anItem->getObjectType() == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anItem);
                if (aContainer != nullptr){
                    contents = aContainer->getAllContents();
                    for (auto content : contents){
                        addAllLexicalData(content);
                    }
                }
            }
        }
    }
    return success;
}


bool Player::removeFromInventory(Item *anItem){
    bool success = false;  
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        success = Character::removeFromInventory(anItem);

        if (success){
            removeAllLexicalData(anItem);

            if (anItem->getObjectType() == ObjectType::CONTAINER){
                aContainer = dynamic_cast<Container*>(anItem);
                if (aContainer != nullptr){
                    contents = aContainer->getAllContents();
                    for (auto content : contents){
                        removeAllLexicalData(content);
                    }
                }
            }
        }
    }
    return success;
}


void Player::addAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            inventoryLexicalData.addNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            inventoryLexicalData.addVerb(verb, anObject);
        }
    }
    
}


void Player::removeAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            inventoryLexicalData.removeNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            inventoryLexicalData.removeVerb(verb, anObject);
        }
    }
}


bool Player::addNounAlias(std::string alias){
    bool success = false;
    Area *location = getLocation();

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    success = InteractiveNoun::addNounAlias(alias);

    if ((location != nullptr) && (success)){
        location->registerAlias(false, alias, this);
    }

    return success;
}


bool Player::removeNounAlias(std::string alias){
    bool success = false;
    Area *location = getLocation();

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    success = InteractiveNoun::removeNounAlias(alias);

    if ((location != nullptr) && (success)){
        location->unregisterAlias(false, alias, this);
    }

    return success;
}


bool Player::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;
    Area *location = getLocation();

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);

    if ((location != nullptr) && (success)){
        location->registerAlias(true, alias, this);
    }

    return success;
}


bool Player::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;
    Area *location = getLocation();

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    success = InteractiveNoun::removeVerbAlias(aCommand, alias);

    if ((location != nullptr) && (success)){
        location->unregisterAlias(true, alias, this);
    }
    
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
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);    
    
    // Setup the object document.
    writer.StartObject();
    writer.String("object");   
    writer.StartObject();  
    writer.String("class");     
    writer.String("PLAYER");  // class of this object
   
    // This is all data in the Player class that is not inherited from other classes.
    writer.String("character_size");
    writer.String(gamedata::CharacterSize_Data::enumToString(this->getSize()).c_str());    
    writer.String("player_class_id");
    writer.Int(this->getPlayerClass()->getID());
    writer.String("username");
    writer.String(this->getUser().c_str());
    writer.String("experience_points");
    writer.Int(this->getExperiencePoints());
    writer.String("level");
    writer.Int(this->getLevel());
    
    // capture this player's quest data
    writer.String("quest_list");    
    std::map<Quest*, std::pair<int, bool>> questMap = this->getQuestList();
    writer.StartArray(); 
    for (auto aQuest = questMap.begin(); aQuest != questMap.end(); aQuest++ ) {
        writer.StartObject();
        writer.String("quest_id");
        writer.Int(aQuest->first->getID());
        writer.String("step");  
        writer.Int(aQuest->second.first);  
        writer.String("complete");
        writer.Bool(aQuest->second.second);  
        writer.EndObject();
    }
    writer.EndArray();    
   
    // This is all data kept by the inherited Combatant class.
    writer.String("max_health");
    writer.Int(this->getMaxHealth());
    writer.String("spawn_location_id");
    writer.Int(this->getSpawnLocation()->getID());
    writer.String("max_special_points");
    writer.Int(this->getMaxSpecialPts());
    writer.String("dexterity");
    writer.Int(this->getDexterity());
    writer.String("strength");
    writer.Int(this->getStrength()); 
    writer.String("intelligence");
    writer.Int(this->getIntelligence());
   
    // This is all data kept by the inherited Character class.
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


Player* Player::deserialize(std::string jsonStr, GameObjectManager* gom){  

    // parse jsonStr into a document object module
    rapidjson::Document objectDoc;
    objectDoc.Parse(jsonStr.c_str());
    
    // Construct a new Player object, getting all the data needed to do so from the objectDoc. 
    const int NO_FILE_DESCRIPTOR = -1;
    Player *newPlayer = new Player(gamedata::CharacterSize_Data::stringToEnum(objectDoc["character_size"].GetString()),
                                   static_cast<PlayerClass*>(gom->getPointer(objectDoc["player_class_id"].GetInt())),  
                                   objectDoc["username"].GetString(),
                                   NO_FILE_DESCRIPTOR,      
                                   objectDoc["max_health"].GetInt(),
                                   static_cast<Area*>(gom->getPointer(objectDoc["spawn_location_id"].GetInt())),
                                   objectDoc["max_special_points"].GetInt(),
                                   objectDoc["dexterity"].GetInt(),
                                   objectDoc["strength"].GetInt(),
                                   objectDoc["intelligence"].GetInt(),
                                   objectDoc["name"].GetString(),
                                   objectDoc["description"].GetString(),
                                   objectDoc["money"].GetInt(),
                                   static_cast<Area*>(gom->getPointer(objectDoc["location_area_id"].GetInt())),
                                   objectDoc["max_inventory_weight"].GetInt(),
                                   objectDoc["interactive_noun_data"]["id"].GetInt() );
    
    // Rebuild the new Player noun alias list. 
    for (auto& noun : objectDoc["interactive_noun_data"]["noun_aliases"].GetArray())            
        newPlayer->InteractiveNoun::addNounAlias(noun.GetString() );    
   
    // Rebuild the new Player action list.
    for (auto& action : objectDoc["interactive_noun_data"]["actions"].GetArray()) {                          
        
        //  get the command from the object doc
        engine::CommandEnum command = gamedata::CommandEnum_Data::stringToEnum(action["command"].GetString());
                
        // add the action to the new Player          
        newPlayer->addAction(command, 
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
            newPlayer->InteractiveNoun::addVerbAlias(command, 
                                  alias["verb_alias"].GetString(), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["direct_object"].GetString()), 
                                  gamedata::GrammarSupport_Data::stringToEnum(alias["grammar"]["indirect_object"].GetString()), 
                                  prepMap );
        }       
    }    
    
    return newPlayer; 
}


std::string Player::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message = getName() + " is " + getDescription() + ".";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;

    resultMsg = getTextAndEffect(CommandEnum::LOOK, anEffect);
    if (resultMsg.compare("false") != 0){
        message += resultMsg;
    }
    if (anEffect != EffectType::NONE){
        effects->push_back(anEffect);
    }

    return message;
}  


std::string Player::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacters, std::vector<EffectType> *effects){
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
std::cout << "player take result = " << message << "\n";
    return message;
}


std::string Player::equip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string strSuccess, resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool success = false;

    if (anItem != nullptr){
        strSuccess = equipItem(anItem);

        if (strSuccess.compare("true") == 0){
            success = true;
        } else if (strSuccess.compare("false") == 0){
            message = "false";
        } else {
            success = true;
            message = "Unequipped the " + strSuccess + ". ";
        }
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Player::unequip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    bool success = false;

    if (anItem != nullptr){
        success = unequipItem(anItem);
    }

    if (success){
        resultMsg += getTextAndEffect(CommandEnum::EQUIP, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Player::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    bool success = false;
    EffectType anEffect = EffectType::NONE;

    if (anItem != nullptr){
        if (aPlayer == this){
            // item is being removed from this player
            success = removeFromInventory(anItem);
        } else if ((destination != nullptr) && (destination->getID() == this->getID())){
            // item is being added to this player
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


std::string Player::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;

    if (anArea != nullptr){
        setLocation(anArea);

        // if the player is in conversation, end the conversation
        if (getInConversation() != nullptr){
            setInConversation(nullptr);
        }

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


std::string Player::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool, std::vector<EffectType> *effects){
    std::string message = "";

    return message;
}


std::string Player::talk(Player *aPlayer, NonCombatant *aNPC, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;

    if (aNPC != nullptr){
        setInConversation(aNPC);

        resultMsg += getTextAndEffect(CommandEnum::TALK, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
} 

// would be better to have NonCombatant do their own selling **************************************************
std::string Player::buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    bool success = false;
    int price;
    EffectType anEffect = EffectType::NONE;

    if (anItem != nullptr){
        // check price  
        price = anItem->getType()->getCost();
        if (price > getMoney()){
            // can't afford the item
            message = "fYou don't have enough money to buy the " + anItem->getName() + ".";
            return message;
        } else {
            // buy the item
            aPlayer->subtractMoney(price);
            aPlayer->getInConversation()->addMoney(price);
            aPlayer->getInConversation()->removeFromInventory(anItem);
            aPlayer->addToInventory(anItem);
            success = true;
            message = "You bought the " + anItem->getName() + " for " + std::to_string(price) + " money.";
        }
    }

    if (success){
        resultMsg = getTextAndEffect(CommandEnum::BUY, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


// would be better to have NonCombatant do their own buying **************************************************
std::string Player::sell(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    bool success = false;
    int price;
    EffectType anEffect = EffectType::NONE;
    NonCombatant *aNPC = getInConversation();

    if (anItem != nullptr){
        // check price  
        price = anItem->getType()->getCost();
        if (price > aNPC->getMoney()){
            // NPC can't afford the item
            message = "f" + aNPC->getName() + " doesn't have enough money to buy the " + anItem->getName() + ".";
            return message;
        } else {
            // buy the item
            aNPC->subtractMoney(price);
            aPlayer->addMoney(price);
            aPlayer->removeFromInventory(anItem);
            aNPC->addToInventory(anItem);
            success = true;
            message = "You sold the " + anItem->getName() + " to " + aNPC->getName() + " for " + std::to_string(price) + " money.";
        }
    }

    if (success){
        resultMsg = getTextAndEffect(CommandEnum::BUY, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
    }

    return message;
}


std::string Player::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Player *aRecipient, std::vector<EffectType> *effects){
    std::string message, resultMsg;
    EffectType anEffect = EffectType::NONE;
    int healAmount = aSkill->getDamage();
    int skillCost = aSkill->getCost();

    if ((this == aPlayer) && (aRecipient != nullptr)){
        // this is the player using the skill
        if (getPlayerClass()->getSpecialSkill() == aSkill){
            // the specified skill can be used 
            if (getCurrentSpecialPts() >= skillCost){
                // the player has enough points to use the skill
                subtractFromCurrSpecialPts(skillCost);
                aRecipient->addToCurrentHealth(healAmount);
                message = "You used the " + aSkill->getName() + " skill on ";
                if (aPlayer == aRecipient){
                    message += "yourself.\015\012You gained ";
                } else {
                    message += aRecipient->getName() + ". They gained ";
                }
                message += std::to_string(healAmount) + " health.";

                // get the effects of useSkill
                resultMsg = getTextAndEffect(CommandEnum::USE_SKILL, anEffect);
                if (resultMsg.compare("false") != 0){
                    message += resultMsg;
                }
                if (anEffect != EffectType::NONE){
                    effects->push_back(anEffect);
                }
            } else {
                message = "fYou don't have enough special points to use the " + aSkill->getName() + " skill.";
            }
        } else {
            // the specified skill isn't available for this player to use
            message = "fYou can't use the " + aSkill->getName() + " skill.";
        }
    } else {
        // this is the recipient of the skill
        message = aPlayer->useSkill(aPlayer, aSkill, nullptr, this, effects);
    }

    return message;
} 


std::string Player::warp(Player *aPlayer, Area *anArea){
    std::string message = "";

    if (anArea != nullptr){
        setLocation(anArea);

        // if the player is in conversation, end the conversation
        if (getInConversation() != nullptr){
            setInConversation(nullptr);
        }
    }

    return message;
} 


InteractiveNoun* Player::copy(){
    return nullptr;
}


/*bool Player::editAttribute(Player *aPlayer, std::string){
    return false;
}*/


bool Player::editWizard(Player *aPlayer){
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
