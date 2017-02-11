/*!
  \file     parser_TextParser_Test.cpp
  \author   David Rigert
  \created  01/29/2017
  \modified 02/10/2017
  \course   CS467, Winter 2017
 
  \details This file contains the unit tests for the TextParser class.
*/

#include <TextParser.hpp>
#include <WordManager.hpp>

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

// Store the player verb map here
parser::WordMap playerVM;
// Store the player noun map here
parser::WordMap playerNM;

// Store the area verb map here
parser::WordMap areaVM;
// Store the area noun map here
parser::WordMap areaNM;

// Reuse this list for candidates
std::vector<parser::TextParseResult> candidates;

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

    }

    static void TearDownTestCase() {
        // Clear WordManager data
        parser::WordManager::resetAll();
    }

    virtual void SetUp() {
        // Reset TextParser
        tp = parser::TextParser();
    }

    virtual void TearDown() {
        // Clear the candidate list
        candidates.clear();

        // Clear the player VerbMap
        playerVM.clear();

        // Clear the area VerbMap
        areaVM.clear();
    }
};

/******************************************
 * Test Cases
 *****************************************/

// Test the happy path of the HELP command
TEST_F(TextParserTest, HelpHappyPath) {
    input = "help";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::HELP);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the HELP command with invalid input after
TEST_F(TextParserTest, HelpInvalidSuffix) {
    input = "help me";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_NOUN, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::HELP);
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    // Unparsed text should contain "me"
    EXPECT_STREQ("me", candidates.begin()->unparsed.c_str());
}

// Test the happy path of the standalone LOOK command
TEST_F(TextParserTest, LookStandaloneHappyPath) {
    std::string input = "look";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::LOOK);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the LOOK command with object
TEST_F(TextParserTest, LookHappyPath) {
    std::vector<std::string> inputs = {
        "look candle",
        "look at candle",
        "look at the candle"
    };
    
    // TODO: Add candle to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::LOOK);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the LISTEN command
TEST_F(TextParserTest, ListenHappyPath) {
    std::string input = "listen";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::LISTEN);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::TAKE);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::PUT);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        EXPECT_EQ(engine::ItemPosition::ON, candidates.begin()->position);
        ASSERT_EQ(1, candidates.begin()->indirect.size());
        EXPECT_TRUE(*candidates.begin()->indirect.begin() == nullptr);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::DROP);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the INVENTORY command
TEST_F(TextParserTest, InventoryHappyPath) {
    std::vector<std::string> inputs = {
        "inventory",
        "inv"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVENTORY);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::MORE);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the EQUIPMENT command
TEST_F(TextParserTest, EquipmentHappyPath) {
    std::vector<std::string> inputs = {
        "equipment",
        "eq"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EQUIPMENT);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EQUIP);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::UNEQUIP);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::UNEQUIP);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the SPEAK command
TEST_F(TextParserTest, SpeakHappyPath) {
    input = "say Hi everyone!";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SPEAK);
    EXPECT_STREQ("Hi everyone!", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}


// Test the happy path of the SHOUT command
TEST_F(TextParserTest, ShoutHappyPath) {
    std::vector<std::string> inputs = {
        "shout HELLO THERE",
        "yell HELLO THERE"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SHOUT);
        EXPECT_STREQ("HELLO THERE", candidates.begin()->unparsed.c_str());
        // Should not be any objects or position
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        candidates.clear();
    }
}

// Test the happy path of the WHISPER command
TEST_F(TextParserTest, WhisperHappyPath) {
    input = "whisper Don't tell anyone to Joe";
    
    // TODO: Add Joe to areaNM
    
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::WHISPER);
    ASSERT_EQ(1, candidates.begin()->indirect.size());
    EXPECT_TRUE(*candidates.begin()->indirect.begin() == nullptr);
    EXPECT_STREQ("Don't tell anyone", candidates.begin()->unparsed.c_str());
    // Should not be any direct object or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test the happy path of the QUIT command
TEST_F(TextParserTest, QuitHappyPath) {
    std::vector<std::string> inputs = {
        "quit",
        "exit",
        "logout"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::QUIT);
        // Should not be any objects, position or unparsed text
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::GO);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object, position, or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::GO);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object, position, or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the STATS command
TEST_F(TextParserTest, StatsHappyPath) {
    input = "stats";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::STATS);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the QUESTS command
