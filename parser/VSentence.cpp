/*!
  \file     VSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VSentence class.
*/
#include "VSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

namespace legacymud { namespace parser {

VSentence::VSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex) {
    Range range = Range(_verb.getRange().end, tokens.size());

    // Prepair container for result
    ParseResult result;
    result.type = _type;
    result.command = _command;
    result.status = ParseStatus::UNPARSED;

    // Find match based on VerbType.
    switch (_type) {
    case VerbType::INVALID:
        result.status = ParseStatus::INVALID_VERB;
        break;
    case VerbType::UNAVAILABLE:
        result.status = ParseStatus::UNAVAILABLE_VERB;
        break;
    case VerbType::GLOBAL:
        // Check for leftover tokens and set status accordingly
        if (range.end != tokens.size()) {
            // Add remaining tokens to unparsed
            result.unparsed = Tokenizer::joinOriginal(tokens, Range(range.end, tokens.size()));
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            result.status = ParseStatus::VALID;
        }
        break;
    case VerbType::LOCAL:
        // Check for leftover tokens and set status accordingly
        if (range.end != tokens.size()) {
            // Add remaining tokens to unparsed
            result.unparsed = Tokenizer::joinOriginal(tokens, Range(range.end, tokens.size()));
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            result.status = ParseStatus::VALID;
        }
        break;
    case VerbType::BUILDER:
        // Check for leftover tokens and set status accordingly
        if (range.end != tokens.size()) {
            // Add remaining tokens to unparsed
            result.unparsed = Tokenizer::joinOriginal(tokens, Range(range.end, tokens.size()));
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            result.status = ParseStatus::VALID;
        }
        break;
    case VerbType::EDITMODE:
        // Check for leftover tokens and set status accordingly
        if (range.end != tokens.size()) {
            // Add remaining tokens to unparsed
            result.unparsed = Tokenizer::joinOriginal(tokens, Range(range.end, tokens.size()));
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            result.status = ParseStatus::VALID;
        }
        break;
    }

    return result;
}

} }
