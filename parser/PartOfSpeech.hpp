/*!
  \file     PartOfSpeech.hpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the PartOfSpeech class and
            its associated enumerated types and structures.
            The PartOfSpeech class is the base class for all parts of speech
            stored in a Sentence object.
*/
#ifndef LEGACYMUD_PARSER_PARTOFSPEECH_HPP
#define LEGACYMUD_PARSER_PARTOFSPEECH_HPP

#include "Tokenizer.hpp"

#include <InteractiveNoun.hpp>

#include <string>
#include <vector>

namespace legacymud { namespace parser {

class LexicalData;

/*!
  \brief Represents a part of speech.

  This class represents a particular part of speech within a sentence.
  It can be a verb, a direct object, a preposition, or an indirect object. 
*/
class PartOfSpeech {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the PartOfSpeech object to an
      invalid state.
    */
    PartOfSpeech();

    /*!
      \brief Finds the longest token sequence that matches the part of speech.

      This function uses \a findWord to find the longest matching sequence of words in \a tokens.
      It only checks the tokens indicated by \a range.

      \param[in]     tokens     Specifies the tokenized input text.
      \param[in,out] range      Specifies the range of tokens to check. This is updated
                                to the actual range of tokens that matched the part of speech.
      \param[in]     findWord   Specifies a pointer to the find function of the container
                                to check for a match in.
      
      \return Returns whether a match was found.
    */
    bool findMatch(const std::vector<Token> &tokens, Range &range, bool (*findWord)(std::string word));

    /*!
      \brief Gets the normalized alias of the matching part of speech.

      This function gets the normalized alias of the part of speech found after calling
      the findMatch() function.

      \return Returns the normalized alias, or an empty string if no matching 
              part of speech was found.
    */
    std::string getAlias() const;

    /*!
      \brief Gets the original alias of the matching part of speech.

      This function gets the original alias, as entered by the player,
      of the part of speech found after calling the findMatch() function. 

      \return Returns the original alias, or an empty string if no matching 
              part of speech was found.
    */
    std::string getOriginalAlias() const;

    /*!
      \brief Gets range of the matching part of speech in the token sequence.

      This function gets the matching range of tokens.

      \return Returns the range of the matching tokens, or a range of \c [0,0)
              if no matching part of speech was found.
    */
    Range getRange() const;

    /*!
      \brief Sets the alias to the specifies \a range of \a tokens.

      This function sets the normalized and original aliases to the token 
      sequence specified by \a range, and sets the instance to a valid state
      if \a range has at least one token in it.
      This is intended to be used for parts of speech that are parsed as raw
      text instead of as an InteractiveNoun.

      \return Returns whether the alias was successfully set.
    */
    bool setAlias(const std::vector<Token> &tokens, Range range);

    /*!
      \brief Gets whether the type of speech contains a valid match.

      This function gets the stored return value of the findMatch() function.

      \return Returns whether the last call to findMatch was successful.
    */
    bool isValid() const;

private:
    bool _isValid;
    std::string _alias;
    std::string _originalAlias;
    Range _range;
};

} }
#endif
