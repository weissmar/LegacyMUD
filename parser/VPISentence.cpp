/*!
  \file     VPISentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VPISentence class.
*/
#include "VPISentence.hpp"

namespace legacymud { namespace parser {

VPISentence::VPISentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex) {

}

// Gets a vector of ParseResult objects based on the parsed input string.
std::vector<ParseResult> VPISentence::getResults() {
    std::vector<ParseResult> results;

    return results;
}

// Gets the ParseStatus of the highest ParseResult.
ParseStatus VPISentence::getResultStatus() {
    return ParseStatus::UNPARSED;
}

} }
