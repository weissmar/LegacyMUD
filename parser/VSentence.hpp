/*!
  \file     VSentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VSentence class.
            The VSentence class represents a Sentence with only a verb.
*/
#ifndef LEGACYMUD_PARSER_VSENTENCE_HPP
#define LEGACYMUD_PARSER_VSENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with only a verb.
*/
class VSentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VSentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VSentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex);

    /*!
      \brief Gets a vector of ParseResult objects based on the parsed input string.
    */
    virtual std::vector<ParseResult> getResults();

    /*!
      \brief Gets the ParseStatus of the highest ParseResult.
    */
    virtual ParseStatus getResultStatus();

private:
    PartOfSpeech _verb;
    ParseStatus _status;
};

} }
#endif
