/*!
  \file    WordManager.h
  \author  David Rigert
  \date    02/02/2017
  \course  CS467, Winter 2017
 
  \details This file contains the declarations for the WordManager class.
           The WordManager class keeps track of all verbs and nouns in use
           by objects in the game world.
*/
#ifndef LEGACYMUD_PARSER_WORDMANAGER_HPP
#define LEGACYMUD_PARSER_WORDMANAGER_HPP

#include "Grammar.hpp"

#include <CommandEnum.hpp>

#include <vector>
#include <map>
#include <string>
#include <utility>

namespace legacymud { namespace parser {

/*! 
  \brief Specifies the struct used to store verb information.
*/
struct VerbInfo {
    Grammar grammar;              //!< Stores the grammar supported by the verb.
    engine::CommandEnum command;  //!< Stores the command mapped to the verb.
    std::string description;      //!< Stores the description of the verb shown in help.
};

/*! 
  \typedef GlobalVerbMap
  \brief Specifies the container type used to store global verbs.
*/
typedef std::map<std::string, VerbInfo> GlobalVerbMap;

/*! 
  \typedef WordCountMap
  \brief Specifies the container type used to store how many times a word is used.
*/
typedef std::map<std::string, unsigned int> WordCountMap;

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
      \brief Adds an entry to the edit mode verb lookup table.

      This function adds the specified string and corresponding VerbInfo object
      to the lookup table of verbs to enter edit mode.
      
      \note     Edit mode verbs have the highest priority. Matches for other
                verbs will not be returned if there is a matching edit mode verb.

      \warning  If \a alias is already in the edit mode verb list, the previous
                entry will be overwritten.

      \param[in]  alias     Specifies the text to use as the verb alias.
      \param[in]  info      Specifies the VerbInfo object that corresponds 
                            to the \a alias text.

      \pre \a alias is a valid, non-empty string.

      \post The specified alias and VerbInfo pair is added to the edit mode verb 
            lookup table.
    */
    static void addEditModeVerb(std::string alias, VerbInfo info);

    /*!
      \brief Adds an entry to the global verb lookup table.

      This function adds the specified string and corresponding VerbInfo object
      to the lookup table of global verbs that can be used in any area.
      
      \note     Local verbs have priority over global verbs. Matches for these
                verbs will not be returned if there are any matching local verbs.

      \warning  If \a alias is already in the global verb list, the previous 
                entry will be overwritten.

      \param[in]  alias     Specifies the text to use as the verb alias.
      \param[in]  info      Specifies the VerbInfo object that corresponds 
                            to the \a alias text.

      \pre \a alias is a valid, non-empty string.

      \post The specified alias and VerbInfo pair is added to the global verb 
            lookup table.
    */
    static void addGlobalVerb(std::string alias, VerbInfo info);

    /*!
      \brief Adds an entry to the world builder verb lookup table.

      This function adds the specified string and corresponding VerbInfo object
      to the lookup table of verbs that can only be used while in world builder 
      mode.
      
      \note     World builder verbs have priority over all other verbs except 
                edit mode. If a match is found for this verb type, no other
                potential matches will be returned.

      \warning  If \a alias is already in the builder verb list, the previous 
                entry will be overwritten.

      \param[in]  alias     Specifies the text to use as the verb alias.
      \param[in]  info      Specifies the VerbInfo object that corresponds 
                            to the \a alias text.

      \pre \a alias is a valid, non-empty string.

      \post The specified alias and VerbInfo pair is added to the builder verb 
            list.
    */
    static void addBuilderVerb(std::string alias, VerbInfo info);

    /*!
      \brief Adds an entry to the lookup table of noun aliases in use.

      This function adds the specified string to the lookup table of all 
      local noun aliases that are in use in the game world. The same noun 
      can (and should) be added to the table for every object it is used in.
      
      \param[in]  noun  Specifies the string to use as the noun alias.

      \pre \a noun is a valid, non-empty string.

      \post The specified \a noun has been added to the noun alias table.
    */
    static void addNoun(std::string noun);

    /*!
      \brief Adds a list of entries to the lookup table of noun aliases in use.

      This function adds all of the strings in the specified list to the 
      lookup table of all local noun aliases that are in use in the game world.
      The same noun can (and should) be added to the table for every object it 
      is used in.
      
      \param[in]  nouns     Specifies a list of strings to use as noun aliases.

      \pre \a nouns is a valid list of strings.
      \pre Each string in \a nouns is a valid, non-empty string.

      \post All of the strings in \a nouns have been added to the noun alias 
            table.
    */
    static void addNouns(const std::vector<std::string> &nouns);

    /*!
      \brief Adds an entry to the lookup table of verb aliases in use.

      This function adds the specified string to the lookup table of all 
      local verb aliases that are in use in the game world. The same verb 
      can (and should) be added to the table for every object it is used in.
      
      \param[in]  verb  Specifies the string to use as the verb alias.

      \pre \a verb is a valid, non-empty string.

      \post The specified \a verb has been added to the verb alias table.
    */
    static void addVerb(std::string verb);

    /*!
      \brief Adds a list of entries to the lookup table of verb aliases in use.

      This function adds all of the strings in the specified list to the 
      lookup table of all local verb aliases that are in use in the game world.
      The same verb can (and should) be added to the table for every object it 
      is used in.
      
      \param[in]  verbs     Specifies a list of strings to use as verb aliases.

      \pre \a verbs is a valid list of strings.
      \pre Each string in \a verbs is a valid, non-empty string.

      \post All of the strings in \a verbs have been added to the verb alias 
            table.
    */
    static void addVerbs(const std::vector<std::string> &verbs);

