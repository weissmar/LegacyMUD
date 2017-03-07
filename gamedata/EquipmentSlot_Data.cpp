/*!
  \file     EquipmentSlot_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the EquipmentSlot_Data class.
*/


#include "EquipmentSlot_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string EquipmentSlot_Data::enumToString(engine::EquipmentSlot slotEnum) {

    std::string slotStr;

    switch (slotEnum) {
        case engine::EquipmentSlot::NONE : slotStr = "NONE";
            break;
        case engine::EquipmentSlot::HEAD : slotStr = "HEAD";
            break;    
        case engine::EquipmentSlot::SHOULDERS : slotStr = "SHOULDERS";
            break; 
        case engine::EquipmentSlot::NECK : slotStr = "NECK";
            break;
        case engine::EquipmentSlot::TORSO : slotStr = "TORSO";
            break;    
        case engine::EquipmentSlot::BELT : slotStr = "BELT";
            break; 
        case engine::EquipmentSlot::LEGS : slotStr = "LEGS";
            break;
        case engine::EquipmentSlot::ARMS : slotStr = "ARMS";
            break;    
        case engine::EquipmentSlot::HANDS : slotStr = "HANDS";
            break; 
        case engine::EquipmentSlot::RIGHT_HAND : slotStr = "RIGHT_HAND";
            break;
        case engine::EquipmentSlot::LEFT_HAND : slotStr = "LEFT_HAND";
            break;    
        case engine::EquipmentSlot::FEET : slotStr = "FEET";
            break; 
        case engine::EquipmentSlot::RIGHT_RING : slotStr = "RIGHT_RING";
            break; 
        case engine::EquipmentSlot::LEFT_RING : slotStr = "LEFT_RING";
            break;                 
    }  

    return slotStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
engine::EquipmentSlot EquipmentSlot_Data::stringToEnum(std::string slotStr) {

    legacymud::engine::EquipmentSlot slotEnum;
    
    if (slotStr == "NONE") 
        slotEnum = engine::EquipmentSlot::NONE;    
    else if (slotStr == "HEAD") 
        slotEnum = engine::EquipmentSlot::HEAD;     
    else if (slotStr == "SHOULDERS") 
        slotEnum = engine::EquipmentSlot::SHOULDERS; 
    else if (slotStr == "NECK") 
        slotEnum = engine::EquipmentSlot::NECK; 
    else if (slotStr == "TORSO") 
        slotEnum = engine::EquipmentSlot::TORSO; 
    else if (slotStr == "BELT") 
        slotEnum = engine::EquipmentSlot::BELT; 
    else if (slotStr == "LEGS") 
        slotEnum = engine::EquipmentSlot::LEGS; 
    else if (slotStr == "ARMS") 
        slotEnum = engine::EquipmentSlot::ARMS; 
    else if (slotStr == "HANDS") 
        slotEnum = engine::EquipmentSlot::HANDS; 
    else if (slotStr == "RIGHT_HAND") 
        slotEnum = engine::EquipmentSlot::RIGHT_HAND; 
    else if (slotStr == "LEFT_HAND") 
        slotEnum = engine::EquipmentSlot::LEFT_HAND; 
    else if (slotStr == "FEET") 
        slotEnum = engine::EquipmentSlot::FEET; 
    else if (slotStr == "RIGHT_RING") 
        slotEnum = engine::EquipmentSlot::RIGHT_RING; 
    else if (slotStr == "LEFT_RING") 
        slotEnum = engine::EquipmentSlot::LEFT_RING;     

    return slotEnum;
}

    
}}  

    