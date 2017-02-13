/*!
  \file     WordMap.cpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/11/2017
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

    if (hasWord(alias)) {
        std::set<engine::InteractiveNoun *> vals = _wordMap[alias];
        auto ret = vals.insert(pObject);
        if (ret.second) {
            _count++;
            success = true;
        }
    }
    else {
        std::set<engine::InteractiveNoun *> vals;
        auto ret = vals.insert(pObject);
        _wordMap[alias] = vals;
        _count++;
        success = ret.second;
    }

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
            results.push_back(*it2);
        }
    }

    return results;
}

std::map<std::string, std::set<engine::InteractiveNoun *>>::iterator WordMap::begin() {
    return _wordMap.begin();
}

std::map<std::string, std::set<engine::InteractiveNoun *>>::iterator WordMap::end() {
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

    if (hasWord(alias)) {
        // Decrement the counter if the object is successfully erased
        if (_wordMap[alias].erase(pObject) == 1) {
            _count--;
            success = true;
        }
        // Remove the alias from the map if there are no objects left
        if (_wordMap[alias].empty()) {
            _wordMap.erase(alias);
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
