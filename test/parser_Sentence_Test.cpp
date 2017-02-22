/*!
  \file     parser_Sentence_Test.cpp
  \author   David Rigert
  \created  02/17/2017
  \modified 02/17/2017
  \course   CS467, Winter 2017
 
  \details This file contains the unit tests for the Sentence class.
*/

#include <VerbType.hpp>

#include <Sentence.hpp>
#include <VDISentence.hpp>
#include <VDPISentence.hpp>
#include <VDPTSentence.hpp>
#include <VDSentence.hpp>
#include <VDTSentence.hpp>
#include <VPISentence.hpp>
#include <VPTSentence.hpp>
#include <VSentence.hpp>
#include <VTISentence.hpp>
#include <VTPISentence.hpp>
#include <VTPTSentence.hpp>
#include <VTSentence.hpp>
#include <Grammar.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Reuse this variable for a new PartOfSpeech object every test
parser::PartOfSpeech verb;

// Points to the Sentence object returned by makeSentence
parser::Sentence *sentence;

/******************************************
 * Test Cases
 *****************************************/
// Test that we get a nullptr back
TEST(SentenceTest, VerbPrepositionNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::NO, true, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a nullptr back
TEST(SentenceTest, VerbIndirectNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::NO, false, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a nullptr back
TEST(SentenceTest, VerbDirectTextPrepositionNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, true, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a nullptr back
TEST(SentenceTest, VerbDirectPrepositionNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::YES, true, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a nullptr back
TEST(SentenceTest, VerbDirectTextIndirectTextNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, false, parser::Grammar::TEXT);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a nullptr back
TEST(SentenceTest, VerbIndirectTextNotSupportedTest) {
    parser::Grammar grammar(parser::Grammar::NO, false, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    EXPECT_EQ(nullptr, sentence);
    if (sentence != nullptr) delete sentence;
}

// Test that we get a VDISentence object back
TEST(SentenceTest, VerbDirectIndirectTest) {
    parser::Grammar grammar(parser::Grammar::YES, false, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VDISentence*>(sentence));
    delete sentence;
}

// Test that we get a VDPISentence object back
TEST(SentenceTest, VerbDirectPrepositionIndirectTest) {
    parser::Grammar grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VDPISentence*>(sentence));
    delete sentence;
}

// Test that we get a VDPTSentence object back
TEST(SentenceTest, VerbDirectPrepositionTextTest) {
    parser::Grammar grammar(parser::Grammar::YES, true, parser::Grammar::TEXT);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VDPTSentence*>(sentence));
    delete sentence;
}

// Test that we get a VDSentence object back
TEST(SentenceTest, VerbDirectTest) {
    parser::Grammar grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VDSentence*>(sentence));
    delete sentence;
}

// Test that we get a VDTSentence object back
TEST(SentenceTest, VerbDirectTextTest) {
    parser::Grammar grammar(parser::Grammar::YES, false, parser::Grammar::TEXT);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VDTSentence*>(sentence));
    delete sentence;
}

// Test that we get a VPISentence object back
TEST(SentenceTest, VerbPrepositionIndirectTest) {
    parser::Grammar grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VPISentence*>(sentence));
    delete sentence;
}

// Test that we get a VPTSentence object back
TEST(SentenceTest, VerbPrepositionTextTest) {
    parser::Grammar grammar(parser::Grammar::NO, true, parser::Grammar::TEXT);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VPTSentence*>(sentence));
    delete sentence;
}

// Test that we get a VSentence object back
TEST(SentenceTest, VerbTest) {
    parser::Grammar grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VSentence*>(sentence));
    delete sentence;
}

// Test that we get a VTISentence object back
TEST(SentenceTest, VerbTextIndirectTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, false, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VTISentence*>(sentence));
    delete sentence;
}

// Test that we get a VTPISentence object back
TEST(SentenceTest, VerbTextPrepositionIndirectTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VTPISentence*>(sentence));
    delete sentence;
}

// Test that we get a VTPTSentence object back
TEST(SentenceTest, VerbTextPrepositionTextTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, true, parser::Grammar::TEXT);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VTPTSentence*>(sentence));
    delete sentence;
}

// Test that we get a VTSentence object back
TEST(SentenceTest, VerbTextTest) {
    parser::Grammar grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    sentence = parser::Sentence::makeSentence(grammar, verb, VerbType::INVALID, engine::CommandEnum::INVALID);
    ASSERT_NE(nullptr, sentence);
    EXPECT_NE(nullptr, dynamic_cast<parser::VTSentence*>(sentence));
    delete sentence;
}

}
