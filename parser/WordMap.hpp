/*!
  \file     WordMap.hpp
  \author   David Rigert
  \created  02/11/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the WordMap class.
            The WordMap class is a container that maps a verb or noun alias
            to a set of pointers to matching InteractiveNoun objects.
*/
#ifndef LEGACYMUD_PARSER_WORDMAP_HPP
#define LEGACYMUD_PARSER_WORDMAP_HPP

#include <string>
#include <map>
#include <set>
#include <vector>

namespace legacymud { namespace engine {
    class InteractiveNoun;
}}

namespace legacymud { namespace parser {

/*!
  \brief Represents a map of verb or noun aliases to the corresponding InteractiveNoun object.

  This class stores verb or noun aliases as a key to find the corresponding
  InteractiveNoun object. It guarantees that every key-value pair is unique.
  All comparisons are case-insensitive.
*/
class WordMap {
public:
    /*!
      \brief Default constructor. Initializes the map to an empty state.
    */
    WordMap();

    /*!
      \brief Adds a word-object pair to the map.

      This function adds the specified \a alias and maps it to the specified
      \a pObject. If the alias-object pair already exists, this function does
      nothing.

      \param[in] alias      Specifies the word alias part.
      \param[in] pObject    Specifies a pointer to the corresponding 
                            engine::InteractiveNoun object.
      
      \return Returns whether the word-object pair was added.
    */
    bool addWord(std::string alias, engine::InteractiveNoun *pObject);

    /*!
      \brief Gets whether the map contains the specified word.

      This function checks whether the specified \a alias is in the map.

      \param[in] alias      Specifies the word to check for.

      \return Returns whether \a alias is in the map.
    */
    bool hasWord(std::string alias) const;

    /*!
      \brief Gets an iterator to the beginning of the internal map.

      \return Returns an iterator to the beginning of the map.
    */
    std::map<std::string, std::set<engine::InteractiveNoun *>>::iterator begin();

    /*!
      \brief Gets an iterator to the end of the internal map.

      \return Returns an iterator to the end of the map.
    */
    std::map<std::string, std::set<engine::InteractiveNoun *>>::iterator end();

    /*!
      \brief Gets a vector of all objects mapped to the specified word.

      This function returns a vector of pointers to all InteractiveNoun objects
      that are mapped to the specified \a alias. This function returns an empty
      vector if the alias does not exist in the map.

      \param[in] alias      Specifies the word to check for.
      
      \return Returns a vector of matching InteractiveNoun pointers.
    */
    std::vector<engine::InteractiveNoun *> getObjects(std::string alias) const;

    /*!
      \brief Gets the number of word-object pairs stored in the map.

      This function gets the total number of InteractiveNoun pointers
      stored in the map.

      \return Returns the total number of word-object pairs.
    */
    size_t getSize() const;

    /*!
      \brief Removes a word-object pair from the map.

      This function removes the specified \a pObject from the map. 
      If no other objects use \a alias, then the alias is also removed.
      If the alias-object pair does not exist, this function does nothing.

      \param[in] alias      Specifies the alias part.
      \param[in] pObject    Specifies a pointer to the corresponding 
                            engine::InteractiveNoun object.
      
      \return Returns whether the word-object pair was removed.
    */
    bool removeWord(std::string alias, engine::InteractiveNoun *pObject);

    /*!
      \brief Clears all entries in the WordMap.

      This function resets the WordMap to its initial state.
    */
    void clear();

private:
    std::map<std::string, std::set<engine::InteractiveNoun *>> _wordMap;
    size_t _count;
};

} }
#endif
