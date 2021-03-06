/*!
  \file     VDPISentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/19/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VDPISentence class.
*/
#include "VDPISentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

#include <utility>

namespace legacymud { namespace parser {

VDPISentence::VDPISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VDPISentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
        // Look for a direct object match
        if (range.start >= range.end) {
            // Direct object is missing--invalid
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            // Find the longest matching noun
            // Find direct object on player
            if (_direct.findMatch(tokens, range, &LexicalData::forwardHasNoun, &playerLex)) {
                auto player = playerLex.getObjectsByNoun(_direct.getAlias());
                _directObjects.insert(_directObjects.end(), player.begin(), player.end());
                result.directAlias = _direct.getAlias();
            }
            // Find direct object in area
            if (_direct.findMatch(tokens, range, &LexicalData::forwardHasNoun, &areaLex)) {
                auto area = areaLex.getObjectsByNoun(_direct.getAlias());
                _directObjects.insert(_directObjects.end(), area.begin(), area.end());
                result.directAlias = _direct.getAlias();
            }

            // Only search through all local objects if VerbType is BUILDER
            // and no objects were found in current area or player
            if (_directObjects.empty() && _type == VerbType::BUILDER) {
                if (_direct.findMatch(tokens, range, WordManager::hasNoun)) {
                    auto allLocal = WordManager::getLocalNouns(_direct.getAlias());
                    _directObjects.insert(_directObjects.end(), allLocal.begin(), allLocal.end());
                    result.directAlias = _direct.getAlias();
                }
            }

            // See if we found any results
            if (_directObjects.size() > 0) {
                // Results found--configure indirect noun in result object
                result.direct = _directObjects;
            }
            else {
                // No results found--check all local nouns to see if invalid or unavailable
                Range unavailRange = range;
                if (_direct.findMatch(tokens, unavailRange, WordManager::hasNoun)) {
                    result.status = ParseStatus::UNAVAILABLE_DIRECT;
                }
                else {
                    result.status = ParseStatus::INVALID_DIRECT;
                }
                result.unparsed = Tokenizer::joinOriginal(tokens, range);
            }
        }

        if (result.status == ParseStatus::UNPARSED) {
            // Look for a preposition match
            range = Range(range.end, tokens.size());
            if (range.start >= range.end) {
                // Preposition missing--invalid
                result.status = ParseStatus::INVALID_PREPOSITION;
            }
            else if (result.status == ParseStatus::UNPARSED) {
                // Find the longest matching preposition
                if (!_preposition.findMatch(tokens, range, &Grammar::forwardHasPreposition, &grammar)) {
                    // Preposition not found--invalid
                    result.status = ParseStatus::INVALID_PREPOSITION;
                    result.unparsed = Tokenizer::joinOriginal(tokens, range);
                }
                else {
                    // Preposition found--set type
                    _prepType = grammar.getPrepositionType(_preposition.getAlias());
                }
            }
        }

        if (result.status == ParseStatus::UNPARSED) {
            // Look for a noun that matches all remaining tokens
            range = Range(range.end, tokens.size());
            if (range.start >= range.end) {
                // No tokens left for indirect object--invalid
                result.status = ParseStatus::INVALID_INDIRECT;
            }
            else {
                // Try to find matching indirect object with remaining tokens
                // Find indirect object on player
                if (_indirect.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &playerLex)) {
                    auto player = playerLex.getObjectsByNoun(_indirect.getAlias());
                    _indirectObjects.insert(_indirectObjects.end(), player.begin(), player.end());
                    result.indirectAlias = _indirect.getAlias();
                }
                // Find indirect object in area
                if (_indirect.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &areaLex)) {
                    auto area = areaLex.getObjectsByNoun(_indirect.getAlias());
                    _indirectObjects.insert(_indirectObjects.end(), area.begin(), area.end());
                    result.indirectAlias = _indirect.getAlias();
                }

                // Only search through all local objects if VerbType is BUILDER
                // and no objects were found in current area or player
                if (_indirectObjects.empty() && _type == VerbType::BUILDER) {
                    if (_indirect.findExactMatch(tokens, range, WordManager::hasNoun)) {
                        auto allLocal = WordManager::getLocalNouns(_indirect.getAlias());
                        _indirectObjects.insert(_indirectObjects.end(), allLocal.begin(), allLocal.end());
                        result.indirectAlias = _indirect.getAlias();
                    }
                }

                // See if we found any results
                if (_indirectObjects.size() > 0) {
                    // Results found--configure indirect noun in result object
                    result.indirect = _indirectObjects;
                    result.status = ParseStatus::VALID;
                }
                else {
                    // No results found--check all local nouns to see if invalid or unavailable
                    if (_indirect.findExactMatch(tokens, range, WordManager::hasNoun)) {
                        result.status = ParseStatus::UNAVAILABLE_INDIRECT;
                    }
                    else {
                        result.status = ParseStatus::INVALID_INDIRECT;
                    }
                    result.unparsed = Tokenizer::joinOriginal(tokens, range);
                }
            }
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
