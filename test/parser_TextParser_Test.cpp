/*!
  \file     parser_TextParser_Test.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/21/2017
  \course   CS467, Winter 2017
 
  \details This file contains the unit tests for the TextParser class.
*/

#include <TextParser.hpp>
#include <LexicalData.hpp>
#include <Container.hpp>
#include <SpecialSkill.hpp>
#include <Item.hpp>
#include <Player.hpp>
#include <Exit.hpp>
#include <Creature.hpp>
#include <NonCombatant.hpp>
#include <Feature.hpp>
#include <Area.hpp>
#include <Action.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Helper function prototypes
void setEditModeVerbs();
void setGlobalVerbs();
void setBuilderVerbs();
void setSentenceTestVerbs();

// Store the input text here
std::string input;

// Store the player lexical data here
parser::LexicalData playerLex;

// Store the area lexical data here
parser::LexicalData areaLex;

// Reuse this list for results
std::vector<parser::ParseResult> results;

// Test fixture to set up and tear down each unit test
class TextParserTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
        // Set up edit mode verb lookup table
        setEditModeVerbs();

        // Set up global verb lookup table
        setGlobalVerbs();

        // Set up world builder lookup table
        setBuilderVerbs();

        // Set up test verbs for all sentence types
        setSentenceTestVerbs();

        // Add ignore words
        parser::WordManager::addIgnoreWord("the");
        parser::WordManager::addIgnoreWord("a");
        parser::WordManager::addIgnoreWord("an");
    }

    static void TearDownTestCase() {
        // Clear WordManager data
        parser::WordManager::resetAll();

    }

    virtual void TearDown() {
        // Clear the candidate list
        results.clear();

        // Clear the player VerbMap
        playerLex.clear();

        // Clear the area VerbMap
        areaLex.clear();
    }
};

/******************************************
 * Test Cases
 *****************************************/
// Test the happy path of the HELP command
TEST_F(TextParserTest, HelpHappyPath) {
    input = "help";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::HELP);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the HELP command with invalid input after
TEST_F(TextParserTest, HelpInvalidSuffix) {
    input = "help me";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::HELP);
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    // Unparsed text should contain "me"
    EXPECT_STREQ("me", results[0].unparsed.c_str());
}

// Test the happy path of the standalone LOOK command
TEST_F(TextParserTest, LookStandaloneHappyPath) {
    std::string input = "look";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::LOOK);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the LOOK command with object
TEST_F(TextParserTest, LookHappyPath) {
    // std::string input = "look candle";
    std::vector<std::string> inputs = {
        "look candle",
        "look at candle",
        "look at the candle"
    }; 
    
    engine::Item candle;
    areaLex.addNoun("candle", &candle);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status) << *it;
        EXPECT_EQ(results[0].command, engine::CommandEnum::LOOK) << *it;
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&candle, results[0].direct[0]) << *it;
        EXPECT_EQ(0, results[0].indirect.size()) << *it;
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position) << *it;
        EXPECT_TRUE(results[0].unparsed.empty()) << *it;
        results.clear();
    }
}

// Test the happy path of the LISTEN command
TEST_F(TextParserTest, ListenHappyPath) {
    std::string input = "listen";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::LISTEN);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the TAKE command
TEST_F(TextParserTest, TakeHappyPath) {
    std::vector<std::string> inputs = {
        "take candle",
        "pick up candle",
        "take the candle",
        "pick up the candle"
    };
    
    engine::Item candle;
    areaLex.addNoun("candle", &candle);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::TAKE);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&candle, results[0].direct[0]);
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the PUT command
TEST_F(TextParserTest, PutHappyPath) {
    std::vector<std::string> inputs = {
        "put torch on table",
        "put torch on the table",
        "put the torch on table",
        "put the torch on the table"
    };
    
    engine::Item torch;
    playerLex.addNoun("torch", &torch);
    engine::Container table;
    areaLex.addNoun("table", &table);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::PUT);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&torch, results[0].direct[0]);
        EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
        ASSERT_EQ(1, results[0].indirect.size()) << *it;
        EXPECT_EQ(&table, results[0].indirect[0]);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the DROP command
TEST_F(TextParserTest, DropHappyPath) {
    std::vector<std::string> inputs = {
        "drop candle",
        "drop the candle"
    };

    engine::Item candle;
    playerLex.addNoun("candle", &candle);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::DROP);
        ASSERT_EQ(1, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the INVENTORY command
TEST_F(TextParserTest, InventoryHappyPath) {
    std::vector<std::string> inputs = {
        "inventory",
        "inv"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::INVENTORY);
        EXPECT_EQ(0, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the MORE command
TEST_F(TextParserTest, MoreItemHappyPath) {
    engine::Item torch;
    areaLex.addNoun("torch", &torch);
    engine::SpecialSkill heal;
    playerLex.addNoun("heal", &heal);

    // Item
    input = "more torch";

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::MORE);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&torch, results[0].direct[0]);
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
    results.clear();

    // Skill
    input = "more heal";

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::MORE);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&heal, results[0].direct[0]);
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
    results.clear();
}

