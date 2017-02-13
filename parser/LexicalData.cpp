/*!
  \file     LexicalData.cpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the LexicalData class.
*/

#include "LexicalData.hpp"

#include "WordManager.hpp"

namespace legacymud { namespace parser {

void LexicalData::addNoun(std::string alias, engine::InteractiveNoun *pObject) {
    if (_nouns.addWord(alias, pObject)) {
        WordManager::addNoun(alias, pObject);
    }
}

void LexicalData::addVerb(std::string alias, engine::InteractiveNoun *pObject) {
    if (_verbs.addWord(alias, pObject)) {
        WordManager::addVerb(alias, pObject);
    }
}

bool LexicalData::hasNoun(std::string alias) const {
    return _nouns.hasWord(alias);
}

bool LexicalData::hasVerb(std::string alias) const {
    return _verbs.hasWord(alias);
}

std::vector<engine::InteractiveNoun *> LexicalData::getObjectsByNoun(std::string alias) const {
    return _nouns.getObjects(alias);
}

std::vector<engine::InteractiveNoun *> LexicalData::getObjectsByVerb(std::string alias) const {
    return _verbs.getObjects(alias);
}

void LexicalData::removeNoun(std::string alias, engine::InteractiveNoun *pObject) {
    if (_nouns.removeWord(alias, pObject)) {
        WordManager::removeNoun(alias, pObject);
    }
}

void LexicalData::removeVerb(std::string alias, engine::InteractiveNoun *pObject) {
    if (_verbs.removeWord(alias, pObject)) {
        WordManager::removeVerb(alias, pObject);
    }
}

void LexicalData::clear() {
    // Remove all data from WordManager
    for (auto it = _nouns.begin(); it != _nouns.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            WordManager::removeNoun(it->first, *it2);
        }
    }
    for (auto it = _verbs.begin(); it != _verbs.end(); ++it) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            WordManager::removeVerb(it->first, *it2);
        }
    }
    _nouns.clear();
    _verbs.clear();
}

} }
