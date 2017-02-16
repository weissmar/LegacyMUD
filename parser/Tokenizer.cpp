/*!
  \file     Tokenizer.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/14/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the Tokenizer class.
*/
#include "Tokenizer.hpp"

#include "WordManager.hpp"

#include <algorithm>
#include <string.h>
#include <sstream>
#include <iostream>

namespace legacymud { namespace parser { 

std::vector<Token> Tokenizer::tokenizeInput(std::string input) {
    std::istringstream iss(input);
    std::vector<Token> tokens;
    std::string token;
    while (iss >> token) {
        Token t;
        t.original = token;
        t.normalized.resize(t.original.size());
        std::transform(t.original.begin(), t.original.end(), t.normalized.begin(), ::tolower);
        tokens.push_back(t);
    }

    return tokens;
}

std::string Tokenizer::joinNormalized(const std::vector<Token> &tokens, Range range, bool skipIgnoreWords) {
    // Immediately return if invalid arguments
    if (range.start < 0       // start is negative
        || range.start >= range.end // start is at or after end
        || range.start >= tokens.size() // start is larger than vector size
        || range.end > tokens.size())   // end is larger than vector size
        return std::string();
    
    std::ostringstream oss;
    // Append space-delimited tokens depending on skipIgnoreWords setting
    for (size_t i = range.start; i < range.end; ++i) {
        if (!(skipIgnoreWords && WordManager::isIgnoreWord(tokens[i].normalized))) {
            if (!oss.str().empty()) {
                oss << " ";
            }
            oss << tokens[i].normalized;
        }
    }

    return oss.str();
}

std::string Tokenizer::joinOriginal(const std::vector<Token> &tokens, Range range) {
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


} }
