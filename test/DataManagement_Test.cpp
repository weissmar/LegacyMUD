/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  2/20/2017
 * Last Modified: 3/7/2017
 * Course:        CS467, Winter 2017
 * Filename:      DataManager_Test.cpp
 *
 * Overview:
 *     Unit tests for the DataManager class.  
 ************************************************************************/
  
#include <DataManager.hpp>
#include <GameObjectManager.hpp> 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Area.hpp>
#include <ArmorType.hpp>
#include <Container.hpp>
#include <Creature.hpp>
#include <CreatureType.hpp>
#include <Exit.hpp>
#include <Feature.hpp>
#include <Item.hpp>
#include <ItemType.hpp>
#include <NonCombatant.hpp>
#include <Player.hpp>
#include <PlayerClass.hpp>
#include <Quest.hpp>
#include <QuestStep.hpp>
#include <SpecialSkill.hpp>
#include <WeaponType.hpp>
#include <InteractiveNoun.hpp>
#include <Grammar.hpp>
#include <map>
#include <vector>        
#include <gtest/gtest.h>

namespace {
   
// create a GameObjectManager and DataManager 
legacymud::gamedata::DataManager* dm = new legacymud::gamedata::DataManager();


/////////////////////////////////////////////////////////////////////////////////
// Area - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeArea) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();    
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Area  (name, short description, long description, area size, id) 
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM,1 );   
    
    // Serialize and Test
    std::string serializedJsonStr = area->serialize();   
    std::string expectedJsonStr = "{\"object\":{\"class\":\"AREA\",\"name\":\"name of area\",\"short_description\":\"short description of area\",\"long_description\":\"longer description\",\"area_size\":\"MEDIUM\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"area 1\",\"name of area\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Area needs for deserialize.  
    // none
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"AREA\",\"name\":\"name of area\",\"short_description\":\"short description of area\",\"long_description\":\"longer description\",\"area_size\":\"MEDIUM\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"area 1\",\"name of area\"],\"actions\":[]}}";
    legacymud::engine::Area *rebuiltArea = legacymud::engine::Area::deserialize(inputJsonStr);   
    std::string reserializedJsonStr = rebuiltArea->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////
// ArmorType - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeArmorType) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();  
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);    
        
    // ArmorType (bonus, resistantTo, weight, rarity, description, name, cost, slotType, id)
    legacymud::engine::ArmorType* armorType = new legacymud::engine::ArmorType(4, legacymud::engine::DamageType::FIRE, 50,
                                                                               legacymud::engine::ItemRarity::COMMON,
                                                                               "a description", "a name", 2500,
                                                                               legacymud::engine::EquipmentSlot::TORSO, 1);                                                  

    // Add to the GameObjectManager. 
    EXPECT_TRUE(gom->addObject(armorType,-1) );   
   
    // Serialize and Test 
    std::string serializedJsonStr = armorType->serialize();
    std::string expectedJsonStr = "{\"object\":{\"class\":\"ARMOR_TYPE\",\"armor_bonus\":4,\"resistant_to\":\"FIRE\",\"name\":\"a name\",\"weight\":50,\"rarity\":\"COMMON\",\"description\":\"a description\",\"cost\":2500,\"slot_type\":\"TORSO\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}}";    
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 

    // Add objects to the GameObjectManager that armorType needs for deserialize. 
    // none
    
    // Deserialize, re-serialize, and test. 
    std::string inputJsonStr = "{\"class\":\"ARMOR_TYPE\",\"armor_bonus\":4,\"resistant_to\":\"FIRE\",\"name\":\"a name\",\"weight\":50,\"rarity\":\"COMMON\",\"description\":\"a description\",\"cost\":2500,\"slot_type\":\"TORSO\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}"; 
    legacymud::engine::ArmorType *rebuiltWeaponType = legacymud::engine::ArmorType::deserialize(inputJsonStr);  
    std::string reserializedJsonStr = rebuiltWeaponType->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );  

    // Clean-up. 
    delete gom;
}


 
/////////////////////////////////////////////////////////////////////////////////
// Container - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeContainer) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();  
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Container.  

    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 1);   
                                                                   
    // ItemType (weight, rarity, description, name, cost, slotType, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT, 2);
                                                                            
    // Container  (capacity, location, position, name, itemeType, id)
    legacymud::engine::Container* container = new legacymud::engine::Container(100, area, legacymud::engine::ItemPosition::GROUND, "name of container", itemType, 3 );   
                                                              
    // Serialize and Test
    std::string serializedJsonStr = container->serialize();      
    std::string expectedJsonStr = "{\"object\":{\"class\":\"CONTAINER\",\"capacity\":100,\"name\":\"name of container\",\"location\":1,\"position\":\"GROUND\",\"item_type_id\":2,\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"item 3\",\"name of container\"],\"actions\":[]}}}";   
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Container needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );   
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"CONTAINER\",\"capacity\":100,\"name\":\"name of container\",\"location\":1,\"position\":\"GROUND\",\"item_type_id\":2,\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"item 3\",\"name of container\"],\"actions\":[]}}";   
    legacymud::engine::Container *rebuiltContainer = legacymud::engine::Container::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltContainer->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. . 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////      
