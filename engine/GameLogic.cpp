/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
 * \course      CS467, Winter 2017
 * \file        GameLogic.cpp
 *
 * \details     Implementation file for GameLogic class. 
 ************************************************************************/

#include "GameLogic.hpp"
#include "GameObjectManager.hpp"
#include "InteractiveNoun.hpp"
#include "Player.hpp"
#include "Creature.hpp"
#include "NonCombatant.hpp"

namespace legacymud { namespace engine {

GameLogic::GameLogic(){

}


GameLogic::GameLogic(const GameLogic &otherGameLogic){

}


GameLogic & GameLogic::operator=(const GameLogic &otherGameLogic){

}


GameLogic::~GameLogic(){

}


bool GameLogic::startGame(bool newGame){

}


bool GameLogic::startGame(bool newGame, const std::string &fileName){

}


bool GameLogic::newPlayerHandler(int fileDescriptor){

}


bool GameLogic::processInput(int numToProcess){

}


bool GameLogic::receivedMessageHandler(std::string message, int fileDescriptor){

}


bool GameLogic::updateCreatures(){

}


bool GameLogic::updatePlayersInCombat(){

}


bool GameLogic::loadPlayer(Player *aPlayer, int fileDescriptor){

}


bool GameLogic::hibernatePlayer(Player *aPlayer){

}


int GameLogic::rollDice(int numSides, int numDice){

}


bool GameLogic::createObject(Player *aPlayer, ObjectType type){

}


ObjectType GameLogic::getObjectType(const std::string &input){

}


bool GameLogic::messagePlayer(Player *aPlayer, std::string message){

}


bool GameLogic::startCombat(Player* aPlayer, Creature *aCreature){

}


bool GameLogic::endCombat(Player *aPlayer, Creature *aCreature){

}


bool GameLogic::startConversation(Player *aPlayer, NonCombatant *aNPC){

}


bool GameLogic::endConversation(Player *aPlayer){

}


bool GameLogic::executeCommand(CommandEnum aCommand, Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam, const std::string &stringParam, ItemPosition aPosition = none){

}


bool GameLogic::helpCommand(Player *aPlayer){

}


bool GameLogic::lookCommand(Player *aPlayer){

}


bool GameLogic::listenCommand(Player *aPlayer){

}


bool GameLogic::lookAtCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam){

}


bool GameLogic::putCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam, ItemPosition aPosition){

}


bool GameLogic::dropCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::inventoryCommand(Player *aPlayer){

}


bool GameLogic::moreCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::equipmentCommand(Player *aPlayer){

}


bool GameLogic::equipCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam){

}


bool GameLogic::unequipCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam){

}


bool GameLogic::transferCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam){

}


bool GameLogic::speakCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *firstParam, const std::string &stringParam){

}


bool GameLogic::quitCommand(Player *aPlayer){

}


bool GameLogic::goCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::moveCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::statsCommand(Player *aPlayer){

}


bool GameLogic::questsCommand(Player *aPlayer){

}


bool GameLogic::skillsCommand(Player *aPlayer){

}


bool GameLogic::attackCommand(Player *aPlayer, InteractiveNoun *firstParam, InteractiveNoun *secondParam){

}


bool GameLogic::talkCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::shopCommand(Player *aPlayer){

}


bool GameLogic::buyCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::sellCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::searchCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::useSkillCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::readCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::breakCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::climbCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::turnCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::pushCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::pullCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::eatCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::drinkCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::editModeCommand(Player *aPlayer){

} 


bool GameLogic::warpCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::copyCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::createCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::editAttributeCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::editAttributeCommand(Player *aPlayer, InteractiveNoun *firstParam, const std::string &stringParam){

}


bool GameLogic::editWizardCommand(Player *aPlayer, InteractiveNoun *firstParam){

}


bool GameLogic::saveCommand(Player *aPlayer){

}


bool GameLogic::saveCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::loadCommand(Player *aPlayer){

}


bool GameLogic::loadCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::deleteCommand(Player *aPlayer, InteractiveNoun *firstParam){

}

}}