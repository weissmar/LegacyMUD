/*!
  \file     TextParser.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/16/2017
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

}}
