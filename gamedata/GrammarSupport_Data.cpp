/*!
  \file     GrammarSupport_Data.cpp
  \author   Keith Adkins
  \created  3/3/2017
  \modified 3/3/2017
  \course   CS467, Winter 2017
 
  \details  Implementation file for the GrammarSupport_Data class.
*/


#include "GrammarSupport_Data.hpp"


namespace legacymud { namespace gamedata {


/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
std::string GrammarSupport_Data::enumToString(parser::Grammar::Support supportEnum) {

    std::string supportStr;

    switch (supportEnum) {
        case parser::Grammar::Support::NO : supportStr = "NO";
            break;
        case parser::Grammar::Support::YES : supportStr = "YES";
            break; 
        case parser::Grammar::Support::TEXT : supportStr = "TEXT";
            break;             
                          
    }  

    return supportStr;   
}

/******************************************************************************
* Function:    enumToString               
*****************************************************************************/
parser::Grammar::Support GrammarSupport_Data::stringToEnum(std::string supportStr) {

    parser::Grammar::Support supportEnum;
    
    if (supportStr == "NO") 
        supportEnum = parser::Grammar::Support::NO;    
    else if (supportStr == "YES") 
        supportEnum = parser::Grammar::Support::YES;     
    else if (supportStr == "TEXT") 
        supportEnum = parser::Grammar::Support::TEXT;  
    
    
    return supportEnum;
}

    
}}  

    