// Creature - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeCreature) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();  
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Creature.  
    
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("a name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

    // CreatureType(size, difficulty, name, specialSkill, attackBonus, armorBonus, resistantTo, weakTo, healPoints, anID)
    legacymud::engine::CreatureType* creatureType = new legacymud::engine::CreatureType(legacymud::engine::CharacterSize::MEDIUM,
                                                                                           legacymud::engine::XPTier::NORMAL,
                                                                                           "a name", skill, 10, 20, 
                                                                                           legacymud::engine::DamageType::WATER, 
                                                                                           legacymud::engine::DamageType::FIRE, 
                                                                                           10.3, 2); 
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 3);   
    
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::SMALL, 4);     
                                                                       
    //  Creature (creatureType, ambulatory, axHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, int anID);
    legacymud::engine::Creature* creature = new legacymud::engine::Creature(creatureType, true, 100, area1, 50, 70, 80, 90, 
                                                                            "a name", "a description", 1500, area2, 100, 5 );
    
    // Serialize and Test
    std::string serializedJsonStr = creature->serialize();   
    std::string expectedJsonStr = "{\"object\":{\"class\":\"CREATURE\",\"creature_type_id\":2,\"ambulatory\":true,\"max_health\":100,\"spawn_location_id\":3,\"max_special_points\":50,\"dexterity\":70,\"strength\":80,\"intelligence\":90,\"name\":\"a name\",\"description\":\"a description\",\"money\":1500,\"location_area_id\":4,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that creature needs for deserialize. 
    EXPECT_TRUE(gom->addObject(skill,-1) );
    EXPECT_TRUE(gom->addObject(creatureType,-1) ); 
    EXPECT_TRUE(gom->addObject(area1,-1) ); 
    EXPECT_TRUE(gom->addObject(area2,-1) ); 
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"CREATURE\",\"creature_type_id\":2,\"ambulatory\":true,\"max_health\":100,\"spawn_location_id\":3,\"max_special_points\":50,\"dexterity\":70,\"strength\":80,\"intelligence\":90,\"name\":\"a name\",\"description\":\"a description\",\"money\":1500,\"location_area_id\":4,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::Creature *rebuiltCreature = legacymud::engine::Creature::deserialize(inputJsonStr, gom);    
    std::string reserializedJsonStr = rebuiltCreature->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. Remove objects from the GameObjectManager.   
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////      
// CreatureType - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeCreatureType) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by CreatureType.  
    
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("a name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

    // CreatureType(size, difficulty, name, specialSkill, attackBonus, armorBonus, resistantTo, weakTo, healPoints, anID)
    legacymud::engine::CreatureType* creatureType = new legacymud::engine::CreatureType(legacymud::engine::CharacterSize::MEDIUM,
                                                                                           legacymud::engine::XPTier::NORMAL,
                                                                                           "a name", skill, 10, 20, 
                                                                                           legacymud::engine::DamageType::WATER, 
                                                                                           legacymud::engine::DamageType::FIRE, 
                                                                                           10.3, 2); 
    
    // Serialize and Test
    std::string serializedJsonStr = creatureType->serialize();    
    std::string expectedJsonStr = "{\"object\":{\"class\":\"CREATURE_TYPE\",\"size\":\"MEDIUM\",\"difficulty\":\"NORMAL\",\"name\":\"a name\",\"special_skill_id\":1,\"attack_bonus\":10,\"armor_bonus\":20,\"resistant_to\":\"WATER\",\"weak_to\":\"FIRE\",\"heal_points\":10.300000190734864,\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"combatant type 2\",\"a name\"],\"actions\":[]}}}"; 
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that creature needs for deserialize. 
    EXPECT_TRUE(gom->addObject(skill,-1) );
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"CREATURE_TYPE\",\"size\":\"MEDIUM\",\"difficulty\":\"NORMAL\",\"name\":\"a name\",\"special_skill_id\":1,\"attack_bonus\":10,\"armor_bonus\":20,\"resistant_to\":\"WATER\",\"weak_to\":\"FIRE\",\"heal_points\":10.300000190734864,\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"combatant type 2\",\"a name\"],\"actions\":[]}}";   
    legacymud::engine::CreatureType *rebuiltCreatureType = legacymud::engine::CreatureType::deserialize(inputJsonStr, gom);    
    std::string reserializedJsonStr = rebuiltCreatureType->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
}


/////////////////////////////////////////////////////////////////////////////////
// Exit with conditional- Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeExitConditionl) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Exit.  

    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of location", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 1);  
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of connect area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 2);                                                                   
                                                                   
    // ItemType (weight, rarity, description, name, cost, slotType, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT, 3);

    // Exit(direction, location, connectArea, isConditional, anItemType, description, altDescription, anID);
    legacymud::engine::Exit* exit = new legacymud::engine::Exit(legacymud::engine::ExitDirection::WEST, area1, area2, true, itemType,
                                                                "some description", "an alt decription", 4);                                                                            
                                                                            
    // Serialize and Test
    std::string serializedJsonStr = exit->serialize();       
    std::string expectedJsonStr = "{\"object\":{\"class\":\"EXIT\",\"exit_direction\":\"WEST\",\"location_id\":1,\"connect_area_id\":2,\"is_conditional\":true,\"condition_itemtype_id\":3,\"description\":\"some description\",\"altdescription\":\"an alt decription\",\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"exit 4\",\"some description\",\"an alt decription\",\"west\",\"w\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Exit needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area1,-1) );
    EXPECT_TRUE(gom->addObject(area2,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );   
   
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"EXIT\",\"exit_direction\":\"WEST\",\"location_id\":1,\"connect_area_id\":2,\"is_conditional\":true,\"condition_itemtype_id\":3,\"description\":\"some description\",\"altdescription\":\"an alt decription\",\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"exit 4\",\"some description\",\"an alt decription\",\"west\",\"w\"],\"actions\":[]}}";
    legacymud::engine::Exit *rebuiltExit = legacymud::engine::Exit::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltExit->serialize();   
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////
// Exit with non-conditional - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeExitNoConditional) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Exit.  

    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of location", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 1);  
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of connect area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 2);                                                                                                                                      

    // Exit(direction, location, connectArea, isConditional, anItemType, description, altDescription, anID);
    legacymud::engine::Exit* exit = new legacymud::engine::Exit(legacymud::engine::ExitDirection::WEST, area1, area2, false, nullptr,
                                                                "some description", "an alt decription", 3);                                                                            
                                                                            
    // Serialize and Test
    std::string serializedJsonStr = exit->serialize();
    std::string expectedJsonStr = "{\"object\":{\"class\":\"EXIT\",\"exit_direction\":\"WEST\",\"location_id\":1,\"connect_area_id\":2,\"is_conditional\":false,\"description\":\"some description\",\"altdescription\":\"an alt decription\",\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"exit 3\",\"some description\",\"an alt decription\",\"west\",\"w\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Exit needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area1,-1) );
    EXPECT_TRUE(gom->addObject(area2,-1) );
     
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"EXIT\",\"exit_direction\":\"WEST\",\"location_id\":1,\"connect_area_id\":2,\"is_conditional\":false,\"description\":\"some description\",\"altdescription\":\"an alt decription\",\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"exit 3\",\"some description\",\"an alt decription\",\"west\",\"w\"],\"actions\":[]}}";
    legacymud::engine::Exit *rebuiltExit = legacymud::engine::Exit::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltExit->serialize();   
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
}


/////////////////////////////////////////////////////////////////////////////////
// Feature with conditional- Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeFeatureConditional) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Feature.  
    
    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area = new legacymud::engine::Area("name of location", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 1); 
                                                                 
    // ItemType (weight, rarity, description, name, cost, slotType, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT, 2);    
    
    // Feature(name, placement, location, isConditional, anItemType, description, altDescription, int anID)
    legacymud::engine::Feature* feature = new legacymud::engine::Feature("a name", "a placement description", area, true,
                                                                          itemType, "a description", "an alt description", 3);
                                                                
    // Serialize and Test
    std::string serializedJsonStr = feature->serialize(); 
    std::string expectedJsonStr = "{\"object\":{\"class\":\"FEATURE\",\"name\":\"a name\",\"placement\":\"a placement description\",\"location_id\":1,\"is_conditional\":true,\"condition_itemtype_id\":2,\"description\":\"a description\",\"altdescription\":\"an alt description\",\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"feature 3\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Exit needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );   
   
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"FEATURE\",\"name\":\"a name\",\"placement\":\"a placement description\",\"location_id\":1,\"is_conditional\":true,\"condition_itemtype_id\":2,\"description\":\"a description\",\"altdescription\":\"an alt description\",\"interactive_noun_data\":{\"id\":3,\"noun_aliases\":[\"feature 3\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::Feature *rebuiltFeature = legacymud::engine::Feature::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltFeature->serialize();   
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;   
}  


