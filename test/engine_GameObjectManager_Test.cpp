/*!
  \file     engine_GameObjectManager_Test.cpp
  \author   David Rigert
  \created  02/20/2017
  \modified 03/02/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the GameObjectManager class.
*/

#include <GameObjectManager.hpp>
#include <Area.hpp>
#include <Creature.hpp>
#include <Player.hpp>
#include <PlayerClass.hpp>
#include <Item.hpp>

#include <gtest/gtest.h>

namespace {

namespace engine = legacymud::engine;

engine::GameObjectManager *gom = nullptr;

// Test fixture to set up and tear down each unit test
class GameObjectManagerTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
    }

    static void TearDownTestCase() {
    }

    virtual void SetUp() {
        gom = new engine::GameObjectManager();
    }

    virtual void TearDown() {
        delete gom;
        gom = nullptr;
    }

};


// Verify default constructor
TEST_F(GameObjectManagerTest, DefaultConstructorTest) {
    EXPECT_TRUE(gom->getCreatures().empty());
    EXPECT_TRUE(gom->getPlayersPtrs().empty());
    EXPECT_TRUE(gom->getPlayersFDs().empty());
    EXPECT_TRUE(gom->getPlayerClasses().empty());
}

// Verify adding a Creature object and getting it back
TEST_F(GameObjectManagerTest, AddGetCreatureTest) {
    engine::Creature *obj = new engine::Creature();
    gom->addObject(obj, 0);
    auto objs = gom->getCreatures();
    ASSERT_EQ(1, objs.size());
    EXPECT_EQ(obj, objs[0]);
}

// Verify adding an active Player object and getting it back
TEST_F(GameObjectManagerTest, AddGetActivePlayerTest) {
    engine::Player *obj = new engine::Player();
    gom->addObject(obj, 1);
    auto objs = gom->getPlayersPtrs();
    ASSERT_EQ(1, objs.size());
    EXPECT_EQ(obj, objs[0]);
    EXPECT_EQ(obj, gom->getPlayerByFD(1));
}

// Verify adding an inactive Player object and getting it back
TEST_F(GameObjectManagerTest, AddGetInactivePlayerTest) {
    engine::Area area;
    engine::PlayerClass playerClass;
    engine::Player *obj = new engine::Player(engine::CharacterSize::SMALL, &playerClass, "username", 0, "Fred", "description", &area);
    gom->addObject(obj, -1);
    auto objs = gom->getPlayersPtrs();
    EXPECT_EQ(0, objs.size());
    EXPECT_EQ(obj, gom->getPlayerByUsername("username"));
}

// Verify loading an inactive Player object
TEST_F(GameObjectManagerTest, LoadInactivePlayerTest) {
    engine::Area area;
    engine::PlayerClass playerClass;
    engine::Player *obj = new engine::Player(engine::CharacterSize::SMALL, &playerClass, "username", 0, "Fred", "description", &area);
    gom->addObject(obj, -1);
    auto objs = gom->getPlayersPtrs();
    EXPECT_EQ(0, objs.size());
    EXPECT_EQ(obj, gom->getPlayerByUsername("username"));
    EXPECT_TRUE(gom->loadPlayer("username", 1));
    objs = gom->getPlayersPtrs();
    ASSERT_EQ(1, objs.size());
    EXPECT_EQ(obj, objs[0]);
}

// Verify getting an object by ID
TEST_F(GameObjectManagerTest, GetObjectByIdTest) {
    engine::Item *obj = new engine::Item();
    gom->addObject(obj, 0);
    EXPECT_EQ(obj, gom->getPointer(obj->getID()));
}

// Verify adding and removing an object
TEST_F(GameObjectManagerTest, AddRemoveObjectTest) {
    engine::Item *obj = new engine::Item();
    int id = obj->getID();
    gom->addObject(obj, 0);
    EXPECT_EQ(obj, gom->getPointer(id));
    gom->removeObject(obj, 0);
    EXPECT_EQ(nullptr, gom->getPointer(id));
}

// Verify getting the file descriptors of players
TEST_F(GameObjectManagerTest, GetPlayerByFdTest) {
    engine::Player *obj1 = new engine::Player();
    engine::Player *obj2 = new engine::Player();
    gom->addObject(obj1, 1);
    gom->addObject(obj2, 2);
    auto objs = gom->getPlayersPtrs();
    EXPECT_EQ(2, objs.size());
    auto playerFDs = gom->getPlayersFDs();
    EXPECT_EQ(2, playerFDs.size());
    for (size_t i = 0; i < playerFDs.size(); ++i) {
        if (playerFDs[i] == 1)
            EXPECT_EQ(obj1, gom->getPlayerByFD(playerFDs[i]));
        else
            EXPECT_EQ(obj2, gom->getPlayerByFD(playerFDs[i]));
    }
}

// Verify adding an active Player object and hibernating it
TEST_F(GameObjectManagerTest, HibernateActivePlayerTest) {
    engine::Area area;
    engine::PlayerClass playerClass;
    engine::Player *obj = new engine::Player(engine::CharacterSize::SMALL, &playerClass, "username", 0, "Fred", "description", &area);
    gom->addObject(obj, 1);
    auto objs = gom->getPlayersPtrs();
    EXPECT_EQ(1, objs.size());
    EXPECT_TRUE(gom->hibernatePlayer(1));
    objs = gom->getPlayersPtrs();
    EXPECT_EQ(0, objs.size());
    EXPECT_EQ(obj, gom->getPlayerByUsername("username"));   
}

}

