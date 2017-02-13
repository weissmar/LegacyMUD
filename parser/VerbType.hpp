/*!
  \file     VerbType.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration for the VerbType enumerated type.
            The VerbType enum represents the type of the verb in a Sentence.
            It is used to determine the priority of the sentence when there are multiple
            sentences with the same validity.
*/
#ifndef LEGACYMUD_PARSER_VERBTYPE_HPP
#define LEGACYMUD_PARSER_VERBTYPE_HPP

/*!
  \enum legacymud::parser::VerbType
  \brief Enumerates the possible types of verb.

  This enumerated type is used to prioritize parse results by verb type.
*/
enum class VerbType {
    INVALID = 0,
    UNAVAILABLE,
    GLOBAL,
    LOCAL,
    BUILDER,
    EDITMODE
};

#endif
