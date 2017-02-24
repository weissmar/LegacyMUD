/*!
  \file     engine_InteractiveNoun_Test.cpp
  \author   David Rigert
  \created  02/22/2017
  \modified 02/22/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the InteractiveNoun class.
*/

#include <Item.hpp>
#include <Action.hpp>
#include <Grammar.hpp>

#include <gtest/gtest.h>
#include <thread>
#include <set>

namespace {

namespace engine = legacymud::engine;
namespace parser = legacymud::parser;

engine::InteractiveNoun *in;

// Test fixture to set up and tear down each unit test
class InteractiveNounTest : public :: testing::Test {
public:
    static void SetUpTestCase() {
    }

    static void TearDownTestCase() {
    }

    virtual void SetUp() {
        in = new engine::Item();
    }

    virtual void TearDown() {
        delete in;
        in = nullptr;
    }

};

std::vector<engine::InteractiveNoun*> thread1Nouns;
std::vector<engine::InteractiveNoun*> thread2Nouns;

// Function for generating objects in a thread
void generateObjects(int threadNo) {
    for (int i = 0; i < 10000; ++i) {
        if (threadNo == 1) {
            thread1Nouns.push_back(new engine::Item());
        }
        else {
            thread2Nouns.push_back(new engine::Item());
        }
    }
}

// Verify default constructor
TEST_F(InteractiveNounTest, DefaultConstructorTest) {
    EXPECT_EQ(0, in->getNounAliases().size());
    EXPECT_EQ(0, in->getVerbAliases().size());
}

/* // Cannot use this until Item has a copy constructor defined.
// Verify copy constructor
TEST(InteractiveNounTest, CopyConstructorTest) {
    int id = in->getID();
    engine::Action *act = in->addAction(engine::CommandEnum::TALK);
    in->addAlias("foo");
    engine::InteractiveNoun *in2 = new engine::Item(*in);
    // Make sure ID is different
    EXPECT_NE(id, in2->getID());
    // Make sure it has the same action
    EXPECT_TRUE(in2->checkAction(engine::CommandEnum::TALK));
    // Make sure it has the same alias
    if (in2->getNounAliases().size() > 0) {
        EXPECT_STREQ(in->getNounAliases()[0].c_str(), in2->getNounAliases()[0].c_str());
    }
    else {
        EXPECT_EQ(in->getNounAliases().size(), in2->getNounAliases().size());
    }
    delete in2;
}
*/

// Verify adding and retrieving an Action
TEST_F(InteractiveNounTest, AddRetrieveActionTest) {
    engine::Action *act = in->addAction(engine::CommandEnum::TALK);
    // Make sure we got an Action instance back
    ASSERT_NE(nullptr, act);
    act->addAlias("foo", new parser::Grammar());
    // Make sure it has the action
    EXPECT_TRUE(in->checkAction(engine::CommandEnum::TALK));
    ASSERT_EQ(1, in->getActions("foo").size());
    // Verify that action points to same object
    EXPECT_EQ(act, in->getActions("foo")[0]);
}

// Verify adding and removing an Action
TEST_F(InteractiveNounTest, AddRemoveActionTest) {
    engine::Action *act = in->addAction(engine::CommandEnum::TALK);
    // Make sure we got an Action instance back
    ASSERT_NE(nullptr, act);
    act->addAlias("foo", new parser::Grammar());
    // Make sure it has the action
    EXPECT_TRUE(in->checkAction(engine::CommandEnum::TALK));
    ASSERT_EQ(1, in->getActions("foo").size());
    // Verify that action points to same object
    EXPECT_EQ(act, in->getActions("foo")[0]);
    // Remove the action and verify that it no longer exists
    EXPECT_TRUE(in->removeAction(engine::CommandEnum::TALK));
    EXPECT_FALSE(in->checkAction(engine::CommandEnum::TALK));
    EXPECT_EQ(0, in->getActions("foo").size());
}

// Verify adding and retrieving a verb alias
TEST_F(InteractiveNounTest, AddRetrieveVerbAliasTest) {
    engine::Action *act = in->addAction(engine::CommandEnum::TALK);
    act->addAlias("foo", new parser::Grammar());
    ASSERT_EQ(1, in->getVerbAliases().size());
    EXPECT_STREQ("foo", in->getVerbAliases()[0].c_str());
}

// Verify adding, retrieving, and removing a noun alias
TEST_F(InteractiveNounTest, AddRetrieveRemoveNounAliasTest) {
    in->addAlias("foo");
    ASSERT_EQ(1, in->getNounAliases().size());
    EXPECT_STREQ("foo", in->getNounAliases()[0].c_str());
    EXPECT_TRUE(in->removeAlias("foo"));
    EXPECT_EQ(0, in->getNounAliases().size());
}

// Verify thread safety of ID generation
TEST_F(InteractiveNounTest, ThreadSafeIdTest) {
    // Generate 10000 objects in each thread simultaneously
    std::thread first(generateObjects, 1);
    std::thread second(generateObjects, 2);
    first.join();
    second.join();

    // Make sure there are no duplicates
    std::set<int> ids;
    int id;
    for (auto it = thread1Nouns.begin(); it != thread1Nouns.end(); ++it) {
        id = (*it)->getID();
        EXPECT_TRUE(ids.insert(id).second) << id << " generated twice\n";
    }
    for (auto it = thread2Nouns.begin(); it != thread2Nouns.end(); ++it) {
        id = (*it)->getID();
        EXPECT_TRUE(ids.insert(id).second) << id << " generated twice\n";
    }

    // Free everything
    for (auto it = thread1Nouns.begin(); it != thread1Nouns.end(); ++it) {
        delete *it;
    }
    for (auto it = thread2Nouns.begin(); it != thread2Nouns.end(); ++it) {
        delete *it;
    }
    thread1Nouns.clear();
    thread2Nouns.clear();
}


}

