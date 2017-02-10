/*!
  \file     TextParser.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/07/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the TextParser class.
*/

#include "TextParser.hpp"

#include <algorithm>
#include <string.h>
#include <sstream>
#include <iostream>

namespace legacymud { namespace parser {

TextParser::~TextParser() {

}

// Converts text input from players into potential commands to be run.
TextParseStatus TextParser::parse(
        const std::string &input, 
        const WordMap &playerVerbMap,
        const WordMap &playerNounMap,
        const WordMap &areaVerbMap,
        const WordMap &areaNounMap,
        std::vector<TextParseResult> &candidates, 
        bool isAdmin, 
        bool editMode
    ) {
    
    // First check if input has any text
    if (input.empty()) {
        return TextParseStatus::INVALID_VERB;
    }

    // Tokenize the input
    std::vector<Token> tokens = tokenizeInput(input);

    std::vector<Match> matches;             // Stores potential matches
    Match m;                                // Stores match information
    std::string alias;                      // Stores an alias
    TextParseResult result;                 // Stores a parsing result

    // FIRST: Check for edit mode verbs if user has permission
    if (isAdmin) {
        m = parseGlobal(tokens, WordManager::hasEditModeVerb, WordManager::getEditModeVerb, playerNounMap, areaNounMap);

        // If we get a valid match, set candidate and immediately return
        if (m.status == TextParseStatus::VALID) {
            // If no direct objects, there will only be one result
            if (m.directObjs.empty()) {
                result.command = m.verbInfo.command;
                result.direct = nullptr;
                result.position = prepositionToPosition(m.preposition);
                result.unparsed = m.unparsed;
                for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                    result.indirect.push_back(it2->second);
                }

                candidates.push_back(result);
            }
            else {
                // Each direct object gets its own result
                for (auto it = m.directObjs.begin(); it != m.directObjs.end(); ++it) {
                    result.command = m.verbInfo.command;
                    result.direct = it->second;
                    result.position = prepositionToPosition(m.preposition);
                    result.unparsed = m.unparsed;
                    for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                        result.indirect.push_back(it2->second);
                    }

                    candidates.push_back(result);
                }
            }

            return TextParseStatus::VALID;
        }
    }

    // SECOND: Check for world builder verbs if in builder mode
    if (editMode) {
        m = parseGlobal(tokens, WordManager::hasBuilderVerb, WordManager::getBuilderVerb, playerNounMap, areaNounMap);

        // If we get a valid match, add to candidate list and return
        if (m.status == TextParseStatus::VALID) {
            // If no direct objects, there will only be one result
            if (m.directObjs.empty()) {
                result.command = m.verbInfo.command;
                result.direct = nullptr;
                result.position = prepositionToPosition(m.preposition);
                result.unparsed = m.unparsed;
                for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                    result.indirect.push_back(it2->second);
                }

                candidates.push_back(result);
            }
            else {
                // Each direct object gets its own result
                for (auto it = m.directObjs.begin(); it != m.directObjs.end(); ++it) {
                    result.command = m.verbInfo.command;
                    result.direct = it->second;
                    result.position = prepositionToPosition(m.preposition);
                    result.unparsed = m.unparsed;
                    for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                        result.indirect.push_back(it2->second);
                    }

                    candidates.push_back(result);
                }
            }

            return TextParseStatus::VALID;
        }
    }

    // THIRD: Check for player verbs
    {

    }

    // FOURTH: Check for area verbs
    {

    }

    // FIFTH: Check for global verbs
    {
        m = parseGlobal(tokens, WordManager::hasGlobalVerb, WordManager::getGlobalVerb, playerNounMap, areaNounMap);

        // If we get a valid match, add to candidate list and return
        if (m.status == TextParseStatus::VALID) {
            // If no direct objects, there will only be one result
            if (m.directObjs.empty()) {
                result.command = m.verbInfo.command;
                result.direct = nullptr;
                result.position = prepositionToPosition(m.preposition);
                result.unparsed = m.unparsed;
                for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                    result.indirect.push_back(it2->second);
                }

                candidates.push_back(result);
            }
            else {
                // Each direct object gets its own result
                for (auto it = m.directObjs.begin(); it != m.directObjs.end(); ++it) {
                    result.command = m.verbInfo.command;
                    result.direct = it->second;
                    result.position = prepositionToPosition(m.preposition);
                    result.unparsed = m.unparsed;
                    for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
                        result.indirect.push_back(it2->second);
                    }

                    candidates.push_back(result);
                }
            }

            return TextParseStatus::VALID;
        }

    }

    // No verb found if we reach here
    // Return unparsed input with INVALID_VERB or UNAVAILABLE_VERB.
    result.command = engine::CommandEnum::INVALID;
    result.direct = nullptr;
    result.position = engine::ItemPosition::NONE;
    result.unparsed = joinOriginalTokens(tokens, Range(0, tokens.size()));
    candidates.push_back(result);

    // SIXTH: Check for unavailable verbs
    Range r = Range(0, tokens.size());
    alias = findLongestGlobalAlias(WordManager::hasVerb, tokens, r);
    if (!alias.empty()) {
        // Verb found but unavailable; return unparsed input with UNAVAILABLE_VERB.
        return TextParseStatus::UNAVAILABLE_VERB;
    }
    else {
        // No verb found; return unparsed input with INVALID_VERB
        return TextParseStatus::INVALID_VERB;
    }
}