// Test the happy path of the EQUIPMENT command
TEST_F(TextParserTest, EquipmentHappyPath) {
    std::vector<std::string> inputs = {
        "equipment",
        "eq"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::EQUIPMENT);
        EXPECT_EQ(0, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EQUIP command
TEST_F(TextParserTest, EquipHappyPath) {
    std::vector<std::string> inputs = {
        "wear helmet",
        "put on helmet",
        "equip helmet",
        "wear the helmet",
        "put on the helmet",
        "equip the helmet"
    };

    engine::Item helmet;
    playerLex.addNoun("helmet", &helmet);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::EQUIP);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&helmet, results[0].direct[0]);
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the UNEQUIP command
TEST_F(TextParserTest, UnequipHappyPath) {
    std::vector<std::string> inputs = {
        "remove helmet",
        "take off helmet",
        "unequip helmet",
        "remove the helmet",
        "take off the helmet",
        "unequip the helmet"
    };

    engine::Item helmet;
    playerLex.addNoun("helmet", &helmet);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::UNEQUIP);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&helmet, results[0].direct[0]);
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TRANSFER command
TEST_F(TextParserTest, TransferHappyPath) {
    std::vector<std::string> inputs = {
        "give torch to Joe",
        "give the torch to Joe"
    };

    engine::Item torch;
    playerLex.addNoun("torch", &torch);
    engine::Player joe;
    areaLex.addNoun("Joe", &joe);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::TRANSFER);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&torch, results[0].direct[0]);
        ASSERT_EQ(1, results[0].indirect.size()) << *it;
        EXPECT_EQ(&joe, results[0].indirect[0]);
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the SPEAK command
TEST_F(TextParserTest, SpeakHappyPath) {
    input = "say Hi everyone!";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SPEAK);
    EXPECT_STREQ("Hi everyone!", results[0].directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}


// Test the happy path of the SHOUT command
TEST_F(TextParserTest, ShoutHappyPath) {
    std::vector<std::string> inputs = {
        "shout HELLO THERE",
        "yell HELLO THERE"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::SHOUT);
        EXPECT_STREQ("HELLO THERE", results[0].directAlias.c_str());
        // Should not be any objects or position
        EXPECT_EQ(0, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        results.clear();
    }
}

// Test the happy path of the WHISPER command
TEST_F(TextParserTest, WhisperHappyPath) {
    input = "whisper Don't tell anyone to Joe";
    
    engine::Player joe;
    areaLex.addNoun("Joe", &joe);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::WHISPER);
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_EQ(&joe, results[0].indirect[0]);
    EXPECT_STREQ("Don't tell anyone", results[0].directAlias.c_str());
    // Should not be any direct object or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test the happy path of the QUIT command
