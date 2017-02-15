/*!
  \file     parser_TextParser_Test.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/11/2017
  \course   CS467, Winter 2017
 
  \details This file contains the unit tests for the TextParser class.
*/

#include <TextParser.hpp>
#include <LexicalData.hpp>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Helper function prototypes
void setEditModeVerbs();
void setGlobalVerbs();
void setBuilderVerbs();

// Reuse this variable for a new parser object every test
parser::TextParser tp;

// Store pointers to all InteractiveNouns instances here.
std::vector<engine::InteractiveNoun *> ins;

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

    }

    static void TearDownTestCase() {
    }

    virtual void SetUp() {
        // // Set up edit mode verb lookup table
        // setEditModeVerbs();

        // Set up global verb lookup table
        setGlobalVerbs();

        // Set up world builder lookup table
        setBuilderVerbs();

        // Reset TextParser
        tp = parser::TextParser();
    }

    virtual void TearDown() {
        // Clear the candidate list
        results.clear();

        // Clear the player VerbMap
        playerLex.clear();

        // Clear the area VerbMap
        areaLex.clear();

        // Clear WordManager data
        parser::WordManager::resetAll();
    }
};

/******************************************
 * Test Cases
 *****************************************/
// Test the happy path of the HELP command
TEST_F(TextParserTest, HelpHappyPath) {
    input = "help";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::HELP);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the HELP command with invalid input after
TEST_F(TextParserTest, HelpInvalidSuffix) {
    input = "help me";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::HELP);
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    // Unparsed text should contain "me"
    EXPECT_STREQ("me", results.begin()->unparsed.c_str());
}

// Test the happy path of the standalone LOOK command
TEST_F(TextParserTest, LookStandaloneHappyPath) {
    std::string input = "look";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::LOOK);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the LOOK command with object
