/*!
  \file     CharacterSize_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the CharacterSize_Data class.
*/


#include "CharacterSize_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string CharacterSize_Data::enumToString(engine::CharacterSize sizeEnum) {

    std::string sizeStr;
    
    switch (sizeEnum) {
    case engine::CharacterSize::TINY : sizeStr = "TINY";
        break;
    case engine::CharacterSize::SMALL : sizeStr = "SMALL";
        break;
    case engine::CharacterSize::MEDIUM : sizeStr = "MEDIUM";
        break;
    case engine::CharacterSize::LARGE : sizeStr = "LARGE";
        break;   
    case engine::CharacterSize::HUGE : sizeStr = "HUGE";
        break;          
    } 
    
    return sizeStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::CharacterSize CharacterSize_Data::stringToEnum(std::string sizeStr) {

    legacymud::engine::CharacterSize sizeEnum;
    
    if (sizeStr == "TINY")                 
        sizeEnum = engine::CharacterSize::TINY;
    else if (sizeStr == "SMALL")                 
        sizeEnum = engine::CharacterSize::SMALL;  
    else if (sizeStr == "MEDIUM")                 
        sizeEnum = engine::CharacterSize::MEDIUM; 
    else if (sizeStr == "LARGE")                 
        sizeEnum = engine::CharacterSize::LARGE;     
    else if (sizeStr == "HUGE")                 
        sizeEnum = engine::CharacterSize::HUGE;  
    
    return sizeEnum;
}

    
}}  

    