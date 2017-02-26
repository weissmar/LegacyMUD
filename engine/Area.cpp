/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/08/2017
 * \modified    02/25/2017
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
#include "CommandEnum.hpp"
#include "Action.hpp"
#include "Container.hpp"
#include <iostream>
#include <algorithm>

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
{ 
    addNounAlias(name);
}


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


const parser::LexicalData& Area::getLexicalData() const{
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
    return contentsLexicalData;
}


std::string Area::getFullDescription(int excludeID) const{
    std::string message = getLongDesc();
    std::vector<Item*> allItems = getItems();
    std::vector<Character*> allCharacters = getCharacters();
    std::vector<Feature*> allFeatures = getFeatures();
    std::vector<Exit*> allExits = getExits();

    message += "\015\012";
    
    for (auto feature : allFeatures){
        message += "You see a ";
        message += feature->getName();
        message += " ";
        message += feature->getPlacement();
        message += ".\015\012";
    }

    for (auto exit : allExits){
        message += exit->getDirectionString();
        message += " you see ";
        message += exit->getName();
        message += ".\015\012";
    }

    if (allItems.size() == 1){
        message += "You see a ";
        message += allItems[0]->getName();
        message += " on the ground.";
    } else if (allItems.size() > 1){
        message += "Around you, you see a ";
        for (size_t i = 0; i < allItems.size(); i++){
            message += allItems[i]->getName();
            if (i == (allItems.size() - 2)){
                message += " and a ";
            } else if (i == (allItems.size() - 1)){
                message += ".";
            } else {
                message += ", a ";    
            }
        }
    }
    message += "\015\012";
    

    for (size_t i = 0; i < allCharacters.size(); i++){
        if (allCharacters[i]->getID() != excludeID){
            if (allCharacters[i]->getObjectType() == ObjectType::NON_COMBATANT){
                message += "You see someone named ";
            } else if (allCharacters[i]->getObjectType() == ObjectType::CREATURE){
                message += "You see a creature named ";
            } else if (allCharacters[i]->getObjectType() == ObjectType::PLAYER){
                message += "You see a player named ";
            }
            message += allCharacters[i]->getName();
            message += ".\015\012";
        }
    }

    return message;
}


bool Area::setName(std::string name){
    std::lock_guard<std::mutex> nameLock(nameMutex);
    removeNounAlias(this->name);
    addNounAlias(name);
    this->name = name;
    return true;
}


bool Area::setShortDesc(std::string shortDescription){
    std::lock_guard<std::mutex> shortDescLock(shortDescMutex);
    this->shortDescription = shortDescription;
    return true;
}


bool Area::setLongDesc(std::string longDescription){
    std::lock_guard<std::mutex> longDescLock(longDescMutex);
    this->longDescription = longDescription;
    return true;
}


bool Area::setSize(AreaSize size){
    this->size.store(size);
    return true;
}

// would be best to remove dynamic_cast ********************************************************
bool Area::addItem(Item *anItem){
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
        itemContents.push_back(anItem);
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

        return true;
    }
    return false;
}


bool Area::addCharacter(Character *aCharacter){
    if (aCharacter != nullptr){
        std::lock_guard<std::mutex> charContentLock(charContentMutex);
        characterContents.push_back(aCharacter);
        addAllLexicalData(aCharacter);
        return true;
    }
    return false;
}


bool Area::addFeature(Feature *aFeature){
    if (aFeature != nullptr){
        std::lock_guard<std::mutex> featContentLock(featContentMutex);
        featureContents.push_back(aFeature);
        addAllLexicalData(aFeature);
        return true;
    }
    return false;
}


bool Area::addExit(Exit *anExit){
    if (anExit != nullptr){
        std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
        exitContents.push_back(anExit);
        addAllLexicalData(anExit);
        return true;
    }
    return false;
}


void Area::addAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            std::cout << "noun alias to add to area: " << noun << "\n";
            contentsLexicalData.addNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            contentsLexicalData.addVerb(verb, anObject);
        }
    }
    
}


void Area::removeAllLexicalData(InteractiveNoun *anObject){
    std::vector<std::string> nounAliases, verbAliases;

    if (anObject != nullptr){
        std::lock_guard<std::mutex> lexicalLock(lexicalMutex);
        nounAliases = anObject->getNounAliases();
        verbAliases = anObject->getVerbAliases();

        for (auto noun : nounAliases){
            std::cout << "noun alias to remove from area: " << noun << "\n";
            contentsLexicalData.removeNoun(noun, anObject);
        }
        for (auto verb : verbAliases){
            contentsLexicalData.removeVerb(verb, anObject);
        }
    }
}