TEST_F(TextParserTest, QuestsHappyPath) {
    input = "quests";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::QUESTS);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the SKILLS command
TEST_F(TextParserTest, SkillsHappyPath) {
    input = "skills";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SKILLS);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the ATTACK command
TEST_F(TextParserTest, AttackHappyPath) {
    std::vector<std::string> inputs = {
        "attack troll",
        "attack the troll"
    };

    // TODO: Add troll to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::ATTACK);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::ATTACK);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the TALK command
TEST_F(TextParserTest, TalkHappyPath) {
    input = "talk to Sarah";

    // TODO: Add Sarah to areaVM

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::TALK);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the SHOP command
TEST_F(TextParserTest, ShopHappyPath) {
    input = "shop";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SHOP);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the BUY command
TEST_F(TextParserTest, BuyHappyPath) {
    input = "buy torch";

    // TODO: Add torch to areaVM

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::BUY);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the SELL command
TEST_F(TextParserTest, SellHappyPath) {
    input = "sell torch";

    // TODO: Add torch to playerVM

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SELL);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SEARCH);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the USE_SKILL command
TEST_F(TextParserTest, UseSkillHappyPath) {
    input = "use heal";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::USE_SKILL);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the USE_SKILL command on a target
TEST_F(TextParserTest, UseSkillOnTargetHappyPath) {
    input = "use heal on Joe";

    // TODO: Add heal to playerVM and Joe to areaNM

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::USE_SKILL);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    ASSERT_EQ(1, candidates.begin()->indirect.size());
    EXPECT_TRUE(*candidates.begin()->indirect.begin() == nullptr);
    // Should not be any position or unparsed text
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the MOVE command
TEST_F(TextParserTest, MoveHappyPath) {
    std::vector<std::string> inputs = {
        "move chair",
        "move the chair"
    };

    // TODO: Add chair to areaVM

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::MOVE);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::READ);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::BREAK);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::CLIMB);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::TURN);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::PUSH);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::PULL);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EAT);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::DRINK);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test the happy path of the EDIT_MODE command
TEST_F(TextParserTest, EditModeHappyPath) {
    input = "editmode";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates, true);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EDIT_MODE);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_STREQ("", candidates.begin()->unparsed.c_str());
}

// Test the EDIT_MODE command without permissions
TEST_F(TextParserTest, EditModeNotAdmin) {
    input = "editmode";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_VERB, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVALID);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_STREQ("editmode", candidates.begin()->unparsed.c_str());
}

// Test the happy path of the WARP command
TEST_F(TextParserTest, WarpHappyPath) {
    input = "warp 1";

    // TODO: Add area with ID of 1 to WordManager::addNoun

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::WARP);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the COPY command
TEST_F(TextParserTest, CopyHappyPath) {
    input = "copy 1";

    // TODO: Add item with ID of 1 to WordManager::addNoun

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::COPY);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the CREATE command
TEST_F(TextParserTest, CreateHappyPath) {
    std::vector<std::string> inputs = {
        "create item",
        "new item"
    };

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::CREATE);
        EXPECT_STREQ("item", candidates.begin()->unparsed.c_str());
        // Should not be any objects or position
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        candidates.clear();
    }
}

