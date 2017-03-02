/*!
  \file     WordMap.cpp
  \author   David Rigert
  \created  02/11/2017
  \modified 03/01/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the WordMap class.
*/

#include "WordMap.hpp"

#include <algorithm>
#include <cctype>

namespace legacymud { namespace parser {

WordMap::WordMap() {
    _count = 0;
}

// Add the specified alias-InteractiveNoun combo to the map.
bool WordMap::addWord(std::string alias, engine::InteractiveNoun *pObject) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    bool success = false;

    if (_wordMap.find(alias) != _wordMap.end()) {
        if (_wordMap[alias].find(pObject) != _wordMap[alias].end()) {
            _wordMap[alias][pObject]++;
            success = true;
        }
    }
    if (!success) {
        _wordMap[alias][pObject] = 1;
        success = true;
    }

    _count++;
    return success;
}

// Get whether the map contains the specified alias.
bool WordMap::hasWord(std::string alias) const {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    return _wordMap.find(alias) != _wordMap.end();
}

// Get all InteractiveNoun objects mapped to the specified alias.
std::vector<engine::InteractiveNoun *> WordMap::getObjects(std::string alias) const {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    std::vector<engine::InteractiveNoun *> results;
    auto it = _wordMap.find(alias);
    if (it != _wordMap.end()) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            results.push_back(it2->first);
        }
    }

    return results;
}

std::map<std::string, std::map<engine::InteractiveNoun *, int>>::iterator WordMap::begin() {
    return _wordMap.begin();
}

std::map<std::string, std::map<engine::InteractiveNoun *, int>>::iterator WordMap::end() {
    return _wordMap.end();
}


// Get the total number of InteractiveNoun objects in the map.
size_t WordMap::getSize() const {
    return _count;
}

// Remove the specified alias-InteractiveNoun combo from the map.
bool WordMap::removeWord(std::string alias, engine::InteractiveNoun *pObject) {
    // Convert string to lowercase
    std::transform(alias.begin(), alias.end(), alias.begin(), ::tolower);

    bool success = false;

    if (_wordMap.find(alias) != _wordMap.end()) {
        if (_wordMap[alias].find(pObject) != _wordMap[alias].end()) {
            _wordMap[alias][pObject]--;
            // Remove key-value pair if counter reaches 0
            if (_wordMap[alias][pObject] < 1) {
                _wordMap[alias].erase(pObject);
            }
            // Remove alias if no objects left
            if (_wordMap[alias].empty()) {
                _wordMap.erase(alias);
            }
            _count--;
            success = true;
        }
    }

    return success;
}

// Clear all data and reset counter
void WordMap::clear() {
    _wordMap.clear();
    _count = 0;
}

} }
