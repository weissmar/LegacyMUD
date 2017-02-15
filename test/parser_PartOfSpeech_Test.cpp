/*!
  \file     parser_PartOfSpeech_Test.cpp
  \author   David Rigert
  \created  02/14/2017
  \modified 02/14/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the PartOfSpeech class.
*/

#include <PartOfSpeech.hpp>
#include <LexicalData.hpp>
#include <Tokenizer.hpp>
#include <WordManager.hpp>
#include <VerbInfo.hpp>
#include <Item.hpp>

#include <gtest/gtest.h>
#include <iostream>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

parser::LexicalData areaLex;
engine::InteractiveNoun *in;
std::vector<parser::Token> tokens;
parser::Range range;


// Test fixture to set up and tear down each unit test
class PartOfSpeechTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
        parser::VerbInfo vi;
        vi.command = engine::CommandEnum::EDIT_MODE;
        parser::WordManager::addEditModeVerb("editmode", vi);
        vi.command = engine::CommandEnum::TAKE;
        parser::WordManager::addGlobalVerb("pick up", vi);

        in = new engine::Item();
        areaLex.addVerb("foo", in);
        areaLex.addNoun("bar", in);
        areaLex.addNoun("baz", in);
    }

    static void TearDownTestCase() {
        // Clear WordManager data
        parser::WordManager::resetAll();
        delete in;
        in = nullptr;
    }

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};


// Verify default constructor
TEST_F(PartOfSpeechTest, DefaultConstructorTest) {
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.getAlias().empty());
    EXPECT_TRUE(pos.getOriginalAlias().empty());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(0, pos.getRange().end);
    EXPECT_FALSE(pos.isValid());
}

// Ability to find global verb at beginning of full token sequence
// No context argument
TEST_F(PartOfSpeechTest, FindGlobalVerbNoContext) {
    tokens = parser::Tokenizer::tokenizeInput("editmode");
    range = parser::Range(0, tokens.size());
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, parser::WordManager::hasEditModeVerb));
    EXPECT_STREQ("editmode", pos.getAlias().c_str());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(1, pos.getRange().end);
}

// Ability to find global verb at beginning of full token sequence
// With null context argument
TEST_F(PartOfSpeechTest, FindGlobalVerb) {
    tokens = parser::Tokenizer::tokenizeInput("editmode");
    range = parser::Range(0, tokens.size());
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, parser::WordManager::hasEditModeVerb, 0));
    EXPECT_STREQ("editmode", pos.getAlias().c_str());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(1, pos.getRange().end);
}

// Ability to find global verb with multiple words at beginning of full token sequence
// No context argument
TEST_F(PartOfSpeechTest, FindGlobalVerbMultiWord) {
    tokens = parser::Tokenizer::tokenizeInput("pick up bar");
    range = parser::Range(0, tokens.size());
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, parser::WordManager::hasGlobalVerb));
    EXPECT_STREQ("pick up", pos.getAlias().c_str());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(2, pos.getRange().end);
}

// Ability to find local verb at beginning of full token sequence
TEST_F(PartOfSpeechTest, FindLocalVerb) {
    tokens = parser::Tokenizer::tokenizeInput("foo bar");
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, &parser::LexicalData::forwardHasVerb, &areaLex));
    EXPECT_STREQ("foo", pos.getAlias().c_str());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(1, pos.getRange().end);
}

// Ability to find local noun in the middle of full token sequence
TEST_F(PartOfSpeechTest, FindLocalNoun) {
    tokens = parser::Tokenizer::tokenizeInput("foo bar baz");
    range = parser::Range(1, tokens.size());
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, &parser::LexicalData::forwardHasNoun, &areaLex));
    EXPECT_STREQ("bar", pos.getAlias().c_str());
    EXPECT_EQ(1, pos.getRange().start);
    EXPECT_EQ(2, pos.getRange().end);
}

// Ability to find each word sequentially
TEST_F(PartOfSpeechTest, FindWordsInSequence) {
    tokens = parser::Tokenizer::tokenizeInput("foo bar baz");
    range = parser::Range(0, tokens.size());
    parser::PartOfSpeech verb;
    EXPECT_TRUE(verb.findMatch(tokens, range, &parser::LexicalData::forwardHasVerb, &areaLex));
    EXPECT_STREQ("foo", verb.getAlias().c_str());
    EXPECT_EQ(0, verb.getRange().start);
    EXPECT_EQ(1, verb.getRange().end);
    range = parser::Range(range.end, tokens.size());
    parser::PartOfSpeech direct;
    EXPECT_TRUE(direct.findMatch(tokens, range, &parser::LexicalData::forwardHasNoun, &areaLex));
    EXPECT_STREQ("bar", direct.getAlias().c_str());
    EXPECT_EQ(1, direct.getRange().start);
    EXPECT_EQ(2, direct.getRange().end);
    range = parser::Range(range.end, tokens.size());
    parser::PartOfSpeech indirect;
    EXPECT_TRUE(indirect.findMatch(tokens, range, &parser::LexicalData::forwardHasNoun, &areaLex));
    EXPECT_STREQ("baz", indirect.getAlias().c_str());
    EXPECT_EQ(2, indirect.getRange().start);
    EXPECT_EQ(3, indirect.getRange().end);
}

// Ability to find local verb regardless of case
TEST_F(PartOfSpeechTest, FindLocalVerbDifferentCase) {
    tokens = parser::Tokenizer::tokenizeInput("FoO bar");
    range = parser::Range(0, tokens.size());
    parser::PartOfSpeech pos;
    EXPECT_TRUE(pos.findMatch(tokens, range, &parser::LexicalData::forwardHasVerb, &areaLex));
    EXPECT_STREQ("foo", pos.getAlias().c_str());
    EXPECT_STREQ("FoO", pos.getOriginalAlias().c_str());
    EXPECT_EQ(0, pos.getRange().start);
    EXPECT_EQ(1, pos.getRange().end);
}

}