/////////////////////////////////////////////////////////////////////////////////
// Feature with no conditional- Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeFeatureNoConditional) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Feature.  
    
    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area = new legacymud::engine::Area("name of location", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM, 1); 
                                                                   
    // Feature(name, placement, location, isConditional, anItemType, description, altDescription, int anID)
    legacymud::engine::Feature* feature = new legacymud::engine::Feature("a name", "a placement description", area, false,
                                                                          nullptr, "a description", "an alt description", 2);
                                                                
    // Serialize and Test
    std::string serializedJsonStr = feature->serialize();    
    std::string expectedJsonStr = "{\"object\":{\"class\":\"FEATURE\",\"name\":\"a name\",\"placement\":\"a placement description\",\"location_id\":1,\"is_conditional\":false,\"description\":\"a description\",\"altdescription\":\"an alt description\",\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"feature 2\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Exit needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );   
   
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"FEATURE\",\"name\":\"a name\",\"placement\":\"a placement description\",\"location_id\":1,\"is_conditional\":false,\"description\":\"a description\",\"altdescription\":\"an alt description\",\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"feature 2\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::Feature *rebuiltFeature = legacymud::engine::Feature::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltFeature->serialize();   
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 
 

/////////////////////////////////////////////////////////////////////////////////      
// Item - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeItem) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Item.  

    // Area (name, short desciption, long description, area size, id) 
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
 
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(nullptr, "GiverMan", "a description", 1000, area, 100, 2);  
                                                                                
    // ItemType (weight, rarity, description, name, cost, slotType, id)                                                             
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT, 3);
                                                                            
    // Item (location, position, name, itemeType, id) 
    legacymud::engine::Item* item = new legacymud::engine::Item(giver, legacymud::engine::ItemPosition::INVENTORY, "name of item", itemType, 4 );   
                                                                   
    // Serialize and Test
    std::string serializedJsonStr = item->serialize();         
    std::string expectedJsonStr = "{\"object\":{\"class\":\"ITEM\",\"name\":\"name of item\",\"location\":2,\"position\":\"INVENTORY\",\"item_type_id\":3,\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"item 4\",\"name of item\"],\"actions\":[]}}}";    
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Item needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(giver,-1) ); 
    EXPECT_TRUE(gom->addObject(itemType,-1) ); 
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"ITEM\",\"name\":\"name of item\",\"location\":2,\"position\":\"INVENTORY\",\"item_type_id\":3,\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"item 4\",\"name of item\"],\"actions\":[]}}";  
    legacymud::engine::Item *rebuiltItem = legacymud::engine::Item::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltItem->serialize(); 
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////     
// ItemType - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeItemType) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
                                                                        
    // ItemType (weight, rarity, description, name, cost, slotType, id)                                                             
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT, 1);
                                                                                                                                     
    // Serialize and Test
    std::string serializedJsonStr = itemType->serialize();    
    std::string expectedJsonStr = "{\"object\":{\"class\":\"ITEM_TYPE\",\"weight\":25,\"rarity\":\"COMMON\",\"description\":\"a description\",\"name\":\"a name\",\"cost\":2545,\"slot_type\":\"BELT\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Item needs for deserialize. 
    // none
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"ITEM_TYPE\",\"weight\":25,\"rarity\":\"COMMON\",\"description\":\"a description\",\"name\":\"a name\",\"cost\":2545,\"slot_type\":\"BELT\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::ItemType *rebuiltItemType = legacymud::engine::ItemType::deserialize(inputJsonStr);   
    std::string reserializedJsonStr = rebuiltItemType->serialize(); 
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 
 

/////////////////////////////////////////////////////////////////////////////////      
// NonCombatant with Quest - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeNonCombatantWithQuest) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
     
    // Create objects needed by NonCombatant

    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1); 
    
    // ItemType (weight, rarity, description, name, cost, equipment slot, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD, 2);  
                                                                            
    // Item (location, position, name, item type, id) 
    legacymud::engine::Item* item = new legacymud::engine::Item(area, legacymud::engine::ItemPosition::GROUND, 
                                                                "item name", itemType, 3 );                                                                     
    
    // Quest  (name, description, reward money, reward item, id)                                               
    legacymud::engine::Quest* quest = new legacymud::engine::Quest("name of quest", "description of quest", 1000, item, 4);
    
    
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* nonCombatant = new legacymud::engine::NonCombatant(quest, "a name", "a description", 1000, area, 100, 5); 
    
    // Serialize and Test
    std::string serializedJsonStr = nonCombatant->serialize(); 
    std::string expectedJsonStr = "{\"object\":{\"class\":\"NON_COMBATANT\",\"quest_id\":4,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":1,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that creature needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );
    EXPECT_TRUE(gom->addObject(item,-1) );
    EXPECT_TRUE(gom->addObject(quest,-1) );
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"NON_COMBATANT\",\"quest_id\":4,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":1,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::NonCombatant *rebuiltNonCombatant = legacymud::engine::NonCombatant::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltNonCombatant->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////      
// NonCombatant without Quest - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeNonCombatantWithOutQuest) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
     
    // Create objects needed by NonCombatant

    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);    
    
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* nonCombatant = new legacymud::engine::NonCombatant(nullptr, "a name", "a description", 1000, area, 100, 5); 
    
    // Serialize and Test
    std::string serializedJsonStr = nonCombatant->serialize();  
    std::string expectedJsonStr = "{\"object\":{\"class\":\"NON_COMBATANT\",\"quest_id\":-1,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":1,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that creature needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );  
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"NON_COMBATANT\",\"quest_id\":-1,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":1,\"max_inventory_weight\":100,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::NonCombatant *rebuiltNonCombatant = legacymud::engine::NonCombatant::deserialize(inputJsonStr, gom);
    std::string reserializedJsonStr = rebuiltNonCombatant->serialize(); 
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
} 


/////////////////////////////////////////////////////////////////////////////////    
// Player - Unit test - serialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializePlayer) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Player.  
   
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("special skill name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

    // PlayerClass (primaryStat, name, special skill, attackBonus, armorBonus, resistantTo, weakTo, float healPoints, int anID);
    legacymud::engine::PlayerClass* playerClass = new legacymud::engine::PlayerClass(45, "playerClass name", skill, 10, 20, 
                                                                                legacymud::engine::DamageType::FIRE,
                                                                                legacymud::engine::DamageType::WATER,
                                                                                35.5, 2 ); 
   
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 3);   
    
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::SMALL, 4);        
   
    //  Player (size, playerClass, username, FD, maxHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, anID);
    legacymud::engine::Player* player = new legacymud::engine::Player(legacymud::engine::CharacterSize::MEDIUM, playerClass, 
                                                                      "a user name", -1, 100, area1, 100, 10,20,30,
                                                                      "a name", "a description", 1000, area2, 120, 5);

    // Serialize and Test
    std::string serializedJsonStr = player->serialize();         
    std::string expectedJsonStr = "{\"object\":{\"class\":\"PLAYER\",\"character_size\":\"MEDIUM\",\"player_class_id\":2,\"username\":\"a user name\",\"experience_points\":0,\"level\":1,\"quest_list\":[],\"max_health\":100,\"spawn_location_id\":3,\"max_special_points\":100,\"dexterity\":10,\"strength\":20,\"intelligence\":30,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":4,\"max_inventory_weight\":120,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Clean-up. 
    delete gom;
}


