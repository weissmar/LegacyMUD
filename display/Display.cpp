/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/30/2017
 * Last Modified: 1/30/2017
 * Course:        CS467, Winter 2017
 * Filename:      Display.cpp
 *
 * Overview:
 *     Implementation file for the Display class.
 ************************************************************************/

#include "Display.h"
#include <string>

namespace legacymud { namespace display {


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Preconditions:    none                        
* Return:           Returns a string with the an ANSI color code. 
*                   An ANSI code that resets all attributes is attached to the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30m";      // black    
            break; 
        case RED: 
            outStr = "\033\[31m";      // red    
            break;
        case GREEN: 
            outStr = "\033\[32m";      // green     
            break;
        case YELLOW:
            outStr = "\033\[33m";       // yellow     
            break;
        case BLUE:
            outStr = "\033\[34m";       // blue     
            break;
        case MAGENTA:
            outStr = "\033\[35m";       // magenta     
            break;
        case CYAN:
            outStr = "\033\[36m";       // cyan     
            break; 
        case WHITE:
            outStr = "\033\[37m";       // cyan     
            break;            
    }
    
    outStr += inStr;            // add the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Input:            Display::Style attribute    text attribute 
* Preconditions:    none                        
* Return:           Returns a string with the an ANSI color code and a style attribute.
*                   An ANSI code that resets all attributes is attached to the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30";        // black        
            break; 
        case RED: 
            outStr = "\033\[31";        // red    
            break;
        case GREEN: 
            outStr = "\033\[32";        // green  
            break;
        case YELLOW:
            outStr = "\033\[33";        // yellow    
            break;
        case BLUE:
            outStr = "\033\[34";        // blue     
            break;
        case MAGENTA:
            outStr = "\033\[35";        // magenta   
            break;
        case CYAN:
            outStr = "\033\[36";        // cyan         
            break; 
        case WHITE:                     // white
            outStr = "\033\[37";           
            break;            
    }
    
    if (attribute == BRIGHT) {
        outStr += ";1";                 // bright
    }
    
    else if (attribute == UNDERSCORE) {
        outStr += ";4";                 // underscore
    }
    
    outStr += 'm';              // end the ANSI escape sequence
    outStr += inStr;            // add the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Input:            Display::Style attribute1    text attribute 
* Input:            Display::Style attribute2    text attribute 
* Preconditions:    none                        
* Return:           Returns a string with the an ANSI color code and two style attributes.
*                   An ANSI code that resets all attributes is attached to the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute1, Display::Style attribute2) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30";         // black 
            break; 
        case RED: 
            outStr = "\033\[31";         // red       
            break;
        case GREEN: 
            outStr = "\033\[32";         // green     
            break;
        case YELLOW:
            outStr = "\033\[33";         // yellow       
            break;
        case BLUE:
            outStr = "\033\[34";         // yellow       
            break;
        case MAGENTA:
            outStr = "\033\[35";         // magenta    
            break;
        case CYAN:
            outStr = "\033\[36";         // cyan     
            break; 
        case WHITE:
            outStr = "\033\[37";         // white  
            break;            
    }
    
    if (attribute1 == BRIGHT || attribute2 == BRIGHT) {             // bright
        outStr += ";1";    
    }
    
    if (attribute1 == UNDERSCORE || attribute2 == UNDERSCORE) {     // underscore
        outStr += ";4";    
    }
    
    outStr += 'm';              // end the ANSI escape sequence
    outStr += inStr;            // add the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Input:            Display::Color backGround   background text color 
* Preconditions:    none                        
* Return:           Returns a string with a foreground and background color code.
*                   An ANSI code that resets all attributes is attached to the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30";      // black    
            break;
        case RED: 
            outStr = "\033\[31";      // red    
            break;
        case GREEN: 
            outStr = "\033\[32";      // green     
            break;
        case YELLOW:
            outStr = "\033\[33";       // yellow     
            break;
        case BLUE:
            outStr = "\033\[34";       // blue     
            break;
        case MAGENTA:
            outStr = "\033\[35";       // magenta     
            break;
        case CYAN:
            outStr = "\033\[36";       // cyan     
            break;
        case WHITE:
            outStr = "\033\[37";       // white    
            break;                               
    }
    
    switch (backGround) {
        case BLACK: 
            outStr += ";40";      // black    
            break;
        case RED: 
            outStr += ";41";      // red    
            break;
        case GREEN: 
            outStr += ";42";      // green     
            break;
        case YELLOW:
            outStr += ";43";       // yellow     
            break;
        case BLUE:
            outStr += ";44";       // blue     
            break;
        case MAGENTA:
            outStr += ";45";       // magenta     
            break;
        case CYAN:
            outStr += ";46";       // cyan     
            break;  
        case 7:
            outStr += ";47";       // cyan     
            break;                                
    }    
    
    outStr += 'm';              // end the ANSI escape sequence
    outStr += inStr;            // insert the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Input:            Display::Color backGround   background text color 
