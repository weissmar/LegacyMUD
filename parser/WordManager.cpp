/*!
  \file    WordManager.h
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the WordManager class.
*/

#include "WordManager.h"

#include <algorithm>
#include <cctype>
#include <mutex>

#include <cassert>

namespace {

// Mutex for global verbs
std::mutex globalVerbLock;

// Mutex for builder verbs
std::mutex builderVerbLock;

// Mutex for verbs
std::mutex verbLock;

// Mutex for nouns
std::mutex nounLock;

}

namespace legacymud { namespace parser {

// Static initialization
std::map<std::string, Verb> WordManager::_globalVerbs;
std::map<std::string, Verb> WordManager::_builderVerbs;
std::map<std::string, unsigned int> WordManager::_nounAliases;
std::map<std::string, unsigned int> WordManager::_verbAliases;

// Adds an entry to the list of global verbs.
void WordManager::addGlobalVerb(std::string name, Verb verb) {
    // Precondition: verify non-empty string
    assert(!name.empty());

    // Convert string to lowercase
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // Block any other threads from accessing _globalVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(globalVerbLock);

    // Add the verb-action pair to the list (or overwrite the existing one).
    _globalVerbs[name] = verb;
}

// Adds an entry to the list of world builder verbs.
void WordManager::addBuilderVerb(std::string name, Verb verb) {
    // Precondition: verify non-empty string
    assert(!name.empty());

    // Convert string to lowercase
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);

    // Block any other threads from accessing _globalVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(builderVerbLock);

    // Add the verb-action pair to the list (or overwrite the existing one).
    _builderVerbs[name] = verb;
}

// Adds an entry to the list of noun aliases in use.
void WordManager::addNoun(std::string noun) {
    // Precondition: verify non-empty string
    assert(!noun.empty());

    // Convert string to lowercase
    std::transform(noun.begin(), noun.end(), noun.begin(), ::tolower);

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(nounLock);

    // Increment the use count in the map.
    // This works even if the noun is not in the map, because unsigned int is value initialized to 0.
    _nounAliases[noun]++;
}

// Adds a list of noun aliases to the list of noun aliases in use.
void WordManager::addNouns(const std::list<std::string> &nouns) {
    for (auto it = nouns.begin(); it != nouns.end(); ++it) {
        // Just call the single word version repeatedly
        addNoun(*it);
    }
}

// Adds an entry to the list of verb aliases in use.
void WordManager::addVerb(std::string verb) {
    // Precondition: verify non-empty string
    assert(!verb.empty());

    // Convert string to lowercase
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(verbLock);

    // Increment the use count in the map.
    // This works even if the verb is not in the map, because unsigned int is value initialized to 0.
    _verbAliases[verb]++;
}

// Adds a list of verb aliases to the list of verb aliases in use.
void WordManager::addVerbs(const std::list<std::string> &verbs) {
    for (auto it = verbs.begin(); it != verbs.end(); ++it) {
        // Just call the single word version repeatedly
        addVerb(*it);
    }
}

// Gets the Verb of the specified global verb.
Verb WordManager::getGlobalVerb(std::string verb) {
    // Precondition: value must be in map
    auto it = _globalVerbs.find(verb);
    assert(it != _globalVerbs.end());

    return _globalVerbs.at(verb);
}

// Gets the Verb of the specified builder verb.
Verb WordManager::getBuilderVerb(std::string verb) {
    // Precondition: value must be in map
    auto it = _builderVerbs.find(verb);
    assert(it != _builderVerbs.end());

    return _builderVerbs.at(verb);
}

// Gets whether the specified noun is in use.
bool WordManager::hasNoun(std::string noun) {
    // Convert string to lowercase
    std::transform(noun.begin(), noun.end(), noun.begin(), ::tolower);

    return _nounAliases.find(noun) != _nounAliases.end();
}

// Gets whether the specified verb is in use.
bool WordManager::hasVerb(std::string verb) {
    // Convert string to lowercase
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);

    return _verbAliases.find(verb) != _verbAliases.end();
}

// Gets whether the specified global verb has been added.
bool WordManager::hasGlobalVerb(std::string verb) {
    // Convert string to lowercase
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);

    return _globalVerbs.find(verb) != _globalVerbs.end();
}

// Gets whether the specified builder verb has been added.
bool WordManager::hasBuilderVerb(std::string verb) {
    // Convert string to lowercase
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);

    return _builderVerbs.find(verb) != _builderVerbs.end();
}

// Removes one use of a noun from the list of noun aliases in use.
void WordManager::removeNoun(std::string noun) {
    // Precondition: noun is non-empty string
    assert(!noun.empty());

    // Convert string to lowercase
    std::transform(noun.begin(), noun.end(), noun.begin(), ::tolower);

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(nounLock);

    // Precondition: noun exists in _nounAliases
    auto it = _nounAliases.find(noun);
    assert(it != _nounAliases.end());
    assert(it->second > 0);
    
    // Decrement count
    it->second--;
    
    // Remove if 0
    if (it->second == 0) {
      _nounAliases.erase(it);
    }
}

// Removes one use of each string from the list of noun aliases in use.
void WordManager::removeNouns(const std::list<std::string> &nouns) {
    for (auto it = nouns.begin(); it != nouns.end(); ++it) {
        // Just call the single word version repeatedly
        removeNoun(*it);
    }
}

// Removes one use of a verb from the list of verb aliases in use.
void WordManager::removeVerb(std::string verb) {
    // Precondition: verb is non-empty string
    assert(!verb.empty());

    // Convert string to lowercase
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(verbLock);

    // Precondition: verb exists in _verbAliases
    auto it = _verbAliases.find(verb);
    assert(it != _verbAliases.end());
    assert(it->second > 0);
    
    // Decrement count
    it->second--;
    
    // Remove if 0
    if (it->second == 0) {
      _verbAliases.erase(it);
    }
}

// Removes one use of each string from the list of verb aliases in use.
void WordManager::removeVerbs(const std::list<std::string> &verbs) {
    for (auto it = verbs.begin(); it != verbs.end(); ++it) {
        // Just call the single word version repeatedly
        removeVerb(*it);
    }
}

// Reset all member variables.
void WordManager::resetAll() {
    // Block any other threads from accessing private members until operation is complete.
    std::lock_guard<std::mutex> globalVerbGuard(globalVerbLock);
    std::lock_guard<std::mutex> builderVerbGuard(builderVerbLock);
    std::lock_guard<std::mutex> nounGuard(nounLock);
    std::lock_guard<std::mutex> verbGuard(verbLock);

    _globalVerbs.clear();
    _builderVerbs.clear();
    _nounAliases.clear();
    _verbAliases.clear();
}

}}
