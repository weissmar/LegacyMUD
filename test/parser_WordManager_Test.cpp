/*!
  \file    parser_WordManager_Test.cpp
  \author  David Rigert
  \date    01/30/2017
  \course  CS467, Winter 2017
 
  \details This file contains the unit tests for the WordManager class.
  \note    Because WordManager is a static class, the resetAll() member
           function must be called after each test or the changes will 
           persist into subsequent tests.
*/

#include <parser.h>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Test fixture to clear the WordManager after every test
class WordManagerTest : public :: testing::Test {
public:
    virtual void TearDown() {
        parser::WordManager::resetAll();
    }
};

// Verify reset functionality
TEST_F(WordManagerTest, ResetTest) {
    std::string word = "foo";
    parser::WordManager::addGlobalVerb(word, engine::ActionType::NONE);
    parser::WordManager::addBuilderVerb(word, engine::ActionType::NONE);
    parser::WordManager::addPreposition(word, engine::PositionType::NONE);
    parser::WordManager::addNoun(word);
    parser::WordManager::addVerb(word);

    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("foo"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("foo"));
    EXPECT_TRUE(parser::WordManager::hasPreposition("foo"));
    EXPECT_TRUE(parser::WordManager::hasNoun("foo"));
    EXPECT_TRUE(parser::WordManager::hasVerb("foo"));

    parser::WordManager::resetAll();

    EXPECT_FALSE(parser::WordManager::hasGlobalVerb("foo"));
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb("foo"));
    EXPECT_FALSE(parser::WordManager::hasPreposition("foo"));
    EXPECT_FALSE(parser::WordManager::hasNoun("foo"));
    EXPECT_FALSE(parser::WordManager::hasVerb("foo"));
}

// Verify that a word can correctly be added to the global verb list
TEST_F(WordManagerTest, AddGlobalVerbTest) {
    std::string word = "eat";
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb(word));
    parser::WordManager::addGlobalVerb(word, engine::ActionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::ActionType::NONE, parser::WordManager::getGlobalVerbAction(word));
}

// Verify that a word can correctly be added to the builder verb list
TEST_F(WordManagerTest, AddBuilderVerbTest) {
    std::string word = "edit";
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb(word));
    parser::WordManager::addBuilderVerb(word, engine::ActionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::ActionType::NONE, parser::WordManager::getBuilderVerbAction(word));
}

// Verify that a word can correctly be added to the preposition list
TEST_F(WordManagerTest, AddPrepositionTest) {
    std::string word = "to";
    EXPECT_FALSE(parser::WordManager::hasPreposition(word));
    parser::WordManager::addPreposition(word, engine::PositionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasPreposition(word));
    EXPECT_EQ(engine::PositionType::NONE, parser::WordManager::getPrepositionPosition(word));
}

// Verify that a word can correctly be overwritten in the global verb list
TEST_F(WordManagerTest, OverwriteGlobalVerbTest) {
    std::string word = "eat";
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb(word));
    parser::WordManager::addGlobalVerb(word, engine::ActionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::ActionType::NONE, parser::WordManager::getGlobalVerbAction(word));
    parser::WordManager::addGlobalVerb(word, engine::ActionType::SOME);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::ActionType::SOME, parser::WordManager::getGlobalVerbAction(word));
}

// Verify that a word can correctly be overwritten in the builder verb list
TEST_F(WordManagerTest, OverwriteBuilderVerbTest) {
    std::string word = "edit";
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb(word));
    parser::WordManager::addBuilderVerb(word, engine::ActionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::ActionType::NONE, parser::WordManager::getBuilderVerbAction(word));
    parser::WordManager::addBuilderVerb(word, engine::ActionType::SOME);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::ActionType::SOME, parser::WordManager::getBuilderVerbAction(word));
}