bool Area::removeItem(Item *anItem){
    Container *aContainer = nullptr;
    std::vector<Item*> contents;

    if (anItem != nullptr){
        std::lock_guard<std::mutex> itemContentLock(itemContentMutex);
        itemContents.erase(std::remove(itemContents.begin(), itemContents.end(), anItem), itemContents.end());
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
        
        return true;
    }
    return false;
}


bool Area::removeCharacter(Character *aCharacter){
    if (aCharacter != nullptr){
        std::lock_guard<std::mutex> charContentLock(charContentMutex);
        characterContents.erase(std::remove(characterContents.begin(), characterContents.end(), aCharacter), characterContents.end());
        removeAllLexicalData(aCharacter);
        return true;
    }
    return false;
}


bool Area::removeFeature(Feature *aFeature){
    if (aFeature != nullptr){
        std::lock_guard<std::mutex> featContentLock(featContentMutex);
        featureContents.erase(std::remove(featureContents.begin(), featureContents.end(), aFeature), featureContents.end());
        removeAllLexicalData(aFeature);
        return true;
    }
    return false;
}


bool Area::removeExit(Exit *anExit){
    if (anExit != nullptr){
        std::lock_guard<std::mutex> exitContentLock(exitContentMutex);
        exitContents.erase(std::remove(exitContents.begin(), exitContents.end(), anExit), exitContents.end());
        removeAllLexicalData(anExit);
        return true;
    }
    return false;
}


bool Area::addNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.addNoun(alias, this);
    success = InteractiveNoun::addNounAlias(alias);

    return success;
}


bool Area::removeNounAlias(std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.removeNoun(alias, this);
    success = InteractiveNoun::removeNounAlias(alias);

    return success;
}


bool Area::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.addVerb(alias, this);
    success = InteractiveNoun::addVerbAlias(aCommand, alias, direct, indirect, prepositions);

    return success;
}


bool Area::removeVerbAlias(CommandEnum aCommand, std::string alias){
    bool success = false;

    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    contentsLexicalData.removeVerb(alias, this);
    success = InteractiveNoun::removeVerbAlias(aCommand, alias);

    return success;
}


bool Area::registerAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            contentsLexicalData.addVerb(alias, anObject);
        } else {
            std::cout << "noun alias to add to area: " << alias << "\n";
            contentsLexicalData.addNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


bool Area::unregisterAlias(bool isVerb, std::string alias, InteractiveNoun *anObject){
    std::lock_guard<std::mutex> lexicalLock(lexicalMutex);

    if (anObject != nullptr){
        if (isVerb){
            contentsLexicalData.removeVerb(alias, anObject);
        } else {
            std::cout << "noun alias to remove from area: " << alias << "\n";
            contentsLexicalData.removeNoun(alias, anObject);
        }
        return true;
    }
    return false;
}


ObjectType Area::getObjectType() const{
    return ObjectType::AREA;
}


std::string Area::serialize(){
    return "";
}


bool Area::deserialize(std::string){
    return false;
}


std::string Area::look(std::vector<EffectType> *effects){
    std::string message = getFullDescription(-1);
    EffectType anEffect = EffectType::NONE;

    message += " ";

    message += getTextAndEffect(CommandEnum::LOOK, anEffect);

    if ((anEffect != EffectType::NONE) && (effects != nullptr)){
        effects->push_back(anEffect);
    }

    return message;
}  


std::string Area::listen(std::vector<EffectType> *effects){
    std::string message;
    EffectType anEffect;

    message = getTextAndEffect(CommandEnum::LISTEN, anEffect);

    if ((anEffect != EffectType::NONE) && (effects != nullptr)){
        effects->push_back(anEffect);
    }

    std::unique_lock<std::mutex> featContentLock(featContentMutex, std::defer_lock);
    std::unique_lock<std::mutex> exitContentLock(exitContentMutex, std::defer_lock);
    std::lock(featContentLock, exitContentLock);

    for (auto feature : featureContents){
        message += feature->listen(effects);
        message += " ";
    }
    for (auto exit : exitContents){
        message += exit->listen(effects);
        message += " ";
    }

    return message;
} 


std::string Area::go(Player *aPlayer, Area *anArea, InteractiveNoun *character, std::vector<EffectType> *effects){
    return "";
}


std::string Area::search(Player *aPlayer, std::vector<EffectType> *effects){
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