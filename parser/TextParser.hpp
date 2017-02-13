/*!
  \file     TextParser.hpp
  \author   David Rigert
  \created  02/02/2017
  \modified 02/12/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the declarations for the TextParser class and
            its associated enumerated types and structures.
            The TextParser class validates text input from users and
            converts it into an enum that the game engine uses to determine
            which action to perform.
*/
#ifndef LEGACYMUD_PARSER_TEXTPARSER_HPP
#define LEGACYMUD_PARSER_TEXTPARSER_HPP

#include "WordManager.hpp"
#include "ParseResult.hpp"
#include "ParseStatus.hpp"

#include <vector>
#include <string>

namespace legacymud { namespace parser {

class LexicalData;

/*!
  \brief Contains a function for parsing text input to determine which command to run.

  This class is used for parsing text commands entered by players. The actual lexical
  data is provided by the game engine or the WordManager class. This class merely encapsulates
  the parsing logic. Each thread should have its own instance of this class.
*/
class TextParser {
public:
    TextParser() {}
    ~TextParser();

    /*!
      \brief Converts text input from players into potential commands to be run.

      This function takes the text entered by a player and analyzes it to determine
      what action the player is attempting to take. If the input string corresponds
      to a valid command, that command is stored in the list specified in the 
      \a candidates parameter.

      The input text is parsed based on the \c Grammar object associated with the verb.
      Every sentence must begin with a verb, and can be configured to include the
      following parts of speech.
        - Direct object
        - Preposition
        - Indirect object

      If any of these parts of speech are invalid, no further processing is
      performed on that result, and the remaining tokens are placed in the 
      \c unparsed member of the ParseResult object.

      
      \param[in]  input     Specifies the input text to parse.
      \param[in]  player    Specifies the lexical data of the Player that
                            enteted the input string.
      \param[in]  area      Specifies the lexical data of the Area that the
                            Player is currently in.
      \param[in]  isAdmin   Specifies whether the player has admin permissions.
                            Defaults to false.
      \param[in]  editMode  Specifies whether the player is in edit mode.
                            Defaults to false.

      \pre \a input is a non-empty string.

      \return Returns a vector of the most complete ParseResult objects.
              If the parser finds multiple matches that are equally valid, it will
              return multiple ParseResult objects. The ParseResult objects with the
              ParseStatus value closest to ParseStatus::VALID are returned.
    */
    std::vector<ParseResult> parse(
        const std::string input, 
        const LexicalData &player,
        const LexicalData &area,
        bool isAdmin = false,
        bool editMode = false
                         );
private:
#if 0
    // Stores a candidate match
    struct Match {
        std::string verbAlias;
        std::map<std::string, engine::InteractiveNoun *> directObjs;
        PrepositionType preposition;
        std::map<std::string, engine::InteractiveNoun *> indirectObjs;
        std::string unparsed;
        std::vector<VerbInfo> verbInfos;
        TextParseStatus status;
    };

    // Manages the result candidates and keeps track of the best so far.
    class ResultContainer {
    public:
        ResultContainer();
        // Gets the best ParseStatus so far.
        ParseStatus getBestStatus() const;
        // Gets the result(s) with the most complete parse status.
        // If there are multiple results with the same ParseStatus,
        // the ones with the highest priority verb type are returned.
        TextParseStatus getBestResults(std::vector<TextParseResult> &results) const;
        // Adds a result to the ResultContainer and updates the highest status so far.
        void addResult(TextParseStatus status, TextParseResult result);
        size_t getResultCount() const;

    private:
        size_t _count;
        ParseStatus _bestSoFar;
        std::multimap<ParseStatus, ParseResult> _results;
    };

    // Find a matching alias in a WordMap
    std::string findLongestLocalAlias(const WordMap &lookupTable, const std::vector<Token> &tokens, Range &range);

    // Find a matching alias using the specified find function
    std::string findLongestGlobalAlias(bool (*hasAlias)(std::string), const std::vector<Token> &tokens, Range &range);

    // Tries to find a global match based on input tokens and maps
    Match parseGlobal(
        const std::vector<Token> &tokens,
        bool (*hasAlias)(std::string),
        std::vector<VerbInfo> (*getVerbs)(std::string),
        const WordMap &playerNounMap,
        const WordMap &areaNounMap
    );

    // Tries to find a local match based on input tokens and maps
    Match parseLocal(
        const std::vector<Token> &tokens,
        const WordMap &playerVerbMap,
        const WordMap &playerNounMap,
        const WordMap &areaVerbMap,
        const WordMap &areaNounMap
    );

    // Converts a PrepositionType value to an ItemPosition value
    engine::ItemPosition prepositionToPosition(PrepositionType preposition);
    
    // Generates TextParseResults from the Match and adds them to the container.
    void addMatchToResults(VerbType type, const Match m, ResultContainer &results);
#endif
};
}}

#endif