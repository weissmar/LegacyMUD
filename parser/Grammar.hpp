/*!
  \file    Grammar.hpp
  \author  David Rigert
  \date    02/04/2017
  \course  CS467, Winter 2017
 
  \details This file contains the declarations for the Grammar class and
           its associated enumerated types and structures.
           The Grammar class represents the object types and prepositions
           supported by a verb.
*/
#ifndef LEGACYMUD_PARSER_GRAMMAR_HPP
#define LEGACYMUD_PARSER_GRAMMAR_HPP

#include <CommandEnum.hpp>

#include <map>
#include <string>

namespace legacymud { namespace parser {

/*!
  \enum legacymud::parser::Preposition
  \brief Enumerates the supported preposition meanings.

  This enumerated type represents the "meanings" of prepositions
  supported by each verb. When registering a verb, the caller must
  specify which prepositions are supported and the corresponding meaning.
*/
enum class PrepositionType {
    NONE      = 0,        //!< Does not have any meaning.
    TO        = (1 << 0), //!< Conveys the meaning of \em to, \em toward, \em at.
    WITH      = (1 << 1), //!< Conveys the meaning of \em with, \em using.
    FROM      = (1 << 2), //!< Conveys the meaning of \em from.
    ON        = (1 << 3), //!< Conveys the meaning of \em on, <i>on top of</i>.
    UNDER     = (1 << 4), //!< Conveys the meaning of \em under, \em underneath, \em below.
    IN        = (1 << 5), //!< Conveys the meaning of \em in, \em inside, \em into.
    ALL       = ~0        //!< All possible meanings.   
};

/*!
  \typedef PrepositionMap

  \brief Defines the map type used to associate each preposition with 
         the meaning of it in the context of a verb.
*/
typedef std::map<std::string, PrepositionType> PrepositionMap;

/*!
  \brief Represents the grammar suported by verb alias of an InteractiveNoun.

  This class stores whether a verb takes a direct and/or indirect object, 
  and all of the prepositions supported by the verb.
*/
class Grammar {
public:
    enum Support {
        NO,       //!< Does not support the grammar type.
        OPTIONAL, //!< Optionally supports the grammar type.
        REQUIRED  //!< Requires the grammar type
    };

    Grammar() : Grammar(engine::CommandEnum::INVALID, Support::NO, Support::NO) {}

    Grammar(engine::CommandEnum command, Support directObject, Support indirectObject);

    /*!
      \brief Adds an entry to the list of supported prepositions.

      This function adds the specified string and corresponding PrepositionType
      to the list of prepositions supported by the verb.
      
      \warning  If \a prep is already in the preposition list,
                the previous entry will be overwritten.

      \param[in]  prep   Specifies the text to use as the verb.
      \param[in]  type   Specifies the meaning of the \a prep text.

      \pre \a prep is a valid, non-empty string.

      \post The specified preposition and position pair is added to the preposition list.
    */
    void addPreposition(std::string prep, PrepositionType meaning);

    /*!
      \brief Gets the meaning of the specified preposition.

      This function gets the PrepositionType that corresponds to the 
      specified preposition. This function returns PrepositionType::NONE 
      if \a prep does not exist.

      \param[in] prep   Specifies the preposition for which to get 
                        the PrepositionType.
      
      \return Returns the PrepositionType that corresponds to \a prep, 
              or PrepositionType::NONE if it is not supported.
    */
    PrepositionType getPrepositionType(std::string prep) const;

    /*!
      \brief Sets the objects supported by the associated verb.

      This function sets whether the associated verb supports direct
      and/or indirect objects. The support can be set to NO, OPTIONAL,
      or REQUIRED. If a verb that requires an indirect object does not
      have one in the input string, the TextParseResult will be set to invalid.

      \param[in] direct   Specifies whether the verb supports a direct object.
      \param[in] indirect Specifies whether the verb supports an indirect object.
    */
    void setObjectSupport(Support direct, Support indirect);

    /*!
      \brief Gets whether the associated verb takes a direct object.

      \return Returns whether the associated verb takes a direct object.
    */
    Support takesDirectObject() const;

    /*!
      \brief Gets whether the associated verb takes an indirect object.

      \return Returns whether the associated verb takes an indirect object.
    */
    Support takesIndirectObject() const;

private:
    engine::CommandEnum _command;
    Support _directObject;
    Support _indirectObject;
    PrepositionMap _prepositions;
};

} }
#endif
