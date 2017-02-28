/*!
  \file     engine_Player_Test.cpp
  \author   David Rigert
  \created  02/23/2017
  \modified 02/23/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the Player class.
*/

#include <Player.hpp>
#include <PlayerClass.hpp>
#include <NonCombatant.hpp>
#include <Quest.hpp>
#include <QuestStep.hpp>
#include <Item.hpp>
#include <ItemType.hpp>
#include <Container.hpp>
#include <Area.hpp>

#include <gtest/gtest.h>

namespace {

namespace engine = legacymud::engine;
namespace parser = legacymud::parser;

// Default constructor test
TEST(PlayerTest, DefaultConstructor) {
    engine::Player player;
    EXPECT_EQ(0, player.getExperiencePoints());
    EXPECT_EQ(1, player.getLevel());
    EXPECT_EQ(engine::CharacterSize::MEDIUM, player.getSize());
    EXPECT_TRUE(player.getUser().empty());
    EXPECT_EQ(nullptr, player.getPlayerClass());
    EXPECT_EQ(nullptr, player.getInConversation());
    EXPECT_EQ(nullptr, player.getNextCommand());
    EXPECT_FALSE(player.isActive());
    EXPECT_EQ(-1, player.getFileDescriptor());
    EXPECT_TRUE(player.queueIsEmpty());
    EXPECT_FALSE(player.isEditMode());
    EXPECT_EQ(0, player.getQuestList().size());
    EXPECT_EQ(engine::ObjectType::PLAYER, player.getObjectType());
}

// Verify that experience is correctly added
TEST(PlayerTest, AddExperienceTest) {
    engine::Player player;
    EXPECT_EQ(0, player.getExperiencePoints());
    EXPECT_EQ(123, player.addToExperiencePts(123));
    EXPECT_EQ(200, player.addToExperiencePts(77));
    EXPECT_EQ(200, player.getExperiencePoints());
}

// Verify that the level is correctly incremented
TEST(PlayerTest, LevelUpTest) {
    engine::Player player;
    EXPECT_EQ(1, player.getLevel());
    EXPECT_TRUE(player.levelUp());
    EXPECT_EQ(2, player.getLevel());
}

// Verify that the size is correctly changed
TEST(PlayerTest, SetSizeTest) {
    engine::Player player;
    EXPECT_EQ(engine::CharacterSize::MEDIUM, player.getSize());
    EXPECT_TRUE(player.setSize(engine::CharacterSize::TINY));
    EXPECT_EQ(engine::CharacterSize::TINY, player.getSize());
}

// Verify that the PlayerClass is correctly set
TEST(PlayerTest, SetPlayerClassTest) {
    engine::Player player;
    engine::PlayerClass playerClass;
    EXPECT_EQ(nullptr, player.getPlayerClass());
    EXPECT_TRUE(player.setPlayerClass(&playerClass));
    EXPECT_EQ(&playerClass, player.getPlayerClass());
}

// Verify that the active state is correctly set
TEST(PlayerTest, SetActiveTest) {
    engine::Player player;
    EXPECT_FALSE(player.isActive());
    EXPECT_TRUE(player.setActive(true));
    EXPECT_TRUE(player.isActive());
}

// Verify that the file descriptor is correctly set
TEST(PlayerTest, SetFileDescriptorTest) {
    engine::Player player;
    EXPECT_EQ(-1, player.getFileDescriptor());
    EXPECT_TRUE(player.setFileDescriptor(1));
    EXPECT_EQ(1, player.getFileDescriptor());
}

// Verify that the active state and file descriptor are correctly set
TEST(PlayerTest, ActivateTest) {
    engine::Player player;
    EXPECT_EQ(-1, player.getFileDescriptor());
    EXPECT_FALSE(player.isActive());
    EXPECT_TRUE(player.activate(1));
    EXPECT_EQ(1, player.getFileDescriptor());
    EXPECT_TRUE(player.isActive());
}

// Verify that the conversation partner is correctly set
TEST(PlayerTest, SetConversationTest) {
    engine::Player player;
    engine::NonCombatant npc;
    EXPECT_EQ(nullptr, player.getInConversation());
    EXPECT_TRUE(player.setInConversation(&npc));
    EXPECT_EQ(&npc, player.getInConversation());
}

// Verify that a command is correctly added and consumed
TEST(PlayerTest, AddCommandTest) {
    engine::Player player;
    engine::Command command;
    command.commandE = engine::CommandEnum::TALK;
    command.firstParam = &player;
    command.secondParam = &player;

    EXPECT_EQ(nullptr, player.getNextCommand());
    EXPECT_TRUE(player.addCommand(&command));
    EXPECT_EQ(&command, player.getNextCommand());
    EXPECT_EQ(nullptr, player.getNextCommand());
}

// Verify that edit mode is correctly set
TEST(PlayerTest, SetEditModeTest) {
    engine::Player player;
    EXPECT_FALSE(player.isEditMode());
    EXPECT_TRUE(player.setEditMode(true));
    EXPECT_TRUE(player.isEditMode());
}

// Verify that a quest can be successfully added
// TODO: Not sure what updateQuest is supposed to be used for
TEST(PlayerTest, AddQuestTest) {
    engine::Player player;
    engine::Quest quest;
    engine::QuestStep step1;
    engine::QuestStep step2;
    quest.addStep(1, &step1);
    quest.addStep(2, &step2);

    EXPECT_TRUE(player.addQuest(&quest, 1));
    auto quests = player.getQuestList();
    ASSERT_EQ(1, quests.size());
    EXPECT_EQ(&quest, quests[0].first);
}

// Verify adding and removing an item to and from inventory
TEST(PlayerTest, AddRemoveItemInventoryTest) {
    engine::Player player;
    engine::Item item;

    EXPECT_FALSE(player.removeFromInventory(&item));
    EXPECT_TRUE(player.addToInventory(&item));
    EXPECT_TRUE(player.removeFromInventory(&item));
    EXPECT_FALSE(player.removeFromInventory(&item));
}

// Verify that all items inside a container are added to player lexical data
TEST(PlayerTest, AddRemoveContainerInventoryTest) {
    engine::Area area("room", "a room", "a long room", engine::AreaSize::SMALL);
    engine::Player player;
    engine::ItemType bagType(1, engine::ItemRarity::COMMON, "a bag", "bag", 1, engine::EquipmentSlot::NONE);
    engine::ItemType keyType(1, engine::ItemRarity::COMMON, "a key", "key", 1, engine::EquipmentSlot::NONE);
    engine::Container bag(10, &area, engine::ItemPosition::GROUND, "bag", &bagType);
    engine::Item key(&area, engine::ItemPosition::GROUND, "key", &keyType);
    EXPECT_TRUE(bag.place(&key, engine::ItemPosition::IN));
    EXPECT_TRUE(player.addToInventory(&bag));
    EXPECT_TRUE(player.getLexicalData().hasNoun("bag"));
    EXPECT_TRUE(player.getLexicalData().hasNoun("key"));
    EXPECT_TRUE(player.removeFromInventory(&bag));
    EXPECT_FALSE(player.getLexicalData().hasNoun("bag"));
    EXPECT_FALSE(player.getLexicalData().hasNoun("key"));
}

}

