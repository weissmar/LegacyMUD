/*!
  \file     Tokenizer.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the Tokenizer class.
*/
#include "Tokenizer.hpp"

#include "WordManager.hpp"

#include <algorithm>
#include <string.h>
#include <sstream>

namespace legacymud { namespace parser { 

std::vector<Token> Tokenizer::tokenizeInput(std::string input) {
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

std::string Tokenizer::joinNormalized(const std::vector<Token> &tokens, Range range, bool skipIgnoreWords) {
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
