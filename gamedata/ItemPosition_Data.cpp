/*!
  \file     ItemPosition_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the ItemPosition_Data class.
*/


#include "ItemPosition_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string ItemPosition_Data::enumToString(engine::ItemPosition positionEnum) {

    std::string positionStr;
    
    switch (positionEnum) {
    case engine::ItemPosition::NONE : positionStr = "NONE";
        break;
    case engine::ItemPosition::GROUND : positionStr = "GROUND";
        break;
    case engine::ItemPosition::INVENTORY : positionStr = "INVENTORY";
        break;
    case engine::ItemPosition::EQUIPPED : positionStr = "EQUIPPED";
        break;         
    case engine::ItemPosition::IN : positionStr = "IN";
        break;   
    case engine::ItemPosition::ON : positionStr = "ON";
        break;  
    case engine::ItemPosition::UNDER : positionStr = "UNDER";
        break;         
    } 
    
    return positionStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::ItemPosition ItemPosition_Data::stringToEnum(std::string positionStr) {

    legacymud::engine::ItemPosition positionEnum;
    
    if (positionStr == "NONE")                 
        positionEnum = engine::ItemPosition::NONE;
    else if (positionStr == "GROUND")                 
        positionEnum = engine::ItemPosition::GROUND;  
    else if (positionStr == "INVENTORY")                 
        positionEnum = engine::ItemPosition::INVENTORY; 
    else if (positionStr == "EQUIPPED")                 
        positionEnum = engine::ItemPosition::EQUIPPED;     
    else if (positionStr == "IN")                 
        positionEnum = engine::ItemPosition::IN;  
    else if (positionStr == "ON")                 
        positionEnum = engine::ItemPosition::ON;  
    else if (positionStr == "UNDER")                 
        positionEnum = engine::ItemPosition::UNDER; 
    
    return positionEnum;
}

    
}}  

    