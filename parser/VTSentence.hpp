/*!
  \file     VTSentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the VTSentence class.
            The VTSentence class represents a Sentence with a verb and
            a text-only direct object.
*/
#ifndef LEGACYMUD_PARSER_VTSENTENCE_HPP
#define LEGACYMUD_PARSER_VTSENTENCE_HPP

#include "Sentence.hpp"
#include "PartOfSpeech.hpp"

#include <InteractiveNoun.hpp>

#include <vector>

namespace legacymud { namespace parser {

/*!
  \brief Represents a Sentence with a verb and a text-only direct object.
*/
class VTSentence : public virtual Sentence {
public:
    /*!
      \brief Default constructor.

      The default constructor initializes the VTSentence to an unparsed state.
      The parse() function must be called before calling getResult().
    */
    VTSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command);

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
    ParseStatus _status;
};

} }
#endif
