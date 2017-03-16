/*********************************************************************//**
 * \author      Rachel Weissman-Hohler
 * \author      Keith Adkins (serializeJustInteractiveNoun function) 
 * \created     02/01/2017
 * \modified    03/16/2017
 * \course      CS467, Winter 2017
 * \file        InteractiveNoun.cpp
 *
 * \details     Implentation file for InteractiveNoun base class. 
 ************************************************************************/

#include "InteractiveNoun.hpp"
#include "Action.hpp"
#include "SpecialSkill.hpp"
#include "Player.hpp"
#include "Item.hpp"
#include "Area.hpp"
#include "NonCombatant.hpp"
#include "Combatant.hpp"
#include <WordManager.hpp>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>
#include <Grammar.hpp>  
#include <CommandEnum_Data.hpp>  
#include <EffectType_Data.hpp>  
#include <GrammarSupport_Data.hpp>  
#include <PrepositionType_Data.hpp> 

namespace legacymud { namespace engine {

std::atomic<int> InteractiveNoun::nextID {1};


int InteractiveNoun::getStaticID(){
    return nextID;
}


void InteractiveNoun::setStaticID(int num){
    nextID = num;
}


InteractiveNoun::InteractiveNoun(int anID) : ID(anID){

}


InteractiveNoun::InteractiveNoun(const InteractiveNoun &otherNoun) : ID(nextID++){
    std::unique_lock<std::mutex> aliasesLock(otherNoun.aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> actionsLock(otherNoun.actionsMutex, std::defer_lock);
    std::lock(aliasesLock, actionsLock);
    
    for (size_t i = 1; i < otherNoun.aliases.size(); i++){
        aliases.push_back(otherNoun.aliases[i]);
    }
        
    if (!otherNoun.actions.empty()){
        for (auto action : otherNoun.actions){
            actions.push_back(new Action(*action));
        }
    }
}


InteractiveNoun & InteractiveNoun::operator=(const InteractiveNoun &otherNoun){
    std::unique_lock<std::mutex> otherAliasesLock(otherNoun.aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> otherActionsLock(otherNoun.actionsMutex, std::defer_lock);
    std::unique_lock<std::mutex> aliasesLock(aliasesMutex, std::defer_lock);
    std::unique_lock<std::mutex> actionsLock(actionsMutex, std::defer_lock);
    std::lock(otherAliasesLock, otherActionsLock, aliasesLock, actionsLock);
    if (this != &otherNoun){
        for (auto action : actions){
            delete action;
        }
        actions.clear();

        for (size_t i = 1; i < otherNoun.aliases.size(); i++){
            aliases.push_back(otherNoun.aliases[i]);
        }

        if (!otherNoun.actions.empty()){
            for (auto action : otherNoun.actions){
                actions.push_back(new Action(*action));
            }
        }        
    }

    return *this;
}


bool InteractiveNoun::operator==(const InteractiveNoun &otherNoun) const{
    bool equal = true;

    if (this->getObjectType() != otherNoun.getObjectType()){
        equal = false;
    } else if (!this->compareObjects(otherNoun)){
        equal = false;
    } else if (!compareActionsAndAliases(otherNoun)){
        equal = false;
    }

    return equal;
}


InteractiveNoun::~InteractiveNoun(){
    for (auto action : actions){
        delete action;
    }
    actions.clear();
}


bool InteractiveNoun::compareActionsAndAliases(const InteractiveNoun &otherNoun) const{
    bool equal = true;
    std::vector<Action*> allActions = this->getAllActions();
    std::vector<Action*> allOtherActions = otherNoun.getAllActions();
    std::vector<std::string> allNounAliases = this->getNounAliases();
    std::vector<std::string> allOtherNounAliases = otherNoun.getNounAliases();

    if (allActions != allOtherActions){
        equal = false;
    } else if (allNounAliases.size() != allOtherNounAliases.size()){
        equal = false;
    } else {
        for (size_t i = 1; i < allNounAliases.size(); i++){
            if (allNounAliases[i] != allOtherNounAliases[i]){
                equal = false;
            }
        }
    }

    return equal;
}


bool InteractiveNoun::compareObjects(const InteractiveNoun &otherObject) const{
    if (getID() == otherObject.getID()){
        return true;
    } else {
        return false;
    }
}


int InteractiveNoun::getID() const{
    return ID;
}


Action* InteractiveNoun::getAction(CommandEnum aCommand) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            return actions[i];
        }
    }
    return nullptr;
}


std::vector<Action*> InteractiveNoun::getActions(std::string alias) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    std::vector<Action*> aliasActions;

    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->isAlias(alias) == true){
            aliasActions.push_back(actions[i]);
        }
    }

    return aliasActions;
}


