/*!
  \file     ExitDirection_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the ExitDirection_Data class.
*/


#include "ExitDirection_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string ExitDirection_Data::enumToString(engine::ExitDirection directionEnum) {

    std::string directionStr;
    
    switch (directionEnum) {
    case engine::ExitDirection::NORTH : directionStr = "NORTH";
        break;
    case engine::ExitDirection::SOUTH : directionStr = "SOUTH";
        break;
    case engine::ExitDirection::EAST : directionStr = "EAST";
        break;
    case engine::ExitDirection::WEST : directionStr = "WEST";
        break;         
    case engine::ExitDirection::NORTHEAST : directionStr = "NORTHEAST";
        break;   
    case engine::ExitDirection::NORTHWEST : directionStr = "NORTHWEST";
        break; 
    case engine::ExitDirection::SOUTHEAST : directionStr = "SOUTHEAST";
        break;
    case engine::ExitDirection::SOUTHWEST : directionStr = "SOUTHWEST";
        break;
    case engine::ExitDirection::UP : directionStr = "UP";
        break;   
    case engine::ExitDirection::DOWN : directionStr = "DOWN";
        break;          
    } 
    
    return directionStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::ExitDirection ExitDirection_Data::stringToEnum(std::string directionStr) {

    legacymud::engine::ExitDirection directionEnum;
    
    if (directionStr == "NORTH")                 
        directionEnum = engine::ExitDirection::NORTH;
    else if (directionStr == "SOUTH")                 
        directionEnum = engine::ExitDirection::SOUTH;  
    else if (directionStr == "EAST")                 
        directionEnum = engine::ExitDirection::EAST; 
    else if (directionStr == "WEST")                 
        directionEnum = engine::ExitDirection::WEST;     
    else if (directionStr == "NORTHEAST")                 
        directionEnum = engine::ExitDirection::NORTHEAST;  
    else if (directionStr == "NORTHWEST")                 
        directionEnum = engine::ExitDirection::NORTHWEST;  
    else if (directionStr == "SOUTHEAST")                 
        directionEnum = engine::ExitDirection::SOUTHEAST; 
    else if (directionStr == "SOUTHWEST")                 
        directionEnum = engine::ExitDirection::SOUTHWEST;     
    else if (directionStr == "UP")                 
        directionEnum = engine::ExitDirection::UP; 
    else if (directionStr == "DOWN")                 
        directionEnum = engine::ExitDirection::DOWN; 
    
    return directionEnum;
}

    
}}  

    