TEST_F(TextParserTest, LookHappyPath) {
    // std::string input = "look candle";
    std::vector<std::string> inputs = {
        "look candle",
        "look at candle",
        "look at the candle"
    }; 
    
    // TODO: Add candle to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::LOOK);
        ASSERT_EQ(1, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the LISTEN command
TEST_F(TextParserTest, ListenHappyPath) {
    std::string input = "listen";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::LISTEN);
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the EDIT_MODE command
TEST_F(TextParserTest, EditModeValidityCheck) {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);

    input = "editmode";
    auto it = parser::WordManager::getEditModeVerbs(input).begin();
    ASSERT_TRUE(it != parser::WordManager::getEditModeVerbs(input).end());
    EXPECT_EQ(engine::CommandEnum::EDIT_MODE, it->command);
    EXPECT_STREQ("editmode", it->description.c_str());
    EXPECT_EQ(parser::Grammar::NO, it->grammar.takesDirectObject());
    EXPECT_EQ(parser::Grammar::NO, it->grammar.takesIndirectObject());
    EXPECT_FALSE(it->grammar.takesPreposition());
}

// Test the happy path of the TAKE command
TEST_F(TextParserTest, TakeHappyPath) {
    std::vector<std::string> inputs = {
        "take candle",
        "pick up candle",
        "take the candle",
        "pick up the candle"
    };
    
    // TODO: Add candle to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::TAKE);
        ASSERT_EQ(1, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
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
    
    // TODO: Add torch to areaVM and table to areaNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::PUT);
        ASSERT_EQ(1, results.begin()->direct.size());
        EXPECT_EQ(engine::ItemPosition::ON, results.begin()->position);
        ASSERT_EQ(1, results.begin()->indirect.size());
        EXPECT_TRUE(*results.begin()->indirect.begin() == nullptr);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the DROP command
TEST_F(TextParserTest, DropHappyPath) {
    std::vector<std::string> inputs = {
        "drop candle",
        "drop the candle"
    };

    // TODO: Add candle to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::DROP);
        ASSERT_EQ(1, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
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
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVENTORY);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the MORE command
TEST_F(TextParserTest, MoreItemHappyPath) {
    std::vector<std::string> inputs = {
        "more torch",
        "more heal"
    };

    // TODO: Add torch and heal to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::MORE);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EQUIPMENT command
TEST_F(TextParserTest, EquipmentHappyPath) {
    std::vector<std::string> inputs = {
        "equipment",
        "eq"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::EQUIPMENT);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
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

    // TODO: Add helmet to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::EQUIP);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
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

    // TODO: Add helmet to playerVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::UNEQUIP);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TRANSFER command
TEST_F(TextParserTest, TransferHappyPath) {
    std::vector<std::string> inputs = {
        "give torch to Joe",
        "give the torch to Joe"
    };

    // TODO: Add torch to playerVM and Joe to areaNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::UNEQUIP);
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the SPEAK command
TEST_F(TextParserTest, SpeakHappyPath) {
    input = "say Hi everyone!";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SPEAK);
    EXPECT_STREQ("Hi everyone!", results.begin()->directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}


// Test the happy path of the SHOUT command
TEST_F(TextParserTest, ShoutHappyPath) {
    std::vector<std::string> inputs = {
        "shout HELLO THERE",
        "yell HELLO THERE"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::SHOUT);
        EXPECT_STREQ("HELLO THERE", results.begin()->directAlias.c_str());
        // Should not be any objects or position
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        results.clear();
    }
}

// Test the happy path of the WHISPER command
TEST_F(TextParserTest, WhisperHappyPath) {
    input = "whisper Don't tell anyone to Joe";
    
    // TODO: Add Joe to areaNM
    
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::WHISPER);
    ASSERT_EQ(1, results.begin()->indirect.size());
    EXPECT_TRUE(*results.begin()->indirect.begin() == nullptr);
    EXPECT_STREQ("Don't tell anyone", results.begin()->directAlias.c_str());
    // Should not be any direct object or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test the happy path of the QUIT command
TEST_F(TextParserTest, QuitHappyPath) {
    std::vector<std::string> inputs = {
        "quit",
        "exit",
        "logout"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::QUIT);
        // Should not be any objects, position or unparsed text
        EXPECT_EQ(0, results.begin()->direct.size());
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
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

    // TODO: Add exit to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::GO);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object, position, or unparsed text
        EXPECT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the implied GO command with exit alias only
TEST_F(TextParserTest, GoImpliedHappyPath) {
    std::vector<std::string> inputs = {
        "north",
        "n"
    };

    // TODO: Add exit to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::GO);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object, position, or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the STATS command
TEST_F(TextParserTest, StatsHappyPath) {
    input = "stats";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::STATS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the QUESTS command
TEST_F(TextParserTest, QuestsHappyPath) {
    input = "quests";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::QUESTS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SKILLS command
TEST_F(TextParserTest, SkillsHappyPath) {
    input = "skills";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SKILLS);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the ATTACK command
TEST_F(TextParserTest, AttackHappyPath) {
    std::vector<std::string> inputs = {
        "attack troll",
        "attack the troll"
    };

    // TODO: Add troll to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::ATTACK);
        EXPECT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the ATTACK command with a skill
TEST_F(TextParserTest, AttackWithSkillHappyPath) {
    std::vector<std::string> inputs = {
        "attack troll with fireball",
        "attack the troll with fireball"
    };

    // TODO: Add troll to areaVM and fireball to playerNM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::ATTACK);
        ASSERT_EQ(1, results.begin()->direct.size());
        ASSERT_EQ(1, results.begin()->indirect.size());
        // Should not be any unparsed text
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TALK command
TEST_F(TextParserTest, TalkHappyPath) {
    input = "talk to Sarah";

    // TODO: Add Sarah to areaVM

    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::TALK);
    ASSERT_EQ(1, results.begin()->indirect.size());
    // Should not be any direct objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SHOP command
TEST_F(TextParserTest, ShopHappyPath) {
    input = "shop";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SHOP);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the BUY command
TEST_F(TextParserTest, BuyHappyPath) {
    input = "buy torch";

    // TODO: Add torch to areaVM

    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::BUY);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SELL command
TEST_F(TextParserTest, SellHappyPath) {
    input = "sell torch";

    // TODO: Add torch to playerVM

    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SELL);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SEARCH command
TEST_F(TextParserTest, SearchHappyPath) {
    std::vector<std::string> inputs = {
        "search chest",
        "open chest",
        "search the chest",
        "open the chest"
    };

    // TODO: Add chest to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::SEARCH);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the USE_SKILL command
TEST_F(TextParserTest, UseSkillHappyPath) {
    input = "use heal";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::USE_SKILL);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the USE_SKILL command on a target
TEST_F(TextParserTest, UseSkillOnTargetHappyPath) {
    input = "use heal on Joe";

    // TODO: Add heal to playerVM and Joe to areaNM

    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::USE_SKILL);
    ASSERT_EQ(1, results.begin()->direct.size());
    ASSERT_EQ(1, results.begin()->indirect.size());
    EXPECT_TRUE(*results.begin()->indirect.begin() == nullptr);
    // Should not be any position or unparsed text
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the MOVE command
TEST_F(TextParserTest, MoveHappyPath) {
    std::vector<std::string> inputs = {
        "move chair",
        "move the chair"
    };

    // TODO: Add chair to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::MOVE);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the READ command
TEST_F(TextParserTest, ReadHappyPath) {
    std::vector<std::string> inputs = {
        "read book",
        "read the book"
    };

    // TODO: Add book to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::READ);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the BREAK command
TEST_F(TextParserTest, BreakHappyPath) {
    std::vector<std::string> inputs = {
        "break book",
        "break the book"
    };

    // TODO: Add book to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::BREAK);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the CLIMB command
TEST_F(TextParserTest, ClimbHappyPath) {
    std::vector<std::string> inputs = {
        "climb ladder",
        "climb the ladder"
    };

    // TODO: Add ladder to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::CLIMB);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the TURN command
TEST_F(TextParserTest, TurnHappyPath) {
    std::vector<std::string> inputs = {
        "turn knob",
        "turn the knob"
    };

    // TODO: Add knob to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::TURN);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the PUSH command
TEST_F(TextParserTest, PushHappyPath) {
    std::vector<std::string> inputs = {
        "push table",
        "push the table"
    };

    // TODO: Add table to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::PUSH);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the PULL command
TEST_F(TextParserTest, PullHappyPath) {
    std::vector<std::string> inputs = {
        "pull table",
        "pull the table"
    };

    // TODO: Add table to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::PULL);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EAT command
TEST_F(TextParserTest, EatHappyPath) {
    std::vector<std::string> inputs = {
        "eat bread",
        "eat the bread"
    };

    // TODO: Add bread to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::EAT);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the DRINK command
TEST_F(TextParserTest, DrinkHappyPath) {
    std::vector<std::string> inputs = {
        "drink potion",
        "drink the potion"
    };

    // TODO: Add potion to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::DRINK);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test the happy path of the EDIT_MODE command
TEST_F(TextParserTest, EditModeHappyPath) {
    parser::VerbInfo vi;
    vi.grammar = parser::Grammar(parser::Grammar::NO, false, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_MODE;
    vi.description = "editmode";
    parser::WordManager::addEditModeVerb("editmode", vi);

    input = "editmode";
    results = tp.parse(input, playerLex, areaLex, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::EDIT_MODE);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_STREQ("", results.begin()->unparsed.c_str());
}

// Test the EDIT_MODE command without permissions
TEST_F(TextParserTest, EditModeNotAdmin) {
    input = "editmode";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVALID);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_STREQ("editmode", results.begin()->unparsed.c_str());
}

// Test the happy path of the WARP command
TEST_F(TextParserTest, WarpHappyPath) {
    input = "warp 1";

    // TODO: Add area with ID of 1 to WordManager::addNoun

    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::WARP);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the COPY command
TEST_F(TextParserTest, CopyHappyPath) {
    input = "copy 1";

    // TODO: Add item with ID of 1 to WordManager::addNoun

    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::COPY);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the CREATE command
TEST_F(TextParserTest, CreateHappyPath) {
    std::vector<std::string> inputs = {
        "create item",
        "new item"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex, true, true);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::CREATE);
        EXPECT_STREQ("item", results.begin()->directAlias.c_str());
        // Should not be any objects or position
        EXPECT_EQ(0, results.begin()->direct.size());
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        results.clear();
    }
}

// Test the happy path of the ADD command
TEST_F(TextParserTest, AddHappyPath) {
    input = "add exit";

    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::CREATE);
    EXPECT_STREQ("exit", results.begin()->directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    ASSERT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test the happy path of the EDIT_ATTRIBUTE command
TEST_F(TextParserTest, EditAttributeHappyPath) {
    input = "edit long description";
    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_DIRECT, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::EDIT_ATTRIBUTE);
    EXPECT_STREQ("long description", results.begin()->directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test the happy path of the EDIT_WIZARD command
TEST_F(TextParserTest, EditWizardHappyPath) {
    input = "edit torch";

    // TODO: Add torch to areaVM

    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::EDIT_WIZARD);
    ASSERT_EQ(1, results.begin()->direct.size());
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SAVE command
TEST_F(TextParserTest, SaveHappyPath) {
    std::string input = "save";
    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SAVE);
    // Should not be any objects, position or unparsed text
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
    EXPECT_TRUE(results.begin()->unparsed.empty());
}

// Test the happy path of the SAVE command with filename
TEST_F(TextParserTest, SaveFilenameHappyPath) {
    std::string input = "save filename.dat";
    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::SAVE);
    EXPECT_STREQ("filename.dat", results.begin()->directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test the happy path of the LOAD command
TEST_F(TextParserTest, LoadHappyPath) {
    std::string input = "load filename.dat";
    results = tp.parse(input, playerLex, areaLex, true, true);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("filename.dat", results.begin()->directAlias.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test the happy path of the DELETE command
TEST_F(TextParserTest, DeleteHappyPath) {
    std::vector<std::string> inputs = {
        "delete potion",
        "delete the potion",
        "delete 1"
    };

    // TODO: Add potion with ID of 1 to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        results = tp.parse(*it, playerLex, areaLex, true, true);
        ASSERT_EQ(1, results.size());
        EXPECT_EQ(parser::ParseStatus::VALID, results[0].status);
        EXPECT_EQ(results.begin()->command, engine::CommandEnum::DELETE);
        ASSERT_EQ(1, results.begin()->direct.size());
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, results.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
        EXPECT_TRUE(results.begin()->unparsed.empty());
        results.clear();
    }
}

// Test an unknown verb only
TEST_F(TextParserTest, InvalidVerbOnly) {
    std::string input = "foo";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo", results.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test an unknown verb with unknown direct object
TEST_F(TextParserTest, InvalidVerbInvalidDirectObject) {
    std::string input = "foo bar";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo bar", results.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}

// Test an unknown verb with known direct object
TEST_F(TextParserTest, InvalidVerbValidDirectObject) {
    std::string input = "foo torch";
    results = tp.parse(input, playerLex, areaLex);
    ASSERT_EQ(1, results.size());
    EXPECT_EQ(parser::ParseStatus::INVALID_VERB, results[0].status);
    EXPECT_EQ(results.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo torch", results.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_EQ(0, results.begin()->direct.size());
    EXPECT_EQ(0, results.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, results.begin()->position);
}


/******************************************
 * Helper Functions
 *****************************************/

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
    vi.grammar = parser::Grammar(parser::Grammar::YES, false, parser::Grammar::NO);
    vi.description = "look";
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
    vi.grammar = parser::Grammar(parser::Grammar::NO, true, parser::Grammar::YES);
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    vi.description = "warp";
    parser::WordManager::addBuilderVerb("warp", vi);

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

