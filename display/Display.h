/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 1/31/2017
 * Course:        CS467, Winter 2017
 * Filename:      Display.h
 *
 * Overview:
 *     Declaration file for the Display class.
 ************************************************************************/
 
#ifndef LEGACYMUD_DISPLAY_H
#define LEGACYMUD_DISPLAY_H

#include <string>

namespace legacymud { namespace display {

/*!
  \brief A class for attaching ANSI style codes to strings.
*/
class Display {
    public:
    
        // Available text foreground and background colors.
        enum Color {
            BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
        };
        
        // Available style attributes.
        enum Style {
            BRIGHT, UNDERSCORE
        };

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
        std::string addStyle(std::string inStr, Display::Color foreGround);
        
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
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute); 
    
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
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute1, Display::Style attribute2); 
        
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
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround);

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
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute);        
        
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
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute1, Display::Style attribute2); 
        
};

}}

#endif