std::vector<TextParser::Token> TextParser::tokenizeInput(const std::string &input) {
    std::vector<Token> tokens;
    char *buffer = new char[input.size() + 1];
    ::strcpy(buffer, input.c_str());
    char *token = ::strtok(buffer, " ");
    while (token) {
        Token t;
        // Store original word
        t.original = token;
        // Store word normalized to lowercase
        t.normalized.resize(t.original.size());
        std::transform(t.original.begin(), t.original.end(), t.normalized.begin(), ::tolower);
        // Add Token to vector
        tokens.push_back(t);
        // Get next token
        token = strtok(NULL, " ");
    }
    delete [] buffer;

    return tokens;
}

std::string TextParser::joinNormalizedTokens(const std::vector<Token> &tokens, Range range, bool skipIgnoreWords) {
    // Immediately return if invalid arguments
    if (range.start < 0       // start is negative
        || range.start >= range.end // start is at or after end
        || range.start >= tokens.size() // start is larger than vector size
        || range.end > tokens.size())   // end is larger than vector size
        return std::string();
    
    std::ostringstream oss;
    size_t i = range.start;
    // Append first token
    if (!(skipIgnoreWords && WordManager::isIgnoreWord(tokens[i].normalized))) {
        oss << tokens[i].normalized;
    }
    // Append remaining tokens separated by space
    for (++i; i < range.end; ++i) {
        if (!(skipIgnoreWords && WordManager::isIgnoreWord(tokens[i].normalized))) {
            oss << " " << tokens[i].normalized;
        }
    }

    return oss.str();
}

std::string TextParser::joinOriginalTokens(const std::vector<Token> &tokens, Range range) {
    // Immediately return if invalid arguments
    if (range.start < 0       // start is negative
        || range.start >= range.end // start is at or after end
        || range.start >= tokens.size() // start is larger than vector size
        || range.end > tokens.size())   // end is larger than vector size
        return std::string();
    
    std::ostringstream oss;
    size_t i = range.start;
    // Append first token
    oss << tokens[i].original;
    // Append remaining tokens separated by space
    for (++i; i < range.end; ++i) {
        oss << " " << tokens[i].original;
    }

    return oss.str();
}

// Find a matching alias in a WordMap
std::string TextParser::findLongestLocalAlias(const WordMap &lookupTable, const std::vector<Token> &tokens, Range &range) {
    if (range.end > tokens.size())
        return std::string();

    // Search for entire string and chop one off the back each time
    for (; range.end > 0; --range.end) {
        // Try with ignore words
        std::string substring = joinNormalizedTokens(tokens, range, false);
        auto it = lookupTable.find(substring);
        if (it != lookupTable.end()) {
            // Found a match; return the range.
            return substring;
        }
        // Try without ignore words
        substring = joinNormalizedTokens(tokens, range, true);
        it = lookupTable.find(substring);
        if (it != lookupTable.end()) {
            // Found a match; return the range.
            return substring;
        }
    }

    // Did not find a match; return empty string.
    return std::string();
}

// Find a matching alias using the specified function
std::string TextParser::findLongestGlobalAlias(bool (*hasAlias)(std::string), const std::vector<Token> &tokens, Range &range) {
    if (range.end > tokens.size())
        return std::string();

    // Search for entire string and chop one off the back each time
    for (; range.end > 0; --range.end) {
        // Try with ignore words
        std::string substring = joinNormalizedTokens(tokens, range, false);
        if (hasAlias(substring)) {
            // Found a match; return it.
            return substring;
        }
        // Try without ignore words
        substring = joinNormalizedTokens(tokens, range, true);
        if (hasAlias(substring)) {
            // Found a match; return it.
            return substring;
        }
    }

    // Did not find a match; return empty string.
    return std::string();
}

