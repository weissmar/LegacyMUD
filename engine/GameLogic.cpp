/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/11/2017
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


bool GameLogic::executeCommand(CommandEnum aCommand, Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, const std::string &stringParam, ItemPosition aPosition){

}


bool GameLogic::helpCommand(Player *aPlayer){

}


bool GameLogic::lookCommand(Player *aPlayer, InteractiveNoun *param){

}


bool GameLogic::listenCommand(Player *aPlayer){

}


bool GameLogic::takeCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){

}


bool GameLogic::putCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj, ItemPosition aPosition){

}


bool GameLogic::dropCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::inventoryCommand(Player *aPlayer){

}


bool GameLogic::moreCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::equipmentCommand(Player *aPlayer){

}


bool GameLogic::equipCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::unequipCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::transferCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){

}


bool GameLogic::speakCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::shoutCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::whisperCommand(Player *aPlayer, InteractiveNoun *indirectObj, const std::string &stringParam){

}


bool GameLogic::quitCommand(Player *aPlayer){

}


bool GameLogic::goCommand(Player *aPlayer, InteractiveNoun *param){

}


bool GameLogic::moveCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::statsCommand(Player *aPlayer){

}


bool GameLogic::questsCommand(Player *aPlayer){

}


bool GameLogic::skillsCommand(Player *aPlayer){

}


bool GameLogic::attackCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){

}


bool GameLogic::talkCommand(Player *aPlayer, InteractiveNoun *param){

}


bool GameLogic::shopCommand(Player *aPlayer){

}


bool GameLogic::buyCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::sellCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::searchCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::useSkillCommand(Player *aPlayer, InteractiveNoun *directObj, InteractiveNoun *indirectObj){

}


bool GameLogic::readCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::breakCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::climbCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::turnCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::pushCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::pullCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::eatCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::drinkCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::editModeCommand(Player *aPlayer){

} 


bool GameLogic::warpCommand(Player *aPlayer, InteractiveNoun *param){

}


bool GameLogic::copyCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::createCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::editAttributeCommand(Player *aPlayer, InteractiveNoun *directObj, const std::string &stringParam){

}


bool GameLogic::editWizardCommand(Player *aPlayer, InteractiveNoun *directObj){

}


bool GameLogic::saveCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::loadCommand(Player *aPlayer, const std::string &stringParam){

}


bool GameLogic::deleteCommand(Player *aPlayer, InteractiveNoun *directObj){

}

}}