// Verify that a word can correctly be overwritten in the preposition list
TEST_F(WordManagerTest, OverwritePrepositionTest) {
    std::string word = "to";
    EXPECT_FALSE(parser::WordManager::hasPreposition(word));
    parser::WordManager::addPreposition(word, engine::PositionType::NONE);
    EXPECT_TRUE(parser::WordManager::hasPreposition(word));
    EXPECT_EQ(engine::PositionType::NONE, parser::WordManager::getPrepositionPosition(word));
    parser::WordManager::addPreposition(word, engine::PositionType::TO);
    EXPECT_TRUE(parser::WordManager::hasPreposition(word));
    EXPECT_EQ(engine::PositionType::TO, parser::WordManager::getPrepositionPosition(word));
}

// Verify that a word can correctly be added to and removed from the list of verbs in use
TEST_F(WordManagerTest, AddRemoveVerbTest) {
    std::string word = "eat";
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
    parser::WordManager::addVerb(word);
    EXPECT_TRUE(parser::WordManager::hasVerb(word));
    parser::WordManager::removeVerb(word);
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
}

// Verify that a word can correctly be added to and removed from the list of nouns in use
TEST_F(WordManagerTest, AddRemoveNounTest) {
    std::string word = "food";
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
    parser::WordManager::addNoun(word);
    EXPECT_TRUE(parser::WordManager::hasNoun(word));
    parser::WordManager::removeNoun(word);
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
}

// Verify that the addVerbs and removeVerbs functions work
TEST_F(WordManagerTest, AddRemoveVerbsTest) {
    std::list<std::string> words = {"eat", "drink", "eat"};
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasVerb(*it));
    parser::WordManager::addVerbs(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_TRUE(parser::WordManager::hasVerb(*it));
    parser::WordManager::removeVerbs(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasVerb(*it));
}

// Verify that the addNouns and removeNouns functions work
TEST_F(WordManagerTest, AddRemoveNounsTest) {
    std::list<std::string> words = {"food", "soda", "food"};
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasNoun(*it));
    parser::WordManager::addNouns(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_TRUE(parser::WordManager::hasNoun(*it));
    parser::WordManager::removeNouns(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasNoun(*it));
}

// Verify support for adding and removing verbs multiple times
TEST_F(WordManagerTest, AddRemoveMultipleVerbTest) {
    std::string word = "eat";
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
    parser::WordManager::addVerb(word);
    parser::WordManager::addVerb(word);
    EXPECT_TRUE(parser::WordManager::hasVerb(word));
    parser::WordManager::removeVerb(word);
    EXPECT_TRUE(parser::WordManager::hasVerb(word));
    parser::WordManager::removeVerb(word);
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
}

// Verify support for adding and removing nouns multiple times
TEST_F(WordManagerTest, AddRemoveMultipleNounTest) {
    std::string word = "food";
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
    parser::WordManager::addNoun(word);
    parser::WordManager::addNoun(word);
    EXPECT_TRUE(parser::WordManager::hasNoun(word));
    parser::WordManager::removeNoun(word);
    EXPECT_TRUE(parser::WordManager::hasNoun(word));
    parser::WordManager::removeNoun(word);
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
}

// Verify support for adding and removing different verbs
TEST_F(WordManagerTest, AddRemoveDifferentVerbTest) {
    std::string word1 = "eat";
    std::string word2 = "drink";
    EXPECT_FALSE(parser::WordManager::hasVerb(word1));
    EXPECT_FALSE(parser::WordManager::hasVerb(word2));
    parser::WordManager::addVerb(word1);
    parser::WordManager::addVerb(word2);
    EXPECT_TRUE(parser::WordManager::hasVerb(word1));
    EXPECT_TRUE(parser::WordManager::hasVerb(word2));
    parser::WordManager::removeVerb(word1);
    EXPECT_FALSE(parser::WordManager::hasVerb(word1));
    EXPECT_TRUE(parser::WordManager::hasVerb(word2));
    parser::WordManager::removeVerb(word2);
    EXPECT_FALSE(parser::WordManager::hasVerb(word1));
    EXPECT_FALSE(parser::WordManager::hasVerb(word2));
}