std::vector<Action*> InteractiveNoun::getAllActions() const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);

    return actions;
}



std::vector<std::string> InteractiveNoun::getNounAliases() const{
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    return aliases;
}


std::vector<std::string> InteractiveNoun::getVerbAliases() const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    std::vector<std::string> verbAliases;
    std::vector<std::string> aliases;

    for (auto action : actions){
        aliases = action->getAliases();
        verbAliases.insert(verbAliases.end(), aliases.begin(), aliases.end());
    }

    return verbAliases;
}


bool InteractiveNoun::checkAction(CommandEnum aCommand) const{
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            return true;
        }
    }
    return false;
}


Action* InteractiveNoun::addAction(CommandEnum aCommand){
    if (!checkAction(aCommand)){
        std::lock_guard<std::mutex> actionsLock(actionsMutex);
        Action *anAction = new Action(aCommand);
        actions.push_back(anAction);
        return anAction;
    } else {
        return getAction(aCommand);
    }
}


Action* InteractiveNoun::addAction(CommandEnum aCommand, bool valid, std::string flavorText, EffectType effect){
    if (!checkAction(aCommand)){
        std::lock_guard<std::mutex> actionsLock(actionsMutex);
        Action *anAction = new Action(aCommand, valid, flavorText, effect);
        actions.push_back(anAction);
        return anAction;
    } else {
        return getAction(aCommand);
    }
}


bool InteractiveNoun::removeAction(CommandEnum aCommand){
    std::lock_guard<std::mutex> actionsLock(actionsMutex);
    int index = -1;

    for (size_t i = 0; i < actions.size(); i++){
        if (actions[i]->getCommand() == aCommand){
            index = i;
        }
    }

    if (index != -1){
        actions.erase(actions.begin() + index);
        return true;
    }

    return false;
}


bool InteractiveNoun::addNounAlias(std::string anAlias){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    bool found = false;

    for (size_t i = 0; i < aliases.size(); i++){
        if (aliases[i] == anAlias)
            found = true;
    }

    if (!found){
        aliases.push_back(anAlias);
        parser::WordManager::addNoun(anAlias, this);
        return true;
    }
    return false;
}


bool InteractiveNoun::removeNounAlias(std::string anAlias){
    std::lock_guard<std::mutex> aliasesLock(aliasesMutex);
    int index = -1;

    for (size_t i = 0; i < aliases.size(); i++){
        if (aliases[i] == anAlias)
            index = i;
    }

    if (index != -1){
        aliases.erase(aliases.begin() + index);
        parser::WordManager::removeNoun(anAlias, this);
        return true;
    }

    return false;
}


bool InteractiveNoun::addVerbAlias(CommandEnum aCommand, std::string alias, parser::Grammar::Support direct, parser::Grammar::Support indirect, std::map<std::string, parser::PrepositionType> prepositions){
    Action *anAction = getAction(aCommand);
    parser::Grammar *aGrammar;

    if (anAction != nullptr){
        if (prepositions.empty()){
            aGrammar = new parser::Grammar(direct, false, indirect);
        } else {
            aGrammar = new parser::Grammar(direct, true, indirect);
            for (auto prep : prepositions){
                aGrammar->addPreposition(prep.first, prep.second);
            }
        }
        anAction->addAlias(alias, aGrammar);
        parser::WordManager::addVerb(alias, this);
        return true;
    }

    return false;
}


