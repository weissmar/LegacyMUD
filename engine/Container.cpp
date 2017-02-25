/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \created     02/09/2017
 * \modified    02/25/2017
 * \course      CS467, Winter 2017
 * \file        Container.cpp
 *
 * \details     Implementation file for Container class. 
 ************************************************************************/

#include "Container.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Area.hpp"

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
    return false;
}


bool Container::place(Item *anItem, ItemPosition position){
    bool success = false; 

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
        // ************************************************************** figure out when to add/remove contents aliases
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


bool Container::deserialize(std::string){
    return false;
}


std::string Container::look(std::vector<EffectType> *effects){
    return "";
}  


std::string Container::take(Player *aPlayer, Item *anItem, InteractiveNoun *aContainer, InteractiveNoun *aCharacter, std::vector<EffectType> *effects){
    std::string message = "";
    EffectType anEffect = EffectType::NONE;
    Area *anArea = nullptr;
    ItemPosition position = getPosition();
    InteractiveNoun *location = getLocation();

    if (this == dynamic_cast<Container*>(aContainer)){
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
        message = getTextAndEffect(CommandEnum::TAKE, anEffect);
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


std::string Container::put(Player *aPlayer, Item *anItem, InteractiveNoun*, ItemPosition position, std::vector<EffectType> *effects){
    return "";
}


std::string Container::drop(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
}


std::string Container::more(std::vector<EffectType> *effects){
    return "";
} 


std::string Container::equip(Player *aPlayer, Item *anItem, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Container::unequip(Player *aPlayer, Item *anItem, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Container::transfer(Player *aPlayer, Item *anItem, InteractiveNoun*, InteractiveNoun*, std::vector<EffectType> *effects){
    return "";
}


std::string Container::move(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
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


std::string Container::read(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::breakIt(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::climb(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::turn(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::push(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::pull(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::eat(Player *aPlayer, std::vector<EffectType> *effects){
    return "";
} 


std::string Container::drink(Player *aPlayer, std::vector<EffectType> *effects){
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
