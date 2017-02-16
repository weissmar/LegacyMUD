/*!
  \file     TextParser.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/15/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation code for the TextParser class.
*/

#include "TextParser.hpp"

#include "LexicalData.hpp"
#include "PartOfSpeech.hpp"
#include "Sentence.hpp"
#include "Tokenizer.hpp"
#include "VerbInfo.hpp"

#include <Action.hpp>
#include <InteractiveNoun.hpp>

#include <algorithm>
#include <string.h>
#include <sstream>
#include <iostream>
#include <queue>


namespace legacymud { namespace parser {

// Comparator class for ParseResults
class ComparePriority {
public:
    bool operator() (const ParseResult lhs, const ParseResult rhs) {
        return static_cast<int>(lhs.status) < static_cast<int>(rhs.status);
    }
};

TextParser::~TextParser() {

}

std::vector<ParseResult> TextParser::parse(
    const std::string input, 
    const LexicalData &player,
    const LexicalData &area,
    bool isAdmin,
    bool editMode
    ) {

    // Stores the sentence parser
    Sentence *parser = nullptr;
    
    // Stores the parse result
    std::priority_queue<ParseResult, std::vector<ParseResult>, ComparePriority> candidates;
    std::vector<ParseResult> results;
    ParseResult result;

    // STEP 1: Tokenize input string
    auto tokens = Tokenizer::tokenizeInput(input);
    Range range = Range(0, tokens.size());

    // STEP 2: Look for a matching verb alias
    PartOfSpeech verb;

    // Check for admin privileges
    if (isAdmin) {
        // FIRST: Check for edit mode verb match
        if (verb.findMatch(tokens, range, WordManager::hasEditModeVerb)) {
            // Found edit mode verb; get grammar rules
            std::vector<VerbInfo> verbs = WordManager::getEditModeVerbs(verb.getAlias());
            // Run parser on each grammar definition and store the candidates
            for (auto it = verbs.begin(); it != verbs.end(); ++it) {
                parser = Sentence::makeSentence(it->grammar, verb, VerbType::EDITMODE, it->command);
                // Skip if unsupported grammar type
                if (parser == nullptr) {
                    std::cerr << "Unsupported grammar type detected in " << verb.getAlias() << std::endl;
                    continue;
                }
                result = parser->getResult(tokens, player, area, it->grammar);
                candidates.push(result);
                //std::cout << "Found edit mode candidate with status of " << static_cast<int>(result.status) << std::endl;
                
                delete parser;
            }

            // Add valid candidates to results
            while (candidates.size() > 0 && candidates.top().status == ParseStatus::VALID) {
                results.push_back(candidates.top());
                candidates.pop();
            }

            // Do no further processing if valid edit mode verb found
            if (!results.empty())
                return results;
        }

        // SECOND: Check for world builder verb match
        verb = PartOfSpeech();
        range = Range(0, tokens.size());
        if (verb.findMatch(tokens, range, WordManager::hasBuilderVerb)) {
            // Found world builder verb; get grammar rules
            std::vector<VerbInfo> verbs = WordManager::getBuilderVerbs(verb.getAlias());
            // Run parser on each grammar definition and store the candidates
            for (auto it = verbs.begin(); it != verbs.end(); ++it) {
                parser = Sentence::makeSentence(it->grammar, verb, VerbType::BUILDER, it->command);
                // Skip if unsupported grammar type
                if (parser == nullptr) {
                    std::cerr << "Unsupported grammar type detected in " << verb.getAlias() << std::endl;
                    continue;
                }
                result = parser->getResult(tokens, player, area, it->grammar);
                //std::cout << "Found builder candidate with status of " << static_cast<int>(result.status) << std::endl;
                candidates.push(result);
                
                delete parser;
            }

            // Add valid candidates to results
            while (candidates.size() > 0 && candidates.top().status == ParseStatus::VALID) {
                results.push_back(candidates.top());
                candidates.pop();
            }

            // Do no further processing if valid builder verb found
            if (!results.empty())
                return results;
        }
    }

    // THIRD: Check for player verb matches
    verb = PartOfSpeech();
    range = Range(0, tokens.size());
    if (verb.findMatch(tokens, range, &LexicalData::forwardHasVerb, &player)) {
        // Found local verb on player; get grammar rules
        std::vector<engine::InteractiveNoun *> ins = player.getObjectsByVerb(verb.getAlias());
       // Run parser on each grammar definition and store the results
        for (auto it = ins.begin(); it != ins.end(); ++it) {
            std::vector<engine::Action *> actions = (*it)->getActions(verb.getAlias());
            for (auto it2 = actions.begin(); it2 != actions.end(); ++it) {
                parser = Sentence::makeSentence(*((*it2)->getGrammar(verb.getAlias())), verb, VerbType::LOCAL, (*it2)->getCommand());
                // Skip if unsupported grammar type
                if (parser == nullptr) {
                    std::cerr << "Unsupported grammar type detected in " << verb.getAlias() << std::endl;
                    continue;
                }
                result = parser->getResult(tokens, player, area, *((*it2)->getGrammar(verb.getAlias())));
                //std::cout << "Found player candidate with status of " << static_cast<int>(result.status) << std::endl;
                candidates.push(result);
            }
            
            delete parser;
        }
    }

    // FOURTH: Check for area verb matches
    verb = PartOfSpeech();
    range = Range(0, tokens.size());
    if (verb.findMatch(tokens, range, &LexicalData::forwardHasVerb, &area)) {
        // Found local verb in area; get grammar rules
        std::vector<engine::InteractiveNoun *> ins = area.getObjectsByVerb(verb.getAlias());
        // Run parser on each grammar definition and store the results
        for (auto it = ins.begin(); it != ins.end(); ++it) {
            std::vector<engine::Action *> actions = (*it)->getActions(verb.getAlias());
            for (auto it2 = actions.begin(); it2 != actions.end(); ++it) {
                parser = Sentence::makeSentence(*((*it2)->getGrammar(verb.getAlias())), verb, VerbType::LOCAL, (*it2)->getCommand());
                // Skip if unsupported grammar type
                if (parser == nullptr) {
                    std::cerr << "Unsupported grammar type detected in " << verb.getAlias() << std::endl;
                    continue;
                }
                result = parser->getResult(tokens, player, area, *((*it2)->getGrammar(verb.getAlias())));
                //std::cout << "Found area candidate with status of " << static_cast<int>(result.status) << std::endl;
                candidates.push(result);
            }
            
            delete parser;
        }
    }

    // Add valid candidates to results
    while (candidates.size() > 0 && candidates.top().status == ParseStatus::VALID) {
        results.push_back(candidates.top());
        candidates.pop();
    }

    // Do no further processing if valid local verb found
    if (!results.empty())
        return results;

    // FIFTH: Check for global verb match
    verb = PartOfSpeech();
    range = Range(0, tokens.size());
    if (verb.findMatch(tokens, range, WordManager::hasGlobalVerb)) {
        // Found global verb; get grammar rules
        std::vector<VerbInfo> verbs = WordManager::getGlobalVerbs(verb.getAlias());
        // Run parser on each grammar definition and store the candidates
        for (auto it = verbs.begin(); it != verbs.end(); ++it) {
            parser = Sentence::makeSentence(it->grammar, verb, VerbType::GLOBAL, it->command);
            // Skip if unsupported grammar type
            if (parser == nullptr) {
                std::cerr << "Unsupported grammar type detected in " << verb.getAlias() << std::endl;
                continue;
            }
            result = parser->getResult(tokens, player, area, it->grammar);
            //std::cout << "Found global candidate with status of " << static_cast<int>(result.status) << std::endl;
            candidates.push(result);
            
            delete parser;
        }

        // Add valid candidates to results
        while (candidates.size() > 0 && candidates.top().status == ParseStatus::VALID) {
            results.push_back(candidates.top());
            candidates.pop();
        }

        // Do no further processing if valid global verb found
        if (!results.empty())
            return results;
    }

    // SIXTH: If there are no candidates yet, check for unavailable verbs
    if (candidates.empty()) {
        result.type = VerbType::INVALID;
        result.command = engine::CommandEnum::INVALID;
        result.unparsed = Tokenizer::joinOriginal(tokens, Range(0, tokens.size()));
        if (verb.findMatch(tokens, range, WordManager::hasVerb)) {
            // Found unavailable verb, set UNAVAILABLE_VERB
            result.status = ParseStatus::UNAVAILABLE_VERB;
        }
        else {
            // No unavailable verb, set INVALID_VERB
            result.status = ParseStatus::INVALID_VERB;
        }
        candidates.push(result);
    }

    // Add best candidates to results
    ParseStatus topStatus = candidates.top().status;
    //std::cout << "Top status of " << static_cast<int>(candidates.top().status) << std::endl;
    while (candidates.size() > 0 && candidates.top().status == topStatus) {
        results.push_back(candidates.top());
        candidates.pop();
    }

    return results;
}

#if 0
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

