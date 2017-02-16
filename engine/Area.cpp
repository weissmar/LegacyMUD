/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/15/2017
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
: InteractiveNoun()
, name("")
, shortDescription("")
, longDescription("")
, size(AreaSize::SMALL)
{ }


Area::Area(std::string name, std::string shortDescription, std::string longDescription, AreaSize size)
: InteractiveNoun()
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


std::string Area::getName() const{
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return name;
} 


std::string Area::getShortDesc() const{
    std::lock_guard<std::mutex> shortDescLock(shortDescMutex);
    return shortDescription;
}


std::string Area::getLongDesc() const{
    std::lock_guard<std::mutex> longDescLock(longDescMutex);
    return longDescription;
}


AreaSize Area::getSize() const{
    return size.load();
}


std::vector<Item*> Area::getItems() const{
    std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
    return itemContents;
}


std::vector<Character*> Area::getCharacters() const{
    std::lock_guard<std::mutex> charContentLock(charContentMutex);
    return characterContents;
}


std::vector<Feature*> Area::getFeatures() const{
    std::lock_guard<std::mutex> featContentLock(featContentMutex);
    return featureContents;
}


std::vector<Exit*> Area::getExits() const{
    std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
    return exitContents;
}


parser::LexicalData Area::getLexicalData() const{
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
    return contentsLexicalData;
}


std::string Area::getFullDescription() const{
    return getLongDesc();
}


bool Area::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    return false;
}


bool Area::setShortDesc(std::string shortDescription){
    std::lock_guard<std::mutex> shortDescLock(shortDescMutex);
    return false;
}


bool Area::setLongDesc(std::string longDescription){
    std::lock_guard<std::mutex> longDescLock(longDescMutex);
    return false;
}


bool Area::setSize(AreaSize size){
    this->size.store(size);
    return true;
}


bool Area::addItem(Item *anItem){
    std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
    itemContents.push_back(anItem);
    return true;
}


bool Area::addCharacter(Character *aCharacter){
    std::lock_guard<std::mutex> charContentLock(charContentMutex);
    characterContents.push_back(aCharacter);
    return true;
}


bool Area::addFeature(Feature *aFeature){
    std::lock_guard<std::mutex> featContentLock(featContentMutex);
    featureContents.push_back(aFeature);
    return true;
}


bool Area::addExit(Exit *anExit){
    std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
    exitContents.push_back(anExit);
    return true;
}


bool Area::removeItem(Item *anItem){
    std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
    return false;
}


bool Area::removeCharacter(Character *aCharacter){
    std::lock_guard<std::mutex> charContentLock(charContentMutex);
    return false;
}


bool Area::removeFeature(Feature *aFeature){
    std::lock_guard<std::mutex> featContentLock(featContentMutex);
    return false;
}


bool Area::removeExit(Exit *anExit){
    std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
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