/*!
  \file     WordManager.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the WordManager class.
*/

#include "WordManager.hpp"

#include <InteractiveNoun.hpp>

#include <algorithm>
#include <cctype>
#include <mutex>

#include <cassert>
#include <iostream>

namespace {

// Mutex for global verbs
std::mutex globalVerbLock;

// Mutex for builder verbs
std::mutex builderVerbLock;

// Mutex for edit mode verbs
std::mutex editModeVerbLock;

// Mutex for local nouns
std::mutex localNounsLock;

// Mutex for local verbs
std::mutex localVerbsLock;

// Mutex for ignore words
std::mutex ignoreWordLock;


// Helper function for adding an alias and VerbInfo pair to a GlobalVerbMap.
void addToMap(legacymud::parser::GlobalVerbMap &verbMap, std::string alias, legacymud::parser::VerbInfo info) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    // Add the alias-VerbInfo pair to the lookup table.
    verbMap.emplace(alias, info);
}

}

namespace legacymud { namespace parser {

// Static initialization
GlobalVerbMap WordManager::_editModeVerbs;
GlobalVerbMap WordManager::_globalVerbs;
GlobalVerbMap WordManager::_builderVerbs;
WordMap WordManager::_localVerbs;
WordMap WordManager::_localNouns;
std::set<std::string> WordManager::_ignoreWords;

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

    // Block any other threads from accessing _localNouns until operation is complete.
    std::lock_guard<std::mutex> guard(localNounsLock);

    // Add pair to WordMap
    if (!_localNouns.addWord(alias, pObj)) {
        std::cerr << "Attempted to add duplicate noun alias '" << alias << "' to object ID " << pObj->getID() << std::endl;
    }
}

// Adds an entry to the in-use verb alias lookup table.
void WordManager::addVerb(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: verify non-empty string
    assert(!alias.empty());
    // Precondition: verify non-null pointer
    assert(pObj != nullptr);

    // Block any other threads from accessing _localVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(localVerbsLock);

    // Add pair to WordMap
    if (!_localVerbs.addWord(alias, pObj)) {
        std::cerr << "Attempted to add duplicate verb alias '" << alias << "' to object ID " << pObj->getID() << std::endl;
    }
}

// Adds a word to the ignore list
void WordManager::addIgnoreWord(std::string word) {
    // Precondition: verify non-empty string
    assert(!word.empty());

    // Convert string to lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // Block any other threads from accessing _ignoreWords until operation is complete.
    std::lock_guard<std::mutex> guard(ignoreWordLock);

    _ignoreWords.insert(word);
}

// Gets the VerbInfos of the specified edit mode verb.
std::vector<VerbInfo> WordManager::getEditModeVerbs(std::string alias) {
    std::lock_guard<std::mutex> guard(editModeVerbLock);

    // Precondition: value must be in map
    assert(_editModeVerbs.find(alias) != _editModeVerbs.end());

    auto range = _editModeVerbs.equal_range(alias);
    std::vector<VerbInfo> results;
    for (auto it = range.first; it != range.second; ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets the VerbInfos of the specified global verb.
std::vector<VerbInfo> WordManager::getGlobalVerbs(std::string alias) {
    std::lock_guard<std::mutex> guard(globalVerbLock);

    // Precondition: value must be in map
    assert(_globalVerbs.find(alias) != _globalVerbs.end());

    auto range = _globalVerbs.equal_range(alias);
    std::vector<VerbInfo> results;
    for (auto it = range.first; it != range.second; ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets the VerbInfos of the specified builder verb.
std::vector<VerbInfo> WordManager::getBuilderVerbs(std::string alias) {
    std::lock_guard<std::mutex> guard(builderVerbLock);

    // Precondition: value must be in map
    assert(_builderVerbs.find(alias) != _builderVerbs.end());

    auto range = _builderVerbs.equal_range(alias);
    std::vector<VerbInfo> results;
    for (auto it = range.first; it != range.second; ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets pointers to local InteractiveNoun objects with the specified noun alias
std::vector<engine::InteractiveNoun *> WordManager::getLocalNouns(std::string alias) {
    std::lock_guard<std::mutex> guard(localNounsLock);
    return _localNouns.getObjects(alias);
}

// Gets whether the specified noun is in use.
bool WordManager::hasNoun(const void *, std::string alias) {
    std::lock_guard<std::mutex> guard(localNounsLock);
    return _localNouns.hasWord(alias);
}

// Gets whether the specified verb is in use.
bool WordManager::hasVerb(const void *, std::string alias) {
    std::lock_guard<std::mutex> guard(localVerbsLock);
    return _localVerbs.hasWord(alias);
}

// Gets whether the specified edit mode verb has been added.
bool WordManager::hasEditModeVerb(const void *, std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    std::lock_guard<std::mutex> guard(editModeVerbLock);
    return _editModeVerbs.find(alias) != _editModeVerbs.end();
}

// Gets whether the specified global verb has been added.
bool WordManager::hasGlobalVerb(const void *, std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    std::lock_guard<std::mutex> guard(globalVerbLock);
    return _globalVerbs.find(alias) != _globalVerbs.end();
}

// Gets whether the specified builder verb has been added.
bool WordManager::hasBuilderVerb(const void *, std::string alias) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    std::lock_guard<std::mutex> guard(builderVerbLock);
    return _builderVerbs.find(alias) != _builderVerbs.end();
}

// Gets whether the specified ignore word has been added.
bool WordManager::isIgnoreWord(const void *, std::string word) {
    // Convert string to lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    std::lock_guard<std::mutex> guard(ignoreWordLock);
    return _ignoreWords.find(word) != _ignoreWords.end();
}

// Removes a noun alias-InteractiveNoun pair from the noun alias lookup table.
void WordManager::removeNoun(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(localNounsLock);

    if (!_localNouns.removeWord(alias, pObj)) {
        std::cerr << "Attempted to remove unknown noun alias '" << alias << "' from object ID " << pObj->getID() << std::endl;        
    }
}

// Removes a verb alias-InteractiveNoun pair from the verb alias lookup table.
void WordManager::removeVerb(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(localVerbsLock);

    if (!_localVerbs.removeWord(alias, pObj)) {
        std::cerr << "Attempted to remove unknown verb alias '" << alias << "' from object ID " << pObj->getID() << std::endl;
    }

}

// Reset all member variables.
void WordManager::resetAll() {
    // Block any other threads from accessing private members until operation is complete.
    std::lock_guard<std::mutex> editModeVerbGuard(editModeVerbLock);
    std::lock_guard<std::mutex> globalVerbGuard(globalVerbLock);
    std::lock_guard<std::mutex> builderVerbGuard(builderVerbLock);
    std::lock_guard<std::mutex> localNounGuard(localNounsLock);
    std::lock_guard<std::mutex> localVerbGuard(localVerbsLock);
    std::lock_guard<std::mutex> ignoreWordGuard(ignoreWordLock);

    _editModeVerbs.clear();
    _globalVerbs.clear();
    _builderVerbs.clear();
    _localNouns.clear();
    _localVerbs.clear();
    _ignoreWords.clear();
}

}}
