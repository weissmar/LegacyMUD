/*!
  \file     LexicalData.hpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/16/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the LexicalData class.
            The LexicalData class is a container for local verb and noun data.
*/
#ifndef LEGACYMUD_PARSER_LEXICALDATA_HPP
#define LEGACYMUD_PARSER_LEXICALDATA_HPP

#include "WordMap.hpp"
#include <InteractiveNoun.hpp>

namespace legacymud { namespace parser {

/*!
  \brief Stores the word maps for local verbs and nouns.

  This class holds word maps for the verbs and nouns of objects in player inventory
  or in an area. It provides mutator and accessor functions, as well as functions
  to determine whether a particular verb or noun exists.
*/
class LexicalData {
public:
    /*!
      \brief Adds a noun alias to the lexical data.

      This function adds a noun alias to the local lexical data and
      adds an entry to the global table of verbs. Call this function whenever
      a new noun alias is added to an InteractiveNoun, or an InteractiveNoun
      is moved into a Player or Area.

      \param[in]  alias   Specifies the alias to add.
      \param[in]  pObject Specifies a pointer to the InteractiveNoun with the alias.
    */
    void addNoun(std::string alias, engine::InteractiveNoun *pObject);

    /*!
      \brief Adds a verb alias to the lexical data.

      This function adds a verb alias to the local lexical data and
      adds an entry to the global table of verbs. Call this function whenever
      a new verb alias is added to an InteractiveNoun, or an InteractiveNoun
      is moved into a Player or Area.

      \param[in]  alias   Specifies the alias to add.
      \param[in]  pObject Specifies a pointer to the InteractiveNoun with the alias.
    */
    void addVerb(std::string alias, engine::InteractiveNoun *pObject);

    /*!
      \brief Gets pointers to InteractiveNoun objects that match the specified noun alias.

      This function gets a vector of pointers to InteractiveNoun objects with
      a noun alias that matches the specified \a alias in the lexical data.

      \param[in]  alias   Specifies the alias to get matching InteractiveNoun pointers for.

      \return Returns a vector of InteractiveNoun pointers that match \a alias.
    */
    std::vector<engine::InteractiveNoun *> getObjectsByNoun(std::string alias) const;

    /*!
      \brief Gets pointers to InteractiveNoun objects that match the specified verb alias.

      This function gets a vector of pointers to InteractiveNoun objects with
      a verb alias that matches the specified \a alias in the lexical data.

      \param[in]  alias   Specifies the alias to get matching InteractiveNoun pointers for.

      \return Returns a vector of InteractiveNoun pointers that match \a alias.
    */
    std::vector<engine::InteractiveNoun *> getObjectsByVerb(std::string alias) const;

    /*!
      \brief Gets whether the lexical data includes the specified noun alias.

      \param[in]  alias   Specifies the alias to check for.

      \return Returns whether \a alias is in the lexical data for nouns.
    */
    bool hasNoun(std::string alias) const;

    /*!
      \brief Gets whether the lexical data includes the specified verb alias.

      \param[in]  alias   Specifies the alias to check for.

      \return Returns whether \a alias is in the lexical data for verbs.
    */
    bool hasVerb(std::string alias) const;

    /*!
      \brief Removes a noun alias from the lexical data.

      This function removes the specified a noun alias and InteractiveNoun pair
      from the local lexical data and also removes it from the global table of verbs.
      Call this function whenever a noun alias is deleted from an InteractiveNoun, 
      or an InteractiveNoun is moved out of a Player or Area.

      \param[in]  alias   Specifies the alias to add.
      \param[in]  pObject Specifies a pointer to the InteractiveNoun with the alias.
    */
    void removeNoun(std::string alias, engine::InteractiveNoun *pObject);

    void removeVerb(std::string alias, engine::InteractiveNoun *pObject);

    void clear();

    /*!
      \brief Forwards the function call to the hasNoun member function.

      This function forwards the caller to the hasNoun() member function
      of the LexicalData instance specified in \a context.

      \param[in] context  Specifies a pointer to the LexicalData instance to use.
      \param[in] arg      Specifies the string argument to use in hasNoun().

      \return Returns the result of hasNoun().
    */
    static bool forwardHasNoun(const void *context, std::string arg);

    /*!
      \brief Forwards the function call to the hasVerb member function.

      This function forwards the caller to the hasVerb() member function
      of the LexicalData instance specified in \a context.

      \param[in] context  Specifies a pointer to the LexicalData instance to use.
      \param[in] arg      Specifies the string argument to use in hasVerb().

      \return Returns the result of hasVerb().
    */
    static bool forwardHasVerb(const void *context, std::string arg);

private:
    WordMap _verbs;
    WordMap _nouns;
};

} }
#endif
