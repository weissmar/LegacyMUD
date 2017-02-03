/*!
  \file    Verb.h
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the declarations for the Verb class and
           its associated enumerated types and structures.
           The Verb class represents the actions and prepositions supported
           by a verb. It is meant to be used as the value in a \c VerbMap.
*/
#ifndef LEGACYMUD_PARSER_VERB_H
#define LEGACYMUD_PARSER_VERB_H

#include <engine.h>

#include <map>
#include <string>

namespace legacymud { namespace parser {

// forward declaration for Verb class
class Verb;

/*!
  \enum legacymud::parser::Preposition
  \brief Enumerates the supported preposition meanings.

  This enumerated type represents the "meanings" of prepositions
  supported by each verb. When registering a verb, the caller must
  specify which prepositions are supported and the corresponding meaning.
*/
enum class PrepositionMeaning {
    NONE    = 0,        //!< Does not have any meaning.
    TO      = 1,        //!< Conveys the meaning of \i to, \i toward, \i at.
    WITH    = (1 << 1), //!< Conveys the meaning of \i with, \i using.
    FROM    = (1 << 2), //!< Conveys the meaning of \i from.
    ON      = (1 << 3), //!< Conveys the meaning of \i on, <i>on top of</i>.
    UNDER   = (1 << 4), //!< Conveys the meaning of \i under, \i underneath, \i below.
    IN      = (1 << 5), //!< Conveys the meaning of \i in, \i inside, \i into.
    ALL     = ~0        //!< All possible meanings.   
};

/*!
  \typedef VerbMap

  \brief Defines the map type used to associate each verb alias with 
         pointers to compatible InteractiveNoun objects.
*/
typedef std::map<std::string, Verb> VerbMap;

/*!
  \typedef PrepositionMap

  \brief Defines the map type used to associate each preposition with 
         the meaning of it in the context of a verb.
*/
typedef std::map<std::string, PrepositionMeaning> PrepositionMap;

/*!
  \brief Represents a verb to be associated with an InteractiveNoun.

  This class stores the action and all of the prepositions supported by the verb.
*/
class Verb {
public:
    Verb();

    Verb(legacymud::engine::ActionType action);

    /*!
      \brief Adds an entry to the list of supported prepositions.

      This function adds the specified string and corresponding Preposition
      to the list of prepositions supported by the verb.
      
      \warning  If \a prep is already in the preposition list,
                the previous entry will be overwritten.

      \param[in]  prep      Specifies the text to use as the verb.
      \param[in]  meaning   Specifies the meaning of the \a prep text.

      \pre \a prep is a valid, non-empty string.

      \post The specified preposition and position pair is added to the preposition list.
    */
    void addPreposition(std::string prep, PrepositionMeaning meaning);

    legacymud::engine::ActionType getAction() const;

    /*!
      \brief Gets the meaning of the specified preposition.

      This function gets the PrepositionMeaning that corresponds to the 
      specified preposition. This function returns PrepositionMeaning::NONE 
      if \a prep does not exist.

      \param[in] prep   Specifies the preposition for which to get 
                        the PrepositionMeaning.
      
      \return Returns the PrepositionMeaning that corresponds to \a prep, 
              or PrepositionMeaning::NONE if it is not supported.
    */
    PrepositionMeaning getPrepositionMeaning(std::string prep) const;

    void setAction(legacymud::engine::ActionType action);
    
private:
    legacymud::engine::ActionType _action;
    PrepositionMap _prepositions;
};

} }
#endif
