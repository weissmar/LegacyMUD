/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/12/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include <cstdlib>
#include "GameLogic.hpp"
#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "NonCombatant.hpp"

namespace legacymud { namespace engine {

GameLogic::GameLogic(){
    manager = new GameObjectManager;
}


GameLogic::GameLogic(const GameLogic &otherGameLogic){
    manager = new GameObjectManager(*otherGameLogic.manager);
}


GameLogic & GameLogic::operator=(const GameLogic &otherGameLogic){
    if (this != &otherGameLogic){
        delete manager;
        manager = new GameObjectManager(*otherGameLogic.manager);
    }

    return *this;
}


GameLogic::~GameLogic(){
    delete manager;
}


bool GameLogic::startGame(bool newGame, const std::string &fileName){
    return false;
}


bool GameLogic::newPlayerHandler(int fileDescriptor){
    return false;
}


bool GameLogic::processInput(int numToProcess){
    // lock for thread-safety
    std::lock_guard<std::mutex> lockGuard(queueMutex);
    std::pair<std::string, int> aMessage;

    for(int i = 0; i < numToProcess; i++){
        // get the next message
        aMessage = messageQueue.front();
        messageQueue.pop();

        // get pointer to player the message is from

        // TO DO: check if message is expected by a wizard

        // send message to parser

    }
    return false;
}


bool GameLogic::receivedMessageHandler(std::string message, int fileDescriptor){
    // lock for thread-safety
    std::lock_guard<std::mutex> lockGuard(queueMutex);

    messageQueue.push(std::make_pair(message, fileDescriptor));

    return true;
}


bool GameLogic::updateCreatures(){
    return false;
}


bool GameLogic::updatePlayersInCombat(){
    return false;
}


bool GameLogic::loadPlayer(Player *aPlayer, int fileDescriptor){
    return false;
}


bool GameLogic::hibernatePlayer(Player *aPlayer){
    return false;
}


int GameLogic::rollDice(int numSides, int numDice){
    int diceTotal = 0;

    for (int i = 0; i < numDice; i++){
        diceTotal += (std::rand() % numSides) + 1;
    }

    return diceTotal;
}


bool GameLogic::createObject(Player *aPlayer, ObjectType type){
    return false;
}


ObjectType GameLogic::getObjectType(const std::string &input){
    ObjectType type = ObjectType::NONE;

    if ((input.compare("Area") == 0) || (input.compare("AREA") == 0) || (input.compare("area") == 0))
        type = ObjectType::AREA;
    // TO DO: Finish this

    return type;
}


bool GameLogic::messagePlayer(Player *aPlayer, std::string message){
    return false;
}


bool GameLogic::startCombat(Player* aPlayer, Creature *aCreature){
    return false;
}


bool GameLogic::endCombat(Player *aPlayer, Creature *aCreature){
    return false;
}


bool GameLogic::startConversation(Player *aPlayer, NonCombatant *aNPC){
    return false;
}


bool GameLogic::endConversation(Player *aPlayer){
    return false;
}


bool GameLogic::executeCommand(CommandEnum aCommand, Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, const std::string &stringParam, ItemPosition aPosition){
    return false;
}


bool GameLogic::helpCommand(Player *aPlayer){
    return false;
}


bool GameLogic::lookCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::listenCommand(Player *aPlayer){
    return false;
}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::putCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, ItemPosition aPosition){
    return false;
}


bool GameLogic::dropCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::inventoryCommand(Player *aPlayer){
    return false;
}


bool GameLogic::moreCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::equipmentCommand(Player *aPlayer){
    return false;
}


bool GameLogic::equipCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::unequipCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::transferCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::speakCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *indirectObj, const std::string &stringParam){
    return false;
}


bool GameLogic::quitCommand(Player *aPlayer){
    return false;
}


bool GameLogic::goCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::moveCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::statsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::questsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::skillsCommand(Player *aPlayer){
    return false;
}


bool GameLogic::attackCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::talkCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::shopCommand(Player *aPlayer){
    return false;
}


bool GameLogic::buyCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::sellCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::searchCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::useSkillCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){
    return false;
}


bool GameLogic::readCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::breakCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::climbCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::turnCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::pushCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::pullCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::eatCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::drinkCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::editModeCommand(Player *aPlayer){
    return false;
} 


bool GameLogic::warpCommand(Player *aPlayer, InteractiveNoun *param){
    return false;
}


bool GameLogic::copyCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::createCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::editAttributeCommand(Player *aPlayer, InteractiveNoun *directObj, const std::string &stringParam){
    return false;
}


bool GameLogic::editWizardCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}


bool GameLogic::saveCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::loadCommand(Player *aPlayer, const std::string &stringParam){
    return false;
}


bool GameLogic::deleteCommand(Player *aPlayer, InteractiveNoun *directObj){
    return false;
}

}}