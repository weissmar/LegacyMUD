/*!
  \file     VDTSentence.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/19/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the VDTSentence class.
*/
#include "VDTSentence.hpp"

#include "LexicalData.hpp"
#include "WordManager.hpp"

namespace legacymud { namespace parser {

VDTSentence::VDTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command) {
    _command = command;
    _verb = verb;
    _type = type;
    _status = ParseStatus::UNPARSED;
}

// Gets a ParseResult object based on the parsed input string.
ParseResult VDTSentence::getResult(const std::vector<Token> &tokens, const LexicalData &playerLex, const LexicalData &areaLex, const Grammar &grammar) {
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
            // Put all remaining tokens into indirectAlias
            range = Range(range.end, tokens.size());
            if (range.start >= range.end) {
                // No tokens left for indirect alias--invalid
                result.status = ParseStatus::INVALID_INDIRECT;
            }
            else {
                result.indirectAlias = Tokenizer::joinOriginal(tokens, range);
                result.status = ParseStatus::VALID;
            }
        }

        break;
    }

    return result;
}

} }
