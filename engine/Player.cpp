/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/10/2017
 * \modified    02/10/2017
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

Player::Player(){

}


Player::Player(CharacterSize size, PlayerClass *aClass, std::string username, int FD, int maxHealth, Area *spawnLocation, int maxSpecialPts, std::string name, std::string description, int money, Area *aLocation, int maxInventoryWeight){

}


Player::Player(const Player &otherPlayer){

}


Player & Player::operator=(const Player &otherPlayer){

}


virtual Player::~Player(){

}


int Player::getExperiencePoints(){

}


int Player::getLevel(){

}


CharacterSize Player::getSize(){

}


std::string Player::getUser(){

}


PlayerClass* Player::getPlayerClass(){

}


NonCombatant* Player::getInConversation(){

}


bool Player::isActive(){

}


int Player::getFileDescriptor(){

}


bool Player::queueIsEmpty(){

}


bool Player::isEditMode(){

}


std::vector<std::tuple<Quest*, int>> Player::getQuestList(){

}


std::multimap<std::string, InteractiveNoun*> Player::getVerbLookup(){

}


std::multimap<std::string, InteractiveNoun*> Player::getNounLookup(){

}


int Player::addToExperiencePts(int gainedXP){

}


bool Player::levelUp(){

}


bool Player::setSize(CharacterSize size){

}


bool Player::setPlayerClass(PlayerClass *aClass){

}


bool Player::setActive(bool active){

} 


bool Player::setFileDescriptor(int FD){

}


bool Player::setInConversation(NonCombatant *anNPC){

}


Command* Player::getNextCommand(){

}


bool Player::addCommand(Command *aCommand){

}


bool Player::setEditMode(bool editing){

}


bool Player::addQuest(Quest *aQuest, int step){

}


bool Player::updateQuest(Quest *aQuest, int step){

} 


bool Player::addVerbs(std::vector<std::string>, InteractiveNoun*){

}


bool Player::addNouns(std::vector<std::string>, InteractiveNoun*){

}


bool Player::removeVerbs(InteractiveNoun*){

}


bool Player::removeNouns(InteractiveNoun*){

}


virtual ObjectType Player::getObjectType(){

}


virtual std::string Player::serialize(){

}


virtual bool Player::deserialize(std::string){

}


virtual std::string Player::look(){

}  


virtual bool Player::take(Player*, Item*, InteractiveNoun*){

}


virtual bool Player::equip(Player*, Item*, InteractiveNoun*){

}


virtual bool Player::unequip(Player*, Item*, InteractiveNoun*){

}


virtual bool Player::transfer(Player*, Item*, InteractiveNoun*, InteractiveNoun*){

}


virtual bool Player::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


virtual bool Player::attack(Player*, Item*, SpecialSkill*, InteractiveNoun*, bool){

}


virtual std::string Player::talk(Player*, NonCombatant*){

} 


virtual bool Player::buy(Player*, Item*){

}


virtual bool Player::sell(Player*, Item*){

}


virtual std::string Player::search(Player*){

} 


virtual std::string Player::useSkill(Player *aPlayer, SpecialSkill *aSkill, InteractiveNoun *character, Combatant *aRecipient, bool playerSkill){

} 


virtual std::string Player::warp(Player*, Area*){

} 


virtual InteractiveNoun* Player::copy(){

}


virtual bool Player::editAttribute(Player*, std::string){

}


virtual bool Player::editWizard(Player*){

}


static std::map<std::string, DataType> Player::getAttributeSignature(){

}

}}