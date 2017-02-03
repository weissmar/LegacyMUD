/*!
  \file    WordManager.h
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the declarations for the WordManager class.
           The WordManager class keeps track of all verbs and nouns in use
           by objects in the game world.
*/
#ifndef LEGACYMUD_PARSER_WORDMANAGER_H
#define LEGACYMUD_PARSER_WORDMANAGER_H

#include <engine.h>
#include "Verb.h"

#include <list>
#include <map>
#include <string>

namespace legacymud { namespace parser {

/*!
  \brief Stores and manages all verbs and nouns supported by the game world.

  The WordManager class keeps track of all verbs and nouns in use
  by objects in the game world. It also stores global verbs that
  are supported anywhere in the game world, and which prepositions
  map to which position.
  This class should not be instantiated. All member functions are static.
*/
class WordManager {
public:
    /*!
      \brief Adds an entry to the list of global verbs.

      This function adds the specified string and corresponding Verb object
      to the list of global verbs that can be used in any area.
      
      \note     Local verbs have priority over global verbs. Matches for these
                verbs will not be returned if there are any matching local verbs.

      \warning  If \a name is already in the global verb list, the previous entry
                will be overwritten.

      \param[in]  name      Specifies the text to use as the verb name.
      \param[in]  verb      Specifies the Verb object that corresponds to the 
                            \a name text.

      \pre \a name is a valid, non-empty string.

      \post The specified name and verb pair is added to the global verb list.
    */
    static void addGlobalVerb(std::string name, Verb verb);

    /*!
      \brief Adds an entry to the list of world builder verbs.

      This function adds the specified string and corresponding Verb object
      to the list of verbs that can only be used while in world builder mode.
      
      \note     World builder verbs have priority over all other verbs. Matches 
                for this verb type will always be first in the returned list.

      \warning  If \a name is already in the builder verb list, the previous entry
                will be overwritten.

      \param[in]  name      Specifies the text to use as the verb name.
      \param[in]  verb      Specifies the Verb object that corresponds to the 
                            \a name text.

      \pre \a name is a valid, non-empty string.

      \post The specified name and verb pair is added to the builder verb list.
    */
    static void addBuilderVerb(std::string name, Verb verb);

    /*!
      \brief Adds an entry to the list of noun aliases in use.

      This function adds the specified string to the list of all noun aliases that
      are in use in the game world. The same noun can (and should) be added to the list
      for every object it is used in.
      
      \param[in]  noun  Specifies the text to use as the noun.

      \pre \a noun is a valid, non-empty string.

      \post The specified \a noun has been added to the noun alias list.
    */
    static void addNoun(std::string noun);

    /*!
      \brief Adds a list of noun aliases to the list of noun aliases in use.

      This function adds all of the strings in the specified list to the list 
      of all noun aliases that are in use in the game world. The same noun can 
      (and should) be added to the list for every object it is used in.
      
      \param[in]  nouns     Specifies a list of strings to use as nouns.

      \pre \a nouns is a valid list of strings.
      \pre each string in \a nouns is a valid, non-empty string.

      \post All of the strings in \a nouns have been added to the noun alias list.
    */
    static void addNouns(const std::list<std::string> &nouns);

    /*!
      \brief Adds an entry to the list of verb aliases in use.

      This function adds the specified string to the list of all verb aliases that
      are in use in the game world. The same verb can (and should) be added to the list
      for every object it is used in.
      
      \param[in]  verb  Specifies the text to use as the verb.

      \pre \a verb is a valid, non-empty string.

      \post The specified \a verb has been added to the verb alias list.
    */
    static void addVerb(std::string verb);

    /*!
      \brief Adds a list of verb aliases to the list of verb aliases in use.

      This function adds all of the strings in the specified list to the list 
      of all verb aliases that are in use in the game world. The same verb can 
      (and should) be added to the list for every object it is used in.
      
      \param[in]  verbs     Specifies a list of strings to use as verbs.

      \pre \a verbs is a valid list of strings.
      \pre Each string in \a verbs is a valid, non-empty string.

      \post All of the strings in \a verbs have been added to the verb alias list.
    */
    static void addVerbs(const std::list<std::string> &verbs);

    /*!
      \brief Gets the Verb object of the specified world builder verb.

      This function gets the Verb object that corresponds to the specified
      global verb. This function asserts if \a name does not exist.

      \param[in] name   Specifies the verb name for which to get the Verb.
      
      \return Returns the Verb object that corresponds to \a name.
    */
    static Verb getBuilderVerb(std::string name);

