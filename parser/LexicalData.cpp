/*!
  \file     LexicalData.cpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/20/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the LexicalData class.
*/

#include "LexicalData.hpp"

#include "WordManager.hpp"

namespace legacymud { namespace parser {

LexicalData::~LexicalData() {
    clear();
}

void LexicalData::addNoun(std::string alias, engine::InteractiveNoun *pObject) {
    std::lock_guard<std::mutex> guard(_nounLock);
    if (_nouns.addWord(alias, pObject)) {
        WordManager::addNoun(alias, pObject);
    }
}

void LexicalData::addVerb(std::string alias, engine::InteractiveNoun *pObject) {
    std::lock_guard<std::mutex> guard(_verbLock);
    if (_verbs.addWord(alias, pObject)) {
        WordManager::addVerb(alias, pObject);
    }
}

bool LexicalData::hasNoun(std::string alias) const {
    std::lock_guard<std::mutex> guard(_nounLock);
    return _nouns.hasWord(alias);
}

bool LexicalData::hasVerb(std::string alias) const {
    std::lock_guard<std::mutex> guard(_verbLock);
    return _verbs.hasWord(alias);
}

std::vector<engine::InteractiveNoun *> LexicalData::getObjectsByNoun(std::string alias) const {
    std::lock_guard<std::mutex> guard(_nounLock);
    return _nouns.getObjects(alias);
}

std::vector<engine::InteractiveNoun *> LexicalData::getObjectsByVerb(std::string alias) const {
    std::lock_guard<std::mutex> guard(_verbLock);
    return _verbs.getObjects(alias);
}

void LexicalData::removeNoun(std::string alias, engine::InteractiveNoun *pObject) {
    std::lock_guard<std::mutex> guard(_nounLock);
    if (_nouns.removeWord(alias, pObject)) {
        WordManager::removeNoun(alias, pObject);
    }
}

void LexicalData::removeVerb(std::string alias, engine::InteractiveNoun *pObject) {
    std::lock_guard<std::mutex> guard(_verbLock);
    if (_verbs.removeWord(alias, pObject)) {
        WordManager::removeVerb(alias, pObject);
    }
}

void LexicalData::clear() {
    // Remove all data from WordManager
    _nounLock.lock();
    for (auto it = _nouns.begin(); it != _nouns.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            WordManager::removeNoun(it->first, *it2);
        }
    }
    _nouns.clear();
    _nounLock.unlock();

    _verbLock.lock();
    for (auto it = _verbs.begin(); it != _verbs.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            WordManager::removeVerb(it->first, *it2);
        }
    }
    _verbs.clear();
    _verbLock.unlock();
}

// Borrowed from http://stackoverflow.com/questions/12662891/passing-a-member-function-as-an-argument-in-c
bool LexicalData::forwardHasNoun(const void *context, std::string arg) {
    return static_cast<const LexicalData*>(context)->hasNoun(arg);
}

bool LexicalData::forwardHasVerb(const void *context, std::string arg) {
    return static_cast<const LexicalData*>(context)->hasVerb(arg);
}

} }
