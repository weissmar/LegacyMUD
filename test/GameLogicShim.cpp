/*!
  \file     GameLogicShim.hpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the GameLogicShim class.
*/

#include "GameLogicShim.hpp"

namespace legacymud { namespace test {

GameLogicShim::GameLogicShim(engine::GameLogic *logic) {
    _logic = logic;
}

int GameLogicShim::validateStringNumber(std::string number, int min, int max){
    return _logic->validateStringNumber(number, min, max);
}

bool GameLogicShim::getValueFromUser(int FD, std::string outMessage, std::string &response){
    return _logic->getValueFromUser(FD, outMessage, response);
}

bool GameLogicShim::loadPlayer(engine::Player *aPlayer, int fileDescriptor){
    return _logic->loadPlayer(aPlayer, fileDescriptor);
}

bool GameLogicShim::hibernatePlayer(engine::Player *aPlayer){
    return _logic->hibernatePlayer(aPlayer);
}

int GameLogicShim::rollDice(int numSides, int numDice){
    return _logic->rollDice(numSides, numDice);
}

bool GameLogicShim::createObject(engine::Player *aPlayer, engine::ObjectType type){
    return _logic->createObject(aPlayer, type);
}

engine::ObjectType GameLogicShim::getObjectType(const std::string &input){
    return _logic->getObjectType(input);
}

void GameLogicShim::messagePlayer(engine::Player *aPlayer, std::string message){
    _logic->messagePlayer(aPlayer, message);
}

void GameLogicShim::messageAllPlayers(engine::Player *aPlayer, std::string message){
    _logic->messageAllPlayers(aPlayer, message);
}

void GameLogicShim::messageAreaPlayers(engine::Player *aPlayer, std::string message, engine::Area *anArea){
    _logic->messageAreaPlayers(aPlayer, message, anArea);
}

bool GameLogicShim::startCombat(engine::Player* aPlayer, engine::Creature *aCreature){
    return _logic->startCombat(aPlayer, aCreature);
}

bool GameLogicShim::endCombat(engine::Player *aPlayer, engine::Creature *aCreature){
    return _logic->endCombat(aPlayer, aCreature);
}

bool GameLogicShim::startConversation(engine::Player *aPlayer, engine::NonCombatant *aNPC){
    return _logic->startConversation(aPlayer, aNPC);
}

bool GameLogicShim::endConversation(engine::Player *aPlayer){
    return _logic->endConversation(aPlayer);
}

void GameLogicShim::handleParseError(engine::Player *aPlayer, parser::ParseResult result){
    _logic->handleParseError(aPlayer, result);
}

void GameLogicShim::printParseResult(parser::ParseResult result){
    _logic->printParseResult(result);
}

void GameLogicShim::handleParseErrorMult(engine::Player *aPlayer, std::vector<parser::ParseResult> results){
    _logic->handleParseErrorMult(aPlayer, results);
}

void GameLogicShim::sendClarifyingQuery(engine::Player *aPlayer, std::vector<engine::InteractiveNoun*> optionsVector){
    _logic->sendClarifyingQuery(aPlayer, optionsVector);
}

engine::InteractiveNoun* GameLogicShim::clarifyChoice(engine::Player *aPlayer, std::vector<engine::InteractiveNoun*> optionsVector){
    return _logic->clarifyChoice(aPlayer, optionsVector);
}

std::string GameLogicShim::blockingGetMsg(engine::Player *aPlayer){
    return _logic->blockingGetMsg(aPlayer);
}

void GameLogicShim::addPlayerMessageQueue(engine::Player *aPlayer){
    _logic->addPlayerMessageQueue(aPlayer);
}

void GameLogicShim::removePlayerMessageQueue(engine::Player *aPlayer){
    _logic->removePlayerMessageQueue(aPlayer);
}

std::string GameLogicShim::getMsgFromPlayerQ(engine::Player *aPlayer){
    return _logic->getMsgFromPlayerQ(aPlayer);
}

bool GameLogicShim::executeCommand(engine::Player *aPlayer, parser::ParseResult result){
    return _logic->executeCommand(aPlayer, result);
}

engine::InteractiveNoun* GameLogicShim::clarifyDirect(engine::Player *aPlayer, parser::ParseResult result){
    return _logic->clarifyDirect(aPlayer, result);
}

engine::InteractiveNoun* GameLogicShim::clarifyIndirect(engine::Player *aPlayer, parser::ParseResult result){
    return _logic->clarifyIndirect(aPlayer, result);
}

bool GameLogicShim::helpCommand(engine::Player *aPlayer){
    return _logic->helpCommand(aPlayer);
}

std::string GameLogicShim::handleEffects(engine::Player *aPlayer, std::vector<engine::EffectType> effects){
    return _logic->handleEffects(aPlayer, effects);
}

bool GameLogicShim::lookCommand(engine::Player *aPlayer, engine::InteractiveNoun *param){
    return _logic->lookCommand(aPlayer, param);
}

bool GameLogicShim::listenCommand(engine::Player *aPlayer){
    return _logic->listenCommand(aPlayer);
}

bool GameLogicShim::takeCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj){
    return _logic->takeCommand(aPlayer, directObj, indirectObj);
}

bool GameLogicShim::putCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj, engine::ItemPosition aPosition){
    return _logic->putCommand(aPlayer, directObj, indirectObj, aPosition);
}

