/*!
  \file     XPTier_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the XPTier_Data class.
*/


#include "XPTier_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string XPTier_Data::enumToString(engine::XPTier tierEnum) {

    std::string tierStr;
    
    switch (tierEnum) {
    case engine::XPTier::TRIVIAL : tierStr = "TRIVIAL";
        break;
    case engine::XPTier::EASY : tierStr = "EASY";
        break;
    case engine::XPTier::NORMAL : tierStr = "NORMAL";
        break;
    case engine::XPTier::HARD : tierStr = "HARD";
        break;         
    case engine::XPTier::LEGENDARY : tierStr = "LEGENDARY";
        break;         
    } 
    
    return tierStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::XPTier XPTier_Data::stringToEnum(std::string tierStr) {

    legacymud::engine::XPTier tierEnum;
    
    if (tierStr == "TRIVIAL")                 
        tierEnum = engine::XPTier::TRIVIAL;
    else if (tierStr == "EASY")                 
        tierEnum = engine::XPTier::EASY;  
    else if (tierStr == "NORMAL")                 
        tierEnum = engine::XPTier::NORMAL; 
    else if (tierStr == "HARD")                 
        tierEnum = engine::XPTier::HARD;     
    else if (tierStr == "LEGENDARY")                 
        tierEnum = engine::XPTier::LEGENDARY;  
    
    return tierEnum;
}

    
}}  

    