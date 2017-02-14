/*!
  \file     WordManager.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/07/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the WordManager class.
*/

#include "WordManager.hpp"

#include <algorithm>
#include <cctype>
#include <mutex>

#include <cassert>

namespace {

// Mutex for global verbs
std::mutex globallocalWordsLock;

// Mutex for builder verbs
std::mutex builderlocalWordsLock;

// Mutex for edit mode verbs
std::mutex editModelocalWordsLock;

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
    std::lock_guard<std::mutex> guard(globallocalWordsLock);

    addToMap(_globalVerbs, alias, info);
}

// Adds an entry to the list of world builder verbs.
void WordManager::addBuilderVerb(std::string alias, VerbInfo info) {
    // Precondition: verify non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _builderVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(builderlocalWordsLock);

    addToMap(_builderVerbs, alias, info);
}

// Adds an entry to the list of edit mode verbs.
void WordManager::addEditModeVerb(std::string alias, VerbInfo info) {
    // Precondition: verify non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _editModeVerbs until operation is complete.
    std::lock_guard<std::mutex> guard(editModelocalWordsLock);

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
    _localNouns.addWord(alias, pObj);
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
    _localVerbs.addWord(alias, pObj);
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
    // Precondition: value must be in map
    auto it = _editModeVerbs.find(alias);

    assert(it != _editModeVerbs.end());

    std::vector<VerbInfo> results;
    for (; it != _editModeVerbs.end(); ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets the VerbInfos of the specified global verb.
std::vector<VerbInfo> WordManager::getGlobalVerbs(std::string alias) {
    // Precondition: value must be in map
    auto it = _globalVerbs.find(alias);
    assert(it != _globalVerbs.end());

    std::vector<VerbInfo> results;
    for (; it != _globalVerbs.end(); ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets the VerbInfos of the specified builder verb.
std::vector<VerbInfo> WordManager::getBuilderVerbs(std::string alias) {
    // Precondition: value must be in map
    auto it = _builderVerbs.find(alias);
    assert(it != _builderVerbs.end());

    std::vector<VerbInfo> results;
    for (; it != _builderVerbs.end(); ++it) {
        results.push_back(it->second);
    }

    return results;
}

// Gets whether the specified noun is in use.
bool WordManager::hasNoun(std::string alias) {
    return _localNouns.hasWord(alias);
}

// Gets whether the specified verb is in use.
bool WordManager::hasVerb(std::string alias) {
    return _localVerbs.hasWord(alias);
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

// Gets whether the specified ignore word has been added.
bool WordManager::isIgnoreWord(std::string word) {
    // Convert string to lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    return _ignoreWords.find(word) != _ignoreWords.end();
}

// Removes a noun alias-InteractiveNoun pair from the noun alias lookup table.
void WordManager::removeNoun(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _nounAliases until operation is complete.
    std::lock_guard<std::mutex> guard(localNounsLock);

    assert(_localNouns.removeWord(alias, pObj));
}

// Removes a verb alias-InteractiveNoun pair from the verb alias lookup table.
void WordManager::removeVerb(std::string alias, engine::InteractiveNoun *pObj) {
    // Precondition: alias is non-empty string
    assert(!alias.empty());

    // Block any other threads from accessing _verbAliases until operation is complete.
    std::lock_guard<std::mutex> guard(localVerbsLock);

    assert(_localVerbs.removeWord(alias, pObj));
}

// Reset all member variables.
void WordManager::resetAll() {
    // Block any other threads from accessing private members until operation is complete.
    std::lock_guard<std::mutex> editModeVerbGuard(editModelocalWordsLock);
    std::lock_guard<std::mutex> globalVerbGuard(globallocalWordsLock);
    std::lock_guard<std::mutex> builderVerbGuard(builderlocalWordsLock);
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
