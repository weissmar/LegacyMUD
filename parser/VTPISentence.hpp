/*!
  \file     VTPISentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VTPISentence class.
            The VTPISentence class represents a Sentence with a verb,
            a text-only direct object, a preposition, and an indirect object.
*/
#ifndef LEGACYMUD_PARSER_VTPISENTENCE_HPP
#define LEGACYMUD_PARSER_VTPISENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"
#include "PrepositionType.hpp"

#include <InteractiveNoun.hpp>

#include <vector>

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with a verb, a text-only direct object, a preposition, and an indirect object.
*/
class VTPISentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VTPISentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VTPISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command);

    /*!
      \brief Gets a ParseResult object based on the parsed input string.
    */
    virtual ParseResult getResult(const std::vector<Token> &tokens, 
                                  const LexicalData &playerLex, 
                                  const LexicalData &areaLex);

private:
    engine::CommandEnum _command;
    VerbType _type;
    PartOfSpeech _verb;
    PartOfSpeech _direct;
    PartOfSpeech _preposition;
    PrepositionType _prepType;
    PartOfSpeech _indirect;
    std::vector<engine::InteractiveNoun*> _indirectObjects;
    ParseStatus _status;
};

} }
#endif
