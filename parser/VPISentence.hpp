/*!
  \file     VPISentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VPISentence class.
            The VPISentence class represents a Sentence with a verb,
            a preposition, and an indirect object.
*/
#ifndef LEGACYMUD_PARSER_VPISENTENCE_HPP
#define LEGACYMUD_PARSER_VPISENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"
#include "PrepositionType.hpp"

#include <InteractiveNoun.hpp>

#include <vector>

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with a verb, a preposition, and an indirect object.
*/
class VPISentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VPISentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VPISentence(VerbType type, const LexicalData &playerLex, const LexicalData &areaLex);

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
    PartOfSpeech _preposition;
    PrepositionType _prepType;
    PartOfSpeech _indirect;
    std::vector<engine::InteractiveNoun*> _indirectObjects;
    ParseStatus _status;
};

} }
#endif
