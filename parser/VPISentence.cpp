/*!
  \file     VPISentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VPISentence class.
*/
#include "VPISentence.hpp"

namespace legacymud { namespace parser {

VPISentence::VPISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VPISentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex) {
    ParseResult result;

    return result;
}

} }
