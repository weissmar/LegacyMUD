/*!
  \file     Grammar.cpp
  \author   David Rigert
  \created  02/02/2017
  \modified 02/16/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the Grammar class.
*/

#include "Grammar.hpp"

#include <algorithm>
#include <cctype>


namespace legacymud { namespace parser {

Grammar::Grammar(Support direct, bool preposition, Support indirect) {
    _directObject = direct;
    _preposition = preposition;
    _indirectObject = indirect;
}
    
void Grammar::addPreposition(std::string prep, PrepositionType type) {
    // Convert string to lowercase
    std::transform(prep.begin(), prep.end(), prep.begin(), ::tolower);

    _prepositions[prep] = type;
}

PrepositionType Grammar::getPrepositionType(std::string prep) const {
    // Convert string to lowercase
    std::transform(prep.begin(), prep.end(), prep.begin(), ::tolower);

    auto it = _prepositions.find(prep);
    if (it != _prepositions.end())
        return it->second;
    else
        return PrepositionType::NONE;
}

bool Grammar::hasPreposition(std::string prep) const {
    // Convert string to lowercase
    std::transform(prep.begin(), prep.end(), prep.begin(), ::tolower);

    auto it = _prepositions.find(prep);
    return it != _prepositions.end();
}

void Grammar::setSupport(Support direct, bool preposition, Support indirect) {
    _directObject = direct;
    _preposition = preposition;
    _indirectObject = indirect;
}

Grammar::Support Grammar::takesIndirectObject() const {
    return _indirectObject;
}

bool Grammar::takesPreposition() const {
    return _preposition;
}

Grammar::Support Grammar::takesDirectObject() const {
    return _directObject;
}

bool Grammar::forwardHasPreposition(const void *context, std::string arg) {
    return static_cast<const Grammar*>(context)->hasPreposition(arg);
}

} }
