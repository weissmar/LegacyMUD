/*!
  \file     VDPTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/15/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VDPTSentence class.
*/
#include "VDPTSentence.hpp"

namespace legacymud { namespace parser {

VDPTSentence::VDPTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VDPTSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
    ParseResult result;

    return result;
}

} }
