/*!
  \file    Verb.cpp
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the Verb class.
*/

#include "Verb.h"

namespace legacymud { namespace parser {

Verb::Verb() {
    _action = engine::ActionType::NONE;
}
    
Verb::Verb(legacymud::engine::ActionType action) {
    _action = action;
}

void Verb::addPreposition(std::string prep, PrepositionMeaning meaning) {
    _prepositions[prep] = meaning;
}

legacymud::engine::ActionType Verb::getAction() const {
    return _action;
}

PrepositionMeaning Verb::getPrepositionMeaning(std::string prep) const {
    auto it = _prepositions.find(prep);
    if (it != _prepositions.end())
        return it->second;
    else
        return PrepositionMeaning::NONE;
}

void Verb::setAction(legacymud::engine::ActionType action) {
    _action = action;
}

} }