/////////////////////////////////////////////////////////////////////////////////     
// Player - Unit test - deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializePlayer) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Player.  
   
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("special skill name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

    // PlayerClass (primaryStat, name, special skill, attackBonus, armorBonus, resistantTo, weakTo, float healPoints, int anID);
    legacymud::engine::PlayerClass* playerClass = new legacymud::engine::PlayerClass(45, "playerClass name", skill, 10, 20, 
                                                                                legacymud::engine::DamageType::FIRE,
                                                                                legacymud::engine::DamageType::WATER,
                                                                                35.5, 2 ); 
   
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 3);   
    
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::SMALL, 4);        
    
    // Add objects to the GameObjectManager that player needs for deserialize. 
    EXPECT_TRUE(gom->addObject(skill,-1) );
    EXPECT_TRUE(gom->addObject(playerClass,-1) ); 
    EXPECT_TRUE(gom->addObject(area1,-1) ); 
    EXPECT_TRUE(gom->addObject(area2,-1) ); 
       
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"PLAYER\",\"character_size\":\"MEDIUM\",\"player_class_id\":2,\"username\":\"a user name\",\"experience_points\":0,\"level\":1,\"quest_list\":[],\"max_health\":100,\"spawn_location_id\":3,\"max_special_points\":100,\"dexterity\":10,\"strength\":20,\"intelligence\":30,\"name\":\"a name\",\"description\":\"a description\",\"money\":1000,\"location_area_id\":4,\"max_inventory_weight\":120,\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"character 5\",\"a name\"],\"actions\":[]}}";  
    legacymud::engine::Player *rebuiltPlayer = legacymud::engine::Player::deserialize(inputJsonStr, gom);  
    std::string reserializedJsonStr = rebuiltPlayer->serialize(); 
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;  
} 

  
/////////////////////////////////////////////////////////////////////////////////     
// PlayerClass - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializePlayerClass) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create objects needed by Player.  
    
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("special skill1 name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

    // PlayerClass (int primaryStat, std::string name, SpecialSkill* skill, int attackBonus, int armorBonus, DamageType resistantTo, DamageType weakTo, float healPoints, int anID);
    legacymud::engine::PlayerClass* playerClass = new legacymud::engine::PlayerClass(45, "playerClass1 name", skill, 10, 20, 
                                                                                legacymud::engine::DamageType::FIRE,
                                                                                legacymud::engine::DamageType::WATER,
                                                                                35.5, 2 );

   // Serialize and Test
    std::string serializedJsonStr = playerClass->serialize();            
    std::string expectedJsonStr = "{\"object\":{\"class\":\"PLAYER_CLASS\",\"primary_stat\":45,\"name\":\"playerClass1 name\",\"special_skill_id\":1,\"attack_bonus\":10,\"armor_bonus\":20,\"resistant_to\":\"FIRE\",\"weak_to\":\"WATER\",\"heal_points\":35.5,\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"combatant type 2\",\"playerClass1 name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
   
    // Add objects to the GameObjectManager that player needs for deserialize. 
    EXPECT_TRUE(gom->addObject(skill,-1) );
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"PLAYER_CLASS\",\"primary_stat\":45,\"name\":\"playerClass1 name\",\"special_skill_id\":1,\"attack_bonus\":10,\"armor_bonus\":20,\"resistant_to\":\"FIRE\",\"weak_to\":\"WATER\",\"heal_points\":35.5,\"interactive_noun_data\":{\"id\":2,\"noun_aliases\":[\"combatant type 2\",\"playerClass1 name\"],\"actions\":[]}}";
    legacymud::engine::PlayerClass *rebuiltPlayerClass = legacymud::engine::PlayerClass::deserialize(inputJsonStr, gom);   
    std::string reserializedJsonStr = rebuiltPlayerClass->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;

}
  

/////////////////////////////////////////////////////////////////////////////////
// Quest with reward item - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeQuestWithReward) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Quest.

    // Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
                                                                 
    // ItemType (weight, rarity, description, name, cost, equipment slot, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD, 2);  
                                                                            
    // Item (location, position, name, item type, id) 
    legacymud::engine::Item* item = new legacymud::engine::Item(area, legacymud::engine::ItemPosition::GROUND, 
                                                                "item name", itemType, 3 );                                                                     
    
    // Quest  (name, description, reward money, reward item, id)                                               
    legacymud::engine::Quest* quest = new legacymud::engine::Quest("name of quest", "description of quest", 1000, item, 4);    
    
    // Add to Quest: Action,VerbAlias, NonCombatant Characters, and QuestSteps 
    
    // addAction(command, valid, flavor text, effect) 
    quest->addAction(legacymud::engine::CommandEnum::LOOK, true, "some flavor text", 
                     legacymud::engine::EffectType::DROP_ALL_ITEMS);   
                     
    std::map<std::string, legacymud::parser::PrepositionType> preps; 
    preps["above"] = legacymud::parser::PrepositionType::ON;         
    
    // addVerbAlias(command, alias,direct object support, indirect object support, prepositions) 
    quest->addVerbAlias(legacymud::engine::CommandEnum::LOOK, "vas far", legacymud::parser::Grammar::Support::YES, 
                       legacymud::parser::Grammar::Support::NO, preps);                         
                       
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(quest, "GiverMan", "a description", 1000, area, 100, 5);                                                                                                                                                            
    legacymud::engine::NonCombatant* receiver = new legacymud::engine::NonCombatant(quest, "ReceiverMan", "a description", 10, area, 100, 6);
    
    // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText, id)
    legacymud::engine::QuestStep* step1 = new legacymud::engine::QuestStep(1, "start", itemType, giver, receiver, "go to step 2",7);                                                                     
    legacymud::engine::QuestStep* step2 = new legacymud::engine::QuestStep(2, "end", itemType, giver, receiver, "end", 8);  
                                                                           
    quest->addStep(step1);
    quest->addStep(step2); 
    
    // Serialize and Test
    std::string serializedJsonStr = quest->serialize();
    std::string expectedJsonStr = "{\"object\":{\"class\":\"QUEST\",\"name\":\"name of quest\",\"description\":\"description of quest\",\"reward_money\":1000,\"reward_item_id\":3,\"quest_steps\":[{\"step_count\":1,\"quest_step_id\":7},{\"step_count\":2,\"quest_step_id\":8}],\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"quest 4\",\"name of quest\"],\"actions\":[{\"command\":\"LOOK\",\"valid\":true,\"flavor_text\":\"some flavor text\",\"effect\":\"DROP_ALL_ITEMS\",\"aliases\":[{\"verb_alias\":\"vas far\",\"grammar\":{\"direct_object\":\"YES\",\"indirect_object\":\"NO\",\"preposition_map\":[{\"preposition\":\"above\",\"preposition_type\":\"ON\"}]}}]}]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Quest needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );
    EXPECT_TRUE(gom->addObject(item,-1) );         
    EXPECT_TRUE(gom->addObject(giver,-1) ); 
    EXPECT_TRUE(gom->addObject(receiver,-1) );
    EXPECT_TRUE(gom->addObject(step1,-1) );  
    EXPECT_TRUE(gom->addObject(step2,-1) );     
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"QUEST\",\"name\":\"name of quest\",\"description\":\"description of quest\",\"reward_money\":1000,\"reward_item_id\":3,\"quest_steps\":[{\"step_count\":1,\"quest_step_id\":7},{\"step_count\":2,\"quest_step_id\":8}],\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"quest 4\",\"name of quest\"],\"actions\":[{\"command\":\"LOOK\",\"valid\":true,\"flavor_text\":\"some flavor text\",\"effect\":\"DROP_ALL_ITEMS\",\"aliases\":[{\"verb_alias\":\"vas far\",\"grammar\":{\"direct_object\":\"YES\",\"indirect_object\":\"NO\",\"preposition_map\":[{\"preposition\":\"above\",\"preposition_type\":\"ON\"}]}}]}]}}";        
    legacymud::engine::Quest *rebuiltQuest = legacymud::engine::Quest::deserialize(inputJsonStr, gom);
    std::string reserializedJsonStr = rebuiltQuest->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;  
}


