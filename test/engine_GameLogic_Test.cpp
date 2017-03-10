/*!
  \file     engine_GameLogic_Test.cpp
  \author   David Rigert
  \created  02/27/2017
  \modified 03/09/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the GameLogic class.
*/

#include "GameLogicShim.hpp"
#include "TestServer.hpp"
#include <Account.hpp>
#include <Server.hpp>
#include <Player.hpp>
#include <PlayerClass.hpp>
#include <Area.hpp>
#include <SpecialSkill.hpp>
#include <GameObjectManager.hpp>
#include <NonCombatant.hpp>

#include <ParseResult.hpp>
#include <VerbType.hpp>

#include <gtest/gtest.h>

#include <fstream>

namespace {

namespace engine = legacymud::engine;
namespace parser = legacymud::parser;
namespace test = legacymud::test;
namespace account = legacymud::account;

engine::GameLogic *logic = nullptr;
test::GameLogicShim *shim = nullptr;
test::TestServer *server = nullptr;
account::Account *acct = nullptr;


// Test fixture to set up and tear down each unit test
class GameLogicTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
    }

    static void TearDownTestCase() {
        // Clean up serialized data
        remove("game.dat");
        remove("game.dat.accounts");
    }

    virtual void SetUp() {
        logic = new engine::GameLogic();
        shim = new test::GameLogicShim(logic);
        server = new test::TestServer();
        acct = new account::Account("game.dat.accounts");
    }

    virtual void TearDown() {
        delete shim;
        delete logic;
        delete server;
        delete acct;
        shim = nullptr;
        logic = nullptr;
        server = nullptr;
        acct = nullptr;
    }

};


// Verify startGame to start a new game
TEST_F(GameLogicTest, StartNewGame) {
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    EXPECT_EQ(acct, shim->getAccountSystem());
    EXPECT_EQ(server, shim->getTelnetServer());
    EXPECT_TRUE(shim->getGameObjectManager() != nullptr);
}

TEST_F(GameLogicTest, LoadAndHibernatePlayerTest) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);

    // Player should be inactive
    EXPECT_FALSE(player->isActive());
    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    ASSERT_TRUE(shim->hibernatePlayer(player));
    // Player should now be inactive
    EXPECT_FALSE(player->isActive());
}

TEST_F(GameLogicTest, GetPlayerMessageFromHandler) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);

    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    // Send a message via handler
    ASSERT_TRUE(logic->receivedMessageHandler("foo message", 0));
    EXPECT_STREQ("foo message", shim->getMessageQueue()->front().first.c_str());
}

TEST_F(GameLogicTest, DieRolls) {
    // Roll 100 times and check upper and lower bounds for various combinations
    int value = 0;
    for (int sides = 1; sides < 20; ++sides) {
        for (int dice = 1; dice < 6; ++dice) {
            for (int i = 0; i < 100; ++i) {
                value = shim->rollDice(sides, dice);
                EXPECT_LE(value, sides * dice) << dice << "d" << sides;
                EXPECT_GE(value, dice) << dice << "d" << sides;
            }
        }
    }
}

TEST_F(GameLogicTest, GetObjectType) {
    EXPECT_EQ(engine::ObjectType::AREA, shim->getObjectType("area"));
    EXPECT_EQ(engine::ObjectType::ARMOR_TYPE, shim->getObjectType("armor type"));
    EXPECT_EQ(engine::ObjectType::CONTAINER, shim->getObjectType("container"));
    EXPECT_EQ(engine::ObjectType::CREATURE, shim->getObjectType("creature"));
    EXPECT_EQ(engine::ObjectType::CREATURE_TYPE, shim->getObjectType("creature type"));
    EXPECT_EQ(engine::ObjectType::EXIT, shim->getObjectType("exit"));
    EXPECT_EQ(engine::ObjectType::FEATURE, shim->getObjectType("feature"));
    EXPECT_EQ(engine::ObjectType::ITEM, shim->getObjectType("item"));
    EXPECT_EQ(engine::ObjectType::ITEM_TYPE, shim->getObjectType("item type"));
    EXPECT_EQ(engine::ObjectType::NON_COMBATANT, shim->getObjectType("non combatant"));
    EXPECT_EQ(engine::ObjectType::PLAYER, shim->getObjectType("player"));
    EXPECT_EQ(engine::ObjectType::PLAYER_CLASS, shim->getObjectType("player class"));
    EXPECT_EQ(engine::ObjectType::QUEST, shim->getObjectType("quest"));
    EXPECT_EQ(engine::ObjectType::QUEST_STEP, shim->getObjectType("quest step"));
    EXPECT_EQ(engine::ObjectType::SPECIAL_SKILL, shim->getObjectType("special skill"));
    EXPECT_EQ(engine::ObjectType::WEAPON_TYPE, shim->getObjectType("weapon type"));
    // Invalid input
    EXPECT_EQ(engine::ObjectType::NONE, shim->getObjectType("asdf"));
    // Empty string
    EXPECT_EQ(engine::ObjectType::NONE, shim->getObjectType(""));
}

TEST_F(GameLogicTest, SendMessageToPlayer) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);

    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    // Send a message to the player
    shim->messagePlayer(player, "foo message");
    EXPECT_STREQ("foo message", server->tryGetToPlayerMsg().c_str());
}

TEST_F(GameLogicTest, GetValueFromUser) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);

    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    // Prompt the player
    std::string response;
    shim->getValueFromUser(0, "foo prompt", response);
    EXPECT_STREQ("foo prompt", server->tryGetToPlayerMsg().c_str());
}

TEST_F(GameLogicTest, StartAndEndConversation) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    engine::NonCombatant *npc = new engine::NonCombatant();
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);
    shim->getGameObjectManager()->addObject(npc, -1);

    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    EXPECT_EQ(nullptr, player->getInConversation());
    EXPECT_TRUE(shim->startConversation(player, npc));
    EXPECT_EQ(npc, player->getInConversation());
    EXPECT_TRUE(shim->endConversation(player));
    EXPECT_EQ(nullptr, player->getInConversation());
}

TEST_F(GameLogicTest, ExecuteCommand) {
    // Start game and load minimum objects
    ASSERT_TRUE(logic->startGame(true, "game.dat", server, acct));
    engine::Area *area = new engine::Area("Area", "Short description", "Long description", engine::AreaSize::SMALL);
    engine::SpecialSkill *skill = new engine::SpecialSkill("Fireball", 10, engine::DamageType::FIRE, 5, 10);
    engine::PlayerClass *playerClass = new engine::PlayerClass(1, "Mage", skill, 0, 0, engine::DamageType::FIRE, engine::DamageType::WATER, 0);
    engine::Player *player = new engine::Player(engine::CharacterSize::TINY, playerClass, "Username", 0, 20, area, 20, "Character name", "Character description", 100, area, 30);
    player->setActive(false);
    shim->getGameObjectManager()->addObject(area, -1);
    shim->getGameObjectManager()->addObject(skill, -1);
    shim->getGameObjectManager()->addObject(playerClass, -1);
    shim->getGameObjectManager()->addObject(player, -1);

    // Attempt to load the player
    ASSERT_TRUE(shim->loadPlayer(player, 0));
    // Player should now be active
    EXPECT_TRUE(player->isActive());
    parser::ParseResult result;
    result.type = parser::VerbType::GLOBAL;
    result.status = parser::ParseStatus::VALID;
    result.command = engine::CommandEnum::HELP;

    EXPECT_TRUE(shim->executeCommand(player, result));
}

}