    /*!
      \brief Gets the Verb object of the specified global verb.

      This function gets the Verb object that corresponds to the specified
      global verb. This function asserts if \a name does not exist.

      \param[in] name   Specifies the verb name for which to get the Verb.
      
      \return Returns the Verb object that corresponds to \a name.
    */
    static Verb getGlobalVerb(std::string name);

    /*!
      \brief Gets whether the specified noun is in use.
      
      This function gets whether the specified noun is currently in use by at least one object.

      \param[in] noun   Specifies the noun to check for.

      \return Returns whether \a noun is in use.
    */
    static bool hasNoun(std::string noun);

    /*!
      \brief Gets whether the specified verb is in use.

      This function gets whether the specified verb is currently in use by at least one object.

      \param[in] verb   Specifies the verb to check for.

      \return Returns whether \a verb is in use.
    */
    static bool hasVerb(std::string verb);

    /*!
      \brief Gets whether the specified global verb has been added.

      \param[in] verb   Specifies the global verb to check for.

      \return Returns whether \a verb has been added.
    */
    static bool hasBuilderVerb(std::string verb);

    /*!
      \brief Gets whether the specified global verb has been added.

      \param[in] verb   Specifies the global verb to check for.

      \return Returns whether \a verb has been added.
    */
    static bool hasGlobalVerb(std::string verb);

    /*!
      \brief Removes one use of a noun from the list of noun aliases in use.

      This function decrements the usage counter for the specified \a noun.
      The same noun can (and should) be removed every time an object that uses
      the noun is destroyed.
      
      \param[in]  noun  Specifies the noun alias to remove.

      \pre \a noun is a valid, non-empty string.
      \pre \a noun exists in the list of noun aliases in use.

      \post The use count for the specified \a noun has been decremented by 1.
            If the use count becomes 0, the entry is removed.
    */
    static void removeNoun(std::string noun);

    /*!
      \brief Removes one use of each string from the list of noun aliases in use.

      This function decrements the usage counter for each string in the specified
      \a nouns list. The same noun can (and should) be removed every time an object 
      that uses the noun is destroyed.
      
      \param[in]  nouns     Specifies the noun aliases to remove.

      \pre \a nouns is a valid list of strings.
      \pre Each string in \a nouns is a valid, non-empty string.
      \pre Each string in \a nouns exists in the list of noun aliases in use.
      \pre If an alias appears multiple times in \a nouns, the counter must be
           at least that number.

      \post The use count for each alias in the specified \a nouns list has 
            been decremented by 1. If the use count becomes 0, the entry is 
            removed.
    */
    static void removeNouns(const std::list<std::string> &nouns);

    /*!
      \brief Removes one use of a verb from the list of verb aliases in use.

      This function decrements the usage counter for the specified \a verb.
      The same verb can (and should) be removed every time an object that uses
      the verb is destroyed.
      
      \param[in]  verb  Specifies the verb alias to remove.

      \pre \a verb is a valid, non-empty string.
      \pre \a verb exists in the list of verb aliases in use.

      \post The use count for the specified \a verb has been decremented by 1.
            If the use count becomes 0, the entry is removed.
    */
    static void removeVerb(std::string verb);

    /*!
      \brief Removes one use of each string from the list of verb aliases in use.

      This function decrements the usage counter for each string in the specified
      \a verbs list. The same verb can (and should) be removed every time an object 
      that uses the verb is destroyed.
      
      \param[in]  verbs     Specifies the verb aliases to remove.

      \pre \a verbs is a valid list of strings.
      \pre Each string in \a verbs is a valid, non-empty string.
      \pre Each string in \a verbs exists in the list of verb aliases in use.
      \pre If an alias appears multiple times in \a verbs, the counter must be
           at least that number.

      \post The use count for each alias in the specified \a verbs list has 
            been decremented by 1. If the use count becomes 0, the entry is 
            removed.
    */
    static void removeVerbs(const std::list<std::string> &verbs);

    /*!
      \brief Resets the WordManager class and clears all words

      This function resets the WordManager class and clears the data
      stored in all member variables.
    */
    static void resetAll();

private:
    // Use a private constructor to prevent instantiation.
    WordManager() {}

    // Store all world builder verbs here.
    static std::map<std::string, Verb> _builderVerbs;

    // Store all global verbs here.
    static std::map<std::string, Verb> _globalVerbs;

    // Store all nouns in the game world here.
    static std::map<std::string, unsigned int> _nounAliases;

    // Store all verbs in the game world here (not including global verbs).
    static std::map<std::string, unsigned int> _verbAliases;

};

}}

#endif
