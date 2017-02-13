/*!
  \file     Tokenizer.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration for the Tokenizer class.
            The Tokenizer class is used to convert an input string into
            a token sequence, and convert a token sequence back into
            a string.
*/
#ifndef LEGACYMUD_PARSER_TOKENIZER_HPP
#define LEGACYMUD_PARSER_TOKENIZER_HPP

#include <string>
#include <vector>

namespace legacymud { namespace parser { 

/*!
  \brief Represents a range in a token list.

  This struct contains the start and end of a token sequence in a token list.
  \a end is not included in the range.
*/
struct Range {
    size_t start;
    size_t end;

    Range() : Range(0, 0) { }
    Range(size_t start, size_t end) {
        this->start = start;
        this->end = end;
    }
};

/*!
  \brief Represents a single token in a list of tokenized words.

  This struct contains the original and normalized text of a particular token
  in a token list. The normalized version is always lowercase.
*/
struct Token {
    std::string original;
    std::string normalized;
};

/*!
  \brief Represents an object for converting between an unstructured input
         string and a normalized token sequence.

  The Tokenizer class is used to convert an input string into a token sequence,
  and convert a token sequence back into a string.
*/
class Tokenizer {
public:
    /*!
      \brief Converts the specified \a input into a token list.

      \param[in] input  Specifies the input to convert.

      \return Returns a vector of Token objects in the order of appearance 
              in \a input.
    */
    static std::vector<Token> tokenizeInput(std::string input);

    /*!
      \brief Converts the specified \a range in \a tokens to a normalized string.

      This function converts the range of tokens specified in \a range to 
      a normalized (lowercase), space-delimited string.

      \param[in] tokens             Specifies the list containing the tokens to convert.
      \param[in] range              Specifies the range in \a tokens to convert and join.
      \param[in] skipIgnoreWords    Specifies whether to omit ignore words from the result.

      \return Returns the sequence of tokens in \a tokens specified by \a range
              as a normalized, lowercase, space-delimited string.
    */
    static std::string joinNormalized(const std::vector<Token> &tokens, Range range, bool skipIgnoreWords);

    /*!
      \brief Converts the specified \a range in \a tokens to a string with the original case.

      This function converts the range of tokens specified in \a range to 
      a space-delimited string with the original character case.

      \param[in] tokens     Specifies the list containing the tokens to convert.
      \param[in] range      Specifies the range in \a tokens to convert and join.

      \return Returns the sequence of tokens in \a tokens specified by \a range
              as a normalized, lowercase, space-delimited string.
    */
    static std::string joinOriginal(const std::vector<Token> &tokens, Range range);

private:
    // Make this private to prevent this class from being instantiated.
    Tokenizer() {}
};
    
} }
#endif
