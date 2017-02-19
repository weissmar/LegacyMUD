/*!
  \file     VTPISentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VTPISentence class.
*/
#include "VTPISentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

#include <utility>

namespace legacymud { namespace parser {

VTPISentence::VTPISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VTPISentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
        // Make sure there are enough tokens (at least 3)
        if (range.end - range.start < 3) {
            result.status = ParseStatus::INVALID_DIRECT;
        }
        // Look for an indirect object match
        Range indirectRange;
        bool found = false;
        for (range.start += 2; range.start < range.end && !found; ++range.start) {
            // Find indirect object on player
            if (_indirect.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &playerLex)) {
                found = true;
                auto player = playerLex.getObjectsByNoun(_indirect.getAlias());
                _indirectObjects.insert(_indirectObjects.end(), player.begin(), player.end());
                result.indirectAlias = _indirect.getAlias();
            }
            // Find indirect object in area
            if (_indirect.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &areaLex)) {
                found = true;
                auto area = areaLex.getObjectsByNoun(_indirect.getAlias());
                _indirectObjects.insert(_indirectObjects.end(), area.begin(), area.end());
                result.indirectAlias = _indirect.getAlias();
            }

            // Only search through all local objects if VerbType is BUILDER
            // and no objects were found in current area or player
            if (_indirectObjects.empty() && _type == VerbType::BUILDER) {
                if (_indirect.findExactMatch(tokens, range, WordManager::hasNoun)) {
                    found = true;
                    auto allLocal = WordManager::getLocalNouns(_indirect.getAlias());
                    _indirectObjects.insert(_indirectObjects.end(), allLocal.begin(), allLocal.end());
                    result.indirectAlias = _indirect.getAlias();
                }
            }

            if (found)
                // Store indirect alias range for later
                indirectRange = range;
        }

        // Check if we found any indirect objects
        if (!found) {
            // No results found--check all local nouns to see if invalid or unavailable
            range = Range(_verb.getRange().end, tokens.size());
            for (range.start += 2; range.start < range.end && !found; ++range.start) {
                if (_indirect.findExactMatch(tokens, range, WordManager::hasNoun)) {
                    found = true;
                    result.status = ParseStatus::UNAVAILABLE_INDIRECT;
                }
            }
            if (!found) {
                result.status = ParseStatus::INVALID_INDIRECT;
            }
        }

        // Set possible preposition range (leaving at least 1 token for direct object)
        range = Range(_verb.getRange().end + 1, indirectRange.start);
        // Look for a preposition match
        if (range.start >= range.end) {
            // Preposition missing--invalid
            result.status = ParseStatus::INVALID_PREPOSITION;
        }
        else if (result.status == ParseStatus::UNPARSED) {
            // Find the longest matching preposition
            for (found = false; range.start < range.end && !found; ++range.start) {
                if (_preposition.findExactMatch(tokens, range, &Grammar::forwardHasPreposition, &grammar)) {
                    // Preposition found
                    found = true;
                }
            }
            if (!found) {
                    // Preposition not found--invalid
                    result.status = ParseStatus::INVALID_PREPOSITION;
            }
        }

        // Set all remaining tokens to the direct alias
        range = Range(_verb.getRange().end, _preposition.getRange().start);
        if (range.start >= range.end) {
            // No tokens left for direct object--invalid
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else if (result.status == ParseStatus::UNPARSED) {
            // Only set if the preposition and indirect object were valid
            result.directAlias = Tokenizer::joinOriginal(tokens, range);
        }

        // If we make it here with a status of UNPARSED, then everything was valid
        if (result.status == ParseStatus::UNPARSED) {
            result.status = ParseStatus::VALID;
            result.indirect = _indirectObjects;
            _prepType = grammar.getPrepositionType(_preposition.getAlias());
        }
        else {
            // Something was not parsed--throw it all into unparsed
            result.unparsed = Tokenizer::joinOriginal(tokens, Range(_verb.getRange().end, tokens.size()));
        }
        
        // Handle preposition
        if (result.status == ParseStatus::VALID) {
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
                // Swap direct and indirect object vectors
                std::swap(result.direct, result.indirect);
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
        break;
    }

    return result;
}

} }
