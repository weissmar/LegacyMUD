/*!
  \file    WordManager.cpp
  \author  David Rigert
  \date    02/04/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the WordManager class.
*/

#include "WordManager.hpp"
#include "Grammar.hpp"

#include <algorithm>
#include <cctype>
#include <mutex>

#include <cassert>

namespace {

// Mutex for global verbs
std::mutex globalVerbLock;

// Mutex for builder verbs
std::mutex builderVerbLock;

// Mutex for edit mode verbs
std::mutex editModeVerbLock;

// Mutex for verbs
std::mutex verbLock;

// Mutex for nouns
std::mutex nounLock;

// Helper function for adding an alias and VerbInfo pair to a GlobalVerbMap.
void addToMap(legacymud::parser::GlobalVerbMap &verbMap, std::string alias, legacymud::parser::VerbInfo info) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Add the verb-action pair to the list (or overwrite the existing one).
    verbMap[alias] = info;
}

}

namespace legacymud { namespace parser {

// Static initialization
GlobalVerbMap WordManager::_editModeVerbs;
GlobalVerbMap WordManager::_globalVerbs;
GlobalVerbMap WordManager::_builderVerbs;
AliasLookupTable WordManager::_nounAliases;
AliasLookupTable WordManager::_verbAliases;

// Adds an entry to the list of global verbs.
void WordManager::addGlobalVerb(std::string alias, VerbInfo info) {
    // Precondition: verify non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _globalVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(globalVerbLock);

    addToMap(_globalVerbs, alias, info);
}

// Adds an entry to the list of world builder verbs.
void WordManager::addBuilderVerb(std::string alias, VerbInfo info) {
    // Precondition: verify non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _builderVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(builderVerbLock);

    addToMap(_builderVerbs, alias, info);
}

// Adds an entry to the list of edit mode verbs.
void WordManager::addEditModeVerb(std::string alias, VerbInfo info) {
    // Precondition: verify non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _editModeVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(editModeVerbLock);

    addToMap(_editModeVerbs, alias, info);
}

// Adds an entry to the in-use noun alias lookup table.
void WordManager::addNoun(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: verify non-empty string
    assert(!alias.empty());
    // Precondition: verify non-null pointer
    assert(pObj != nullptr);

    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(nounLock);

    // Add pair to multimap
    _nounAliases.emplace(alias, pObj);
}

// Adds an entry to the in-use verb alias lookup table.
void WordManager::addVerb(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: verify non-empty string
    assert(!alias.empty());
    // Precondition: verify non-null pointer
    assert(pObj != nullptr);

    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(verbLock);

    // Increment the use count in the map.
    // This works even if the verb is not in the map, because unsigned int is value initialized to 0.
    _verbAliases.emplace(alias, pObj);
}

// Gets the VerbInfo of the specified edit mode verb.
VerbInfo WordManager::getEditModeVerb(std::string alias) {
    // Precondition: value must be in map
    auto it = _editModeVerbs.find(alias);
    assert(it != _editModeVerbs.end());

    return _editModeVerbs.at(alias);
}

// Gets the VerbInfo of the specified global verb.
VerbInfo WordManager::getGlobalVerb(std::string alias) {
    // Precondition: value must be in map
    auto it = _globalVerbs.find(alias);
    assert(it != _globalVerbs.end());

    return _globalVerbs.at(alias);
}

// Gets the VerbInfo of the specified builder verb.
VerbInfo WordManager::getBuilderVerb(std::string alias) {
    // Precondition: value must be in map
    auto it = _builderVerbs.find(alias);
    assert(it != _builderVerbs.end());

    return _builderVerbs.at(alias);
}

// Gets whether the specified noun is in use.
bool WordManager::hasNoun(std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _nounAliases.find(alias) != _nounAliases.end();
}

// Gets whether the specified verb is in use.
bool WordManager::hasVerb(std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _verbAliases.find(alias) != _verbAliases.end();
}

// Gets whether the specified edit mode verb has been added.
bool WordManager::hasEditModeVerb(std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _editModeVerbs.find(alias) != _editModeVerbs.end();
}

// Gets whether the specified global verb has been added.
bool WordManager::hasGlobalVerb(std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _globalVerbs.find(alias) != _globalVerbs.end();
}

// Gets whether the specified builder verb has been added.
bool WordManager::hasBuilderVerb(std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _builderVerbs.find(alias) != _builderVerbs.end();
}

// Removes a noun alias-InteractiveNoun pair from the noun alias lookup table.
void WordManager::removeNoun(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(nounLock);

    auto it = _nounAliases.find(alias);
    // Remove first match found
    for (; it != _nounAliases.end(); ++it) {
        if (it->second == pObj) {
            _nounAliases.erase(it);
            return;
        }
    }

    // Precondition: alias-pointer pair is in lookup table
    assert(it != _nounAliases.end());
}

// Removes a verb alias-InteractiveNoun pair from the verb alias lookup table.
void WordManager::removeVerb(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(verbLock);

    auto it = _verbAliases.find(alias);
    // Remove first match found
    for (; it != _verbAliases.end(); ++it) {
        if (it->second == pObj) {
            _verbAliases.erase(it);
            return;
        }
    }

    // Precondition: alias-pointer pair is in lookup table
    assert(it != _verbAliases.end());
}

// Reset all member variables.
void WordManager::resetAll() {
    // Block any other threads from accessing private members until operation is complete.
    std::lock_guard<std::mutex> editModeVerbGuard(editModeVerbLock);
    std::lock_guard<std::mutex> globalVerbGuard(globalVerbLock);
    std::lock_guard<std::mutex> builderVerbGuard(builderVerbLock);
    std::lock_guard<std::mutex> nounGuard(nounLock);
    std::lock_guard<std::mutex> verbGuard(verbLock);

    _editModeVerbs.clear();
    _globalVerbs.clear();
    _builderVerbs.clear();
    _nounAliases.clear();
    _verbAliases.clear();
}

}}