    std::vector<Match> matches;     // Stores potential matches
    Match m;                        // Stores match information
    std::string alias;              // Stores an alias
    TextParseResult result;         // Stores a parsing result
    ResultContainer results;        // Maintains the parsing results by priority

    // FIRST: Check for edit mode verbs if user has permission
    if (isAdmin) {
        m = parseGlobal(tokens, WordManager::hasEditModeVerb, WordManager::getEditModeVerbs, playerNounMap, areaNounMap);

        addMatchToResults(VerbType::EDITMODE, m, results);

        // Stop if we find a valid edit mode verb
        if (results.getBestStatus() == TextParseStatus::VALID) {
            return results.getBestResults(candidates);
        }
    }

    // SECOND: Check for world builder verbs if in builder mode
    if (editMode) {
        m = parseGlobal(tokens, WordManager::hasBuilderVerb, WordManager::getBuilderVerbs, playerNounMap, areaNounMap);

        addMatchToResults(VerbType::BUILDER, m, results);

        // Stop if we find a valid world builder verb
        if (results.getBestStatus() == TextParseStatus::VALID) {
            return results.getBestResults(candidates);
        }
    }

    // THIRD: Check for player verbs
    {
        m = parseLocal(tokens, playerVerbMap, playerNounMap, areaVerbMap, areaNounMap);
        result = TextParseResult(VerbType::LOCAL);

    }

