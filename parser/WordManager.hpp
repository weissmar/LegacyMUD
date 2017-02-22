/*!
  \file     WordManager.hpp
  \author   David Rigert
  \created  02/02/2017
  \modified 02/18/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the WordManager class.
            The WordManager class keeps track of all verbs and nouns in use
            by objects in the game world.
*/
#ifndef LEGACYMUD_PARSER_WORDMANAGER_HPP
#define LEGACYMUD_PARSER_WORDMANAGER_HPP

#include "Grammar.hpp"
#include "VerbInfo.hpp"
#include "WordMap.hpp"

#include <CommandEnum.hpp>

#include <vector>
#include <map>
#include <set>
#include <string>
#include <utility>

namespace legacymud { namespace engine {
    class InteractiveNoun;
}}

namespace legacymud { namespace parser {

/*! 
  \typedef GlobalVerbMap
  \brief Specifies the container type used to store global verbs.
*/
typedef std::multimap<std::string, VerbInfo> GlobalVerbMap;

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
      \brief Adds the specified word to the ignore list.

      When a word is in the ignore list, the parser will attempt to find a match
      both with and without that word. This is meant to be used for articles such
      as \em the, \em a, and \em and.

      \pre \a word is a valid, non-mepty string.

      \post The specified word is in the ignore list.
    */
    static void addIgnoreWord(std::string word);

    /*!
      \brief Adds an entry to the lookup table of noun aliases in use.

      This function adds the specified string to the lookup table of all 
      noun aliases that are in use in the game world. Every time a noun alias
      is added to an InteractiveNoun, it should be registered here.
      
      \param[in]  alias   Specifies the string to use as the noun alias.
      \param[in]  pObj    Specifies a pointer to the InteractiveNoun object
                          that has the alias.

      \pre \a alias is a valid, non-empty string.
      \pre \a pObj is a non-null pointer to a valid InteractiveNoun instance.

      \post The specified \a alias has been mapped to \a pObj in the noun lookup table.
    */
    static void addNoun(std::string alias, engine::InteractiveNoun *pObj);

    /*!
      \brief Adds an entry to the lookup table of verb aliases in use.

      This function adds the specified string to the lookup table of all 
      local verb aliases that are in use in the game world. Every time a verb 
      alias is added to an InteractiveNoun, it should be registered here.
      
      \param[in]  alias   Specifies the string to use as the verb alias.
      \param[in]  pObj    Specifies a pointer to the InteractiveNoun object
                          that has the alias.

      \pre \a alias is a valid, non-empty string.
      \pre \a pObj is a non-null pointer to a valid InteractiveNoun instance.

      \post The specified \a alias has been mapped to \a pObj in the verb lookup table.
    */
    static void addVerb(std::string alias, engine::InteractiveNoun *pObj);

    /*!
      \brief Gets the VerbInfo object of the specified edit mode verb.

      This function gets the VerbInfo object that corresponds to the specified
      edit mode verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns a list of VerbInfo objects that corresponds to \a alias.
    */
    static std::vector<VerbInfo> getEditModeVerbs(std::string alias);

    /*!
      \brief Gets the VerbInfo object of the specified world builder verb.

      This function gets the VerbInfo object that corresponds to the specified
      world builder verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns a list of VerbInfo objects that corresponds to \a alias.
    */
    static std::vector<VerbInfo> getBuilderVerbs(std::string alias);

    /*!
      \brief Gets the VerbInfo object of the specified global verb.

      This function gets the VerbInfo object that corresponds to the specified
      global verb. This function asserts if \a alias does not exist.

      \param[in] alias  Specifies the verb alias for which to get the VerbInfo.
      
      \return Returns a list of VerbInfo objects that corresponds to \a alias.
    */
    static std::vector<VerbInfo> getGlobalVerbs(std::string alias);

    /*!
      \brief Gets pointers to all local nouns in the game world with the specified alias.

      This function gets a vector of pointers to all InteractiveNoun objects in
      the game world with the specified noun \a alias.
      This is intended for use with world builder verbs.

      \param[in] alias  Specifies the alias alias for which to get the InteractiveNoun pointers.
      
      \return Returns a list of pointers to InteractiveNoun objects with \a alias as a noun alias.
    */
    static std::vector<engine::InteractiveNoun *> getLocalNouns(std::string alias);

    /*!
      \brief Gets whether the specified noun alias is in use.
      
      This function gets whether the specified noun alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] alias    Specifies the noun alias to check for.

      \return Returns whether \a alias is in use.
    */
    static bool hasNoun(const void *context, std::string alias);

    /*!
      \brief Gets whether the specified noun alias is in use.
      
      This function gets whether the specified noun alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] alias    Specifies the noun alias to check for.

      \return Returns whether \a alias is in use.
    */
    static bool hasNoun(std::string alias) { return hasNoun(0, alias); }

