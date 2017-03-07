/*!
  \file     ItemRarity_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the ItemRarityData class.
*/


#include "ItemRarity_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string ItemRarity_Data::enumToString(engine::ItemRarity rarityEnum) {

    std::string rarityStr;
    
    switch (rarityEnum) {
    case engine::ItemRarity::COMMON : rarityStr = "COMMON";
        break;
    case engine::ItemRarity::UNCOMMON : rarityStr = "UNCOMMON";
        break;
    case engine::ItemRarity::RARE : rarityStr = "RARE";
        break;
    case engine::ItemRarity::LEGENDARY : rarityStr = "LEGENDARY";
        break;      
    case engine::ItemRarity::QUEST : rarityStr = "QUEST";
        break;              
    } 
    
    return rarityStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::ItemRarity ItemRarity_Data::stringToEnum(std::string rarityStr) {

    legacymud::engine::ItemRarity rarityEnum;
    
    if (rarityStr == "COMMON")                 
        rarityEnum = engine::ItemRarity::COMMON;
    else if (rarityStr == "UNCOMMON")                 
        rarityEnum = engine::ItemRarity::UNCOMMON;  
    else if (rarityStr == "RARE")                 
        rarityEnum = engine::ItemRarity::RARE; 
    else if (rarityStr == "LEGENDARY")                 
        rarityEnum = engine::ItemRarity::LEGENDARY; 
    else if (rarityStr == "QUEST")                 
        rarityEnum = engine::ItemRarity::QUEST;    

    return rarityEnum;
}

    
}}  

    