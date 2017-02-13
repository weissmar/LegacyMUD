/*!
  \file     VDSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VDSentence class.
*/
#include "VDSentence.hpp"

namespace legacymud { namespace parser {

VDSentence::VDSentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex) {

}

// Gets a vector of ParseResult objects based on the parsed input string.
std::vector<ParseResult> VDSentence::getResults() {
    std::vector<ParseResult> results;

    return results;
}

// Gets the ParseStatus of the highest ParseResult.
ParseStatus VDSentence::getResultStatus() {
    return ParseStatus::UNPARSED;
}

} }
