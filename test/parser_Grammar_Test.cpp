/*!
  \file     parser_Grammar_Test.cpp
  \author   David Rigert
  \created  02/02/2017
  \modified 02/07/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the Grammar class.
*/

#include <Grammar.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;

// Verify default constructor
TEST(GrammarTest, DefaultConstructorTest) {
    parser::Grammar g = parser::Grammar();
    EXPECT_EQ(parser::Grammar::NO, g.takesDirectObject());
    EXPECT_FALSE(g.takesPreposition());
    EXPECT_EQ(parser::Grammar::NO, g.takesIndirectObject());
}

// Verify constructor that sets direct/indirect object support
TEST(GrammarTest, ObjectSupportConstructorTest) {
    parser::Grammar g = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::TEXT);
    EXPECT_EQ(parser::Grammar::YES, g.takesDirectObject());
    EXPECT_TRUE(g.takesPreposition());
    EXPECT_EQ(parser::Grammar::TEXT, g.takesIndirectObject());
}

// Verify adding a preposition and getting it back
TEST(GrammarTest, AddAndGetPrepositionTest) {
    parser::Grammar g = parser::Grammar();
    g.addPreposition("none", parser::PrepositionType::NONE);
    g.addPreposition("to", parser::PrepositionType::TO);
    g.addPreposition("with", parser::PrepositionType::WITH);
    g.addPreposition("of", parser::PrepositionType::OF);
    g.addPreposition("from", parser::PrepositionType::FROM);
    g.addPreposition("on", parser::PrepositionType::ON);
    g.addPreposition("under", parser::PrepositionType::UNDER);
    g.addPreposition("in", parser::PrepositionType::IN);
    EXPECT_EQ(parser::PrepositionType::NONE, g.getPrepositionType("none"));
    EXPECT_EQ(parser::PrepositionType::TO, g.getPrepositionType("to"));
    EXPECT_EQ(parser::PrepositionType::WITH, g.getPrepositionType("with"));
    EXPECT_EQ(parser::PrepositionType::OF, g.getPrepositionType("of"));
    EXPECT_EQ(parser::PrepositionType::FROM, g.getPrepositionType("from"));
    EXPECT_EQ(parser::PrepositionType::ON, g.getPrepositionType("on"));
    EXPECT_EQ(parser::PrepositionType::UNDER, g.getPrepositionType("under"));
    EXPECT_EQ(parser::PrepositionType::IN, g.getPrepositionType("in"));
}

// Verify that getting an unknown preposition returns NONE
TEST(GrammarTest, GetUnknownPrepositionTest) {
    parser::Grammar g = parser::Grammar();
    EXPECT_EQ(parser::PrepositionType::NONE, g.getPrepositionType("foo"));
}

// Verify that checking for an unknown preposition returns false,
// and checking for a known preposition returns true.
TEST(GrammarTest, HasPrepositionTest) {
    parser::Grammar g = parser::Grammar();
    EXPECT_FALSE(g.hasPreposition("foo"));
    g.addPreposition("foo", parser::PrepositionType::NONE);
    EXPECT_TRUE(g.hasPreposition("foo"));
}

// Verify changing object support with setter function
TEST(GrammarTest, SetObjectSupportTest) {
    parser::Grammar g = parser::Grammar();
    g.setSupport(parser::Grammar::YES, true, parser::Grammar::TEXT);
    EXPECT_EQ(parser::Grammar::YES, g.takesDirectObject());
    EXPECT_TRUE(g.takesPreposition());
    EXPECT_EQ(parser::Grammar::TEXT, g.takesIndirectObject());
    // Change it to something else
    g.setSupport(parser::Grammar::TEXT, false, parser::Grammar::NO);
    EXPECT_EQ(parser::Grammar::TEXT, g.takesDirectObject());
    EXPECT_FALSE(g.takesPreposition());
    EXPECT_EQ(parser::Grammar::NO, g.takesIndirectObject());
}


}

