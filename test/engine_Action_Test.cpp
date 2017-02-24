/*!
  \file     engine_Action_Test.cpp
  \author   David Rigert
  \created  02/22/2017
  \modified 02/22/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the Action class.
*/

#include <Item.hpp>
#include <Action.hpp>
#include <Grammar.hpp>

#include <gtest/gtest.h>

namespace {

namespace engine = legacymud::engine;
namespace parser = legacymud::parser;

// Default constructor test
TEST(ActionTest, DefaultConstructor) {
    engine::Action act;
    EXPECT_EQ(engine::CommandEnum::INVALID, act.getCommand());
    EXPECT_FALSE(act.getValid());
    EXPECT_TRUE(act.getFlavorText().empty());
    EXPECT_EQ(engine::EffectType::NONE, act.getEffect());
    EXPECT_EQ(0, act.getAliases().size());
}

// Command constructor test
TEST(ActionTest, CommandConstructor) {
    engine::Action act(engine::CommandEnum::TALK);
    EXPECT_EQ(engine::CommandEnum::TALK, act.getCommand());
    EXPECT_FALSE(act.getValid());
    EXPECT_TRUE(act.getFlavorText().empty());
    EXPECT_EQ(engine::EffectType::NONE, act.getEffect());
    EXPECT_EQ(0, act.getAliases().size());
}

// Full constructor test
TEST(ActionTest, FullConstructor) {
    engine::Action act(engine::CommandEnum::TALK, true, "foo", engine::EffectType::DAMAGE);
    EXPECT_EQ(engine::CommandEnum::TALK, act.getCommand());
    EXPECT_TRUE(act.getValid());
    EXPECT_STREQ("foo", act.getFlavorText().c_str());
    EXPECT_EQ(engine::EffectType::DAMAGE, act.getEffect());
    EXPECT_EQ(0, act.getAliases().size());
}

// Copy constructor test
TEST(ActionTest, CopyConstructor) {
    engine::Action act(engine::CommandEnum::TALK, true, "foo", engine::EffectType::DAMAGE);
    act.addAlias("bar", new parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES));
    EXPECT_EQ(engine::CommandEnum::TALK, act.getCommand());
    EXPECT_TRUE(act.getValid());
    EXPECT_STREQ("foo", act.getFlavorText().c_str());
    EXPECT_EQ(engine::EffectType::DAMAGE, act.getEffect());
    ASSERT_EQ(1, act.getAliases().size());
    EXPECT_STREQ("bar", act.getAliases()[0].c_str());
    
    engine::Action act2(act);
    EXPECT_EQ(engine::CommandEnum::TALK, act2.getCommand());
    EXPECT_TRUE(act2.getValid());
    EXPECT_STREQ("foo", act2.getFlavorText().c_str());
    EXPECT_EQ(engine::EffectType::DAMAGE, act2.getEffect());
    ASSERT_EQ(1, act2.getAliases().size());
    EXPECT_STREQ("bar", act2.getAliases()[0].c_str());
    // Grammar should not be same instance but should be configured the same
    EXPECT_NE(act.getGrammar("bar"), act2.getGrammar("bar"));
    ASSERT_NE(nullptr, act.getGrammar("bar"));
    ASSERT_NE(nullptr, act2.getGrammar("bar"));
    EXPECT_EQ(act.getGrammar("bar")->takesDirectObject(), act2.getGrammar("bar")->takesDirectObject());
    EXPECT_EQ(act.getGrammar("bar")->takesIndirectObject(), act2.getGrammar("bar")->takesIndirectObject());
    EXPECT_EQ(act.getGrammar("bar")->takesPreposition(), act2.getGrammar("bar")->takesPreposition());
}

// Assignment operator test
TEST(ActionTest, AssignmentOperator) {
    engine::Action act(engine::CommandEnum::TALK, true, "foo", engine::EffectType::DAMAGE);
    act.addAlias("bar", new parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES));
    EXPECT_EQ(engine::CommandEnum::TALK, act.getCommand());
    EXPECT_TRUE(act.getValid());
    EXPECT_STREQ("foo", act.getFlavorText().c_str());
    EXPECT_EQ(engine::EffectType::DAMAGE, act.getEffect());
    ASSERT_EQ(1, act.getAliases().size());
    EXPECT_STREQ("bar", act.getAliases()[0].c_str());
    
    engine::Action act2 = act;
    EXPECT_EQ(engine::CommandEnum::TALK, act2.getCommand());
    EXPECT_TRUE(act2.getValid());
    EXPECT_STREQ("foo", act2.getFlavorText().c_str());
    EXPECT_EQ(engine::EffectType::DAMAGE, act2.getEffect());
    ASSERT_EQ(1, act2.getAliases().size());
    EXPECT_STREQ("bar", act2.getAliases()[0].c_str());
    // Grammar should not be same instance but should be configured the same
    EXPECT_NE(act.getGrammar("bar"), act2.getGrammar("bar"));
    ASSERT_NE(nullptr, act.getGrammar("bar"));
    ASSERT_NE(nullptr, act2.getGrammar("bar"));
    EXPECT_EQ(act.getGrammar("bar")->takesDirectObject(), act2.getGrammar("bar")->takesDirectObject());
    EXPECT_EQ(act.getGrammar("bar")->takesIndirectObject(), act2.getGrammar("bar")->takesIndirectObject());
    EXPECT_EQ(act.getGrammar("bar")->takesPreposition(), act2.getGrammar("bar")->takesPreposition());
}

// Get and set flavor text
TEST(ActionTest, SetGetFlavorText) {
    engine::Action act;
    EXPECT_TRUE(act.getFlavorText().empty());
    EXPECT_TRUE(act.setFlavorText("foobar"));
    EXPECT_STREQ("foobar", act.getFlavorText().c_str());
}

// Get and set the valid flag
TEST(ActionTest, SetGetValidity) {
    engine::Action act;
    EXPECT_FALSE(act.getValid());
    EXPECT_TRUE(act.setValid(true));
    EXPECT_TRUE(act.getValid());
}

// Get and set the effect type
TEST(ActionTest, SetGetEffectType) {
    engine::Action act;
    EXPECT_EQ(engine::EffectType::NONE, act.getEffect());
    EXPECT_TRUE(act.setEffect(engine::EffectType::DAMAGE));
    EXPECT_EQ(engine::EffectType::DAMAGE, act.getEffect());
}

// Get, set and remove the grammar and verb alias
TEST(ActionTest, SetGetRemoveGrammarAndAlias) {
    engine::Action act;
    parser::Grammar *gram = new parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    act.addAlias("bar", gram);
    EXPECT_TRUE(act.isAlias("bar"));
    ASSERT_EQ(1, act.getAliases().size());
    EXPECT_STREQ("bar", act.getAliases()[0].c_str());
    ASSERT_EQ(gram, act.getGrammar("bar"));
    auto gramMap = act.getAliasesAndGrammar();
    ASSERT_TRUE(gramMap.find("bar") != gramMap.end());
    ASSERT_EQ(gram, gramMap["bar"]);

    ASSERT_TRUE(act.removeAlias("bar"));
    EXPECT_FALSE(act.isAlias("bar"));
    EXPECT_EQ(0, act.getAliases().size());
    gramMap = act.getAliasesAndGrammar();
    ASSERT_FALSE(gramMap.find("bar") != gramMap.end());
}

}

