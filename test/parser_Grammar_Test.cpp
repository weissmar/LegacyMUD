/*!
  \file    parser_Grammar_Test.cpp
  \author  David Rigert
  \date    02/05/2017
  \course  CS467, Winter 2017
 
  \details This file contains the unit tests for the Grammar class.
*/

#include <Grammar.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;

// Verify default constructor
TEST(GrammarTest, DefaultConstructorTest) {
    parser::Grammar g = parser::Grammar();
    EXPECT_EQ(parser::Grammar::NO, g.takesDirectObject());
    EXPECT_EQ(parser::Grammar::NO, g.takesIndirectObject());
}

// Verify constructor that sets direct/indirect object support
TEST(GrammarTest, ObjectSupportConstructorTest) {
    parser::Grammar g = parser::Grammar(parser::Grammar::OPTIONAL, parser::Grammar::REQUIRED);
    EXPECT_EQ(parser::Grammar::OPTIONAL, g.takesDirectObject());
    EXPECT_EQ(parser::Grammar::REQUIRED, g.takesIndirectObject());
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

// Verify changing object support with setter function
TEST(GrammarTest, SetObjectSupportTest) {
    parser::Grammar g = parser::Grammar();
    g.setObjectSupport(parser::Grammar::OPTIONAL, parser::Grammar::REQUIRED);
    EXPECT_EQ(parser::Grammar::OPTIONAL, g.takesDirectObject());
    EXPECT_EQ(parser::Grammar::REQUIRED, g.takesIndirectObject());
    // Change it to something else
    g.setObjectSupport(parser::Grammar::NO, parser::Grammar::OPTIONAL);
    EXPECT_EQ(parser::Grammar::NO, g.takesDirectObject());
    EXPECT_EQ(parser::Grammar::OPTIONAL, g.takesIndirectObject());
}


}

