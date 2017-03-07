/*!
  \file     DamageType_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the DamageType_Data class.
*/


#include "DamageType_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string DamageType_Data::enumToString(engine::DamageType typeEnum) {

    std::string typeStr;
    
    switch (typeEnum) {
    case engine::DamageType::NONE : typeStr = "NONE";
        break;
    case engine::DamageType::CRUSHING : typeStr = "CRUSHING";
        break;
    case engine::DamageType::PIERCING : typeStr = "PIERCING";
        break;
    case engine::DamageType::ELECTRIC : typeStr = "ELECTRIC";
        break;   
    case engine::DamageType::FIRE : typeStr = "FIRE";
        break; 
    case engine::DamageType::WATER : typeStr = "WATER";
        break;
    case engine::DamageType::WIND : typeStr = "WIND";
        break;
    case engine::DamageType::EARTH : typeStr = "EARTH";
        break;   
    case engine::DamageType::HEAL : typeStr = "HEAL";
        break;          
    } 
    
    return typeStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::DamageType DamageType_Data::stringToEnum(std::string typeStr) {

    legacymud::engine::DamageType typeEnum;
    
    if (typeStr == "NONE")                 
        typeEnum = engine::DamageType::NONE;
    else if (typeStr == "CRUSHING")                 
        typeEnum = engine::DamageType::CRUSHING;  
    else if (typeStr == "PIERCING")                 
        typeEnum = engine::DamageType::PIERCING; 
    else if (typeStr == "ELECTRIC")                 
        typeEnum = engine::DamageType::ELECTRIC;     
    else if (typeStr == "FIRE")                 
        typeEnum = engine::DamageType::FIRE;  
    else if (typeStr == "WATER")                 
        typeEnum = engine::DamageType::WATER;  
    else if (typeStr == "WIND")                 
        typeEnum = engine::DamageType::WIND; 
    else if (typeStr == "EARTH")                 
        typeEnum = engine::DamageType::EARTH;     
    else if (typeStr == "HEAL")                 
        typeEnum = engine::DamageType::HEAL; 
    
    return typeEnum;
}

    
}}  

    