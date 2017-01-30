/*!
  \file    parser_TextParser_Test.cpp
  \author  David Rigert
  \date    01/29/2017
  \course  CS467, Winter 2017
 
  \details This file contains the unit tests for the TextParser class.
*/

#include <parser.h>

#include <gtest/gtest.h>

namespace {

namespace parser = legacymud::parser;
namespace engine = legacymud::engine;

// Reuse this pointer for a new parser object every test
parser::TextParser *tp;

// Store InteractiveNoun objects here
std::list<engine::InteractiveNoun> inList;

// Reuse this VerbMap for each test
parser::VerbMap vm;

// Reuse this list for candidates
std::list<parser::TextParseResult> candidates;

// Test fixture to set up and tear down each unit test
class TextParserTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
        // Set up global verb list
        parser::WordManager::addGlobalVerb("go", engine::ActionType::NONE);
        parser::WordManager::addGlobalVerb("help", engine::ActionType::NONE);

        // Set up preposition list
        parser::WordManager::addPreposition("to", engine::PositionType::NONE);
    }

    static void TearDownTestCase() {
        // Clear WordManager data
        parser::WordManager::resetAll();
    }

    virtual void SetUp() {
        // Instantiate TextParser
        tp = new parser::TextParser();
    }

    virtual void TearDown() {
        // Destroy TextParser and free memory
        delete tp;
        tp = nullptr;

        // Clear the candidate list
        candidates.clear();

        // Clear the VerbMap
        vm.clear();

        // Clear the InteractiveNoun list
        inList.clear();
    }
};


// Test the happy path of the HELP command
TEST_F(TextParserTest, HelpHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_TRUE(candidates.begin()->message.empty());
}

// Test the HELP command with invalid input after
TEST_F(TextParserTest, HelpInvalidSuffix) {
    std::string input = "help me";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::INVALID_NOUN, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_STREQ(candidates.begin()->message.c_str(), "me");
}

// Test the happy path of the standalone LOOK command
TEST_F(TextParserTest, LookStandaloneHappyPath) {
    std::string input = "look";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
    EXPECT_TRUE(candidates.begin()->direct == nullptr);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_TRUE(candidates.begin()->message.empty());
}

// Test the happy path of the transitive LOOK command
TEST_F(TextParserTest, LookTransitiveHappyPath) {
    std::string input = "look candle";
    engine::InteractiveNoun in;
    in.name = "candle";
    inList.push_back(in);

    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
    EXPECT_EQ(candidates.begin()->direct, &in);
    EXPECT_EQ(0, candidates.begin()->indirect.size());
    EXPECT_TRUE(candidates.begin()->message.empty());
}

// Test the happy path of the intransitive LOOK command
TEST_F(TextParserTest, LookIntransitiveHappyPath) {
    std::list<std::string> inputs = {
        "look at candle",
        "look at the candle"
    };
    engine::InteractiveNoun in;
    in.name = "candle";
    inList.push_back(in);

    for (auto it = inputs.begin(); it != inputs.end(); ++it) {
        auto result = tp->parse(*it, vm, candidates);
        EXPECT_EQ(parser::TextParseStatus::VALID, result);
        ASSERT_EQ(1, candidates.size());
        EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
        EXPECT_TRUE(candidates.begin()->direct == nullptr);
        ASSERT_EQ(1, candidates.begin()->indirect.size());
        EXPECT_TRUE(*candidates.begin()->indirect.begin() == &in);
        EXPECT_TRUE(candidates.begin()->message.empty());
    }
}

// Test the happy path of the LISTEN command
TEST_F(TextParserTest, ListenHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the TAKE command
TEST_F(TextParserTest, TakeHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the DROP command
TEST_F(TextParserTest, DropHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the INVENTORY command
TEST_F(TextParserTest, InventoryHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the MORE command
TEST_F(TextParserTest, MoreHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EQUIPMENT command
TEST_F(TextParserTest, EquipmentHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EQUIP command
TEST_F(TextParserTest, EquipHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the UNEQUIP command
TEST_F(TextParserTest, UnequipHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the TRANSFER command
TEST_F(TextParserTest, TransferHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SPEAK command
TEST_F(TextParserTest, SpeakHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SHOUT command
TEST_F(TextParserTest, ShoutHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the WHISPER command
TEST_F(TextParserTest, WhisperHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the QUIT command
TEST_F(TextParserTest, QuitHappyPath) {
    std::string input = "quit";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the GO command
TEST_F(TextParserTest, GoHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the STATS command
TEST_F(TextParserTest, StatsHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the QUESTS command
TEST_F(TextParserTest, QuestsHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SKILLS command
TEST_F(TextParserTest, SkillsHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the ATTACK command
TEST_F(TextParserTest, AttackHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the ATTACK_WITH command
TEST_F(TextParserTest, AttackWithHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the TALK command
TEST_F(TextParserTest, TalkHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SHOP command
TEST_F(TextParserTest, ShopHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the BUY command
TEST_F(TextParserTest, BuyHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SELL command
TEST_F(TextParserTest, SellHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SEARCH command
TEST_F(TextParserTest, SearchHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the USE_SKILL command
TEST_F(TextParserTest, UseSkillHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the MOVE command
TEST_F(TextParserTest, MoveHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the READ command
TEST_F(TextParserTest, ReadHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the BREAK command
TEST_F(TextParserTest, BreakHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the CLIMB command
TEST_F(TextParserTest, ClimbHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the TURN command
TEST_F(TextParserTest, TurnHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the PUSH command
TEST_F(TextParserTest, PushHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the PULL command
TEST_F(TextParserTest, PullHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EAT command
TEST_F(TextParserTest, EatHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the DRINK command
TEST_F(TextParserTest, DrinkHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EDIT_MODE command
TEST_F(TextParserTest, EditModeHappyPath) {
    std::string input = "editmode";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the WARP command
TEST_F(TextParserTest, WarpHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the COPY command
TEST_F(TextParserTest, CopyHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the CREATE command
TEST_F(TextParserTest, CreateHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the ADD command
TEST_F(TextParserTest, AddHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EDIT_ATTRIBUTE command
TEST_F(TextParserTest, EditAttributeHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the EDIT_WIZARD command
TEST_F(TextParserTest, EditWizardHappyPath) {
    std::string input = "help";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the SAVE command
TEST_F(TextParserTest, SaveHappyPath) {
    std::string input = "save";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the LOAD command
TEST_F(TextParserTest, LoadHappyPath) {
    std::string input = "load";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

// Test the happy path of the DELETE command
TEST_F(TextParserTest, DeleteHappyPath) {
    std::string input = "delete";
    auto result = tp->parse(input, vm, candidates);
    EXPECT_EQ(parser::TextParseStatus::VALID, result);
    ASSERT_EQ(1, candidates.size());
    EXPECT_EQ(candidates.begin()->action, engine::ActionType::NONE);
}

}