bool GameLogicShim::dropCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->dropCommand(aPlayer, directObj);
}

bool GameLogicShim::inventoryCommand(engine::Player *aPlayer){
    return _logic->inventoryCommand(aPlayer);
}

bool GameLogicShim::moreCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->moreCommand(aPlayer, directObj);
}

bool GameLogicShim::equipmentCommand(engine::Player *aPlayer){
    return _logic->equipmentCommand(aPlayer);
}

bool GameLogicShim::equipCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->equipCommand(aPlayer, directObj);
}

bool GameLogicShim::unequipCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->unequipCommand(aPlayer, directObj);
}

bool GameLogicShim::transferCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj){
    return _logic->transferCommand(aPlayer, directObj, indirectObj);
}

bool GameLogicShim::speakCommand(engine::Player *aPlayer, const std::string &stringParam){
    return _logic->speakCommand(aPlayer, stringParam);
}

bool GameLogicShim::shoutCommand(engine::Player *aPlayer, const std::string &stringParam){
    return _logic->shoutCommand(aPlayer, stringParam);
}

bool GameLogicShim::whisperCommand(engine::Player *aPlayer, engine::InteractiveNoun *indirectObj, const std::string &stringParam){
    return _logic->whisperCommand(aPlayer, indirectObj, stringParam);
}

bool GameLogicShim::quitCommand(engine::Player *aPlayer){
    return _logic->quitCommand(aPlayer);
}

bool GameLogicShim::goCommand(engine::Player *aPlayer, engine::InteractiveNoun *param){
    return _logic->goCommand(aPlayer, param);
}

bool GameLogicShim::moveCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->moveCommand(aPlayer, directObj);
}

bool GameLogicShim::statsCommand(engine::Player *aPlayer){
    return _logic->statsCommand(aPlayer);
}

bool GameLogicShim::questsCommand(engine::Player *aPlayer){
    return _logic->questsCommand(aPlayer);
}

bool GameLogicShim::skillsCommand(engine::Player *aPlayer){
    return _logic->skillsCommand(aPlayer);
}

bool GameLogicShim::attackCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj){
    return _logic->attackCommand(aPlayer, directObj, indirectObj);
}

bool GameLogicShim::talkCommand(engine::Player *aPlayer, engine::InteractiveNoun *param){
    return _logic->talkCommand(aPlayer, param);
}

bool GameLogicShim::shopCommand(engine::Player *aPlayer){
    return _logic->shopCommand(aPlayer);
}

bool GameLogicShim::buyCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->buyCommand(aPlayer, directObj);
}

bool GameLogicShim::sellCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->sellCommand(aPlayer, directObj);
}

bool GameLogicShim::searchCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->searchCommand(aPlayer, directObj);
}

bool GameLogicShim::useSkillCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, engine::InteractiveNoun *indirectObj){
    return _logic->useSkillCommand(aPlayer, directObj, indirectObj);
}

bool GameLogicShim::readCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->readCommand(aPlayer, directObj);
}

bool GameLogicShim::breakCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->breakCommand(aPlayer, directObj);
}

bool GameLogicShim::climbCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->climbCommand(aPlayer, directObj);
}

bool GameLogicShim::turnCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->turnCommand(aPlayer, directObj);
}

bool GameLogicShim::pushCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->pushCommand(aPlayer, directObj);
}

bool GameLogicShim::pullCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->pullCommand(aPlayer, directObj);
}

bool GameLogicShim::eatCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->eatCommand(aPlayer, directObj);
}

bool GameLogicShim::drinkCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->drinkCommand(aPlayer, directObj);
}

bool GameLogicShim::editModeCommand(engine::Player *aPlayer){
    return _logic->editModeCommand(aPlayer);
} 

bool GameLogicShim::warpCommand(engine::Player *aPlayer, engine::InteractiveNoun *param){
    return _logic->warpCommand(aPlayer, param);
}

bool GameLogicShim::copyCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->copyCommand(aPlayer, directObj);
}

bool GameLogicShim::createCommand(engine::Player *aPlayer, const std::string &stringParam){
    return _logic->createCommand(aPlayer, stringParam);
}

bool GameLogicShim::editAttributeCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj, const std::string &stringParam){
    return _logic->editAttributeCommand(aPlayer, directObj, stringParam);
}

bool GameLogicShim::editWizardCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->editWizardCommand(aPlayer, directObj);
}

bool GameLogicShim::saveCommand(engine::Player *aPlayer, const std::string &stringParam){
    return _logic->saveCommand(aPlayer, stringParam);
}

bool GameLogicShim::loadCommand(engine::Player *aPlayer, const std::string &stringParam){
    return _logic->loadCommand(aPlayer, stringParam);
}

bool GameLogicShim::deleteCommand(engine::Player *aPlayer, engine::InteractiveNoun *directObj){
    return _logic->deleteCommand(aPlayer, directObj);
}

}}