/////////////////////////////////////////////////////////////////////////////////
// Quest no reward item - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeQuestNoReward) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Quest                                                                     
    
    // Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of area1", "short description of area1", "longer description1", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
                                                                 
                                                                 // Area (name, short desciption, long description2, area size, id)    
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of area2", "short description of area2", "longer description2", 
                                                                 legacymud::engine::AreaSize::LARGE, 2);   
    
        // ItemType (weight, rarity, description, name, cost, equipment slot, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD, 3); 
    
    // Quest  (name, description, reward money, reward item, id)                                               
    legacymud::engine::Quest* quest = new legacymud::engine::Quest("name of quest", "description of quest", 1000, nullptr, 4);    
    
    // Add to Quest: Action,VerbAlias, NonCombatant Characters, and QuestSteps 
    
    // addAction(command, valid, flavor text, effect) 
    quest->addAction(legacymud::engine::CommandEnum::LOOK, true, "some flavor text", 
                     legacymud::engine::EffectType::DROP_ALL_ITEMS);   
                     
    std::map<std::string, legacymud::parser::PrepositionType> preps; 
    preps["above"] = legacymud::parser::PrepositionType::ON;         
    
    // addVerbAlias(command, alias,direct object support, indirect object support, prepositions) 
    quest->addVerbAlias(legacymud::engine::CommandEnum::LOOK, "vas far", legacymud::parser::Grammar::Support::YES, 
                       legacymud::parser::Grammar::Support::NO, preps);                         
                       
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(quest, "GiverMan", "a description", 1000, area1, 100, 5);                                                                                                                                                            
    legacymud::engine::NonCombatant* receiver = new legacymud::engine::NonCombatant(quest, "ReceiverMan", "a description", 10, area2, 100, 6);
    
    // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText, id)
    legacymud::engine::QuestStep* step1 = new legacymud::engine::QuestStep(1, "start", itemType, giver, receiver, "go to step 2",7);                                                                     
    legacymud::engine::QuestStep* step2 = new legacymud::engine::QuestStep(2, "end", itemType, giver, receiver, "end", 8);  
                                                                           
    quest->addStep(step1);
    quest->addStep(step2); 
    
    // Serialize and Test
    std::string serializedJsonStr = quest->serialize();    
    std::string expectedJsonStr =  "{\"object\":{\"class\":\"QUEST\",\"name\":\"name of quest\",\"description\":\"description of quest\",\"reward_money\":1000,\"reward_item_id\":-1,\"quest_steps\":[{\"step_count\":1,\"quest_step_id\":7},{\"step_count\":2,\"quest_step_id\":8}],\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"quest 4\",\"name of quest\"],\"actions\":[{\"command\":\"LOOK\",\"valid\":true,\"flavor_text\":\"some flavor text\",\"effect\":\"DROP_ALL_ITEMS\",\"aliases\":[{\"verb_alias\":\"vas far\",\"grammar\":{\"direct_object\":\"YES\",\"indirect_object\":\"NO\",\"preposition_map\":[{\"preposition\":\"above\",\"preposition_type\":\"ON\"}]}}]}]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Quest needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area1,-1) );
    EXPECT_TRUE(gom->addObject(area2,-1) );   
    EXPECT_TRUE(gom->addObject(giver,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );    
    EXPECT_TRUE(gom->addObject(receiver,-1) );
    EXPECT_TRUE(gom->addObject(step1,-1) );  
    EXPECT_TRUE(gom->addObject(step2,-1) );     
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr =  "{\"class\":\"QUEST\",\"name\":\"name of quest\",\"description\":\"description of quest\",\"reward_money\":1000,\"reward_item_id\":-1,\"quest_steps\":[{\"step_count\":1,\"quest_step_id\":7},{\"step_count\":2,\"quest_step_id\":8}],\"interactive_noun_data\":{\"id\":4,\"noun_aliases\":[\"quest 4\",\"name of quest\"],\"actions\":[{\"command\":\"LOOK\",\"valid\":true,\"flavor_text\":\"some flavor text\",\"effect\":\"DROP_ALL_ITEMS\",\"aliases\":[{\"verb_alias\":\"vas far\",\"grammar\":{\"direct_object\":\"YES\",\"indirect_object\":\"NO\",\"preposition_map\":[{\"preposition\":\"above\",\"preposition_type\":\"ON\"}]}}]}]}}";

    legacymud::engine::Quest *rebuiltQuest = legacymud::engine::Quest::deserialize(inputJsonStr, gom);
    std::string reserializedJsonStr = rebuiltQuest->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;  
}


