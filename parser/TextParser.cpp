/*!
  \file     TextParser.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/07/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the TextParser class.
*/

#include "TextParser.hpp"

namespace legacymud { namespace parser {

TextParser::~TextParser() {

}

// Converts text input from players into potential commands to be run.
TextParseStatus TextParser::parse(
        const std::string &input, 
        const WordMap &playerVerbMap,
        const WordMap &playerNounMap,
        const WordMap &areaVerbMap,
        const WordMap &areaNounMap,
        std::vector<TextParseResult> &candidates, 
        bool isAdmin, 
        bool editMode
    ) {
    return TextParseStatus::INVALID_VERB;
}


}}
