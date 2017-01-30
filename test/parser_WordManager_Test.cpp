/*!
  \file    parser_WordManager_Test.cpp
  \author  David Rigert
  \date    01/29/2017
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

// Test fixture to clear the WordManager after every test
class WordManagerTest : public :: testing::Test {
public:
    virtual void TearDown() {
        parser::WordManager::resetAll();
    }
};

// Verify that global word lists are blank by default
TEST_F(WordManagerTest, EmptyListTest) {
    EXPECT_EQ(0, parser::WordManager::getAllNouns().size());
    EXPECT_EQ(0, parser::WordManager::getAllVerbs().size());
    EXPECT_EQ(0, parser::WordManager::getPrepositions().size());
    EXPECT_EQ(0, parser::WordManager::getGlobalVerbs().size());
}

// Verify that a word can correctly be added to the global verb list
TEST_F(WordManagerTest, AddGlobalVerbTest) {
    std::string word = "eat";
    parser::WordManager::addGlobalVerb(word, legacymud::engine::ActionType::NONE);
    EXPECT_EQ(1, parser::WordManager::getGlobalVerbs().size());
    EXPECT_TRUE(parser::WordManager::getGlobalVerbs().at(word) == legacymud::engine::ActionType::NONE);
}


}

