/*!
  \file    TextParser.h
  \author  David Rigert
  \date    01/26/2017
  \course  CS467, Winter 2017
 
  \details This file contains the declarations for the TextParser class and
           its associated enumerated types and structures.
           The TextParser class validates text input from users and
           converts it into an enum that the game engine uses to determine
           which action to perform.
*/
#ifndef LEGACYMUD_PARSER_TEXTPARSER_H
#define LEGACYMUD_PARSER_TEXTPARSER_H

#include <list>
#include <map>
#include <string>

namespace legacymud { namespace parser {

/*!
  \typedef VerbMap

  \brief Defines the multimap type used to associate each verb with 
         pointers to compatible InteractiveNoun objects.
*/
typedef std::multimap<std::string, legacymud::engine::InteractiveNoun *> VerbMap;

/*!
  \enum legacymud::parser::TextParseStatus
  \brief Enumerates the results of the text parsing operation.

  This enumerated type is returned by the legacymud::parser::TextParser::parse()
  function to communicate the result of the operation to the caller.
*/
enum class TextParseStatus {
    VALID,              //!< Input string contains all valid words.
    INVALID_VERB,       //!< Input string contains invalid verb.
    INVALID_NOUN,       //!< Input string contains valid verb but invalid noun.
    UNAVAILABLE_VERB,   //!< Input string contains valid verb but not in current area.
    UNAVAILABLE_NOUN    //!< Input string contains valid noun but not in current area.
};

/*!
  \brief Stores the parsing result of the TextParser.

  This structure stores the parsing result of the TextParser.parse() function.
  The data is only valid if the TextParser.parse() function returns
  \c TextParseStatus::VALID.
*/
struct TextParseResult {
    /*!
      \brief The ActionType that the verb maps to.
    */
    legacymud::engine::ActionType action;

    /*!
      \brief The object that matches the direct noun of the input text.

      This member only points to an object if the input text has a direct noun
      that matches one of the verb aliases of an object in the current area,
      or a global verb. If there is no match, this is set to nullptr.
    */
    legacymud::engine::InteractiveNoun *direct;

    /*!
      \brief A list of objects that match the indirect noun of the input text.

      This list only contains objects if the input text has an indirect noun.
      The list will contain all objects with an alias that matches the indirect noun.
      If the sentence does not have an indirect noun, this list is empty.
    */
    std::list<legacymud::engine::InteractiveNoun *> indirect;

    /*!
      \brief The position of the direct object relative to the indirect object.

      This value is determined by the preposition of the input text.
    */
    legacymud::engine::PositionType position;

    /*!
      \brief Stores any message text associated with the action.

      This value is only used when the action is associated with a message,
      such as SAY, WHISPER and SHOUT.
    */
    std::string message;
};

/*!
  \brief Contains a function for parsing text input to determine which command to run.

  This class is used for parsing text commands entered by players. The actual lexical
  data is provided by the game engine or the WordManager class. This class merely encapsulates
  the parsing logic. Each thread should have its own instance of this class.
*/
class TextParser {
public:
    TextParser();
    ~TextParser();

    /*!
      \brief Converts text input from players into potential commands to be run.

      This function takes the text entered by a player and analyzes it to determine
      what action the player is attempting to take. If the input string corresponds
      to a valid command, that command is stored in the list specified in the 
      \a candidates parameter.

      The input text is parsed in the following order:
        -# Verb
        -# Direct object
        -# Preposition
        -# Indirect object

      If any of these parts of speech are invalid, the function immediately returns 
      an unsuccessful TextParseStatus value without analyzing the rest of the text.
      
      \param[in]  input         Specifies the input text to parse.
      \param[in]  areaVerbs     Specifies a map of verbs for all objects in the current
                                area, mapped to a pointer to the object that the verb
                                belongs to.
      \param[out] candidates    Holds the valid actions and matching objects found
                                by the parser.

      \pre \a input, \a areaVerbs, and \a candidates must point to valid objects.

      \post \a candidates contains a TextParseResult for each potential match.

      \return Returns whether the input text was successfully converted into an action.

      \retval TextParseStatus::VALID            There is at least one successful
                                                TextParseResult in \a candidates.
      \retval TextParseStatus::INVALID_VERB     The input text does not begin with a valid verb.
      \retval TextParseStatus::INVALID_NOUN     The input text begins with a valid verb,
                                                but includes an invalid direct or indirect noun.
                                                This result may also be returned if the text
                                                includes an invalid preposition.
      \retval TextParseStatus::UNAVAILABLE_VERB The input text begins with a valid verb,
                                                but it does not work with any object in
                                                the current area.
      \retval TextParseStatus::UNAVAILABLE_NOUN The input text contains all valid words,
                                                but the direct or indirect noun does not match
                                                any of the objects in the current area.
    */
    TextParseStatus parse(
        const std::string &input, 
        const VerbMap &areaVerbs,
        std::list<TextParseResult> &candidates
                         );

private:

};

}}

#endif