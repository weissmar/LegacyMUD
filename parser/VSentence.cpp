/*!
  \file     VSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VSentence class.
*/
#include "VSentence.hpp"

namespace legacymud { namespace parser {

VSentence::VSentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex) {

}

// Gets a vector of ParseResult objects based on the parsed input string.
std::vector<ParseResult> VSentence::getResults() {
    std::vector<ParseResult> results;

    return results;
}

// Gets the ParseStatus of the highest ParseResult.
ParseStatus VSentence::getResultStatus() {
    return ParseStatus::UNPARSED;
}

} }