TEST_F(TextParserTest, QuitHappyPath) {
    std::vector<std::string> inputs = {
        "quit",
        "exit",
        "logout"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::QUIT);
        // Should not be any objects, position or unparsed text
        EXPECT_EQ(0, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the GO command
TEST_F(TextParserTest, GoHappyPath) {
    std::vector<std::string> inputs = {
        "go north",
        "go to north",
        "go to the north"
    };

    engine::Exit north;
    areaLex.addNoun("north", &north);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::GO);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&north, results[0].direct[0]);
        // Should not be any indirect object, position, or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the implied GO command with exit alias only
TEST_F(TextParserTest, GoImpliedHappyPath) {
    std::vector<std::string> inputs = {
        "north",
        "n"
    };

    engine::Area area;
    engine::Area destination;
    engine::Exit north(engine::ExitDirection::NORTH, engine::EffectType::NONE, &area, &destination, false, nullptr, "an exit to the north", "");
    ASSERT_TRUE(north.addNounAlias("north"));
    ASSERT_TRUE(north.addNounAlias("n"));
    engine::Action *act = north.addAction(engine::CommandEnum::GO);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("north", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    ASSERT_TRUE(act->addAlias("n", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("north", &north);
    areaLex.addVerb("n", &north);
    areaLex.addNoun("north", &north);
    areaLex.addNoun("n", &north);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::GO);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&north, results[0].direct[0]);
        // Should not be any indirect object, position, or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the STATS command
TEST_F(TextParserTest, StatsHappyPath) {
    input = "stats";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::STATS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the QUESTS command
TEST_F(TextParserTest, QuestsHappyPath) {
    input = "quests";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::QUESTS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the SKILLS command
TEST_F(TextParserTest, SkillsHappyPath) {
    input = "skills";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SKILLS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the ATTACK command
TEST_F(TextParserTest, AttackHappyPath) {
    std::vector<std::string> inputs = {
        "attack troll",
        "attack the troll"
    };

    engine::Creature troll;
    areaLex.addNoun("troll", &troll);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::ATTACK);
        ASSERT_EQ(1, results[0].direct.size());
        EXPECT_EQ(&troll, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the ATTACK command with a skill
TEST_F(TextParserTest, AttackWithSkillHappyPath) {
    std::vector<std::string> inputs = {
        "attack troll with fireball",
        "attack the troll with fireball"
    };

    engine::Creature troll;
    areaLex.addNoun("troll", &troll);
    engine::SpecialSkill fb;
    playerLex.addNoun("fireball", &fb);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::ATTACK);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&troll, results[0].direct[0]);
        ASSERT_EQ(1, results[0].indirect.size()) << *it;
        EXPECT_EQ(&fb, results[0].indirect[0]);
        // Should not be any unparsed text
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TALK command
TEST_F(TextParserTest, TalkHappyPath) {
    input = "talk to Sarah";

    engine::NonCombatant sarah;
    areaLex.addNoun("Sarah", &sarah);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::TALK);
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_EQ(&sarah, results[0].indirect[0]);
    // Should not be any direct objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the SHOP command
TEST_F(TextParserTest, ShopHappyPath) {
    input = "shop";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SHOP);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the BUY command
TEST_F(TextParserTest, BuyHappyPath) {
    input = "buy torch";

    engine::Item torch;
    areaLex.addNoun("torch", &torch);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::BUY);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&torch, results[0].direct[0]);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the SELL command
TEST_F(TextParserTest, SellHappyPath) {
    input = "sell torch";

    engine::Item torch;
    playerLex.addNoun("torch", &torch);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SELL);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&torch, results[0].direct[0]);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the SEARCH command
TEST_F(TextParserTest, SearchHappyPath) {
    std::vector<std::string> inputs = {
        "search chest",
        "open chest",
        "search the chest",
        "open the chest"
    };

    engine::Container chest;
    areaLex.addNoun("chest", &chest);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::SEARCH);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&chest, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the USE_SKILL command
TEST_F(TextParserTest, UseSkillHappyPath) {
    input = "use heal";

    engine::SpecialSkill heal;
    playerLex.addNoun("heal", &heal);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::USE_SKILL);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&heal, results[0].direct[0]);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the USE_SKILL command on a target
TEST_F(TextParserTest, UseSkillOnTargetHappyPath) {
    input = "use heal on Joe";

    engine::SpecialSkill heal;
    playerLex.addNoun("heal", &heal);
    engine::Player joe;
    areaLex.addNoun("Joe", &joe);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::USE_SKILL);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&heal, results[0].direct[0]);
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_EQ(&joe, results[0].indirect[0]);
    // Should not be any position or unparsed text
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the MOVE command
TEST_F(TextParserTest, MoveHappyPath) {
    std::vector<std::string> inputs = {
        "move chair",
        "move the chair"
    };

    engine::Item chair;
    engine::Action *act = chair.addAction(engine::CommandEnum::MOVE);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("move", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("move", &chair);
    areaLex.addNoun("chair", &chair);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::MOVE);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&chair, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the READ command
TEST_F(TextParserTest, ReadHappyPath) {
    std::vector<std::string> inputs = {
        "read book",
        "read the book"
    };

    engine::Item book;
    engine::Action *act = book.addAction(engine::CommandEnum::READ);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("read", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("read", &book);
    areaLex.addNoun("book", &book);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::READ);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&book, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the BREAK command
TEST_F(TextParserTest, BreakHappyPath) {
    std::vector<std::string> inputs = {
        "break book",
        "break the book"
    };

    engine::Item book;
    engine::Action *act = book.addAction(engine::CommandEnum::BREAK);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("break", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("break", &book);
    areaLex.addNoun("book", &book);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::BREAK);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&book, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the CLIMB command
TEST_F(TextParserTest, ClimbHappyPath) {
    std::vector<std::string> inputs = {
        "climb ladder",
        "climb the ladder"
    };

    engine::Item ladder;
    engine::Action *act = ladder.addAction(engine::CommandEnum::CLIMB);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("climb", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("climb", &ladder);
    areaLex.addNoun("ladder", &ladder);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::CLIMB);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&ladder, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TURN command
TEST_F(TextParserTest, TurnHappyPath) {
    std::vector<std::string> inputs = {
        "turn knob",
        "turn the knob"
    };

    engine::Feature knob;
    engine::Action *act = knob.addAction(engine::CommandEnum::TURN);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("turn", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("turn", &knob);
    areaLex.addNoun("knob", &knob);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::TURN);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&knob, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the PUSH command
TEST_F(TextParserTest, PushHappyPath) {
    std::vector<std::string> inputs = {
        "push table",
        "push the table"
    };

    engine::Item table;
    engine::Action *act = table.addAction(engine::CommandEnum::PUSH);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("push", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("push", &table);
    areaLex.addNoun("table", &table);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::PUSH);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&table, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the PULL command
TEST_F(TextParserTest, PullHappyPath) {
    std::vector<std::string> inputs = {
        "pull table",
        "pull the table"
    };

    engine::Item table;
    engine::Action *act = table.addAction(engine::CommandEnum::PULL);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("pull", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("pull", &table);
    areaLex.addNoun("table", &table);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::PULL);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&table, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EAT command
TEST_F(TextParserTest, EatHappyPath) {
    std::vector<std::string> inputs = {
        "eat bread",
        "eat the bread"
    };

    engine::Item bread;
    engine::Action *act = bread.addAction(engine::CommandEnum::EAT);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("eat", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("eat", &bread);
    areaLex.addNoun("bread", &bread);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::EAT);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&bread, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the DRINK command
TEST_F(TextParserTest, DrinkHappyPath) {
    std::vector<std::string> inputs = {
        "drink potion",
        "drink the potion"
    };

    engine::Item potion;
    engine::Action *act = potion.addAction(engine::CommandEnum::DRINK);
    ASSERT_NE(nullptr, act);
    ASSERT_TRUE(act->addAlias("drink", new parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO)));
    areaLex.addVerb("drink", &potion);
    playerLex.addNoun("potion", &potion);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::DRINK);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&potion, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EDIT_MODE command
TEST_F(TextParserTest, EditModeHappyPath) {
    input = "editmode";
    results = parser::TextParser::parse(input, playerLex, areaLex, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::EDIT_MODE);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_STREQ("", results[0].unparsed.c_str());
}

// Test the EDIT_MODE command without permissions
TEST_F(TextParserTest, EditModeNotAdmin) {
    input = "editmode";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_STREQ("editmode", results[0].unparsed.c_str());
}

// Test the happy path of the WARP command
TEST_F(TextParserTest, WarpHappyPath) {
    std::vector<std::string> inputs = {
        "warp 1",
        "warp to 1"
    };

    engine::Area area;
    parser::WordManager::addNoun("1", &area);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex, true, true);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::WARP);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&area, results[0].direct[0]);
        EXPECT_STREQ("1", results[0].directAlias.c_str());
        // Should not be any indirect objects, position or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        results.clear();
    }

    parser::WordManager::removeNoun("1", &area);
}

// Test the happy path of the COPY command
TEST_F(TextParserTest, CopyHappyPath) {
    input = "copy 1";

    engine::Item item;
    parser::WordManager::addNoun("1", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::COPY);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_EQ(&item, results[0].direct[0]);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());

    parser::WordManager::removeNoun("1", &item);
}

// Test the happy path of the CREATE command
TEST_F(TextParserTest, CreateHappyPath) {
    std::vector<std::string> inputs = {
        "create item",
        "new item"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex, true, true);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::CREATE);
        EXPECT_STREQ("item", results[0].directAlias.c_str());
        // Should not be any objects or position
        EXPECT_EQ(0, results[0].direct.size());
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        results.clear();
    }
}

// Test the happy path of the ADD command
TEST_F(TextParserTest, AddHappyPath) {
    input = "add exit";

    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::CREATE);
    EXPECT_STREQ("exit", results[0].directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test the happy path of the EDIT_ATTRIBUTE command
TEST_F(TextParserTest, EditAttributeHappyPath) {
    input = "edit long description";
    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::EDIT_ATTRIBUTE);
    EXPECT_STREQ("long description", results[0].directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test the happy path of the EDIT_WIZARD command
TEST_F(TextParserTest, EditWizardHappyPath) {
    input = "edit torch";

    engine::Item torch;
    areaLex.addNoun("torch", &torch);

    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(2, results.size());
    if (results[0].command == engine::CommandEnum::EDIT_WIZARD) {
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::EDIT_WIZARD);
        ASSERT_EQ(1, results[0].direct.size());
        EXPECT_EQ(&torch, results[0].direct[0]);
        // Should not be any indirect objects, position or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
    }
    else {
        EXPECT_EQ(parser::ParseStatus::VALID, results[1].status);
        EXPECT_EQ(results[1].command, engine::CommandEnum::EDIT_WIZARD);
        ASSERT_EQ(1, results[1].direct.size());
        EXPECT_EQ(&torch, results[1].direct[0]);
        // Should not be any indirect objects, position or unparsed text
        EXPECT_EQ(0, results[1].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[1].position);
        EXPECT_TRUE(results[1].unparsed.empty());
    }
}

// Test the happy path of the SAVE command
TEST_F(TextParserTest, SaveHappyPath) {
    std::string input = "save";
    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SAVE);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    EXPECT_TRUE(results[0].unparsed.empty());
}

// Test the happy path of the SAVE command with filename
TEST_F(TextParserTest, SaveFilenameHappyPath) {
    std::string input = "save filename.dat";
    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::SAVE);
    EXPECT_STREQ("filename.dat", results[0].directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test the happy path of the LOAD command
TEST_F(TextParserTest, LoadHappyPath) {
    std::string input = "load filename.dat";
    results = parser::TextParser::parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::LOAD);
    EXPECT_STREQ("filename.dat", results[0].directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test the happy path of the DELETE command
TEST_F(TextParserTest, DeleteHappyPath) {
    std::vector<std::string> inputs = {
        "delete potion",
        "delete the potion",
        "delete 1"
    };

    engine::Item potion;
    areaLex.addNoun("potion", &potion);
    parser::WordManager::addNoun("1", &potion);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = parser::TextParser::parse(*it, playerLex, areaLex, true, true);
        ASSERT_EQ(1, results.size()) << *it;
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results[0].command, engine::CommandEnum::DELETE);
        ASSERT_EQ(1, results[0].direct.size()) << *it;
        EXPECT_EQ(&potion, results[0].direct[0]);
        // Should not be any indirect object or unparsed text
        EXPECT_EQ(0, results[0].indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
        EXPECT_TRUE(results[0].unparsed.empty());
        results.clear();
    }
    parser::WordManager::removeNoun("1", &potion);
}

// Test an unknown verb only
TEST_F(TextParserTest, InvalidVerbOnly) {
    std::string input = "foo";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo", results[0].unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test an unknown verb with unknown direct object
TEST_F(TextParserTest, InvalidVerbInvalidDirectObject) {
    std::string input = "foo bar";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo bar", results[0].unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test an unknown verb with known direct object
TEST_F(TextParserTest, InvalidVerbValidDirectObject) {
    std::string input = "foo torch";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo torch", results[0].unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test an unavailable verb
TEST_F(TextParserTest, UnavailableVerb) {
    parser::LexicalData lex;
    engine::Item item;
    lex.addVerb("foo", &item);
    std::string input = "foo";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_VERB, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo", results[0].unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //lex.clear();
}

// Test an unavailable noun
TEST_F(TextParserTest, UnavailableDirectNoun) {
    parser::LexicalData lex;
    engine::Item item;
    lex.addNoun("foo", &item);
    std::string input = "look foo";
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::LOOK);
    EXPECT_STREQ("foo", results[0].unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //lex.clear();
}

// Test a verb-direct object-indirect object sentence
TEST_F(TextParserTest, VDISentenceTest) {
    input = "vdi item item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item);
    EXPECT_STREQ("item", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    // Should not be any position
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-indirect object sentence
TEST_F(TextParserTest, VDISentenceDirectUnavailableTest) {
    input = "vdi item 1 item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    areaLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, areaLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 1 item 2", results[0].unparsed.c_str());
    // Should not be any position
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-indirect object sentence
TEST_F(TextParserTest, VDISentenceDirectInvalidTest) {
    input = "vdi item 3 item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    areaLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, areaLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 3 item 2", results[0].unparsed.c_str());
    // Should not be any position
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-indirect object sentence
TEST_F(TextParserTest, VDISentenceIndirectUnavailableTest) {
    input = "vdi item 1 item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    areaLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, playerLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(2, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    // Should not be any position
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-indirect object sentence
TEST_F(TextParserTest, VDISentenceIndirectInvalidTest) {
    input = "vdi item 1 item 3";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    areaLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, playerLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(2, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 3", results[0].unparsed.c_str());
    // Should not be any position
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOnTest) {
    input = "vdpi item 1 on item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item1);
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item2);
    EXPECT_STREQ("item 2", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOfTest) {
    input = "vdpi item 1 of item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item2);
    EXPECT_STREQ("item 2", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item1);
    EXPECT_STREQ("item 1", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOnInvalidDirectTest) {
    input = "vdpi item 1 on item 2";
    
    engine::Item item2;
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 1 on item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOfInvalidDirectTest) {
    input = "vdpi item 1 of item 2";
    
    engine::Item item2;
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 1 of item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOnUnavailableDirectTest) {
    input = "vdpi item 1 on item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 2", &item2);
    parser::LexicalData otherLex;
    otherLex.addNoun("item 1", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 1 on item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOfUnavailableDirectTest) {
    input = "vdpi item 1 of item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 2", &item2);
    parser::LexicalData otherLex;
    otherLex.addNoun("item 1", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 1 of item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceInvalidPrepositionTest) {
    input = "vdpi item 1 with item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item1);
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("with item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceMissingPrepositionTest) {
    input = "vdpi item 1 item 2";
    
    engine::Item item1;
    engine::Item item2;
    playerLex.addNoun("item 1", &item1);
    playerLex.addNoun("item 2", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item1);
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOnInvalidIndirectTest) {
    input = "vdpi item 1 on item 2";
    
    engine::Item item1;
    playerLex.addNoun("item 1", &item1);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item1);
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOfInvalidIndirectTest) {
    input = "vdpi item 1 of item 2";
    
    engine::Item item1;
    playerLex.addNoun("item 1", &item1);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item1);
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOnUnavailableIndirectTest) {
    input = "vdpi item 1 on item 2";
    
    engine::Item item2;
    parser::LexicalData otherArea;
    otherArea.addNoun("item 2", &item2);
    areaLex.addNoun("item 1", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect object sentence
TEST_F(TextParserTest, VDPISentenceOfUnavailableIndirectTest) {
    input = "vdpi item 1 of item 2";
    
    engine::Item item2;
    parser::LexicalData otherArea;
    otherArea.addNoun("item 2", &item2);
    areaLex.addNoun("item 1", &item2);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_STREQ("item 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOnTest) {
    input = "vdpt item on text";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOfTest) {
    input = "vdpt item of text";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_STREQ("text", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item);
    EXPECT_STREQ("item", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOnInvalidDirectTest) {
    input = "vdpt item on text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item on text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOfInvalidDirectTest) {
    input = "vdpt item of text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item of text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOnUnavailableDirectTest) {
    input = "vdpt item on text";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item on text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceOfUnavailableDirectTest) {
    input = "vdpt item of text";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item of text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceMissingPrepositionTest) {
    input = "vdpt item text";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceInvalidPrepositionTest) {
    input = "vdpt item with text";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("with text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-preposition-indirect text sentence
TEST_F(TextParserTest, VDPTSentenceMissingIndirectTest) {
    input = "vdpt item on";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object sentence
TEST_F(TextParserTest, VDSentenceTest) {
    input = "vd item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object sentence
TEST_F(TextParserTest, VDSentenceInvalidDirectTest) {
    input = "vd item";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object sentence
TEST_F(TextParserTest, VDSentenceUnavailableDirectTest) {
    input = "vd item";

    parser::LexicalData otherLex;
    engine::Item item;
    otherLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-direct object sentence
TEST_F(TextParserTest, VDSentenceMissingDirectTest) {
    input = "vd";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-text sentence
TEST_F(TextParserTest, VDTSentenceTest) {
    input = "vdt item text";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-text sentence
TEST_F(TextParserTest, VDTSentenceInvalidDirectTest) {
    input = "vdt item text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-direct object-text sentence
TEST_F(TextParserTest, VDTSentenceUnavailableDirectTest) {
    input = "vdt item text";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-direct object-text sentence
TEST_F(TextParserTest, VDTSentenceMissingIndirectTest) {
    input = "vdt item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-indirect object sentence
TEST_F(TextParserTest, VPISentenceTest) {
    input = "vpi on item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item);
    EXPECT_STREQ("item", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-preposition-indirect object sentence
TEST_F(TextParserTest, VPISentenceInvalidPrepositionTest) {
    input = "vpi with item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("with item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-indirect object sentence
TEST_F(TextParserTest, VPISentenceMissingPrepositionTest) {
    input = "vpi item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-indirect object sentence
TEST_F(TextParserTest, VPISentenceInvalidIndirectTest) {
    input = "vpi on item";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-text sentence
TEST_F(TextParserTest, VPTSentenceTest) {
    input = "vpt on text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-preposition-indirect text sentence
TEST_F(TextParserTest, VPTSentenceInvalidPrepositionTest) {
    input = "vpt with text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("with text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-text sentence
TEST_F(TextParserTest, VPTSentenceMissingPrepositionTest) {
    input = "vpt text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-text sentence
TEST_F(TextParserTest, VPTSentenceMissingIndirectTest) {
    input = "vpt on";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-preposition-indirect object sentence
TEST_F(TextParserTest, VSentenceTest) {
    input = "v";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-indirect object sentence
TEST_F(TextParserTest, VTISentenceTest) {
    input = "vti text item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_STREQ("text", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item);
    EXPECT_STREQ("item", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-indirect object sentence
TEST_F(TextParserTest, VTISentenceInvalidIndirectTest) {
    input = "vti text item";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("text item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-indirect object sentence
TEST_F(TextParserTest, VTISentenceUnavailableIndirectTest) {
    input = "vti text item";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("text item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-text-indirect object sentence
TEST_F(TextParserTest, VTISentenceMissingDirectTest) {
    input = "vti item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);

    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOnTest) {
    input = "vtpi text on item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_STREQ("text", results[0].directAlias.c_str());
    ASSERT_EQ(1, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirect[0] == &item);
    EXPECT_STREQ("item", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOfTest) {
    input = "vtpi text of item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    ASSERT_EQ(1, results[0].direct.size());
    EXPECT_TRUE(results[0].direct[0] == &item);
    EXPECT_STREQ("item", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOnInvalidIndirectTest) {
    input = "vtpi text on item";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text on item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOfInvalidIndirectTest) {
    input = "vtpi text of item";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text of item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOnUnavailableIndirectTest) {
    input = "vtpi text on item";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text on item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceOfUnavailableIndirectTest) {
    input = "vtpi text of item";
    
    engine::Item item;
    parser::LexicalData otherLex;
    otherLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::UNAVAILABLE_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text of item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
    //otherLex.clear();
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceInvalidPrepositionTest) {
    input = "vtpi text with item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text with item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceMissingPrepositionTest) {
    input = "vtpi text item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_PREPOSITION, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-indirect object sentence
TEST_F(TextParserTest, VTPISentenceMissingDirectTest) {
    input = "vtpi on item";
    
    engine::Item item;
    playerLex.addNoun("item", &item);
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("on item", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOnTest) {
    input = "vtpt text part 1 on text part 2";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_STREQ("text part 1", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text part 2", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::ON, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOfTest) {
    input = "vtpt text part 1 of text part 2";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    // Direct and indirect objects should be reversed because of "of" preposition
    EXPECT_STREQ("text part 2", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_STREQ("text part 1", results[0].indirectAlias.c_str());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOnMissingIndirectTest) {
    input = "vtpt text part 1 on";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("text part 1 on", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOfMissingIndirectTest) {
    input = "vtpt text part 1 of";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_INDIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("text part 1 of", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOnMissingDirectTest) {
    input = "vtpt on text part 2";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("on text part 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTPTSentenceOfMissingDirectTest) {
    input = "vtpt of text part 2";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_STREQ("of text part 2", results[0].unparsed.c_str());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTSentenceTest) {
    input = "vt text";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_STREQ("text", results[0].directAlias.c_str());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

// Test a verb-text-preposition-text sentence
TEST_F(TextParserTest, VTSentenceMissingDirectTest) {
    input = "vt";
    
    results = parser::TextParser::parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results[0].command, engine::CommandEnum::INVALID);
    EXPECT_EQ(0, results[0].direct.size());
    EXPECT_TRUE(results[0].directAlias.empty());
    EXPECT_EQ(0, results[0].indirect.size());
    EXPECT_TRUE(results[0].indirectAlias.empty());
    EXPECT_TRUE(results[0].unparsed.empty());
    EXPECT_EQ(engine::ItemPosition::NONE, results[0].position);
}

/******************************************
 * Helper Functions
 *****************************************/

void setSentenceTestVerbs() {
    // VDISentence
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::YES);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VDI";
    parser::WordManager::addGlobalVerb("vdi", vi);

    // VDPISentence
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VDPI";
    parser::WordManager::addGlobalVerb("vdpi", vi);

    // VDPTSentence
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::TEXT);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VDPT";
    parser::WordManager::addGlobalVerb("vdpt", vi);

    // VDSentence
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VD";
    parser::WordManager::addGlobalVerb("vd", vi);
    
    // VDTSentence
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::TEXT);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VDT";
    parser::WordManager::addGlobalVerb("vdt", vi);

    // VPISentence
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VPI";
    parser::WordManager::addGlobalVerb("vpi", vi);

    // VPTSentence
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::TEXT);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VPT";
    parser::WordManager::addGlobalVerb("vpt", vi);

    // VSentence
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "V";
    parser::WordManager::addGlobalVerb("v", vi);

    // VTISentence
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::YES);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VTI";
    parser::WordManager::addGlobalVerb("vti", vi);

    // VTPISentence
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VTPI";
    parser::WordManager::addGlobalVerb("vtpi", vi);

    // VTPTSentence
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::TEXT);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VTPT";
    parser::WordManager::addGlobalVerb("vtpt", vi);

    // VTSentence
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::INVALID;
    vi.description = "VT";
    parser::WordManager::addGlobalVerb("vt", vi);
}

// Set up the edit mode verbs
void setEditModeVerbs() {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);
}

void setGlobalVerbs() {
    parser::VerbInfo vi;

    // HELP command
    vi.command = engine::CommandEnum::HELP;
    vi.description = "help";
    parser::WordManager::addGlobalVerb("help", vi);

    // LOOK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::LOOK;
    vi.description = "look";
    parser::WordManager::addGlobalVerb("look", vi);
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    parser::WordManager::addGlobalVerb("look", vi);
    vi.description = "look at";
    parser::WordManager::addGlobalVerb("look at", vi);

    // LISTEN command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::LISTEN;
    vi.description = "listen";
    parser::WordManager::addGlobalVerb("listen", vi);

    // TAKE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TAKE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "pick up";
    parser::WordManager::addGlobalVerb("pick up", vi);
    vi.description = "take";
    parser::WordManager::addGlobalVerb("take", vi);
    
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    // "pick up" alias
    vi.description = "pick up";
    parser::WordManager::addGlobalVerb("pick up", vi);
    // "take" additionally supports "from"
    vi.grammar.addPreposition("from", parser::PrepositionType::FROM);
    vi.description = "take";
    parser::WordManager::addGlobalVerb("take", vi);

    // PUT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::PUT;
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    vi.description = "put";
    parser::WordManager::addGlobalVerb("put", vi);

    // DROP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::DROP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "drop";
    parser::WordManager::addGlobalVerb("drop", vi);

    // INVENTORY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::INVENTORY;
    vi.description = "inventory";
    parser::WordManager::addGlobalVerb("inventory", vi);
    vi.description = "inv";
    parser::WordManager::addGlobalVerb("inv", vi);

    // MORE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::MORE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "more";
    parser::WordManager::addGlobalVerb("more", vi);

    // EQUIPMENT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EQUIPMENT;
    vi.description = "eq";
    parser::WordManager::addGlobalVerb("eq", vi);
    vi.description = "equipment";
    parser::WordManager::addGlobalVerb("equipment", vi);

    // EQUIP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EQUIP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "wear";
    parser::WordManager::addGlobalVerb("wear", vi);
    vi.description = "put on";
    parser::WordManager::addGlobalVerb("put on", vi);
    vi.description = "equip";
    parser::WordManager::addGlobalVerb("equip", vi);

    // UNEQUIP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::UNEQUIP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "remove";
    parser::WordManager::addGlobalVerb("remove", vi);
    vi.description = "take off";
    parser::WordManager::addGlobalVerb("take off", vi);
    vi.description = "unequip";
    parser::WordManager::addGlobalVerb("unequip", vi);

    // TRANSFER command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TRANSFER;
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "give";
    parser::WordManager::addGlobalVerb("give", vi);

    // SPEAK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SPEAK;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "say";
    parser::WordManager::addGlobalVerb("say", vi);

    // SHOUT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SHOUT;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "shout";
    parser::WordManager::addGlobalVerb("shout", vi);
    vi.description = "yell";
    parser::WordManager::addGlobalVerb("yell", vi);

    // WHISPER command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::WHISPER;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "whisper";
    parser::WordManager::addGlobalVerb("whisper", vi);

    // QUIT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUIT;
    vi.description = "quit";
    parser::WordManager::addGlobalVerb("quit", vi);
    vi.description = "exit";
    parser::WordManager::addGlobalVerb("exit", vi);
    vi.description = "logout";
    parser::WordManager::addGlobalVerb("logout", vi);

    // GO command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::GO;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "go";
    parser::WordManager::addGlobalVerb("go", vi);
    vi.description = "go to";
    parser::WordManager::addGlobalVerb("go to", vi);

    // STATS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::STATS;
    vi.description = "stats";
    parser::WordManager::addGlobalVerb("stats", vi);

    // QUESTS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUESTS;
    vi.description = "quests";
    parser::WordManager::addGlobalVerb("quests", vi);

    // SKILLS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SKILLS;
    vi.description = "skills";
    parser::WordManager::addGlobalVerb("skills", vi);

    // ATTACK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::ATTACK;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "attack";
    parser::WordManager::addGlobalVerb("attack", vi);

    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("with", parser::PrepositionType::WITH);
    vi.grammar.addPreposition("using", parser::PrepositionType::WITH);
    vi.description = "attack with";
    parser::WordManager::addGlobalVerb("attack", vi);

    // TALK command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::TALK;
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "talk";
    parser::WordManager::addGlobalVerb("talk", vi);

    // SHOP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SHOP;
    vi.description = "shop";
    parser::WordManager::addGlobalVerb("shop", vi);

    // BUY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::BUY;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "buy";
    parser::WordManager::addGlobalVerb("buy", vi);

    // SELL command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SELL;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "sell";
    parser::WordManager::addGlobalVerb("sell", vi);

    // SEARCH command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SEARCH;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "search";
    parser::WordManager::addGlobalVerb("search", vi);
    vi.description = "open";
    parser::WordManager::addGlobalVerb("open", vi);

    // USE_SKILL command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::USE_SKILL;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "use";
    parser::WordManager::addGlobalVerb("use", vi);
    
    vi.grammar = parser::Grammar(parser::Grammar::YES, true, parser::Grammar::YES);
    vi.grammar.addPreposition("on", parser::PrepositionType::TO);
    vi.description = "use on";
    parser::WordManager::addGlobalVerb("use", vi);

}

void setBuilderVerbs() {
    parser::VerbInfo vi;

    // WARP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::WARP;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "warp";
    parser::WordManager::addBuilderVerb("warp", vi);
    vi.description = "warp to";
    parser::WordManager::addBuilderVerb("warp to", vi);

    // COPY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::COPY;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "copy";
    parser::WordManager::addBuilderVerb("copy", vi);

    // CREATE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::CREATE;
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "create";
    parser::WordManager::addBuilderVerb("create", vi);
    vi.description = "new";
    parser::WordManager::addBuilderVerb("new", vi);
    vi.description = "add";
    parser::WordManager::addBuilderVerb("add", vi);

    // EDIT_ATTRIBUTE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EDIT_ATTRIBUTE;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::TEXT);
    vi.description = "edit (object) (attrib)";
    parser::WordManager::addBuilderVerb("edit", vi);

    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "edit (attrib)";
    parser::WordManager::addBuilderVerb("edit", vi);
    
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, true, parser::Grammar::YES);
    vi.grammar.addPreposition("of", parser::PrepositionType::OF);
    vi.description = "edit (attrib) of (object)";
    parser::WordManager::addBuilderVerb("edit", vi);


    // EDIT_WIZARD command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EDIT_WIZARD;
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "edit (object)";
    parser::WordManager::addBuilderVerb("edit", vi);

    // SAVE command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SAVE;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.description = "save";
    parser::WordManager::addBuilderVerb("save", vi);

    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.description = "save filename";
    parser::WordManager::addBuilderVerb("save", vi);

    // LOAD command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::LOAD;
    vi.description = "load";
    parser::WordManager::addBuilderVerb("load", vi);

    // DELETE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::DELETE;
    vi.description = "delete";
    parser::WordManager::addBuilderVerb("delete", vi);
    
}

}

