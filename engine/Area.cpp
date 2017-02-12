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

Area::Area()
: SerializableNoun()
, InteractiveNoun()
, UniqueNoun()
, name("")
, shortDescription("")
, longDescription("")
, size(AreaSize::SMALL)
{ }


Area::Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size)
: SerializableNoun()
, InteractiveNoun()
, UniqueNoun()
, name(name)
, shortDescription(shortDescription)
, longDescription(longDescription)
, size(size)
{ }


/*Area::Area(const Area &otherArea){

}


Area & Area::operator=(const Area &otherArea){

}


Area::~Area(){

}*/


std::string Area::getName(){
    return name;
} 


std::string Area::getShortDesc(){
    return shortDescription;
}


std::string Area::getLongDesc(){
    return longDescription;
}


AreaSize Area::getSize(){
    return size;
}


std::vector<Item*> Area::getItems(){
    return itemContents;
}


std::vector<Character*> Area::getCharacters(){
    return characterContents;
}


std::vector<Feature*> Area::getFeatures(){
    return featureContents;
}


std::vector<Exit*> Area::getExits(){
    return exitContents;
}


std::multimap<std::string, InteractiveNoun*> Area::getVerbLookup(){
    return verbLookup;
}


std::multimap<std::string, InteractiveNoun*> Area::getNounLookup(){
    return nounLookup;
}


bool Area::setName(std::string name){
    return false;
}


bool Area::setShortDesc(std::string shortDescription){
    return false;
}


bool Area::setLongDesc(std::string longDescription){
    return false;
}


bool Area::setSize(AreaSize size){
    return false;
}


bool Area::addItem(Item *anItem){
    return false;
}


bool Area::addCharacter(Character *aCharacter){
    return false;
}


bool Area::addFeature(Feature *aFeature){
    return false;
}


bool Area::addExit(Exit *anExit){
    return false;
}


bool Area::removeItem(Item *anItem){
    return false;
}


bool Area::removeCharacter(Character *aCharacter){
    return false;
}


bool Area::removeFeature(Feature *aFeature){
    return false;
}


bool Area::removeExit(Exit *anExit){
    return false;
}


ObjectType Area::getObjectType(){
    return ObjectType::AREA;
}


std::string Area::serialize(){
    return "";
}


bool Area::deserialize(std::string){
    return false;
}


std::string Area::look(){
    return "";
}  


std::string Area::listen(){
    return "";
} 


bool Area::go(Player *aPlayer, Area *anArea, InteractiveNoun *character){
    return false;
}


std::string Area::search(Player *aPlayer){
    return "";
} 


std::string Area::warp(Player *aPlayer, Area *anArea){
    return "";
} 


InteractiveNoun* Area::copy(){
    return nullptr;
}


bool Area::editAttribute(Player *aPlayer, std::string attribute){
    return false;
}


bool Area::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Area::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["name"] = DataType::STRING_TYPE;
    signature["short description"] = DataType::STRING_TYPE;
    signature["long description"] = DataType::STRING_TYPE;
    signature["size"] = DataType::AREA_SIZE;

    return signature;
}

}}