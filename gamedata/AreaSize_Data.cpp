/*!
  \file     AreaSize_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the AreaSize_Data class.
*/


#include "AreaSize_Data.hpp"

namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string AreaSize_Data::enumToString(engine::AreaSize sizeEnum) {

    std::string sizeStr;
    
    switch (sizeEnum) {
    case engine::AreaSize::NONE : sizeStr = "NONE";
        break;
    case engine::AreaSize::SMALL : sizeStr = "SMALL";
        break;
    case engine::AreaSize::MEDIUM : sizeStr = "MEDIUM";
        break;
    case engine::AreaSize::LARGE : sizeStr = "LARGE";
        break;                   
    } 
    
    return sizeStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::AreaSize AreaSize_Data::stringToEnum(std::string sizeStr) {
    legacymud::engine::AreaSize sizeEnum;
    
    if (sizeStr == "NONE")                 
        sizeEnum = engine::AreaSize::NONE;
    else if (sizeStr == "SMALL")                 
        sizeEnum = engine::AreaSize::SMALL;  
    else if (sizeStr == "MEDIUM")                 
        sizeEnum = engine::AreaSize::MEDIUM; 
    else if (sizeStr == "LARGE")                 
        sizeEnum = engine::AreaSize::LARGE;     

    return sizeEnum;
}

    
}}  

    