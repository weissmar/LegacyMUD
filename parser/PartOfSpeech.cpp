/*!
  \file     PartOfSpeech.cpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the implementation of the PartOfSpeech class.
*/

#include "PartOfSpeech.hpp"

namespace legacymud { namespace parser {

PartOfSpeech::PartOfSpeech() {
    _isValid = false;
    _range = Range(0, 0);
}

bool PartOfSpeech::findMatch(const std::vector<Token> &tokens, Range &range, bool (*findWord)(const void *, std::string word), const void *context) {
    if (range.end > tokens.size()) {
        _alias = std::string();
        _originalAlias = std::string();
        _isValid = false;
        return _isValid;
    }

    Range originalRange = range;
    _isValid = false;

    // Search for entire range and chop one off the back each time
    for (; range.end > 0; --range.end) {
        // Try with ignore words
        std::string substring = Tokenizer::joinNormalized(tokens, range, false);
        if (!substring.empty() && findWord(context, substring)) {
            // Found a match
            _alias = substring;
            _originalAlias = Tokenizer::joinOriginal(tokens, range);
            _isValid = true;
            break;
        }
        else {
            // Try without ignore words
            substring = Tokenizer::joinNormalized(tokens, range, true);
            if (!substring.empty() && findWord(context, substring)) {
                // Found a match
                _alias = substring;
                _originalAlias = Tokenizer::joinOriginal(tokens, range);
                _isValid = true;
                break;
            }
        }
    }
    // Store range of match
    _range = range;

    if (!_isValid) {
        // Did not find a match.
        _alias = std::string();
        _originalAlias = std::string();
        range = originalRange;
        _isValid = false;
    }

    return _isValid;
}

std::string PartOfSpeech::getAlias() const {
    return _alias;
}

std::string PartOfSpeech::getOriginalAlias() const {
    return _originalAlias;
}

Range PartOfSpeech::getRange() const {
    return _range;
}

bool PartOfSpeech::setAlias(const std::vector<Token> &tokens, Range range) {
    if (range.end > tokens.size()) {
        _alias = std::string();
        _originalAlias = std::string();
        _isValid = false;
        return _isValid;
    }

    std::string normalized = Tokenizer::joinNormalized(tokens, range, false);
    std::string original = Tokenizer::joinOriginal(tokens, range);

    if (!normalized.empty() && !original.empty()) {
        _alias = normalized;
        _originalAlias = original;
        _range = range;
        _isValid = true;
    }
    else {
        _range = Range(0, 0);
        _alias = std::string();
        _originalAlias = std::string();
        _isValid = false;
    }

    return _isValid;
}

bool PartOfSpeech::isValid() const {
    return _isValid;
}

} }
