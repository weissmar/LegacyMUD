/*!
  \file     Sentence.hpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the Sentence class and
            its associated enumerated types and structures.
            The Sentence class represents one interpretation of the input string.
*/
#ifndef LEGACYMUD_PARSER_SENTENCE_HPP
#define LEGACYMUD_PARSER_SENTENCE_HPP

#include "ParseResult.hpp"
#include "VerbType.hpp"
#include "ParseStatus.hpp"

#include <vector>

namespace legacymud { namespace parser {

class Grammar;
class LexicalData;

/*!
  \brief Represents one interpretation of the input string.

  This abstract class holds an interpretation of the input string based on 
  the specified VerbType.
*/
class Sentence {
public:
    /*!
      \brief Creates an instance of a Sentence subclass based on Grammar rules.

      This factory function is used to create an instance of Sentence subclass based
      on the specified Grammar \a rules.

      \param[in] rules      Specifies the Grammar rules to use when making the sentence.
      \param[in] type       Specifies the type of verb (e.g. edit mode, builder, global, local, etc.)
      \param[in] playerLex  Specifies the lexical data for objects in the player inventory.
      \param[in] areaLex    Specifies the lexical data for objects in the current area.

      \return Returns a pointer to a concrete Sentence subclass object.
    */
    static Sentence *makeSentence(Grammar rules, VerbType type, const LexicalData &playerLex, const LexicalData &areaLex);

    /*!
      \brief Gets a vector of ParseResult objects based on the parsed input string.
    */
    virtual std::vector<ParseResult> getResults() = 0;

    /*!
      \brief Gets the ParseStatus of the highest ParseResult.
    */
    virtual ParseStatus getResultStatus() = 0;
};

} }
#endif