    /*!
      \brief Gets whether the specified verb is in use.

      This function gets whether the specified verb alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] alias    Specifies the verb alias to check for.

      \return Returns whether \a alias is in use.
    */
    static bool hasVerb(const void *context, std::string alias);

    /*!
      \brief Gets whether the specified verb is in use.

      This function gets whether the specified verb alias is currently in use
      by at least one InteractiveNoun object.

      \param[in] alias    Specifies the verb alias to check for.

      \return Returns whether \a alias is in use.
    */
    static bool hasVerb(std::string alias) { return hasVerb(0, alias); }

    /*!
      \brief Gets whether the specified edit mode verb alias has been added.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] alias    Specifies the edit mode verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasEditModeVerb(const void *context, std::string alias);

    /*!
      \brief Gets whether the specified edit mode verb alias has been added.

      \param[in] alias    Specifies the edit mode verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasEditModeVerb(std::string alias) { return hasEditModeVerb(0, alias); }

    /*!
      \brief Gets whether the specified world builder verb alias has been added.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] alias    Specifies the world builder verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasBuilderVerb(const void *context, std::string alias);

    /*!
      \brief Gets whether the specified world builder verb alias has been added.

      \param[in] alias    Specifies the world builder verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasBuilderVerb(std::string alias) { return hasBuilderVerb(0, alias); }

    /*!
      \brief Gets whether the specified global verb alias has been added.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] alias    Specifies the global verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasGlobalVerb(const void *context, std::string alias);

    /*!
      \brief Gets whether the specified global verb alias has been added.

      \param[in] alias    Specifies the global verb alias to check for.

      \return Returns whether \a alias has been added.
    */
    static bool hasGlobalVerb(std::string alias) { return hasGlobalVerb(0, alias); }

    /*!
      \brief Gets whether the specified word is an ignored work.

      \param[in] context  This parameter is for compatibility with the
                          \c findWord function pointer of the 
                          PartOfSpeech::findMatch() function.
                          The value entered here is ignored.
                          Use hasNoun(std::string) instead.
      \param[in] word     Specifies the word to check against the ignore word list.

      \return Returns whether \a word should be ignored.
    */
    static bool isIgnoreWord(const void *context, std::string word);

    /*!
      \brief Gets whether the specified word is an ignored work.

      \param[in] word     Specifies the word to check against the ignore word list.

      \return Returns whether \a word should be ignored.
    */
    static bool isIgnoreWord(std::string word) { return isIgnoreWord(0, word); }

    /*!
      \brief Removes the specified noun alias-InteractiveNoun pair from the lookup table.

      This function removes the specified alias-to-InteractiveNoun pair from the lookup
      table of all nouns in use. Call this function whenever a noun alias is removed
      from an InteractiveNoun object. If the InteractiveNoun object is being destroyed,
      call this function once for each noun alias of the InteractiveNoun. 
      
      \param[in]  alias   Specifies the noun alias to remove.
      \param[in]  pObj    Specifies a pointer to the InteractiveNoun object
                          that has the alias.

      \pre \a alias is a valid, non-empty string.
      \pre \a alias exists in the lookup table of noun aliases.

      \post The \a alias and \a pObj pair is no longer in the noun lookup table.
    */
    static void removeNoun(std::string alias, engine::InteractiveNoun *pObj);

    /*!
      \brief Removes the specified verb alias-InteractiveNoun pair from the lookup table.

      This function removes the specified alias-to-InteractiveNoun pair from the lookup
      table of all verbs in use. Call this function whenever a verb alias is removed
      from an InteractiveNoun object. If the InteractiveNoun object is being destroyed,
      call this function once for each verb alias of the InteractiveNoun. 
      
      \param[in]  alias   Specifies the verb alias to remove.
      \param[in]  pObj    Specifies a pointer to the InteractiveNoun object
                          that has the alias.

      \pre \a alias is a valid, non-empty string.
      \pre \a alias exists in the lookup table of verb aliases.

      \post The \a alias and \a pObj pair is no longer in the verb lookup table.
    */
    static void removeVerb(std::string alias, engine::InteractiveNoun *pObj);

    /*!
      \brief Resets the WordManager class and clears all words

      This function resets the WordManager class and clears the data
      stored in all member variables.
    */
    static void resetAll();

private:
    // Use a private constructor to prevent instantiation.
    WordManager() {}

    // Store the verbs to enable edit mode here.
    static GlobalVerbMap _editModeVerbs;
    
    // Store all world builder verbs here.
    static GlobalVerbMap _builderVerbs;

    // Store all global verbs here.
    static GlobalVerbMap _globalVerbs;

    // Store a map of all nouns in the game world here.
    static WordMap _localNouns;

    // Store a map of all verbs in the game world here.
    static WordMap _localVerbs;

    // Store ignore words
    static std::set<std::string> _ignoreWords;

};

}}

#endif
