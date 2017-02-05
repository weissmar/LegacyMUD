/*!
  \file    TextParser.cpp
  \author  David Rigert
  \date    02/04/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the TextParser class.
*/

#include "TextParser.hpp"

namespace legacymud { namespace parser {

TextParser::~TextParser() {

}

// Converts text input from players into potential commands to be run.
TextParseStatus TextParser::parse(
        const std::string &input, 
        const VerbMap &playerVerbMap, 
        const VerbMap &areaVerbMap,
        std::vector<TextParseResult> &candidates, 
        bool isAdmin, 
        bool editMode
    ) {
    return TextParseStatus::INVALID_VERB;
}


}}
