/*!
  \file     VTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VTSentence class.
*/
#include "VTSentence.hpp"

namespace legacymud { namespace parser {

VTSentence::VTSentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex) {

}

// Gets a vector of ParseResult objects based on the parsed input string.
std::vector<ParseResult> VTSentence::getResults() {
    std::vector<ParseResult> results;

    return results;
}

// Gets the ParseStatus of the highest ParseResult.
ParseStatus VTSentence::getResultStatus() {
    return ParseStatus::UNPARSED;
}

} }