    /*!
      \brief Gets the VerbInfo object of the specified edit mode verb.

      This function gets the VerbInfo object that corresponds to the specified
      edit mode verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns the VerbInfo object that corresponds to \a alias.
    */
    static VerbInfo getEditModeVerb(std::string alias);

    /*!
      \brief Gets the VerbInfo object of the specified world builder verb.

      This function gets the VerbInfo object that corresponds to the specified
      world builder verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns the VerbInfo object that corresponds to \a alias.
    */
    static VerbInfo getBuilderVerb(std::string alias);

    /*!
      \brief Gets the VerbInfo object of the specified global verb.

      This function gets the VerbInfo object that corresponds to the specified
      global verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns the VerbInfo object that corresponds to \a alias.
    */
    static VerbInfo getGlobalVerb(std::string alias);

    /*!
      \brief Gets whether the specified noun alias is in use.
      
      This function gets whether the specified noun alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] noun   Specifies the noun alias to check for.

      \return Returns whether \a noun is in use.
    */
    static bool hasNoun(std::string noun);

    /*!
      \brief Gets whether the specified verb is in use.

      This function gets whether the specified verb alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] verb   Specifies the verb alias to check for.

      \return Returns whether \a verb is in use.
    */
    static bool hasVerb(std::string verb);

    /*!
      \brief Gets whether the specified edit mode verb has been added.

      \param[in] verb   Specifies the edit mode verb alias to check for.

      \return Returns whether \a verb has been added.
    */
    static bool hasEditModeVerb(std::string verb);

    /*!
      \brief Gets whether the specified world builder verb has been added.

      \param[in] verb   Specifies the world builder verb alias to check for.

      \return Returns whether \a verb has been added.
    */
    static bool hasBuilderVerb(std::string verb);

    /*!
      \brief Gets whether the specified global verb has been added.

      \param[in] verb   Specifies the global verb alias to check for.

      \return Returns whether \a verb has been added.
    */
    static bool hasGlobalVerb(std::string verb);

    /*!
      \brief Removes one use of a noun from the lookup table of noun aliases in use.

      This function decrements the usage counter for the specified \a noun.
      The same noun can (and should) be removed every time an object that uses
      the noun is destroyed.
      
      \param[in]  noun  Specifies the noun alias to remove.

      \pre \a noun is a valid, non-empty string.
      \pre \a noun exists in the table of noun aliases in use.

      \post The use count for the specified \a noun has been decremented by 1.
            If the use count becomes 0, the entry is removed from the table.
    */
    static void removeNoun(std::string noun);

    /*!
      \brief Removes one use of each string from the lookup table of noun aliases in use.

      This function decrements the usage counter for each string in the specified
      \a nouns list. The same noun can (and should) be removed every time an object 
      that uses the noun is destroyed.
      
      \param[in]  nouns     Specifies the noun aliases to remove.

      \pre \a nouns is a valid list of strings.
      \pre Each string in \a nouns is a valid, non-empty string.
      \pre Each string in \a nouns exists in the table of noun aliases in use.
      \pre If an alias appears multiple times in \a nouns, the counter must be
           at least that number.

      \post The use count for each alias in the specified \a nouns list has 
            been decremented by 1. If the use count becomes 0, the entry is 
            removed from the table.
    */
    static void removeNouns(const std::vector<std::string> &nouns);

    /*!
      \brief Removes one use of a verb from the lookup table of verb aliases in use.

      This function decrements the usage counter for the specified \a verb.
      The same verb can (and should) be removed every time an object that uses
      the verb is destroyed.
      
      \param[in]  verb  Specifies the verb alias to remove.

      \pre \a verb is a valid, non-empty string.
      \pre \a verb exists in the table of verb aliases in use.

      \post The use count for the specified \a verb has been decremented by 1.
            If the use count becomes 0, the entry is removed from the table.
    */
    static void removeVerb(std::string verb);

    /*!
      \brief Removes one use of each string from the lookup table of verb aliases in use.

      This function decrements the usage counter for each string in the specified
      \a verbs list. The same verb can (and should) be removed every time an object 
      that uses the verb is destroyed.
      
      \param[in]  verbs     Specifies the verb aliases to remove.

      \pre \a verbs is a valid list of strings.
      \pre Each string in \a verbs is a valid, non-empty string.
      \pre Each string in \a verbs exists in the table of verb aliases in use.
      \pre If an alias appears multiple times in \a verbs, the counter must be
           at least that number.

      \post The use count for each alias in the specified \a verbs list has 
            been decremented by 1. If the use count becomes 0, the entry is 
            removed from the table.
    */
    static void removeVerbs(const std::vector<std::string> &verbs);

    /*!
      \brief Resets the WordManager class and clears all words

      This function resets the WordManager class and clears the data
      stored in all member variables.
    */
    static void resetAll();

private:
    // Use a private constructor to prevent instantiation.
    WordManager() {}

    // Utility class to store the provided inputs into the specified private map
    static void addToMap(GlobalVerbMap &map, std::string alias, VerbInfo info);

    // Store the verbs to enable edit mode here.
    static GlobalVerbMap _editModeVerbs;
    
    // Store all world builder verbs here.
    static GlobalVerbMap _builderVerbs;

    // Store all global verbs here.
    static GlobalVerbMap _globalVerbs;

    // Store all nouns in the game world here.
    static WordCountMap _nounAliases;

    // Store all verbs in the game world here (not including global verbs).
    static WordCountMap _verbAliases;

};

}}

#endif
