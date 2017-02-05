/*!
  \file    Verb.cpp
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the Grammar class.
*/

#include "Grammar.hpp"

namespace legacymud { namespace parser {

Grammar::Grammar(engine::CommandEnum command, Support directObject, Support indirectObject) {
    _command = command;
    _directObject = directObject;
    _indirectObject = indirectObject;
}
    
void Grammar::addPreposition(std::string prep, PrepositionType meaning) {
    _prepositions[prep] = meaning;
}

PrepositionType Grammar::getPrepositionType(std::string prep) const {
    auto it = _prepositions.find(prep);
    if (it != _prepositions.end())
        return it->second;
    else
        return PrepositionType::NONE;
}

Grammar::Support Grammar::takesIndirectObject() const {
    return _indirectObject;
}

Grammar::Support Grammar::takesDirectObject() const {
    return _directObject;
}

} }
