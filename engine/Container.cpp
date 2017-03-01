/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    03/01/2017
 * \course      CS467, Winter 2017
 * \file        Container.cpp
 *
 * \details     Implementation file for Container class. 
 ************************************************************************/

#include "Container.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Area.hpp"
#include "ItemType.hpp"
#include <algorithm>

namespace legacymud { namespace engine {

const int DEFAULT_CAPACITY = 10;

Container::Container()
: Item()
, insideCapacity(DEFAULT_CAPACITY)
{ }


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type)
: Item(location, position, name, type)
, insideCapacity(capacity)
{ }


Container::Container(int capacity, InteractiveNoun* location, ItemPosition position, std::string name, ItemType *type, int anID)
: Item(location, position, name, type, anID)
, insideCapacity(capacity)
{ }


/*Container::Container(const Container &otherContainer){ 

}


Container & Container::operator=(const Container &otherContainer){

}


Container::~Container(){

}*/


bool Container::isEmpty() const{
    std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
    std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
    std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
    std::lock(underLock, insideLock, onTopOfLock);

    return under.empty() && inside.empty() && onTopOf.empty();
}

bool Container::isContained(Item *anItem) const{
    if (anItem != nullptr){
        std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
        std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
        std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
        std::lock(underLock, insideLock, onTopOfLock);

        for (auto item : under){
            if (item == anItem){
                return true;
            }
        }
        for (auto item : inside){
            if (item == anItem){
                return true;
            }
        }
        for (auto item : onTopOf){
            if (item == anItem){
                return true;
            }
        }
    }
    return false;
}

bool Container::remove(Item *anItem){
    bool success = false; 
    Container *aContainer;
    InteractiveNoun *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;
    ItemPosition position;

    if (anItem != nullptr){
        position = anItem->getPosition();
        if (position == ItemPosition::IN){
            std::lock_guard<std::mutex> insideLock(insideMutex);
            inside.erase(std::remove(inside.begin(), inside.end(), anItem), inside.end());
            success = true;
        } else if (position == ItemPosition::ON){
            std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
            onTopOf.erase(std::remove(onTopOf.begin(), onTopOf.end(), anItem), onTopOf.end());
            success = true;
        } else if (position == ItemPosition::UNDER){
            std::lock_guard<std::mutex> underLock(underMutex);
            under.erase(std::remove(under.begin(), under.end(), anItem), under.end());
            success = true;
        }
    }

    if (success){
        while ((location->getObjectType() != ObjectType::PLAYER) && (location->getObjectType() != ObjectType::AREA)){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                location = aContainer->getLocation();
            } else {
                return false;
            }
        }

        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            location->unregisterAlias(false, noun, anItem);
        }
        for (auto verb : verbAliases){
            location->unregisterAlias(true, verb, anItem);
        }
    }

    return success;
}

// would be best to remove dynamic casts ***********************************************************
// should check capacity ****************************************
bool Container::place(Item *anItem, ItemPosition position){
    bool success = false; 
    Container *aContainer;
    InteractiveNoun *location = getLocation();
    std::vector<std::string> nounAliases, verbAliases;

    if (anItem != nullptr){
        if (position == ItemPosition::IN){
            std::lock_guard<std::mutex> insideLock(insideMutex);
            inside.push_back(anItem);
            success = true;
        } else if (position == ItemPosition::ON){
            std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
            onTopOf.push_back(anItem);
            success = true;
        } else if (position == ItemPosition::UNDER){
            std::lock_guard<std::mutex> underLock(underMutex);
            under.push_back(anItem);
            success = true;
        }
    }

    if (success){
        while ((location->getObjectType() != ObjectType::PLAYER) && (location->getObjectType() != ObjectType::AREA)){
            aContainer = dynamic_cast<Container*>(location);
            if (aContainer != nullptr){
                location = aContainer->getLocation();
            } else {
                return false;
            }
        }

        nounAliases = anItem->getNounAliases();
        verbAliases = anItem->getVerbAliases();

        for (auto noun : nounAliases){
            location->registerAlias(false, noun, anItem);
        }
        for (auto verb : verbAliases){
            location->registerAlias(true, verb, anItem);
        }
    }

    return success;
}


std::vector<Item*> Container::getInsideContents() const{
    std::lock_guard<std::mutex> insideLock(insideMutex);
    return inside;
}


std::vector<Item*> Container::getUnderContents() const{
    std::lock_guard<std::mutex> underLock(underMutex);
    return under;
}


std::vector<Item*> Container::getTopContents() const{
    std::lock_guard<std::mutex> onTopOfLock(onTopOfMutex);
    return onTopOf;
}

std::vector<Item*> Container::getAllContents() const{
    std::unique_lock<std::mutex> onTopOfLock(onTopOfMutex, std::defer_lock);
    std::unique_lock<std::mutex> underLock(underMutex, std::defer_lock);
    std::unique_lock<std::mutex> insideLock(insideMutex, std::defer_lock);
    std::lock(onTopOfLock, underLock, insideLock);

    std::vector<Item*> allContents;
    allContents.insert(allContents.end(), inside.begin(), inside.end());
    allContents.insert(allContents.end(), under.begin(), under.end());
    allContents.insert(allContents.end(), onTopOf.begin(), onTopOf.end());

    return allContents;
}


int Container::getInsideCapacity() const{
    return insideCapacity.load();
}


bool Container::setInsideCapacity(int capacity){
    insideCapacity.store(capacity);

    return true;
}


