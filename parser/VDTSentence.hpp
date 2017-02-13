/*!
  \file     VDTSentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VDTSentence class.
            The VDTSentence class represents a Sentence with a verb,
            a direct object, and a text-only indirect object.
*/
#ifndef LEGACYMUD_PARSER_VDTSENTENCE_HPP
#define LEGACYMUD_PARSER_VDTSENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"

#include <InteractiveNoun.hpp>

#include <vector>

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with a verb, a direct object, and a text-only indirect object.
*/
class VDTSentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VDTSentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VDTSentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex);

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
    PartOfSpeech _direct;
    std::vector<engine::InteractiveNoun*> _directObjects;
    PartOfSpeech _indirect;
    ParseStatus _status;
};

} }
#endif
