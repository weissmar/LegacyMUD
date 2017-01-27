/*************************************************************************
 * Author:        David Rigert
 * Date Created:  01/26/2017
 * Last Modified: 
 * Course:        CS467, Winter 2017
 * Filename:      WordManager.h
 *
 * Overview:
 *     The WordManager class keeps track of all verbs and nouns in use
 *     by objects in the game world. It also stores global verbs that
 *     are supported anywhere in the game world, and which prepositions
 *     map to which position.
 *     This class should not be instantiated. All member functions are static.
 ************************************************************************/
#ifndef LEGACYMUD_PARSER_WORDMANAGER_H
#define LEGACYMUD_PARSER_WORDMANAGER_H

#include <list>
#include <map>
#include <mutex>
#include <string>

class WordManager {
public:
    static void addGlobalVerb(const std::string &verb, legacymud::engine::ActionType);

    static void addPreposition(const std::string &preposition, legacymud::engine::PositionType);

    static void addNoun(const std::string &noun);

    static void addNouns(const std::list<const std::string> &nouns);

    static void addVerb(const std::string &verb);

    static void addVerbs(const std::list<const std::string> &verbs);

    static const std::map<const std::string, unsigned int> &getAllNouns();

    static const std::map<const std::string, unsigned int> &getAllVerbs();

    static const std::map<const std::string, legacymud::engine::ActionType> &getGlobalVerbs();

    static void removeNoun(const std::string &noun);

    static void removeNouns(const std::list<const std::string> &nouns);

    static void removeVerb(const std::string &verb);

    static void removeVerbs(const std::list<const std::string> &verbs);

private:
    // Use a private constructor to prevent instantiation.
    WordManager() {}

    static std::map<std::string, legacymud::engine::ActionType> _globalVerbs;
    static std::map<std::string, legacymud::engine::PositionType> _prepositions;
    static std::map<std::string, unsigned int> _nounsInUse;
    static std::map<std::string, unsigned int> _verbsInUse;

};

#endif