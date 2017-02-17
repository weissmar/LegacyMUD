/*!
  \file     VPTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/16/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VPTSentence class.
*/
#include "VPTSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

namespace legacymud { namespace parser {

VPTSentence::VPTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VPTSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
    Range range = Range(_verb.getRange().end, tokens.size());

    // Prepare container for result
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
    case VerbType::LOCAL:
    case VerbType::BUILDER:
    case VerbType::EDITMODE:
        // Check for leftover tokens and set status accordingly
        if (range.start == tokens.size()) {
            // No text to parse--invalid
            result.status = ParseStatus::INVALID_PREPOSITION;
        }
        else {
            // Try to find preposition match based on grammar
            if (!_preposition.findMatch(tokens, range, &Grammar::forwardHasPreposition, &grammar)) {
                // Preposition not found--invalid
                result.status = ParseStatus::INVALID_PREPOSITION;
                // Add remaining text to unparsed
                result.unparsed = Tokenizer::joinOriginal(tokens, range);
            }
            else {
                // Preposition found--set type
                _prepType = grammar.getPrepositionType(_preposition.getAlias());
                // Start from next token until end of token list
                range = Range(range.end, tokens.size());
                // Check for leftover tokens
                if (range.start == tokens.size()) {
                    // No remaining tokens for text part--invalid
                    result.status = ParseStatus::INVALID_INDIRECT;
                }
                else {
                    // Add all remaining text to indirectAlias
                    result.indirectAlias = Tokenizer::joinOriginal(tokens, range);
                    result.status = ParseStatus::VALID;
                }
            }
        }
        break;
    }

    return result;
}

} }
