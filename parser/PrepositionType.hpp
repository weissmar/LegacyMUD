/*!
  \file     PrepositionType.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/21/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration for the PrepositionType enumerated type.
            The PrepositionType enum indicates the lexical meaning of a preposition.
*/
#ifndef LEGACYMUD_PARSER_PREPOSITIONTYPE_HPP
#define LEGACYMUD_PARSER_PREPOSITIONTYPE_HPP

namespace legacymud { namespace parser {
/*!
  \enum legacymud::parser::PrepositionType
  \brief Enumerates the supported preposition meanings.

  This enumerated type represents the "meanings" of prepositions
  supported by each verb. When registering a verb, the caller must
  specify which prepositions are supported and the corresponding meaning.
*/
enum class PrepositionType {
    NONE      = 0,        //!< Does not have any meaning.
    TO        = (1 << 0), //!< Conveys the meaning of \em to, \em toward, \em at.
    WITH      = (1 << 1), //!< Conveys the meaning of \em with, \em using.
    OF        = (1 << 2), //!< The indirect object possesses the direct object. This makes the parser swap the direct and indirect objects in the ParseResult instance.
    FROM      = (1 << 3), //!< Conveys the meaning of \em from.
    ON        = (1 << 4), //!< Conveys the meaning of \em on, <i>on top of</i>.
    UNDER     = (1 << 5), //!< Conveys the meaning of \em under, \em underneath, \em below.
    IN        = (1 << 6), //!< Conveys the meaning of \em in, \em inside, \em into.
    ALL       = ~0        //!< All possible meanings.   
};

} }
#endif
