/*!
  \file     VDSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/21/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VDSentence class.
*/
#include "VDSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

#include <iostream>

namespace legacymud { namespace parser {

VDSentence::VDSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VDSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
    case VerbType::LOCAL:
        // Implied GO command requires special code path to use verb as direct object
        if (_command == engine::CommandEnum::GO && range.start == range.end) {
            range.start = 0;
            range.end = tokens.size();
        }
        //FALLTHROUGH
    case VerbType::GLOBAL:
    case VerbType::BUILDER:
    case VerbType::EDITMODE:
        // Look for a direct object match
        if (range.start >= range.end) {
            // Direct object is missing--invalid
            result.status = ParseStatus::INVALID_DIRECT;
        }
        else {
            // Search for a noun that matches all tokens exactly
            // Find direct object on player
            if (_direct.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &playerLex)) {
                auto player = playerLex.getObjectsByNoun(_direct.getAlias());
                _directObjects.insert(_directObjects.end(), player.begin(), player.end());
                result.directAlias = _direct.getAlias();
            }
            // Find direct object in area
            if (_direct.findExactMatch(tokens, range, &LexicalData::forwardHasNoun, &areaLex)) {
                auto area = areaLex.getObjectsByNoun(_direct.getAlias());
                _directObjects.insert(_directObjects.end(), area.begin(), area.end());
                result.directAlias = _direct.getAlias();
            }

            // Only search through all local objects if VerbType is BUILDER
            // and no objects were found in current area or player
            if (_directObjects.empty() && _type == VerbType::BUILDER) {
                if (_direct.findExactMatch(tokens, range, WordManager::hasNoun)) {
                    auto allLocal = WordManager::getLocalNouns(_direct.getAlias());
                    _directObjects.insert(_directObjects.end(), allLocal.begin(), allLocal.end());
                    result.directAlias = _direct.getAlias();
                }
            }

            // See if we found any results
            if (_directObjects.size() > 0) {
                // Results found--configure direct noun in result object
                result.direct = _directObjects;
                result.status = ParseStatus::VALID;
            }
            else {
                // No results found--check all local nouns to see if invalid or unavailable
                if (_direct.findMatch(tokens, range, WordManager::hasNoun)) {
                    result.status = ParseStatus::UNAVAILABLE_DIRECT;
                }
                else {
                    result.status = ParseStatus::INVALID_DIRECT;
                }
                result.unparsed = Tokenizer::joinOriginal(tokens, range);
            }
        }

        break;
    }

    return result;
}

} }