/////////////////////////////////////////////////////////////////////////////////
// QuestStepWithRecieverAndGiver - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeQuestStepWithReceiverAndGiver) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Quest.  
                                                                 
    // Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
    // ItemType (weight, rarity, description, name, cost, equipment slot, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD, 2);  
    
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(nullptr, "GiverMan", "a description", 1000, area, 100, 3);                                                                                                                                                            
    legacymud::engine::NonCombatant* receiver = new legacymud::engine::NonCombatant(nullptr, "ReceiverMan", "a description", 10, area, 100, 4);
    
     // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText, id)
    legacymud::engine::QuestStep* step = new legacymud::engine::QuestStep(1, "start", itemType, giver, receiver, "go to step 2",5); 
    
    // Serialize and Test
    std::string serializedJsonStr = step->serialize();   
    std::string expectedJsonStr = "{\"object\":{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":2,\"giver_id\":3,\"receiver_id\":4,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Quest needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );        
    EXPECT_TRUE(gom->addObject(giver,-1) ); 
    EXPECT_TRUE(gom->addObject(receiver,-1) );     
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":2,\"giver_id\":3,\"receiver_id\":4,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}";
    legacymud::engine::QuestStep *rebuiltQuestStep = legacymud::engine::QuestStep::deserialize(inputJsonStr, gom);    
    std::string reserializedJsonStr = rebuiltQuestStep->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;       
}


/////////////////////////////////////////////////////////////////////////////////
// QuestStepNoReciever - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeQuestStepNoReceiver) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Quest.  
                                                                 
    // Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
    // ItemType (weight, rarity, description, name, cost, equipment slot, id)
    legacymud::engine::ItemType* itemType = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD, 2);  
    
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(nullptr, "GiverMan", "a description", 1000, area, 100, 3);                                                                                                                                                            
    
     // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText, id)
    legacymud::engine::QuestStep* step = new legacymud::engine::QuestStep(1, "start", itemType, giver, nullptr, "go to step 2",5); 
    
    // Serialize and Test
    std::string serializedJsonStr = step->serialize();   
    std::string expectedJsonStr =  "{\"object\":{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":2,\"giver_id\":3,\"receiver_id\":-1,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Quest needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );
    EXPECT_TRUE(gom->addObject(itemType,-1) );        
    EXPECT_TRUE(gom->addObject(giver,-1) );     
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr =  "{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":2,\"giver_id\":3,\"receiver_id\":-1,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}";

    legacymud::engine::QuestStep *rebuiltQuestStep = legacymud::engine::QuestStep::deserialize(inputJsonStr, gom);    
    std::string reserializedJsonStr = rebuiltQuestStep->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;       
}


/////////////////////////////////////////////////////////////////////////////////
// QuestStepNoGiverNoItem - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
 TEST(DataManagementTest, SerializeDeserializeQuestStepNoGiverNoItem) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // Create objects needed by Quest.  
                                                                 
    // Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE, 1);   
    
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight, id)
    legacymud::engine::NonCombatant* receiver = new legacymud::engine::NonCombatant(nullptr, "GiverMan", "a description", 1000, area, 100, 3);                                                                                                                                                            
    
     // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText, id)
    legacymud::engine::QuestStep* step = new legacymud::engine::QuestStep(1, "start", nullptr, nullptr, receiver, "go to step 2",5); 
    
    // Serialize and Test
    std::string serializedJsonStr = step->serialize();   
    std::string expectedJsonStr =  "{\"object\":{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":-1,\"giver_id\":-1,\"receiver_id\":3,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
    
    // Add objects to the GameObjectManager that Quest needs for deserialize. 
    EXPECT_TRUE(gom->addObject(area,-1) );       
    EXPECT_TRUE(gom->addObject(receiver,-1) );     
    
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"QUEST_STEP\",\"ordinal_number\":1,\"description\":\"start\",\"fetch_item_type_id\":-1,\"giver_id\":-1,\"receiver_id\":3,\"completion_text\":\"go to step 2\",\"interactive_noun_data\":{\"id\":5,\"noun_aliases\":[\"quest step 5\",\"step 1\"],\"actions\":[]}}";
    legacymud::engine::QuestStep *rebuiltQuestStep = legacymud::engine::QuestStep::deserialize(inputJsonStr, gom);    
    std::string reserializedJsonStr = rebuiltQuestStep->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;       
}


/////////////////////////////////////////////////////////////////////////////////     
// SpecialSkill - Unit test - serialize and deserialize 
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeSpecialSkill) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
    
    // Initialize staticID    
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // SpecialSkill(name, damage, damageType, cost, cooldown, anID);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("special skill name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2, 1);

   // Serialize and Test
    std::string serializedJsonStr = skill->serialize();             
    std::string expectedJsonStr = "{\"object\":{\"class\":\"SPECIAL_SKILL\",\"name\":\"special skill name\",\"damage\":20,\"damage_type\":\"PIERCING\",\"cost\":10,\"cooldown\":2,\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"skill 1\",\"special skill name\"],\"actions\":[]}}}";   
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 
   
    // Add objects to the GameObjectManager that player needs for deserialize. 
    //  none
      
    // Deserialize, re-serialize, and test.
    std::string inputJsonStr = "{\"class\":\"SPECIAL_SKILL\",\"name\":\"special skill name\",\"damage\":20,\"damage_type\":\"PIERCING\",\"cost\":10,\"cooldown\":2,\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"skill 1\",\"special skill name\"],\"actions\":[]}}";
    legacymud::engine::SpecialSkill *rebuiltSpecialSkill = legacymud::engine::SpecialSkill::deserialize(inputJsonStr);
    std::string reserializedJsonStr = rebuiltSpecialSkill->serialize();  
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr );

    // Clean-up. 
    delete gom;
}


/////////////////////////////////////////////////////////////////////////////////
// WeaponType - Unit test - serialize and deserialize
/////////////////////////////////////////////////////////////////////////////////
TEST(DataManagementTest, SerializeDeserializeWeaponType) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager();
      
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0); 
    
    // WeaponType (damage, damage type, range, critMultiplier, weight, rarity, description, name, cost, slotType, id)      
    legacymud::engine::WeaponType* weaponType = new legacymud::engine::WeaponType(25, legacymud::engine::DamageType::CRUSHING, 
                                                                                  legacymud::engine::AreaSize::SMALL, 2, 15,
                                                                                  legacymud::engine::ItemRarity::LEGENDARY,
                                                                                  "a description", "a name", 1500,
                                                                                  legacymud::engine::EquipmentSlot::HANDS, 1);                                                                                   
                                                                                     
    // Serialize and Test
    std::string serializedJsonStr = weaponType->serialize();
    std::string expectedJsonStr = "{\"object\":{\"class\":\"WEAPON_TYPE\",\"damage\":25,\"damage_type\":\"CRUSHING\",\"range\":\"SMALL\",\"crit_multiplier\":2,\"name\":\"a name\",\"weight\":15,\"rarity\":\"LEGENDARY\",\"description\":\"a description\",\"cost\":1500,\"slot_type\":\"HANDS\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}}";
    EXPECT_EQ(expectedJsonStr, serializedJsonStr ); 

    // Add objects to the GameObjectManager that weaponType needs for deserialize. 
    // none

    // Deserialize, re-serialize, and test. 
    std::string inputJsonStr = "{\"class\":\"WEAPON_TYPE\",\"damage\":25,\"damage_type\":\"CRUSHING\",\"range\":\"SMALL\",\"crit_multiplier\":2,\"name\":\"a name\",\"weight\":15,\"rarity\":\"LEGENDARY\",\"description\":\"a description\",\"cost\":1500,\"slot_type\":\"HANDS\",\"interactive_noun_data\":{\"id\":1,\"noun_aliases\":[\"item type 1\",\"a name\"],\"actions\":[]}}";
    legacymud::engine::WeaponType *rebuiltWeaponType = legacymud::engine::WeaponType::deserialize(inputJsonStr); 
    std::string reserializedJsonStr = rebuiltWeaponType->serialize();
    std::string newExpectedJsonStr = "{\"object\":" + inputJsonStr + "}";
    EXPECT_EQ(newExpectedJsonStr, reserializedJsonStr ); 

    // Clean-up. 
    delete gom;            
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Saving and Load 
///////////////////////////////////////////////////////////////////////////////////////////////////   

