/*!
  \file     VSentence.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/13/2017
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
    VSentence(PartOfSpeech verb, VerbType type, engine::CommandEnum command);

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
    ParseStatus _status;
};

} }
#endif
