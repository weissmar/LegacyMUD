/*!
  \file     DataManager.cpp
  \author   Keith Adkins
  \created  2/20/2017
  \modified 3/7/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the DataManager class.
*/

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <GameObjectManager.hpp>
#include <InteractiveNoun.hpp>
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
#include <ItemPosition.hpp>
#include "DataManager.hpp"

namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    saveGame               
*****************************************************************************/
bool DataManager::saveGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr) {

    // Get all the game objects
    std::map<int, engine::InteractiveNoun*> gameObjectMap;
    gameObjectMap = gameObjectManagerPtr->getAllObjects();
    
    // Create the document object model structure.
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> baseWriter(buffer);   
    baseWriter.StartObject(); 
    baseWriter.String("nextID");
    baseWriter.Int(engine::InteractiveNoun::getStaticID());
    baseWriter.String("AREA");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("ARMOR_TYPE");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("CONTAINER");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("CREATURE");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("CREATURE_TYPE");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("EXIT");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("FEATURE");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("ITEM");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("ITEM_TYPE");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("NON_COMBATANT");
    baseWriter.StartArray();
    baseWriter.EndArray();    
    baseWriter.String("PLAYER");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("PLAYER_CLASS");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("QUEST");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("QUEST_STEP");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("SPECIAL_SKILL");
    baseWriter.StartArray();
    baseWriter.EndArray();
    baseWriter.String("WEAPON_TYPE");
    baseWriter.StartArray();
    baseWriter.EndArray(); 
    baseWriter.EndObject();
    
    // parse into a document object module
    rapidjson::Document allDataDoc;
    allDataDoc.Parse(buffer.GetString());
    
    // Iterate through all the game objects serializing each and adding them to the all data document
    for (auto object = gameObjectMap.begin(); object != gameObjectMap.end(); object++ ) {
        
        // serialize an object
        std::string jsonStr = object->second->serialize();
    
        // convert string to an object document so it can be pushed into the all data document.  
        rapidjson::Document objectDoc;
        objectDoc.Parse(jsonStr.c_str());
               
        // Add the object to its array.
        switch (object->second->getObjectType() ) {
            case engine::ObjectType::AREA :
                allDataDoc["AREA"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::ARMOR_TYPE :
                allDataDoc["ARMOR_TYPE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break;  
            case engine::ObjectType::CONTAINER :
                allDataDoc["CONTAINER"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::CREATURE :
                allDataDoc["CREATURE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::CREATURE_TYPE :
                allDataDoc["CREATURE_TYPE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::EXIT :
                allDataDoc["EXIT"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::FEATURE :
                allDataDoc["FEATURE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::ITEM :
                allDataDoc["ITEM"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::ITEM_TYPE :
                allDataDoc["ITEM_TYPE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::NON_COMBATANT :
                allDataDoc["NON_COMBATANT"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::PLAYER :
                allDataDoc["PLAYER"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::PLAYER_CLASS :
                allDataDoc["PLAYER_CLASS"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::QUEST :
                allDataDoc["QUEST"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::QUEST_STEP :
                allDataDoc["QUEST_STEP"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::SPECIAL_SKILL :
                allDataDoc["SPECIAL_SKILL"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break; 
            case engine::ObjectType::WEAPON_TYPE :
                allDataDoc["WEAPON_TYPE"].PushBack(rapidjson::Value(objectDoc["object"], allDataDoc.GetAllocator()), allDataDoc.GetAllocator()); 
                break;   
            case engine::ObjectType::NONE : 
                break;                
        }           
    } 

    // Write all the all data document to a buffer for output.
    buffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> outWriter(buffer);  
    allDataDoc.Accept(outWriter);
    
    // Open the file and save the data to disk. 
    std::ofstream outFile(filename); 
    if (outFile.is_open() ) {      
        outFile << buffer.GetString();
        outFile.close();    // close file
        return true;
    }
    else 
        return false;       // error opening file
      
}


/******************************************************************************
* Function:    loadGame                 
*****************************************************************************/
bool DataManager::loadGame(std::string filename, legacymud::engine::GameObjectManager* gameObjectManagerPtr) {
    
    // load the data from disk
    std::ifstream inFile(filename);    // input stream for account file
    std::string gameData = "";
    std::string line;
    
    if (inFile.is_open() ) {       
        while (getline(inFile, line) ){           
            gameData += line;
        }
        inFile.close();     // close the file
    }
    else 
        return false;       // error opening file  
    
    // parse the data to recover each objects json string
    rapidjson::Document dom;
    dom.Parse(gameData.c_str()); 
    
    // Deserialize all AREA objects
    rapidjson::StringBuffer inBuffer;
    for (auto& jsonObject : dom["AREA"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Area *rebuiltArea = engine::Area::deserialize(inBuffer.GetString());
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltArea,-1);
    }   
    
    // Deserialize all ITEM_TYPE objects
    for (auto& jsonObject : dom["ITEM_TYPE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::ItemType *rebuiltItemType = engine::ItemType::deserialize(inBuffer.GetString());
         
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltItemType,-1);
    }
    
    // Deserialize all EXIT objects
    for (auto& jsonObject : dom["EXIT"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Exit *rebuiltExit = engine::Exit::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add this exit to it's location area.
        rebuiltExit->getLocation()->addExit(rebuiltExit);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltExit,-1);
    }   

    // Deserialize all FEATURE objects
    for (auto& jsonObject : dom["FEATURE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Feature *rebuiltFeature = engine::Feature::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add this feature to it's location area.
        rebuiltFeature->getLocation()->addFeature(rebuiltFeature);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltFeature,-1);
    }  

    // Deserialize all SPECIAL_SKILL objects
    for (auto& jsonObject : dom["SPECIAL_SKILL"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::SpecialSkill *rebuiltSpecialSkill = engine::SpecialSkill::deserialize(inBuffer.GetString());
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltSpecialSkill,-1);
    } 

    // Deserialize all PLAYER_CLASS objects
    for (auto& jsonObject : dom["PLAYER_CLASS"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::PlayerClass *rebuiltPlayerClass = engine::PlayerClass::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltPlayerClass,-1);
    } 

    // Deserialize all PLAYER objects.  Note, player quest data is deserialized after quests are loaded.
    for (auto& jsonObject : dom["PLAYER"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Player *rebuiltPlayer = engine::Player::deserialize(inBuffer.GetString(), gameObjectManagerPtr);

        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltPlayer,-1);      
    }  

    // Deserialize all CREATURE_TYPE objects
    for (auto& jsonObject : dom["CREATURE_TYPE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::CreatureType *rebuiltCreatureType = engine::CreatureType::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltCreatureType,-1);      
    }     

    // Deserialize all CREATURE objects
    for (auto& jsonObject : dom["CREATURE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Creature *rebuiltCreature = engine::Creature::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add this creature to it's location area.
        rebuiltCreature->getLocation()->addCharacter(rebuiltCreature);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltCreature,-1);      
    } 

    // Deserialize all WEAPON_TYPE objects
    for (auto& jsonObject : dom["WEAPON_TYPE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::WeaponType *rebuiltWeaponType = engine::WeaponType::deserialize(inBuffer.GetString());
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltWeaponType,-1);      
    }    

    // Deserialize all ARMOR_TYPE objects
    for (auto& jsonObject : dom["ARMOR_TYPE"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::ArmorType *rebuiltArmorType = engine::ArmorType::deserialize(inBuffer.GetString());
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltArmorType,-1);      
    }  
    
    // Deserialize all NON_COMBATANT objects
    for (auto& jsonObject : dom["NON_COMBATANT"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::NonCombatant *rebuiltNonCombatant = engine::NonCombatant::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add this character to it's location area.
        rebuiltNonCombatant->getLocation()->addCharacter(rebuiltNonCombatant);        
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltNonCombatant,-1);      
    }         
    
    // Deserialize all CONTAINER objects.
    bool isComplete = false;
    int countOfCompleteObjects = 0;
    int sizeOfArray = dom["CONTAINER"].GetArray().Size();
    // Loop needed to account for containers in containers
    while (!isComplete) {       
        for (auto& jsonObject : dom["CONTAINER"].GetArray()) {          

           // Write object to a buffer and convert to a string for deserialization
            inBuffer.Clear();
            rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
            jsonObject.Accept(inWriter);    

            // Deserialize the data and rebuild it's object.
            engine::Container *rebuiltContainer = engine::Container::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
            
            if (rebuiltContainer != nullptr) {  
                // Add this container to it's location
                switch (rebuiltContainer->getPosition()) {
                case engine::ItemPosition::NONE :
                    break;
                case engine::ItemPosition::GROUND :     // adds container to an area            
                    static_cast<engine::Area*>(rebuiltContainer->getLocation())->addItem(rebuiltContainer); 
                    break;
                case engine::ItemPosition::INVENTORY :  // adds container to a character             
                    static_cast<engine::Character*>(rebuiltContainer->getLocation())->addToInventory(rebuiltContainer);  
                    break;
                case engine::ItemPosition::EQUIPPED :   // adds container to a character                                        
                    static_cast<engine::Character*>(rebuiltContainer->getLocation())->equipItem(rebuiltContainer);       
                    break;    
                case engine::ItemPosition::IN :         // adds container to a container
                    static_cast<engine::Container*>(rebuiltContainer->getLocation())->place(rebuiltContainer, engine::ItemPosition::IN); 
                    break; 
                case engine::ItemPosition::ON :         // adds container to a container                        
                    static_cast<engine::Container*>(rebuiltContainer->getLocation())->place(rebuiltContainer, engine::ItemPosition::ON); 
                    break; 
                case engine::ItemPosition::UNDER :      // adds container to a container                  
                    static_cast<engine::Container*>(rebuiltContainer->getLocation())->place(rebuiltContainer, engine::ItemPosition::UNDER); 
                    break;             
                } 
                
                // Add the object to the GameObjectManager.
                gameObjectManagerPtr->addObject(rebuiltContainer,-1);  
                countOfCompleteObjects ++;  
                if (countOfCompleteObjects == sizeOfArray)
                    isComplete = true;
            }   
        } 
    }
   
    // Deserialize all ITEM objects
    for (auto& jsonObject : dom["ITEM"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Item *rebuiltItem = engine::Item::deserialize(inBuffer.GetString(), gameObjectManagerPtr);

        // Add this item to it's location
        switch (rebuiltItem->getPosition()) {
        case engine::ItemPosition::NONE :
            break;
        case engine::ItemPosition::GROUND :     // adds item to an area            
            static_cast<engine::Area*>(rebuiltItem->getLocation())->addItem(rebuiltItem);         
            break;
        case engine::ItemPosition::INVENTORY :  // adds item to a character             
            static_cast<engine::Character*>(rebuiltItem->getLocation())->addToInventory(rebuiltItem);  
            break;
        case engine::ItemPosition::EQUIPPED :   // adds item to a character                                        
            static_cast<engine::Character*>(rebuiltItem->getLocation())->equipItem(rebuiltItem);       
            break;    
        case engine::ItemPosition::IN :         // adds item to a container
            static_cast<engine::Container*>(rebuiltItem->getLocation())->place(rebuiltItem, engine::ItemPosition::IN); 
            break; 
        case engine::ItemPosition::ON :         // adds item to a container                        
            static_cast<engine::Container*>(rebuiltItem->getLocation())->place(rebuiltItem, engine::ItemPosition::ON); 
            break; 
        case engine::ItemPosition::UNDER :      // adds item to a container                  
            static_cast<engine::Container*>(rebuiltItem->getLocation())->place(rebuiltItem, engine::ItemPosition::UNDER); 
            break;             
        } 
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltItem,-1);      
    }     
    
    // Deserialize all QUEST_STEP objects
    for (auto& jsonObject : dom["QUEST_STEP"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::QuestStep *rebuiltQuestStep = engine::QuestStep::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltQuestStep,-1);      
    }  
    
    // Deserialize all QUEST objects
    for (auto& jsonObject : dom["QUEST"].GetArray()) {          

       // Write object to a buffer and convert to a string for deserialization
        inBuffer.Clear();
        rapidjson::Writer<rapidjson::StringBuffer> inWriter(inBuffer);  
        jsonObject.Accept(inWriter);    

        // Deserialize the data and rebuild it's object.
        engine::Quest *rebuiltQuest = engine::Quest::deserialize(inBuffer.GetString(), gameObjectManagerPtr);
        
        // Add the object to the GameObjectManager.
        gameObjectManagerPtr->addObject(rebuiltQuest,-1);      
    }  

    // Load all PLAYER objects quest data. Note this needs to follow the quest load. 
    for (auto& jsonObject : dom["PLAYER"].GetArray()) { 
        engine::Player* thisPlayer;
        thisPlayer = static_cast<engine::Player*>(gameObjectManagerPtr->getPointer(jsonObject["interactive_noun_data"]["id"].GetInt()));
    
        for (auto& aQuest : jsonObject["quest_list"].GetArray()) {  
            thisPlayer->addOrUpdateQuest(static_cast<engine::Quest*>(gameObjectManagerPtr->getPointer(aQuest["quest_id"].GetInt())),
                                        aQuest["step"].GetInt(),
                                        aQuest["complete"].GetBool());
        }        
    }
    
    // Load all NON_COMBATANT objects quest data. Note this needs to follow the quest load. 
    for (auto& jsonObject : dom["NON_COMBATANT"].GetArray()) { 
        engine::NonCombatant* thisNonCombatant;
        thisNonCombatant = static_cast<engine::NonCombatant*>(gameObjectManagerPtr->getPointer(jsonObject["interactive_noun_data"]["id"].GetInt()));
        // set the quest of each non combatant
        if (jsonObject["quest_id"].GetInt() == -1)
            thisNonCombatant->setQuest(nullptr);
        else
            thisNonCombatant->setQuest(static_cast<engine::Quest*>(gameObjectManagerPtr->getPointer(jsonObject["quest_id"].GetInt())));         
    }
    
    // after all objects are created, reset the staticId
    engine::InteractiveNoun::setStaticID(dom["nextID"].GetInt());

    return true;   
}


}}  

    