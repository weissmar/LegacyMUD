/*!
  \file     EffectType_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the EffectType_Data class.
*/


#include "EffectType_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string EffectType_Data::enumToString(engine::EffectType effectEnum) {

    std::string effectStr;

    switch (effectEnum) {
        case engine::EffectType::NONE : effectStr = "NONE";
            break;
        case engine::EffectType::DAMAGE : effectStr = "DAMAGE";
            break; 
        case engine::EffectType::FALL : effectStr = "FALL";
            break;
        case engine::EffectType::LONG_FALL : effectStr = "LONG_FALL";
            break;
        case engine::EffectType::LOST_ITEM : effectStr = "LOST_ITEM";
            break;
        case engine::EffectType::DROP_ALL_ITEMS : effectStr = "DROP_ALL_ITEMS";
            break;
        case engine::EffectType::GAIN_MONEY : effectStr = "GAIN_MONEY";
            break;
        case engine::EffectType::LOSE_MONEY : effectStr = "LOSE_MONEY";
            break;
        case engine::EffectType::HEAL : effectStr = "HEAL";
            break;
        case engine::EffectType::GET_SPECIAL_POINTS : effectStr = "GET_SPECIAL_POINTS";
            break;                            
    }  

    return effectStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::EffectType EffectType_Data::stringToEnum(std::string effectStr) {

    legacymud::engine::EffectType effectEnum;
    
    if (effectStr == "NONE") 
        effectEnum = engine::EffectType::NONE;    
    else if (effectStr == "DAMAGE") 
        effectEnum = engine::EffectType::DAMAGE;     
    else if (effectStr == "FALL") 
        effectEnum = engine::EffectType::FALL; 
    else if (effectStr == "LONG_FALL") 
        effectEnum = engine::EffectType::LONG_FALL;     
    else if (effectStr == "LOST_ITEM") 
        effectEnum = engine::EffectType::LOST_ITEM; 
    else if (effectStr == "DROP_ALL_ITEMS") 
        effectEnum = engine::EffectType::DROP_ALL_ITEMS;     
    else if (effectStr == "GAIN_MONEY") 
        effectEnum = engine::EffectType::GAIN_MONEY; 
    else if (effectStr == "LOSE_MONEY") 
        effectEnum = engine::EffectType::LOSE_MONEY;     
    else if (effectStr == "HEAL") 
        effectEnum = engine::EffectType::HEAL;   
    else if (effectStr == "GET_SPECIAL_POINTS") 
        effectEnum = engine::EffectType::GET_SPECIAL_POINTS;   
    
    return effectEnum;
}

    
}}  

    