// Tries to find a global match based on input tokens and maps
TextParser::Match TextParser::parseGlobal(const std::vector<Token> &tokens, bool (*hasAlias)(std::string), VerbInfo (*getVerb)(std::string), const WordMap &playerNounMap, const WordMap &areaNounMap) {
    // Stores match information
    Match m;
    // Stores the range of tokens being checked
    Range range = Range(0, tokens.size());
    // Temporary storage for alias match
    std::string alias;

    // Search for verb match
    m.verbAlias = findLongestGlobalAlias(hasAlias, tokens, range);
    if (!m.verbAlias.empty()) {
        // Get verb grammar info
        m.verbInfo = getVerb(m.verbAlias);

        // Set range to start after verb
        range.start = range.end;
        range.end = tokens.size();

        // Parse remaining input based on grammar
        switch (m.verbInfo.grammar.takesDirectObject()) {
        case Grammar::NO:
            // Do nothing
            break;
        case Grammar::OPTIONAL:
            // Check for direct object match on player
            alias = findLongestLocalAlias(playerNounMap, tokens, range);
            if (!alias.empty()) {
                for (auto it = playerNounMap.find(alias); it != playerNounMap.end(); ++it) {
                    m.directObjs[it->first] = it->second;
                }
            }
            // Reset range and check for direct object match in area
            range.end = tokens.size();
            alias = findLongestLocalAlias(areaNounMap, tokens, range);
            if (!alias.empty()) {
                for (auto it = areaNounMap.find(alias); it != areaNounMap.end(); ++it) {
                    m.directObjs[it->first] = it->second;
                }
            }

            // Update range to after direct object
            range.start = range.end;
            range.end = tokens.size();
            break;
        case Grammar::REQUIRED:
            // Check for direct object match on player
            alias = findLongestLocalAlias(playerNounMap, tokens, range);
            if (!alias.empty()) {
                for (auto it = playerNounMap.find(alias); it != playerNounMap.end(); ++it) {
                    m.directObjs[it->first] = it->second;
                }
            }
            // Reset range and check for direct object match in area
            range.end = tokens.size();
            alias = findLongestLocalAlias(areaNounMap, tokens, range);
            if (!alias.empty()) {
                for (auto it = areaNounMap.find(alias); it != areaNounMap.end(); ++it) {
                    m.directObjs[it->first] = it->second;
                }
            }

            // Make sure we got at least one match
            if (m.directObjs.size() == 0) {
                // No matches, check global noun list
                range.end = tokens.size();
                alias = findLongestGlobalAlias(WordManager::hasNoun, tokens, range);

                // Set result status based on whether the noun is used anywhere
                m.status = alias.empty() ? TextParseStatus::INVALID_NOUN : TextParseStatus::UNAVAILABLE_NOUN;

                // Add any unparsed text to Match object
                range.end = tokens.size();
                m.unparsed = joinOriginalTokens(tokens, range);
                
                // Return invalid match because grammar requirement not met
                return m;
            }

            // Update range to after direct object
            range.start = range.end;
            range.end = tokens.size();
            break;
        case Grammar::TEXT:
            // If no further grammar required, dump everything into unparsed text and return
            if (m.verbInfo.grammar.takesIndirectObject() == Grammar::NO) {
                m.unparsed = joinOriginalTokens(tokens, range);
                
                // Set status based on whether there is any unparsed text
                m.status = m.unparsed.empty() ? TextParseStatus::INVALID_NOUN : TextParseStatus::VALID;

                return m;
            }
            // Try to find indirect object at end, moving toward the front
            // Write a findObject function?

            // If indirect object is found, check if prior word is preposition
            break;
        case Grammar::OPTIONALTEXT:
            // If no further grammar is required, dump everything into unparsed text and return
            if (m.verbInfo.grammar.takesIndirectObject() == Grammar::NO) {
                m.unparsed = joinOriginalTokens(tokens, range);
                
                // Set status to valid, since this grammar part is optional
                m.status = TextParseStatus::VALID;

                return m;
            }
            // Try to find indirect object at end, moving toward the front
            break;
        }

        // Parsing succeeded if we reach here.
        m.status = TextParseStatus::VALID;
    }
    else {
        // No verb found; add all tokens to unparsed
        m.status = TextParseStatus::INVALID_VERB;
        range.end = tokens.size();
        m.unparsed = joinOriginalTokens(tokens, range);
    }

    return m;
}

// Tries to find a local match based on input tokens and maps
TextParser::Match TextParser::parseLocal(const std::vector<Token> &tokens, const WordMap &playerVerbMap, const WordMap &playerNounMap, const WordMap &areaVerbMap, const WordMap &areaNounMap) {
    Match m;

    return m;
}

engine::ItemPosition TextParser::prepositionToPosition(PrepositionType preposition) {
    switch (preposition) {
    case PrepositionType::ON:
        return engine::ItemPosition::ON;
    case PrepositionType::UNDER:
        return engine::ItemPosition::UNDER;
    case PrepositionType::IN:
        return engine::ItemPosition::IN;
    default:
        return engine::ItemPosition::NONE;
    }
}


}}
