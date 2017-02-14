/*!
  \file     VDPISentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VDPISentence class.
            The VDPISentence class represents a Sentence with a verb, a direct 
            object, a preposition, and an indirect object.
*/
#ifndef LEGACYMUD_PARSER_VDPISENTENCE_HPP
#define LEGACYMUD_PARSER_VDPISENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"
#include "PrepositionType.hpp"

#include <InteractiveNoun.hpp>

#include <vector>

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with a verb, a direct object, a preposition, and an indirect object.
*/
class VDPISentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VDPISentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VDPISentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command);

    /*!
      \brief Gets a ParseResult object based on the parsed input string.

      \param[in] tokens     Specifies the token sequence to parse.
      \param[in] playerLex  Specifies the lexical data for objects in the player inventory.
      \param[in] areaLex    Specifies the lexical data for objects in the current area.
    */
    virtual ParseResult getResult(const std::vector<Token> &tokens, 
                                  const LexicalData &playerLex, 
                                  const LexicalData &areaLex);

private:
    engine::CommandEnum _command;
    VerbType _type;
    PartOfSpeech _verb;
    PartOfSpeech _direct;
    std::vector<engine::InteractiveNoun*> _directObjects;
    PartOfSpeech _preposition;
    PrepositionType _prepType;
    PartOfSpeech _indirect;
    std::vector<engine::InteractiveNoun*> _indirectObjects;
    ParseStatus _status;
};

} }
#endif