//  data management save data
TEST(DataManagementTest, SaveAllData) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager(); 
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create the objects
    
    // ArmorType (bonus, resistantTo, weight, rarity, description, name, cost, slotType)
    legacymud::engine::ArmorType* armorType1 = new legacymud::engine::ArmorType(4, legacymud::engine::DamageType::FIRE, 50,
                                                                               legacymud::engine::ItemRarity::COMMON,
                                                                               "a description", "a name", 2500,
                                                                               legacymud::engine::EquipmentSlot::TORSO);  
    
    // WeaponType (damage, damage type, range, critMultiplier, weight, rarity, description, name, cost, slotType)      
    legacymud::engine::WeaponType* weaponType1 = new legacymud::engine::WeaponType(25, legacymud::engine::DamageType::CRUSHING, 
                                                                                  legacymud::engine::AreaSize::SMALL, 2, 15,
                                                                                  legacymud::engine::ItemRarity::LEGENDARY,
                                                                                  "a description", "a name", 1500,
                                                                                  legacymud::engine::EquipmentSlot::HANDS);                                                                                 
    
    // Area (name, short desciption, long description, area size)    
    legacymud::engine::Area* area1 = new legacymud::engine::Area("name of area1", "short description of area1", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE);   
                                                                 
    // ItemType (weight, rarity, description, name, cost, equipment slot)
    legacymud::engine::ItemType* itemType1 = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::RARE, 
                                                                            "description of itemtype", "name of item type", 2500, 
                                                                            legacymud::engine::EquipmentSlot::HEAD);  
                                                                            
    // Item (location, position, name, item type) 
    legacymud::engine::Item* item1 = new legacymud::engine::Item(area1, legacymud::engine::ItemPosition::GROUND, 
                                                                "item name", itemType1 );                                                                     
    
    // Quest  (name, description, reward money, reward item)                                               
    legacymud::engine::Quest* quest1 = new legacymud::engine::Quest("name of quest", "description of quest", 1000, item1);                        
                       
    // NonCombatant(Quest, name, description, money, location, maxinventoryweight)
    legacymud::engine::NonCombatant* giver = new legacymud::engine::NonCombatant(quest1, "GiverMan", "a description", 1000, area1, 100);                                                                                                                                                            
    legacymud::engine::NonCombatant* receiver = new legacymud::engine::NonCombatant(quest1, "ReceiverMan", "a description", 10, area1, 100);
    
    // QuestStep (ordinalNumber, description, anItemType, giver, receiver, completionText)
    legacymud::engine::QuestStep* step1 = new legacymud::engine::QuestStep(1, "start", itemType1, giver, receiver, "go to step 2");                                                                     
    legacymud::engine::QuestStep* step2 = new legacymud::engine::QuestStep(2, "end", itemType1, giver, receiver, "end");  
                                                                           
    quest1->addStep(step1);
    quest1->addStep(step2); 
    
    // Area (name, short desciption, long description, area size) 
    legacymud::engine::Area* area2 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM);   
                                                                   
    // ItemType (weight, rarity, description, name, cost, slotType)
    legacymud::engine::ItemType* itemType2 = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT);
                                                                            
    // Container  (capacity, location, position, name, itemeType)
    legacymud::engine::Container* container1 = new legacymud::engine::Container(100, area2, legacymud::engine::ItemPosition::GROUND, "name of container", itemType2);  

    // SpecialSkill(name, damage, damageType, cost, cooldown);
    legacymud::engine::SpecialSkill* skill1 = new legacymud::engine::SpecialSkill("a name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2);

    // CreatureType(size, difficulty, name, specialSkill, attackBonus, armorBonus, resistantTo, weakTo, healPoints)
    legacymud::engine::CreatureType* creatureType1 = new legacymud::engine::CreatureType(legacymud::engine::CharacterSize::MEDIUM,
                                                                                           legacymud::engine::XPTier::NORMAL,
                                                                                           "a name", skill1, 10, 20, 
                                                                                           legacymud::engine::DamageType::WATER, 
                                                                                           legacymud::engine::DamageType::FIRE, 
                                                                                           10.3); 
    // Area (name, short description, long description, area size)
    legacymud::engine::Area* area3 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE);   
    
    // Area (name, short description, long description, area size)
    legacymud::engine::Area* area4 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::SMALL);     
                                                                       
    //  Creature (creatureType, ambulatory, axHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, int anID);
    legacymud::engine::Creature* creature1 = new legacymud::engine::Creature(creatureType1, true, 100, area3, 50, 70, 80, 90, 
                                                                            "a name", "a description", 1500, area4, 100, 30 );
                                                                                                                                                       
   // Area (name, short desciption, long description, area size) 
    legacymud::engine::Area* area5 = new legacymud::engine::Area("name of location", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM);  
    legacymud::engine::Area* area6 = new legacymud::engine::Area("name of connect area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::MEDIUM);                                                                   
                                                                   
    // ItemType (weight, rarity, description, name, cost, slotType)
    legacymud::engine::ItemType* itemType3 = new legacymud::engine::ItemType(25, legacymud::engine::ItemRarity::COMMON, 
                                                                            "a description", "a name", 2545,
                                                                            legacymud::engine::EquipmentSlot::BELT);

    // Exit(direction, location, connectArea, isConditional, anItemType, description, altDescription);
    legacymud::engine::Exit* exit1 = new legacymud::engine::Exit(legacymud::engine::ExitDirection::WEST, area5, area6, true, itemType3,
                                                                "some description", "an alt decription");       

    // Feature(name, placement, location, isConditional, anItemType, description, altDescription)
    legacymud::engine::Feature* feature1 = new legacymud::engine::Feature("a name", "a placement description", area6, true,
                                                                          itemType3, "a description", "an alt description");


    // SpecialSkill(name, damage, damageType, cost, cooldown);
    legacymud::engine::SpecialSkill* skill2 = new legacymud::engine::SpecialSkill("special skill name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 2);

    // PlayerClass (primaryStat, name, special skill, attackBonus, armorBonus, resistantTo, weakTo, float healPoints);
    legacymud::engine::PlayerClass* playerClass1 = new legacymud::engine::PlayerClass(45, "playerClass name", skill2, 10, 20, 
                                                                                legacymud::engine::DamageType::FIRE,
                                                                                legacymud::engine::DamageType::WATER,
                                                                                35.5); 
   
    // Area (name, short description, long description, area size)
    legacymud::engine::Area* area7 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::LARGE);   
    
    // Area (name, short description, long description, area size, id)
    legacymud::engine::Area* area8 = new legacymud::engine::Area("name of area", "short description of area", "longer description", 
                                                                 legacymud::engine::AreaSize::SMALL);        
   
    //  Player (size, playerClass, username, FD, maxHealth, spawnLocation, maxSpecialPts, dexterity, strength, intelligence, name, description, money, aLocation, maxInventoryWeight, anID);
    legacymud::engine::Player* player1 = new legacymud::engine::Player(legacymud::engine::CharacterSize::MEDIUM, playerClass1, 
                                                                      "a user name", -1, 100, area7, 100, 10,20,30,
                                                                      "a name", "a description", 1000, area8, 120, 31);                                                                          
    
    // Add to player: Action,VerbAlias, NonCombatant Characters, and QuestSteps 
    
    // addAction(command, valid, flavor text, effect) 
    player1->addAction(legacymud::engine::CommandEnum::LOOK, true, "some flavor text", 
                     legacymud::engine::EffectType::DROP_ALL_ITEMS);   
                     
    std::map<std::string, legacymud::parser::PrepositionType> preps; 
    preps["above"] = legacymud::parser::PrepositionType::ON;         
    
    // addVerbAlias(command, alias,direct object support, indirect object support, prepositions) 
    player1->addVerbAlias(legacymud::engine::CommandEnum::LOOK, "vas far", legacymud::parser::Grammar::Support::YES, 
                       legacymud::parser::Grammar::Support::NO, preps);        
    
    // put the ItemType object in the GameObjectManager
    EXPECT_TRUE(gom->addObject(area1,-1) ); 
    EXPECT_TRUE(gom->addObject(area2,-1) ); 
    EXPECT_TRUE(gom->addObject(area3,-1) ); 
    EXPECT_TRUE(gom->addObject(area4,-1) ); 
    EXPECT_TRUE(gom->addObject(area5,-1) ); 
    EXPECT_TRUE(gom->addObject(area6,-1) ); 
    EXPECT_TRUE(gom->addObject(area7,-1) ); 
    EXPECT_TRUE(gom->addObject(area8,-1) ); 
    EXPECT_TRUE(gom->addObject(playerClass1,-1) ); 
    EXPECT_TRUE(gom->addObject(player1,-1) );  
    EXPECT_TRUE(gom->addObject(skill1,-1) ); 
    EXPECT_TRUE(gom->addObject(skill2,-1) );         
    EXPECT_TRUE(gom->addObject(feature1,-1) ); 
    EXPECT_TRUE(gom->addObject(exit1,-1) );
    EXPECT_TRUE(gom->addObject(itemType1,-1) );
    EXPECT_TRUE(gom->addObject(itemType2,-1) );
    EXPECT_TRUE(gom->addObject(itemType3,-1) );
    EXPECT_TRUE(gom->addObject(creatureType1,-1) );
    EXPECT_TRUE(gom->addObject(creature1,-1) );
    EXPECT_TRUE(gom->addObject(container1,-1) );   
    EXPECT_TRUE(gom->addObject(step1,-1) );
    EXPECT_TRUE(gom->addObject(step2,-1) );
    EXPECT_TRUE(gom->addObject(receiver,-1) );
    EXPECT_TRUE(gom->addObject(giver,-1) );
    EXPECT_TRUE(gom->addObject(quest1,-1) ); 
    EXPECT_TRUE(gom->addObject(item1,-1) );
    EXPECT_TRUE(gom->addObject(weaponType1,-1) );
    EXPECT_TRUE(gom->addObject(armorType1,-1) );
    
    // save the game
    EXPECT_TRUE(dm->saveGame("gamedata1.txt", gom) );   

    delete gom;   
}