// Verify support for adding and removing different verbs
TEST_F(WordManagerTest, AddRemoveDifferentNounTest) {
    std::string word1 = "food";
    std::string word2 = "soda";
    EXPECT_FALSE(parser::WordManager::hasNoun(word1));
    EXPECT_FALSE(parser::WordManager::hasNoun(word2));
    parser::WordManager::addNoun(word1);
    parser::WordManager::addNoun(word2);
    EXPECT_TRUE(parser::WordManager::hasNoun(word1));
    EXPECT_TRUE(parser::WordManager::hasNoun(word2));
    parser::WordManager::removeNoun(word1);
    EXPECT_FALSE(parser::WordManager::hasNoun(word1));
    EXPECT_TRUE(parser::WordManager::hasNoun(word2));
    parser::WordManager::removeNoun(word2);
    EXPECT_FALSE(parser::WordManager::hasNoun(word1));
    EXPECT_FALSE(parser::WordManager::hasNoun(word2));
}

// Verify behavior when attempting to remove word too many times
TEST_F(WordManagerTest, RemoveVerbTooManyTimesTest) {
    std::string word = "eat";
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
    parser::WordManager::addVerb(word);
    parser::WordManager::addVerb(word);
    EXPECT_TRUE(parser::WordManager::hasVerb(word));
    parser::WordManager::removeVerb(word);
    EXPECT_TRUE(parser::WordManager::hasVerb(word));
    parser::WordManager::removeVerb(word);
    EXPECT_FALSE(parser::WordManager::hasVerb(word));
    EXPECT_DEATH(parser::WordManager::removeVerb(word), "");
}

// Verify behavior when attempting to remove word too many times
TEST_F(WordManagerTest, RemoveNounTooManyTimesTest) {
    std::string word = "food";
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
    parser::WordManager::addNoun(word);
    parser::WordManager::addNoun(word);
    EXPECT_TRUE(parser::WordManager::hasNoun(word));
    parser::WordManager::removeNoun(word);
    EXPECT_TRUE(parser::WordManager::hasNoun(word));
    parser::WordManager::removeNoun(word);
    EXPECT_FALSE(parser::WordManager::hasNoun(word));
    EXPECT_DEATH(parser::WordManager::removeNoun(word), "");
}

// Verify behavior when attempting to use removeVerbs with word that appears more than map
TEST_F(WordManagerTest, RemoveVerbsTooManyTimesTest) {
    std::list<std::string> words = {"eat", "eat"};
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasVerb(*it));
    parser::WordManager::addVerbs(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_TRUE(parser::WordManager::hasVerb(*it));
    parser::WordManager::removeVerb("eat"); // remove one manually
    EXPECT_TRUE(parser::WordManager::hasVerb("eat"));
    EXPECT_DEATH(parser::WordManager::removeVerbs(words), "");
}

// Verify behavior when attempting to use removeNouns with word that appears more than map
TEST_F(WordManagerTest, RemoveNounsTooManyTimesTest) {
    std::list<std::string> words = {"food", "food"};
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_FALSE(parser::WordManager::hasNoun(*it));
    parser::WordManager::addNouns(words);
    for (auto it = words.begin(); it != words.end(); ++it)
        EXPECT_TRUE(parser::WordManager::hasNoun(*it));
    parser::WordManager::removeNoun("food"); // remove one manually
    EXPECT_TRUE(parser::WordManager::hasNoun("food"));
    EXPECT_DEATH(parser::WordManager::removeNouns(words), "");
}

// Verify case insensitivity
TEST_F(WordManagerTest, CaseInsensitivityTest) {
    std::string word = "FooBar";
    parser::WordManager::addGlobalVerb(word, engine::ActionType::NONE);
    parser::WordManager::addBuilderVerb(word, engine::ActionType::NONE);
    parser::WordManager::addPreposition(word, engine::PositionType::NONE);
    parser::WordManager::addNoun(word);
    parser::WordManager::addVerb(word);

    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("foobar"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("foobar"));
    EXPECT_TRUE(parser::WordManager::hasPreposition("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasPreposition("foobar"));
    EXPECT_TRUE(parser::WordManager::hasNoun("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasNoun("foobar"));
    EXPECT_TRUE(parser::WordManager::hasVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasVerb("foobar"));
}

}