bool InteractiveNoun::removeVerbAlias(CommandEnum aCommand, std::string alias){
    Action *anAction = getAction(aCommand);

    if (anAction != nullptr){
        anAction->removeAlias(alias);
        parser::WordManager::removeVerb(alias, this);
        return true;
    }
    return false;
}


std::string InteractiveNoun::getTextAndEffect(CommandEnum aCommand, EffectType &anEffect) const{
    std::string message = "false";
    Action *anAction = nullptr;

    anAction = this->getAction(aCommand);
    if (anAction != nullptr){
        if (anAction->getValid()){
            message = anAction->getFlavorText();
            if (!message.empty()){
                message += "\015\012";
            }
            anEffect = anAction->getEffect();
        } 
    }

    return message;
}


std::string InteractiveNoun::serializeJustInteractiveNoun() {
    rapidjson::StringBuffer buffer;  
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer); 
    
    writer.StartObject();
   
   // from InteractiveNoun class
    
    writer.String("id");                           // this object's id
    writer.Int(this->getID());  

    // noun alliases in InteractiveNoun
    std::vector<std::string> nounAliases = this->getNounAliases();                                                    
    writer.String("noun_aliases");                 // noun alliases

    writer.StartArray();
    for (size_t i = 0; i < nounAliases.size(); i++){
        writer.String(nounAliases[i].c_str());    
    }
    writer.EndArray();

    // actions in InteractiveNoun  
    std::vector<Action*> allActions = this->getAllActions();
    writer.String("actions");                       // actions
    writer.StartArray();
    for (size_t i = 0; i < allActions.size(); i++){
        writer.StartObject();
        writer.String("command");                   // command
        writer.String(gamedata::CommandEnum_Data::enumToString(allActions[i]->getCommand()).c_str());
        
        writer.String("valid");                         // valid
        writer.Bool(allActions[i]->getValid());                   

        writer.String("flavor_text");                   // flavor_text
        writer.String(allActions[i]->getFlavorText().c_str());

        writer.String("effect");                        // effect
        writer.String(gamedata::EffectType_Data::enumToString(allActions[i]->getEffect()).c_str());
        
        // from Grammar class and needed by InteractiveNoun
        writer.String("aliases");                         // aliases
        std::map<std::string, parser::Grammar*> allAliasesAndGrammerMap = allActions[i]->getAliasesAndGrammar();
        writer.StartArray();
        // loop over map getting all
        for (auto alias = allAliasesAndGrammerMap.begin(); alias != allAliasesAndGrammerMap.end(); alias++ ) {
            writer.StartObject();
            writer.String("verb_alias");             // verb alias
            writer.String(alias->first.c_str());            
            writer.String("grammar");                // grammer
            writer.StartObject();
            writer.String("direct_object");          // direct object 
            writer.String(gamedata::GrammarSupport_Data::enumToString(alias->second->takesDirectObject()).c_str() );
            writer.String("indirect_object");        // indirect_object            
            writer.String(gamedata::GrammarSupport_Data::enumToString(alias->second->takesIndirectObject()).c_str() );                     
            writer.String("preposition_map");        // preposition_map
            std::map<std::string, parser::PrepositionType> prepositionMap = alias->second->getAllPrepositions();
            writer.StartArray();
            for (auto prep = prepositionMap.begin(); prep != prepositionMap.end(); prep++ ) {
                writer.StartObject();
                writer.String("preposition");
                writer.String(prep->first.c_str());
                writer.String("preposition_type");      // preposition type
                writer.String(gamedata::PrepositionType_Data::enumToString(prep->second).c_str() );                
                writer.EndObject();
            }
            writer.EndArray();
            writer.EndObject();
            writer.EndObject();
        } 
        writer.EndArray();       
        writer.EndObject();
    } 
    writer.EndArray();   // end of stuff from InteractiveNounClass   
       
    writer.EndObject();
    
    return buffer.GetString();
}

}}