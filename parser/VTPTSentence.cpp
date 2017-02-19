/*!
  \file     VTPTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VTPTSentence class.
*/
#include "VTPTSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

#include <utility>

namespace legacymud { namespace parser {

VTPTSentence::VTPTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VTPTSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
        if (range.end - range.start < 3 || range.start == tokens.size()) {
            // We need at least 3 tokens to have a prep with text on each side
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            // To find the preposition, search all remaining tokens except first and last.
            // Decrement the span by 1 each turn and keep searching, back-to-front.
            bool found = false;
            size_t tCount = range.end - range.start;
            Range prepRange;
            size_t start = 0, end = 0;
            for (size_t spread = tCount - 2; (spread > 0) && !found; --spread) {
                for (end = range.end - 1, start = end - spread; (start > range.start) && !found; --end, --start) {
                    prepRange.start = start;
                    prepRange.end = end;
                    if (_preposition.findExactMatch(tokens, prepRange, &Grammar::forwardHasPreposition, &grammar)) {
                        // Preposition found
                        found = true;
                    }
                }
            }
            // Check if we found a preposition
            if (!found) {
                // No preposition--invalid
                result.status = ParseStatus::INVALID_PREPOSITION;
                result.unparsed = Tokenizer::joinOriginal(tokens, range);
            }
            else {
                // Set the preposition type
                _prepType = grammar.getPrepositionType(_preposition.getAlias());
                // Set directAlias to everything before the preposition
                result.directAlias = Tokenizer::joinOriginal(tokens, Range(range.start, prepRange.start));
                // Set indirectAlias to everything after the preposition
                result.indirectAlias = Tokenizer::joinOriginal(tokens, Range(prepRange.end, tokens.size()));
                result.status = ParseStatus::VALID;

                // Handle preposition
                switch (_prepType) {
                case PrepositionType::ON:
                    result.position = engine::ItemPosition::ON;
                    break;
                case PrepositionType::IN:
                    result.position = engine::ItemPosition::IN;
                    break;
                case PrepositionType::UNDER:
                    result.position = engine::ItemPosition::UNDER;
                    break;
                case PrepositionType::OF:
                    // Swap direct and indirect aliases
                    std::swap(result.directAlias, result.indirectAlias);
                    // FALLTHROUGH
                case PrepositionType::TO:
                case PrepositionType::WITH:
                case PrepositionType::FROM:
                case PrepositionType::ALL:
                case PrepositionType::NONE:
                    result.position = engine::ItemPosition::NONE;
                    break;
                }
            }
        }
        break;
    }

    return result;
}

} }