// Load data and compare with previous saved
TEST(DataManagementTest, LoadAllData) {
    legacymud::engine::GameObjectManager* newGom = new legacymud::engine::GameObjectManager();   

    // load the game followed by saving the game to a different file
    EXPECT_TRUE(dm->loadGame("gamedata1.txt", newGom) );
    EXPECT_TRUE(dm->saveGame("gamedata2.txt", newGom) );

    // load the two files and check for equality
    std::ifstream inFile1("gamedata1.txt");    
    std::string gameData1 = "";
    std::string line;
    if (inFile1.is_open() ) {       
        while (getline(inFile1, line) ){           
            gameData1 += line;
        }
        inFile1.close();    // close the file
    } 
    std::ifstream inFile2("gamedata2.txt");    
    std::string gameData2 = "";
    if (inFile2.is_open() ) {       
        while (getline(inFile2, line) ){           
            gameData2 += line;
        }
        inFile2.close();    // close the file
    } 

    // check for equality
    EXPECT_EQ(gameData1, gameData2 );

    // clean up
    delete newGom;
    remove("gamedata1.txt");   
    remove("gamedata2.txt");   
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Saving and Load 
///////////////////////////////////////////////////////////////////////////////////////////////////   

//  data management creating a starter file
TEST(DataManagementTest, SaveDefaultDataFile) {
    legacymud::engine::GameObjectManager* gom = new legacymud::engine::GameObjectManager(); 
    
    // Initialize staticID
    legacymud::engine::InteractiveNoun::setStaticID(0);
    
    // Create the objects                                                                     
    
    // Starting Area (name, short desciption, long description, area size, id)    
    legacymud::engine::Area* area = new legacymud::engine::Area("default area name", "short description of area", "longer description of area", 
                                                                 legacymud::engine::AreaSize::LARGE, 0);   
                                                                 
    // SpecialSkill(name, damage, damageType, cost, cooldown);
    legacymud::engine::SpecialSkill* skill = new legacymud::engine::SpecialSkill("default special skill name", 20, 
                                                                                legacymud::engine::DamageType::PIERCING, 
                                                                                10, 1);

    // PlayerClass (primaryStat, name, special skill, attackBonus, armorBonus, resistantTo, weakTo, float healPoints);
    legacymud::engine::PlayerClass* playerClass = new legacymud::engine::PlayerClass(45, "playerClass name", skill, 10, 20, 
                                                                                legacymud::engine::DamageType::FIRE,
                                                                                legacymud::engine::DamageType::WATER,
                                                                                35.5, 2); 
        
    
    // put the objects in the GameObjectManager
    EXPECT_TRUE(gom->addObject(area,-1) ); 
    EXPECT_TRUE(gom->addObject(skill,-1) );
    EXPECT_TRUE(gom->addObject(playerClass,-1) );
    
    // save the game
    EXPECT_TRUE(dm->saveGame("default_game_data.txt", gom) );   

    delete gom;   
}

}