    // FOURTH: Check for area verbs
    {
        result = TextParseResult(VerbType::LOCAL);

    }

    // FIFTH: Check for global verbs
    {
        m = parseGlobal(tokens, WordManager::hasGlobalVerb, WordManager::getGlobalVerbs, playerNounMap, areaNounMap);
        
        addMatchToResults(VerbType::GLOBAL, m, results);

        // Stop if we find a valid global verb
        if (results.getBestStatus() == TextParseStatus::VALID) {
            return results.getBestResults(candidates);
        }
    }

    // SIXTH: Check for unavailable verbs if no other valid verbs have been found
    if (results.getBestStatus() == TextParseStatus::INVALID_VERB) {
        // No usable verb found if we reach here
        // Check if verb is available anywhere
        Range r = Range(0, tokens.size());
        alias = findLongestGlobalAlias(WordManager::hasVerb, tokens, r);
        if (!alias.empty()) {
            // Verb found but unavailable; clear old candidates and add unparsed input with UNAVAILABLE_VERB.
            candidates.clear();
            result = TextParseResult(VerbType::UNAVAILABLE);
            result.command = engine::CommandEnum::INVALID;
            result.direct = nullptr;
            result.position = engine::ItemPosition::NONE;
            result.unparsed = joinOriginalTokens(tokens, Range(0, tokens.size()));
            candidates.push_back(result);
            return TextParseStatus::UNAVAILABLE_VERB;
        }
    }

