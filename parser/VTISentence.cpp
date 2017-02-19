/*!
  \file     VTISentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VTISentence class.
*/
#include "VTISentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

namespace legacymud { namespace parser {

VTISentence::VTISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VTISentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
        // Make sure there are enough tokens (at least 2)
        if (range.end - range.start < 2) {
            result.status = ParseStatus::INVALID_DIRECT;
        }
        // Look for an indirect object match
        Range indirectRange;
        bool found = false;
        for (range.start += 1; range.start < range.end && !found; ++range.start) {
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
            for (range.start += 1; range.start < range.end && !found; ++range.start) {
                if (_indirect.findExactMatch(tokens, range, WordManager::hasNoun)) {
                    found = true;
                    result.status = ParseStatus::UNAVAILABLE_INDIRECT;
                }
            }
            if (!found) {
                result.status = ParseStatus::INVALID_INDIRECT;
            }
        }

        // Set all remaining tokens to the direct alias
        range = Range(_verb.getRange().end, indirectRange.start);
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
        }
        break;
    }

    return result;
}

} }