ObjectType Container::getObjectType() const{
    return ObjectType::CONTAINER;
}


std::string Container::serialize(){
    return "";
}


InteractiveNoun* Container::deserialize(std::string){
    return nullptr; 
}


std::string Container::look(Player *aPlayer, std::vector<EffectType> *effects){
    std::string message;
    ItemType *aType = getType();

    message = "The " + getName() + " is ";
    message += aType->getDescription() + ".";
    message += " It looks like it might contain something.";

    return message;
}  

// would be best to remove dynamic casts ***********************************************************
std::string Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect = EffectType::NONE;
    Area *anArea = nullptr;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

    if ((aContainer != nullptr) && (this->getID() == aContainer->getID())){
        // this is the containing object
        if (isContained(anItem)){
            remove(anItem);
        }
    } else {
        // this is the item being taken

        // check if this item is contained within a container
        if ((position == ItemPosition::IN) || (position == ItemPosition::ON) || (position == ItemPosition::UNDER)){
            if ((location == aContainer) && (aContainer != nullptr)){
                aContainer->take(nullptr, this, aContainer, nullptr, nullptr);
            } else {
                return "false";
            }
        } else if (position == ItemPosition::GROUND) {
            // location is an area
            anArea = dynamic_cast<Area*>(location);
            if (anArea != nullptr){
                // remove item from area
                anArea->removeItem(this);
            } else {
                return "false";
            }
        } else {
            return "false";
        }

        setPosition(ItemPosition::INVENTORY);
        // get results of take for this object
        resultMsg = getTextAndEffect(CommandEnum::TAKE, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }

        // call this function on player or character, and container
        if (aCharacter != nullptr){
            // aCharacter is doing the taking
            setLocation(aCharacter);
            message += aCharacter->take(nullptr, this, nullptr, aCharacter, effects);
        } else {
            // aPlayer is doing the taking
            setLocation(aPlayer);
            message += aPlayer->take(aPlayer, this, nullptr, nullptr, effects);
        }
    }

    return message;
}

// would be best to remove dynamic casts ***********************************************************
std::string Container::put(Player *aPlayer, Item *anItem, InteractiveNoun *containingNoun, ItemPosition position, std::vector<EffectType> *effects){
    Area *anArea;
    InteractiveNoun *location;
    ItemPosition currPosition;
    std::string message = "";
    std::string resultMsg;
    EffectType anEffect;

    if (anItem != nullptr){
        // this is the containing item
        place(anItem, position);
    } else {
        // this is the item being placed
        location = getLocation();
        currPosition = getPosition();

        // check if this item is contained within a container
        if ((currPosition == ItemPosition::IN) || (currPosition == ItemPosition::ON) || (currPosition == ItemPosition::UNDER)){
            return "false";
        } else if (currPosition == ItemPosition::GROUND) {
            // location is an area
            anArea = dynamic_cast<Area*>(location);
            if (anArea != nullptr){
                // remove item from area
                anArea->removeItem(this);
            } else {
                return "false";
            }
        } else if ((currPosition == ItemPosition::INVENTORY) || (currPosition == ItemPosition::EQUIPPED)){
            // location is a character
            if (location->getID() == aPlayer->getID()){
                aPlayer->removeFromInventory(this);
            } else {
                return "false";
            }
        } else {
            return "false";
        }

        setPosition(position);
        // get results of put for this object
        resultMsg = getTextAndEffect(CommandEnum::PUT, anEffect);
        if (resultMsg.compare("false") != 0){
            message += resultMsg;
        }
        if (anEffect != EffectType::NONE){
            effects->push_back(anEffect);
        }
        // call this function on containingNoun
        if (containingNoun != nullptr){
            setLocation(containingNoun);
            message += containingNoun->put(aPlayer, this, nullptr, position, effects);
        } 
    }

    return message;
}


std::string Container::more(Player *aPlayer){
    std::string message = Item::more(aPlayer);

    message += "Inside Capacity: " + std::to_string(getInsideCapacity()) + "\015\012";
    
    return message;
} 


std::string Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = Item::equip(aPlayer, anItem, aCharacter, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun *aCharacter, InteractiveNoun *destination, std::vector<EffectType> *effects){
    std::string message = Item::transfer(aPlayer, anItem, aCharacter, destination, effects);

    if (message.compare("false") != 0){
        // remove contents? *********************************************************************
    }

    return message;
}


std::string Container::attack(Player *aPlayer, Item *anItem, SpecialSkill*, InteractiveNoun*, bool playerAttacker, std::vector<EffectType> *effects){
    return "";
}


std::string Container::buy(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    return "";
}


std::string Container::sell(Player *aPlayer, Item *anItem, std::vector<EffectType> *effects){
    return "";
}


std::string Container::search(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


InteractiveNoun* Container::copy(){
    return nullptr;
}


bool Container::editAttribute(Player *aPlayer, std::string){
    return false;
}


bool Container::editWizard(Player *aPlayer){
    return false;
}


std::map<std::string, DataType> Container::getAttributeSignature(){
    std::map<std::string, DataType> signature;

    signature["capacity"] = DataType::INT_TYPE;
    signature["location"] = DataType::INTERACTIVE_NOUN_PTR;
    signature["position"] = DataType::ITEM_POSITION;
    signature["name"] = DataType::STRING_TYPE;
    signature["description"] = DataType::STRING_TYPE;
    signature["type"] = DataType::ITEM_TYPE_PTR;

    return signature;
}

}}