    // Return highest result
    return results.getBestResults(candidates);
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

// Generates TextParseResults from the Match and adds them to the container.
void TextParser::addMatchToResults(VerbType type, const Match m, ResultContainer &results) {
    TextParseResult result;
    result.type = type;
    // If no direct objects, there will only be one result
    if (m.directObjs.empty()) {
        result.command = m.verbInfo.command;
        result.direct = nullptr;
        result.position = prepositionToPosition(m.preposition);
        result.unparsed = m.unparsed;
        for (auto it2 = m.indirectObjs.begin(); it2 != m.indirectObjs.end(); ++it2) {
            result.indirect.push_back(it2->second);
        }

        // Add result to container
        results.addResult(m.status, result);
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

            // Add result to container
            results.addResult(m.status, result);
        }
    }

}

// Tries to find a global match based on input tokens and maps
TextParser::Match TextParser::parseGlobal(const std::vector<Token> &tokens, bool (*hasAlias)(std::string), std::vector<VerbInfo> (*getVerbs)(std::string), const WordMap &playerNounMap, const WordMap &areaNounMap) {
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
        m.verbInfos = getVerbs(m.verbAlias);

        // Set range to start after verb
        range.start = range.end;
        range.end = tokens.size();

        // Check remaining input for direct object based on grammar
        for (auto verbInfoIt = m.verbInfos.begin(); verbInfoIt !+ m.verbInfos.end(); ++verbInfoIt) {
            switch (verbInfoIt->grammar.takesDirectObject()) {
            case Grammar::NO:
                switch (verbInfoIt->grammar.takesIndirectObject()) {
                case Grammar::NO:
                    // Supports verb only
                    break;
                case Grammar::YES:
                    // Supports indirect object only (with possible preposition)
                    break;
                case Grammar::TEXT:
                    // Supports indirect object text (with possible preposition)
                    break;
                }
                break;
            case Grammar::YES:
                switch (verbInfoIt->grammar.takesIndirectObject()) {
                case Grammar::NO:
                    // Supports verb and direct object only
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
                case Grammar::YES:
                    // Supports verb, direct object, and indirect object (with possible preposition)
                    break;
                case Grammar::TEXT:
                    // Supports verb, direct object, and indirect object text (with possible preposition)
                    break;
                }
                break;
            case Grammar::TEXT:
                switch (verbInfoIt->grammar.takesIndirectObject()) {
                case Grammar::NO:
                    // Supports verb and direct object text
                    break;
                case Grammar::YES:
                    // Supports verb, direct object text, and indirect object (with possible preposition)
                    break;
                case Grammar::TEXT:
                    // Supports verb, direct object text, and indirect object text (with possible preposition)
                    break;
                }
                break;
            }
        }
        switch (m.verbInfo.grammar.takesDirectObject()) {
        case Grammar::NO:
            switch (m.verbInfo.grammar.takesDirectObject()) {
            }
            // Do nothing
            break;
        case Grammar::OPTIONAL:
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
                
                // Text is required, so set status to invalid if no unparsed text
                m.status = m.unparsed.empty() ? TextParseStatus::INVALID_NOUN : TextParseStatus::VALID;

                return m;
            }
            else {
                // Try to find indirect object at end, moving toward the front
                // Write a findObjectFromEnd function?

                // If indirect object is found, check if prior word is preposition

            }
            break;
        case Grammar::OPTIONALTEXT:
            // If no further grammar is required, dump everything into unparsed text and return
            if (m.verbInfo.grammar.takesIndirectObject() == Grammar::NO) {
                m.unparsed = joinOriginalTokens(tokens, range);
                
                // Set status to valid, since this grammar part is optional
                m.status = TextParseStatus::VALID;

                return m;
            }
            else {
                // Try to find indirect object at end, moving toward the front

            }
            break;
        }

        // Parsing succeeded if we reach here.
        m.status = TextParseStatus::VALID;
    }
    else {
        // No verb found; add all tokens to unparsed
        m.status = TextParseStatus::INVALID_VERB;
        m.verbInfo = VerbInfo();
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


//********************************
// ResultContainer class
//********************************
TextParser::ResultContainer::ResultContainer() {
    // Default to lowest
    _bestSoFar = TextParseStatus::INVALID_VERB;
}

// Gets the best TextParseStatus so far.
TextParseStatus TextParser::ResultContainer::getBestStatus() const {
    return _bestSoFar;
}

// Gets the result(s) with the most complete parse status.
TextParseStatus TextParser::ResultContainer::getBestResults(std::vector<TextParseResult> &results) const {
    int highestPriority = 0;
    // Find result(s) with highest priority by verb type
    for (auto it = _results.find(_bestSoFar); it != _results.end(); ++it) {
        int priority = static_cast<int>(it->second.type);
        if (priority >= highestPriority) {
            highestPriority = priority;
        }
    }
    // Add the highest priority objects to the results
    for (auto it = _results.find(_bestSoFar); it != _results.end(); ++it) {
        int priority = static_cast<int>(it->second.type);
        if (priority == highestPriority) {
            results.push_back(it->second);
        }
    }

    return _bestSoFar;
}

// Adds a result to the ResultContainer and updates the highest status so far.
void TextParser::ResultContainer::addResult(TextParseStatus status, TextParseResult result) {
    // Update highest status as needed
    if (static_cast<int>(status) > static_cast<int>(_bestSoFar)) {
        _bestSoFar = status;
    }
    // Add result to map
    _results.emplace(status, result);

    // Increment counter
    _count++;
}

// Gets the total number of results in the container.
size_t TextParser::ResultContainer::getResultCount() const {
    return _count;
}
#endif
}}