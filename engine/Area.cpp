/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/09/2017
 * \course      CS467, Winter 2017
 * \file        Area.cpp
 *
 * \details     Implementation file for Area class. 
 ************************************************************************/

#include "Area.hpp"
#include "Character.hpp"
#include "Exit.hpp"
#include "Item.hpp"
#include "Feature.hpp"

namespace legacymud { namespace engine {

Area::Area(){

}


Area::Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size){

}


Area::Area(const Area &otherArea){

}


Area & Area::operator=(const Area &otherArea){

}


Area::~Area(){

}


std::string Area::getName(){

} 


std::string Area::getShortDesc(){

}


std::string Area::getLongDesc(){

}


AreaSize Area::getSize(){

}


std::vector<Item*> Area::getItems(){

}


std::vector<Character*> Area::getCharacters(){

}


std::vector<Feature*> Area::getFeatures(){

}


std::vector<Exit*> Area::getExits(){

}


std::multimap<std::string, InteractiveNoun*> Area::getVerbLookup(){

}


std::multimap<std::string, InteractiveNoun*> Area::getNounLookup(){

}


bool Area::setName(std::string name){

}


bool Area::setShortDesc(std::string shortDescription){

}


bool Area::setLongDesc(std::string longDescription){

}


bool Area::setSize(AreaSize size){

}


bool Area::addItem(Item *anItem){

}


bool Area::addCharacter(Character *aCharacter){

}


bool Area::addFeature(Feature *aFeature){

}


bool Area::addExit(Exit *anExit){

}


bool Area::removeItem(Item *anItem){

}


bool Area::removeCharacter(Character *aCharacter){

}


bool Area::removeFeature(Feature *aFeature){

}


bool Area::removeExit(Exit *anExit){

}


ObjectType Area::getObjectType(){

}


std::string Area::serialize(){

}


bool Area::deserialize(std::string){

}


std::string Area::look(){

}  


std::string Area::listen(){

} 


bool Area::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){

}


std::string Area::search(Player *aPlayer){

} 


std::string Area::warp(Player *aPlayer, Area *anArea){

} 


InteractiveNoun* Area::copy(){

}


bool Area::editAttribute(Player *aPlayer, std::string attribute){

}


bool Area::editWizard(Player *aPlayer){

}


std::map<std::string, DataType> Area::getAttributeSignature(){

}

}}