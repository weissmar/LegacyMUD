/*!
  \file     ParseStatus.hpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration for the ParseStatus enumerated type.
            The ParseStatus enum indicates the result of a parsing operation.
*/
#ifndef LEGACYMUD_PARSER_PARSESTATUS_HPP
#define LEGACYMUD_PARSER_PARSESTATUS_HPP

namespace legacymud { namespace parser {
/*!
  \enum legacymud::parser::ParseStatus
  \brief Enumerates the results of the text parsing operation.

  This enumerated type is returned by the legacymud::parser::TextParser::parse()
  function to communicate the result of the operation to the caller.
*/
enum class ParseStatus {
    UNPARSED = 0,         //!< Input string has not been parsed.
    INVALID_VERB,         //!< Input string contains invalid verb.
    UNAVAILABLE_VERB,     //!< Input string contains valid verb but not in current area.
    INVALID_DIRECT,       //!< Input string contains an invalid noun alias in the direct object position.
    UNAVAILABLE_DIRECT,   //!< Input string contains a valid noun alias in the direct object position but not in current area.
    INVALID_PREPOSITION,  //!< Input string contains an invalid preposition.
    INVALID_INDIRECT,     //!< Input string contains an invalid noun alias in the indirect object position.
    UNAVAILABLE_INDIRECT, //!< Input string contains a valid noun alias in the indirect object position but not in current area.
    VALID                 //!< Input string contains all valid words.
};

} }
#endif
