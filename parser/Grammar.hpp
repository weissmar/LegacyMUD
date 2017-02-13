/*!
  \file     Grammar.hpp
  \author   David Rigert
  \created  02/02/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the Grammar class and
            its associated types.
            The Grammar class represents the object types and prepositions
            supported by a verb.
*/
#ifndef LEGACYMUD_PARSER_GRAMMAR_HPP
#define LEGACYMUD_PARSER_GRAMMAR_HPP

#include "PrepositionType.hpp"

#include <map>
#include <string>

namespace legacymud { namespace parser {

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
        NO,   //!< Does not support the object type.
        YES,  //!< Requires the object type.
        TEXT  //!< Requires text that does not match an InteractiveNoun alias.
    };

    /*!
      \brief Default constructor.

      The default constructor initializes the object to:
        - No direct object
        - No preposition
        - No indirect object
    */
    Grammar() : Grammar(Support::NO, false, Support::NO) {}

    /*!
      \brief Constructor that specifies grammar rules.

      This constructor creates an instance with the grammar rules
      preconfigured to the specified values.

      \param[in] direct       Specifies whether the verb supports a direct object.
      \param[in] preposition  Specifies whether the verb supports a preposition.
      \param[in] indirect     Specifies whether the verb supports an indirect object.
    */
    Grammar(Support direct, bool preposition, Support indirect);

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
    void addPreposition(std::string prep, PrepositionType type);

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
      \brief Gets whether the grammar rules support the specified preposition.

      This function determines whether the specified preposition is supported
      by the Grammar rules.

      \param[in] prep   Specifies the preposition to check for.
      
      \return Returns whether \a prep is supported.
    */
    bool hasPreposition(std::string prep) const;

    /*!
      \brief Sets the objects supported by the associated verb.

      This function sets whether the associated verb supports direct
      and/or indirect objects. The support can be set to NO, OPTIONAL,
      or REQUIRED. If a verb that requires an indirect object does not
      have one in the input string, the TextParseResult will be set to invalid.

      \param[in] direct       Specifies whether the verb supports a direct object.
      \param[in] preposition  Specifies whether the verb supports a preposition.
      \param[in] indirect     Specifies whether the verb supports an indirect object.
    */
    void setSupport(Support direct, bool preposition, Support indirect);

    /*!
      \brief Gets whether the associated verb takes a direct object.

      \return Returns whether the associated verb takes a direct object.
    */
    Support takesDirectObject() const;

    /*!
      \brief Gets whether the associated verb takes a preposition.

      \return Returns whether the associated verb takes a preposition.
    */
    bool takesPreposition() const;

    /*!
      \brief Gets whether the associated verb takes an indirect object.

      \return Returns whether the associated verb takes an indirect object.
    */
    Support takesIndirectObject() const;

private:
    Support _directObject;
    bool _preposition;
    Support _indirectObject;
    PrepositionMap _prepositions;
};

} }
#endif