* Input:            Display::Style attribute    text attribute 
* Preconditions:    none                        
* Return:           Returns a string with a foreground and background color code and a
*                   style attribute. An ANSI code that resets all attributes is attached to 
*                   the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30";      // black    
            break;
        case RED: 
            outStr = "\033\[31";      // red    
            break;
        case GREEN: 
            outStr = "\033\[32";      // green     
            break;
        case YELLOW:
            outStr = "\033\[33";       // yellow     
            break;
        case BLUE:
            outStr = "\033\[34";       // blue     
            break;
        case MAGENTA:
            outStr = "\033\[35";       // magenta     
            break;
        case CYAN:
            outStr = "\033\[36";       // cyan     
            break;
        case WHITE:
            outStr = "\033\[37";       // white    
            break;                               
    }
    
    switch (backGround) {
        case BLACK: 
            outStr += ";40";      // black    
            break;
        case RED: 
            outStr += ";41";      // red    
            break;
        case GREEN: 
            outStr += ";42";      // green     
            break;
        case YELLOW:
            outStr += ";43";       // yellow     
            break;
        case BLUE:
            outStr += ";44";       // blue     
            break;
        case MAGENTA:
            outStr += ";45";       // magenta     
            break;
        case CYAN:
            outStr += ";46";       // cyan     
            break;  
        case 7:
            outStr += ";47";       // cyan     
            break;                                
    }    
    
    if (attribute == BRIGHT) {              // bright
        outStr += ";1";    
    }
    
    else if (attribute == UNDERSCORE) {     // underscore
        outStr += ";4";    
    }
    
    outStr += 'm';              // end the ANSI escape sequence
    outStr += inStr;            // insert the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}


/******************************************************************************
* Function:    addStyle
* Description: This function attaches ANSI color codes to a string.
*
* Input:            std::string inStr           input text
* Input:            Display::Color foreGround   text color             
* Input:            Display::Color backGround   background text color 
* Input:            Display::Style attribute    text attribute 
* Preconditions:    none                        
* Return:           Returns a string with a foreground and background color code and two
*                   style attributes. An ANSI code that resets all attributes is attached to 
*                   the end of the string.   
*****************************************************************************/
std::string Display::addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute1, Display::Style attribute2) {   
    
    std::string outStr = "";
    
    switch (foreGround) {
        case BLACK: 
            outStr = "\033\[30";      // black    
            break;
        case RED: 
            outStr = "\033\[31";      // red    
            break;
        case GREEN: 
            outStr = "\033\[32";      // green     
            break;
        case YELLOW:
            outStr = "\033\[33";       // yellow     
            break;
        case BLUE:
            outStr = "\033\[34";       // blue     
            break;
        case MAGENTA:
            outStr = "\033\[35";       // magenta     
            break;
        case CYAN:
            outStr = "\033\[36";       // cyan     
            break;
        case WHITE:
            outStr = "\033\[37";       // white    
            break;                               
    }
    
    switch (backGround) {
        case BLACK: 
            outStr += ";40";      // black    
            break;
        case RED: 
            outStr += ";41";      // red    
            break;
        case GREEN: 
            outStr += ";42";      // green     
            break;
        case YELLOW:
            outStr += ";43";       // yellow     
            break;
        case BLUE:
            outStr += ";44";       // blue     
            break;
        case MAGENTA:
            outStr += ";45";       // magenta     
            break;
        case CYAN:
            outStr += ";46";       // cyan     
            break;  
        case 7:
            outStr += ";47";       // cyan     
            break;                                
    }    
    
    if (attribute1 == BRIGHT || attribute2 == BRIGHT) {
        outStr += ";1";                                     // bright
    }
    
    if (attribute1 == UNDERSCORE || attribute2 == UNDERSCORE) {
        outStr += ";4";                                     // underscore
    }
    
    outStr += 'm';              // end the ANSI escape sequence
    outStr += inStr;            // insert the text
    outStr += "\033\[0m";       // reset all attributes
    
    return outStr;
}

}}