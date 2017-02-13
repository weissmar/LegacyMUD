/*!
  \file     VerbInfo.hpp
  \author   David Rigert
  \created  02/12/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declaration of the VerbInfo struct.
            The VerbInfo struct is used by the parser to keep track of information
            about a particular definition of a global verb.
*/
#ifndef LEGACYMUD_PARSER_VERBINFO_HPP
#define LEGACYMUD_PARSER_VERBINFO_HPP

#include "Grammar.hpp"

#include <CommandEnum.hpp>

#include <string>

namespace legacymud { namespace parser {

/*! 
  \brief Specifies the struct used to store verb information.
*/
struct VerbInfo {
    Grammar grammar;              //!< Stores the grammar supported by the verb.
    engine::CommandEnum command;  //!< Stores the command mapped to the verb.
    std::string description;      //!< Stores the description of the verb shown in help.

    VerbInfo() {
      grammar = Grammar(Grammar::Support::NO, false, Grammar::Support::NO);
      command = engine::CommandEnum::INVALID;
    }
};

}}

#endif
