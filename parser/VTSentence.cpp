/*!
  \file     VTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VTSentence class.
*/
#include "VTSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

namespace legacymud { namespace parser {

VTSentence::VTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VTSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
    Range range = Range(_verb.getRange().end, tokens.size());

    // Prepare container for result
    ParseResult result;
    result.type = _type;
    result.command = _command;
    result.status = ParseStatus::UNPARSED;
    result.position = engine::ItemPosition::NONE;

    // Find match based on VerbType.
    switch (_type) {
    case VerbType::INVALID:
        result.status = ParseStatus::INVALID_VERB;
        break;
    case VerbType::UNAVAILABLE:
        result.status = ParseStatus::UNAVAILABLE_VERB;
        break;
    case VerbType::GLOBAL:
    case VerbType::LOCAL:
    case VerbType::BUILDER:
    case VerbType::EDITMODE:
        // Check for leftover tokens and set status accordingly
        if (range.start == tokens.size()) {
            // Text input missing--invalid
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            // Add all remaining text to directAlias
            result.directAlias = Tokenizer::joinOriginal(tokens, range);
            result.status = ParseStatus::VALID;
        }
        break;
    }

    return result;
}

} }
