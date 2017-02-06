/*!
  \file    parser_WordManager_Test.cpp
  \author  David Rigert
  \date    02/05/2017
  \course  CS467, Winter 2017
 
  \details This file contains the unit tests for the WordManager class.
  \note    Because WordManager is a static class, the resetAll() member
           function must be called after each test or the changes will 
           persist into subsequent tests.
*/

#include <WordManager.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Test fixture to clear the WordManager after every test
class WordManagerTest : public :: testing::Test {
public:
    virtual void SetUp() {

    }

    virtual void TearDown() {
        parser::WordManager::resetAll();
    }
};

// Verify reset functionality
TEST_F(WordManagerTest, ResetTest) {
    std::string word = "foo";
    parser::VerbInfo vi;
    parser::WordManager::addEditModeVerb(word, vi);
    parser::WordManager::addGlobalVerb(word, vi);
    parser::WordManager::addBuilderVerb(word, vi);
    parser::WordManager::addNoun(word);
    parser::WordManager::addVerb(word);

    EXPECT_TRUE(parser::WordManager::hasEditModeVerb("foo"));
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("foo"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("foo"));
    EXPECT_TRUE(parser::WordManager::hasNoun("foo"));
    EXPECT_TRUE(parser::WordManager::hasVerb("foo"));

    parser::WordManager::resetAll();

    EXPECT_FALSE(parser::WordManager::hasEditModeVerb("foo"));
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb("foo"));
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb("foo"));
    EXPECT_FALSE(parser::WordManager::hasNoun("foo"));
    EXPECT_FALSE(parser::WordManager::hasVerb("foo"));
}

// Verify that a word can correctly be added to the edit mode verb lookup table
TEST_F(WordManagerTest, AddEditModeVerbTest) {
    std::string word = "editmode";
    // Grammar objects default to NO direct and NO indirect object
    parser::VerbInfo vi;
    // Set the command to EDIT_MODE
    vi.command = engine::CommandEnum::EDIT_MODE;
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb(word));
    parser::WordManager::addGlobalVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::CommandEnum::EDIT_MODE, parser::WordManager::getGlobalVerb(word).command);
}

// Verify that a word can correctly be added to the global verb lookup table
TEST_F(WordManagerTest, AddGlobalVerbTest) {
    std::string word = "eat";
    parser::VerbInfo vi;
    // Set the command to EAT
    vi.command = engine::CommandEnum::EAT;
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb(word));
    parser::WordManager::addGlobalVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::CommandEnum::EAT, parser::WordManager::getGlobalVerb(word).command);
}

// Verify that a word can correctly be added to the builder verb lookup table
TEST_F(WordManagerTest, AddBuilderVerbTest) {
    std::string word = "edit";
    parser::VerbInfo vi;
    // Set the command to EDIT_WIZARD
    vi.command = engine::CommandEnum::EDIT_WIZARD;
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb(word));
    parser::WordManager::addBuilderVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::CommandEnum::EDIT_WIZARD, parser::WordManager::getBuilderVerb(word).command);
}

// Verify that a word can correctly be overwritten in the edit mode verb lookup table
TEST_F(WordManagerTest, OverwriteEditModeVerbTest) {
    std::string word = "editmode";
    parser::VerbInfo vi;
    // Set the command to INVALID
    vi.command = engine::CommandEnum::INVALID;
    EXPECT_FALSE(parser::WordManager::hasEditModeVerb(word));
    parser::WordManager::addEditModeVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasEditModeVerb(word));
    EXPECT_EQ(engine::CommandEnum::INVALID, parser::WordManager::getEditModeVerb(word).command);
    // Change the command to EDIT_MODE and set again
    vi.command = engine::CommandEnum::EDIT_MODE;
    parser::WordManager::addEditModeVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasEditModeVerb(word));
    EXPECT_EQ(engine::CommandEnum::EDIT_MODE, parser::WordManager::getEditModeVerb(word).command);
}

// Verify that a word can correctly be overwritten in the global verb lookup table
TEST_F(WordManagerTest, OverwriteGlobalVerbTest) {
    std::string word = "eat";
    parser::VerbInfo vi;
    // Set the command to INVALID
    vi.command = engine::CommandEnum::INVALID;
    EXPECT_FALSE(parser::WordManager::hasGlobalVerb(word));
    parser::WordManager::addGlobalVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::CommandEnum::INVALID, parser::WordManager::getGlobalVerb(word).command);
    // Change the command to EAT and set again
    vi.command = engine::CommandEnum::EAT;
    parser::WordManager::addGlobalVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb(word));
    EXPECT_EQ(engine::CommandEnum::EAT, parser::WordManager::getGlobalVerb(word).command);
}

// Verify that a word can correctly be overwritten in the builder verb lookup table
TEST_F(WordManagerTest, OverwriteBuilderVerbTest) {
    std::string word = "edit";
    parser::VerbInfo vi;
    // Set the command to INVALID
    vi.command = engine::CommandEnum::INVALID;
    EXPECT_FALSE(parser::WordManager::hasBuilderVerb(word));
    parser::WordManager::addBuilderVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::CommandEnum::INVALID, parser::WordManager::getBuilderVerb(word).command);
    // Change the command to EDIT_WIZARD and set again
    vi.command = engine::CommandEnum::EDIT_WIZARD;
    parser::WordManager::addBuilderVerb(word, vi);
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb(word));
    EXPECT_EQ(engine::CommandEnum::EDIT_WIZARD, parser::WordManager::getBuilderVerb(word).command);
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
    std::vector<std::string> words = {"eat", "drink", "eat"};
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
    std::vector<std::string> words = {"food", "soda", "food"};
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
    std::vector<std::string> words = {"eat", "eat"};
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
    std::vector<std::string> words = {"food", "food"};
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
    parser::VerbInfo vi;

    parser::WordManager::addEditModeVerb(word, vi);
    parser::WordManager::addGlobalVerb(word, vi);
    parser::WordManager::addBuilderVerb(word, vi);
    parser::WordManager::addNoun(word);
    parser::WordManager::addVerb(word);

    EXPECT_TRUE(parser::WordManager::hasEditModeVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasEditModeVerb("foobar"));
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasGlobalVerb("foobar"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasBuilderVerb("foobar"));
    EXPECT_TRUE(parser::WordManager::hasNoun("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasNoun("foobar"));
    EXPECT_TRUE(parser::WordManager::hasVerb("FOOBAR"));
    EXPECT_TRUE(parser::WordManager::hasVerb("foobar"));
}

}

