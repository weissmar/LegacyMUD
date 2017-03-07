/*!
  \file     PrepositionType_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the PrepositionType_Data class.
*/


#include "PrepositionType_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string PrepositionType_Data::enumToString(parser::PrepositionType prepositionEnum) {

    std::string prepositionStr;

    switch (prepositionEnum) {
        case parser::PrepositionType::NONE : prepositionStr = "NONE";
            break;
        case parser::PrepositionType::TO : prepositionStr = "TO";
            break; 
        case parser::PrepositionType::WITH : prepositionStr = "WITH";
            break; 
        case parser::PrepositionType::OF : prepositionStr = "OF";
            break;
        case parser::PrepositionType::FROM : prepositionStr = "FROM";
            break; 
        case parser::PrepositionType::ON : prepositionStr = "ON";
            break; 
        case parser::PrepositionType::UNDER : prepositionStr = "UNDER";
            break;
        case parser::PrepositionType::IN : prepositionStr = "IN";
            break; 
        case parser::PrepositionType::ALL : prepositionStr = "ALL";
            break;                                      
    }  

    return prepositionStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
parser::PrepositionType PrepositionType_Data::stringToEnum(std::string prepositionStr) {

    parser::PrepositionType prepositionEnum;
    
    if (prepositionStr == "NONE") 
        prepositionEnum = parser::PrepositionType::NONE;    
    else if (prepositionStr == "TO") 
        prepositionEnum = parser::PrepositionType::TO;     
    else if (prepositionStr == "WITH") 
        prepositionEnum = parser::PrepositionType::WITH; 
    else if (prepositionStr == "OF") 
        prepositionEnum = parser::PrepositionType::OF;     
    else if (prepositionStr == "FROM") 
        prepositionEnum = parser::PrepositionType::FROM; 
    else if (prepositionStr == "ON") 
        prepositionEnum = parser::PrepositionType::ON;     
    else if (prepositionStr == "UNDER") 
        prepositionEnum = parser::PrepositionType::UNDER; 
    else if (prepositionStr == "IN") 
        prepositionEnum = parser::PrepositionType::IN;     
    else if (prepositionStr == "ALL") 
        prepositionEnum = parser::PrepositionType::ALL;     
      
    return prepositionEnum;
}

    
}}  

    