// Test the happy path of the ADD command
TEST_F(TextParserTest, AddHappyPath) {
    input = "add exit";

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::CREATE);
    EXPECT_STREQ("exit", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    ASSERT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test the happy path of the EDIT_ATTRIBUTE command
TEST_F(TextParserTest, EditAttributeHappyPath) {
    input = "edit long description";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_NOUN, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EDIT_ATTRIBUTE);
    EXPECT_STREQ("long description", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test the happy path of the EDIT_WIZARD command
TEST_F(TextParserTest, EditWizardHappyPath) {
    input = "edit torch";

    // TODO: Add torch to areaVM

    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::EDIT_WIZARD);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    // Should not be any indirect objects, position or unparsed text
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the SAVE command
TEST_F(TextParserTest, SaveHappyPath) {
    std::string input = "save";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SAVE);
    // Should not be any objects, position or unparsed text
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
    EXPECT_TRUE(candidates.begin()->unparsed.empty());
}

// Test the happy path of the SAVE command with filename
TEST_F(TextParserTest, SaveFilenameHappyPath) {
    std::string input = "save filename.dat";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::SAVE);
    EXPECT_STREQ("filename.dat", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test the happy path of the LOAD command
TEST_F(TextParserTest, LoadHappyPath) {
    std::string input = "load filename.dat";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("filename.dat", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
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
        auto result = tp.parse(*it, playerVM, playerNM, areaVM, areaNM, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::DELETE);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        // Should not be any indirect object or unparsed text
        ASSERT_EQ(0, candidates.begin()->indirect.size());
        EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
        EXPECT_TRUE(candidates.begin()->unparsed.empty());
        candidates.clear();
    }
}

// Test an unknown verb only
TEST_F(TextParserTest, InvalidVerbOnly) {
    std::string input = "foo";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_VERB, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test an unknown verb with unknown direct object
TEST_F(TextParserTest, InvalidVerbInvalidDirectObject) {
    std::string input = "foo bar";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_VERB, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo bar", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}

// Test an unknown verb with known direct object
TEST_F(TextParserTest, InvalidVerbValidDirectObject) {
    std::string input = "foo torch";
    auto result = tp.parse(input, playerVM, playerNM, areaVM, areaNM, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_VERB, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->command, engine::CommandEnum::INVALID);
    EXPECT_STREQ("foo torch", candidates.begin()->unparsed.c_str());
    // Should not be any objects or position
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_EQ(engine::ItemPosition::NONE, candidates.begin()->position);
}


/******************************************
 * Helper Functions
 *****************************************/

// Set up the edit mode verbs
void setEditModeVerbs() {
    parser::VerbInfo vi;
    vi.command = engine::CommandEnum::EDIT_MODE;
    parser::WordManager::addEditModeVerb("editmode", vi);
}

void setGlobalVerbs() {
    parser::VerbInfo vi;

    // HELP command
    vi.command = engine::CommandEnum::HELP;
    parser::WordManager::addGlobalVerb("help", vi);

    // LOOK command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::LOOK;
    parser::WordManager::addGlobalVerb("look", vi);
    parser::WordManager::addGlobalVerb("look at", vi);

    // LISTEN command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::LISTEN;
    parser::WordManager::addGlobalVerb("listen", vi);

    // TAKE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::OPTIONAL);
    vi.command = engine::CommandEnum::TAKE;
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    // "pick up" alias
    parser::WordManager::addGlobalVerb("pick up", vi);
    // "take" additionally supports "from"
    vi.grammar.addPreposition("from", parser::PrepositionType::FROM);
    parser::WordManager::addGlobalVerb("take", vi);

    // PUT command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::REQUIRED);
    vi.command = engine::CommandEnum::PUT;
    vi.grammar.addPreposition("on", parser::PrepositionType::ON);
    vi.grammar.addPreposition("under", parser::PrepositionType::UNDER);
    vi.grammar.addPreposition("in", parser::PrepositionType::IN);
    parser::WordManager::addGlobalVerb("put", vi);

    // DROP command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::DROP;
    parser::WordManager::addGlobalVerb("drop", vi);

    // INVENTORY command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::INVENTORY;
    parser::WordManager::addGlobalVerb("inventory", vi);
    parser::WordManager::addGlobalVerb("inv", vi);

    // MORE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::MORE;
    parser::WordManager::addGlobalVerb("more", vi);

    // EQUIPMENT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::EQUIPMENT;
    parser::WordManager::addGlobalVerb("eq", vi);
    parser::WordManager::addGlobalVerb("equipment", vi);

    // EQUIP command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EQUIP;
    parser::WordManager::addGlobalVerb("wear", vi);
    parser::WordManager::addGlobalVerb("put on", vi);
    parser::WordManager::addGlobalVerb("equip", vi);

    // UNEQUIP command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::UNEQUIP;
    parser::WordManager::addGlobalVerb("remove", vi);
    parser::WordManager::addGlobalVerb("take off", vi);
    parser::WordManager::addGlobalVerb("unequip", vi);

    // TRANSFER command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::REQUIRED);
    vi.command = engine::CommandEnum::TRANSFER;
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    parser::WordManager::addGlobalVerb("give", vi);

    // SPEAK command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, parser::Grammar::NO);
    vi.command = engine::CommandEnum::SPEAK;
    parser::WordManager::addGlobalVerb("say", vi);

    // SHOUT command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, parser::Grammar::NO);
    vi.command = engine::CommandEnum::SHOUT;
    parser::WordManager::addGlobalVerb("shout", vi);
    parser::WordManager::addGlobalVerb("yell", vi);

    // WHISPER command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, parser::Grammar::REQUIRED);
    vi.command = engine::CommandEnum::WHISPER;
    vi.grammar.addPreposition("to", parser::PrepositionType::TO);
    parser::WordManager::addGlobalVerb("whisper", vi);

    // QUIT command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUIT;
    parser::WordManager::addGlobalVerb("quit", vi);
    parser::WordManager::addGlobalVerb("exit", vi);
    parser::WordManager::addGlobalVerb("logout", vi);

    // GO command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::GO;
    parser::WordManager::addGlobalVerb("go", vi);
    parser::WordManager::addGlobalVerb("go to", vi);

    // STATS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::STATS;
    parser::WordManager::addGlobalVerb("stats", vi);

    // QUESTS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::QUESTS;
    parser::WordManager::addGlobalVerb("quests", vi);

    // SKILLS command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SKILLS;
    parser::WordManager::addGlobalVerb("skills", vi);

    // ATTACK command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::OPTIONAL);
    vi.command = engine::CommandEnum::ATTACK;
    vi.grammar.addPreposition("with", parser::PrepositionType::WITH);
    vi.grammar.addPreposition("using", parser::PrepositionType::WITH);
    parser::WordManager::addGlobalVerb("attack", vi);

    // TALK command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::NO, parser::Grammar::REQUIRED);
    vi.command = engine::CommandEnum::TALK;
    parser::WordManager::addGlobalVerb("talk to", vi);

    // SHOP command
    vi = parser::VerbInfo();
    vi.command = engine::CommandEnum::SHOP;
    parser::WordManager::addGlobalVerb("shop", vi);

    // BUY command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::BUY;
    parser::WordManager::addGlobalVerb("buy", vi);

    // SELL command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::SELL;
    parser::WordManager::addGlobalVerb("sell", vi);

    // SEARCH command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::SEARCH;
    parser::WordManager::addGlobalVerb("search", vi);
    parser::WordManager::addGlobalVerb("open", vi);

    // USE_SKILL command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::OPTIONAL);
    vi.command = engine::CommandEnum::USE_SKILL;
    vi.grammar.addPreposition("on", parser::PrepositionType::TO);
    parser::WordManager::addGlobalVerb("use", vi);

}

void setBuilderVerbs() {
    parser::VerbInfo vi;

    // WARP command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::WARP;
    parser::WordManager::addBuilderVerb("warp to", vi);

    // COPY command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::COPY;
    parser::WordManager::addBuilderVerb("copy", vi);

    // CREATE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, parser::Grammar::NO);
    vi.command = engine::CommandEnum::CREATE;
    parser::WordManager::addBuilderVerb("create", vi);
    parser::WordManager::addBuilderVerb("new", vi);
    parser::WordManager::addBuilderVerb("add", vi);

    // EDIT_ATTRIBUTE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::TEXT);
    vi.command = engine::CommandEnum::EDIT_ATTRIBUTE;
    parser::WordManager::addBuilderVerb("edit", vi);

    // EDIT_WIZARD command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::EDIT_WIZARD;
    parser::WordManager::addBuilderVerb("edit", vi);

    // SAVE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::OPTIONALTEXT, parser::Grammar::NO);
    vi.command = engine::CommandEnum::SAVE;
    parser::WordManager::addBuilderVerb("save", vi);

    // LOAD command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::TEXT, parser::Grammar::NO);
    vi.command = engine::CommandEnum::LOAD;
    parser::WordManager::addBuilderVerb("load", vi);

    // DELETE command
    vi = parser::VerbInfo();
    vi.grammar = parser::Grammar(parser::Grammar::REQUIRED, parser::Grammar::NO);
    vi.command = engine::CommandEnum::DELETE;
    parser::WordManager::addBuilderVerb("delete", vi);
    
}

}

