/*!
  \file     engine_GameLogic_Test.cpp
  \author   David Rigert
  \created  02/27/2017
  \modified 02/27/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the GameLogic class.
*/

#include "GameLogicShim.hpp"
#include "TestServer.hpp"
#include <Account.hpp>
#include <Server.hpp>

#include <gtest/gtest.h>

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
    }

    virtual void SetUp() {
        logic = new engine::GameLogic();
        shim = new test::GameLogicShim(logic);
        server = new test::TestServer();
        acct = new account::Account("");
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
    EXPECT_TRUE(logic->startGame(true, "", server, acct));
    EXPECT_EQ(acct, shim->getAccountSystem());
    EXPECT_EQ(server, shim->getTelnetServer());
    EXPECT_TRUE(shim->getGameObjectManager() != nullptr);
}

}

