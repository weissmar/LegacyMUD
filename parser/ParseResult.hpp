/*!
  \file     ParseResult.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration of the ParseResult struct.
            The ParseResult struct is returned to the engine and used
            to determine which action to perform.
*/
#ifndef LEGACYMUD_PARSER_PARSERESULT_HPP
#define LEGACYMUD_PARSER_PARSERESULT_HPP

#include "VerbType.hpp"
#include "ParseStatus.hpp"

#include <CommandEnum.hpp>
#include <ItemPosition.hpp>

#include <string>
#include <vector>

namespace legacymud { namespace engine {
    class InteractiveNoun;
} }

namespace legacymud { namespace parser {

/*!
  \brief Stores a parsing result of the TextParser.

  This structure stores the parsing result of the TextParser.parse() function.
  The data is only valid if \c status is ParseStatus::VALID.
*/
struct ParseResult {
    /*!
      \brief The type of verb.
    */
    VerbType type;

    /*!
      \brief The parse status of this result.

      If this is anything other than ParseStatus::VALID, the result is invalid
      and should only be used for error handling.
    */
    ParseStatus status;

    /*!
      \brief The \c CommandEnum that the verb maps to.
    */
    engine::CommandEnum command;

    /*!
      \brief A vector of objects that match the direct noun of the input text.

      This vector only contains objects if the input text has a direct noun
      that matches one or more objects within the scope defined by the VerbType.
      If there is no match, this vector is empty.
    */
    std::vector<engine::InteractiveNoun *> direct;

    /*!
      \brief Stores the input text that matched the direct InteractiveNoun(s).

      This variable holds the input text that matched the direct 
      InteractiveNoun(s) in the result. If the Grammar has a text-type direct
      object, this holds the matching text.
      If the Grammar does not have a direct object or no matching InteractiveNoun
      could be found, this string is empty.
    */
    std::string directAlias;

    /*!
      \brief A vector of objects that match the indirect noun of the input text.

      This vector only contains objects if the input text has an indirect noun.
      that matches one or more objects within the scope defined by the VerbType.
      If the sentence does not have an indirect noun, this list is empty.
    */
    std::vector<engine::InteractiveNoun *> indirect;

    /*!
      \brief Stores the input text that matched the indirect InteractiveNoun(s).

      This variable holds the input text that matched the indirect 
      InteractiveNoun(s) in the result. If the Grammar has a text-type indirect
      object, this holds the matching text.
      If the Grammar does not have an indirect object or no matching InteractiveNoun
      could be found, this string is empty.
    */
    std::string indirectAlias;

    /*!
      \brief The position that the direct object should be placed in.

      This specifies the intended position of the direct object.
      If no position was specified, this will be ItemPosition::NONE.
    */
    engine::ItemPosition position;

    /*!
      \brief Stores any unparsed text after the action.

      This variable holds all text that could not be parsed.
    */
    std::string unparsed;

    ParseResult() : ParseResult(VerbType::INVALID) {}
    
    ParseResult(VerbType t) {
        type = t;
        command = engine::CommandEnum::INVALID;
        position = engine::ItemPosition::NONE;
    }
};

} }
#endif
