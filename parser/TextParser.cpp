/*!
  \file    TextParser.cpp
  \author  David Rigert
  \date    01/29/2017
  \course  CS467, Winter 2017
 
  \details This file contains the implementation code for the TextParser class.
*/

#include "TextParser.h"
#include "Verb.h"

namespace legacymud { namespace parser {

TextParser::TextParser() {

}

TextParser::~TextParser() {

}

// Converts text input from players into potential commands to be run.
TextParseStatus TextParser::parse(const std::string &input, const std::list<legacymud::engine::InteractiveNoun *> &areaNouns, std::list<TextParseResult> &candidates) {
    return TextParseStatus::INVALID_VERB;
}


}}
