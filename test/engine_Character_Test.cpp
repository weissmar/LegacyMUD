/*!
  \file     engine_Character_Test.cpp
  \author   David Rigert
  \created  02/24/2017
  \modified 02/24/2017
  \course   CS467, Winter 2017
 
  \details  This file contains the unit tests for the Character class.
*/

#include <NonCombatant.hpp>
#include <Area.hpp>
#include <Item.hpp>
#include <ArmorType.hpp>

#include <gtest/gtest.h>

namespace {

namespace engine = legacymud::engine;
namespace parser = legacymud::parser;

// Default constructor test
TEST(CharacterTest, DefaultConstructor) {
    engine::NonCombatant npc;
    EXPECT_TRUE(npc.getName().empty());
    EXPECT_TRUE(npc.getDescription().empty());
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_EQ(nullptr, npc.getLocation());
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
    EXPECT_EQ(0, npc.getEquipped().size());
    EXPECT_EQ(10, npc.getMaxInventoryWeight());
}

// Verify that the name is correctly changed
TEST(CharacterTest, SetNameTest) {
    engine::NonCombatant npc;
    EXPECT_TRUE(npc.getName().empty());
    EXPECT_TRUE(npc.setName("foo bar"));
    EXPECT_STREQ("foo bar", npc.getName().c_str());
}

// Verify that the description is correctly changed
TEST(CharacterTest, SetDescriptionTest) {
    engine::NonCombatant npc;
    EXPECT_TRUE(npc.getDescription().empty());
    EXPECT_TRUE(npc.setDescription("foo bar"));
    EXPECT_STREQ("foo bar", npc.getDescription().c_str());
}

// Verify that the money is correctly changed
TEST(CharacterTest, SetMoneyTest) {
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_TRUE(npc.setMoney(42));
    EXPECT_EQ(42, npc.getMoney());
    EXPECT_TRUE(npc.setMoney(1337));
    EXPECT_EQ(1337, npc.getMoney());
}

// Verify that setting an illegal money value does not work
TEST(CharacterTest, SetNegativeMoneyTest) {
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_FALSE(npc.setMoney(-42));
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_TRUE(npc.setMoney(1337));
    EXPECT_EQ(1337, npc.getMoney());
    EXPECT_FALSE(npc.setMoney(-42));
    EXPECT_EQ(1337, npc.getMoney());
}

// Verify that the money is correctly added and subtracted
TEST(CharacterTest, AddSubtractMoneyTest) {
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_TRUE(npc.addMoney(42));
    EXPECT_EQ(42, npc.getMoney());
    EXPECT_TRUE(npc.addMoney(1337));
    EXPECT_EQ(1379, npc.getMoney());
    EXPECT_TRUE(npc.subtractMoney(42));
    EXPECT_EQ(1337, npc.getMoney());
}

// Verify that adding and subtracting negative amounts of money does not work
TEST(CharacterTest, AddSubtractNegativeMoneyTest) {
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getMoney());
    EXPECT_TRUE(npc.addMoney(42));
    EXPECT_EQ(42, npc.getMoney());
    EXPECT_FALSE(npc.addMoney(-1337));
    EXPECT_EQ(42, npc.getMoney());
    EXPECT_FALSE(npc.subtractMoney(-42));
    EXPECT_EQ(42, npc.getMoney());
}

// Verify that the location is correctly set
TEST(CharacterTest, SetLocationTest) {
    engine::Area area;
    engine::NonCombatant npc;
    EXPECT_EQ(nullptr, npc.getLocation());
    EXPECT_TRUE(npc.setLocation(&area));
    EXPECT_EQ(&area, npc.getLocation());
}

// Verify adding and removing an item to and from inventory
TEST(CharacterTest, AddItemToInventoryTest) {
    engine::Item item;
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
    EXPECT_TRUE(npc.addToInventory(&item));
    ASSERT_EQ(1, npc.getInventory().size());
    ASSERT_EQ(1, npc.getItemsInventory().size());
    EXPECT_EQ(&item, npc.getItemsInventory()[0]);
    EXPECT_TRUE(npc.removeFromInventory(&item));
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
}

// Verify that you cannot add the same item to inventory twice
TEST(CharacterTest, AddItemToInventoryTwiceTest) {
    engine::Item item;
    engine::NonCombatant npc;
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
    EXPECT_TRUE(npc.addToInventory(&item));
    EXPECT_FALSE(npc.addToInventory(&item));
    ASSERT_EQ(1, npc.getInventory().size());
    ASSERT_EQ(1, npc.getItemsInventory().size());
}

// Verify equipping and unequipping a wearable item in inventory
TEST(CharacterTest, EquipInventoryItemTest) {
    engine::Area area;
    engine::NonCombatant npc(nullptr, "name", "description", 10, &area, 100);
    engine::ArmorType itemType(1, engine::DamageType::NONE, 1, engine::ItemRarity::COMMON, "a helmet", "helmet", 1, engine::EquipmentSlot::HEAD);
    engine::Item item(&area, engine::ItemPosition::INVENTORY, "foo", &itemType);
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
    EXPECT_EQ(0, npc.getEquipped().size());
    EXPECT_TRUE(npc.addToInventory(&item));
    ASSERT_EQ(1, npc.getInventory().size());
    ASSERT_EQ(1, npc.getItemsInventory().size());
    EXPECT_EQ(&item, npc.getItemsInventory()[0]);
    EXPECT_EQ(0, npc.getEquipped().size());
    EXPECT_STREQ("true", npc.equipItem(&item).c_str());
    ASSERT_EQ(1, npc.getEquipped().size());
    EXPECT_EQ(engine::EquipmentSlot::HEAD, npc.getEquipped()[0].first);
}

// Verify equipping and unequipping a wearable item on the ground
TEST(CharacterTest, EquipGroundItemTest) {
    engine::Area area;
    engine::NonCombatant npc(nullptr, "name", "description", 10, &area, 100);
    engine::ArmorType itemType(1, engine::DamageType::NONE, 1, engine::ItemRarity::COMMON, "a helmet", "helmet", 1, engine::EquipmentSlot::HEAD);
    engine::Item item(&area, engine::ItemPosition::GROUND, "foo", &itemType);
    EXPECT_EQ(0, npc.getInventory().size());
    EXPECT_EQ(0, npc.getItemsInventory().size());
    EXPECT_EQ(0, npc.getEquipped().size());
    EXPECT_STREQ("true", npc.equipItem(&item).c_str());
    ASSERT_EQ(1, npc.getEquipped().size());
    ASSERT_EQ(1, npc.getInventory().size());
    ASSERT_EQ(1, npc.getItemsInventory().size());
    EXPECT_EQ(&item, npc.getItemsInventory()[0]);
    EXPECT_EQ(engine::EquipmentSlot::HEAD, npc.getEquipped()[0].first);
}

// Verify equipping and unequipping a wearable item
TEST(CharacterTest, UnequipItemTest) {
    engine::Area area;
    engine::NonCombatant npc(nullptr, "name", "description", 10, &area, 100);
    engine::ArmorType itemType(1, engine::DamageType::NONE, 1, engine::ItemRarity::COMMON, "a helmet", "helmet", 1, engine::EquipmentSlot::HEAD);
    engine::Item item(&area, engine::ItemPosition::GROUND, "foo", &itemType);
    EXPECT_EQ(0, npc.getEquipped().size());
    EXPECT_STREQ("true", npc.equipItem(&item).c_str());
    ASSERT_EQ(1, npc.getEquipped().size());
    EXPECT_TRUE(npc.unequipItem(&item));
    EXPECT_EQ(0, npc.getEquipped().size());
}

}

