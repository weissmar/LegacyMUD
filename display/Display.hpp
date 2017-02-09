/*!
  \file     Display.hpp
  \author   Keith Adkins
  \created  1/31/2017
  \modified 2/08/2017
  \course   CS467, Winter 2017
 
  \details  Declaration file for the Display class.
*/

 
#ifndef LEGACYMUD_DISPLAY_HPP
#define LEGACYMUD_DISPLAY_HPP

#include <string>

namespace legacymud { namespace display {


/*!
  \brief Attaches ANSI codes to a string.
  
  This class attaches ANSI codes to a string.  Supported are ANSI codes for color, brightness,
  and underscore.
*/
class Display {
    public:
    
        /* Available text foreground and background colors. */
        enum Color {
            BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE
        };
        
        /* Available style attributes. */
        enum Style {
            BRIGHT, UNDERSCORE
        };
               
        /*!
          \brief Adds an ANSI foregound color code to a string.
          
          This function adds an ANSI foregound color code to a string.
          
          \param[in]  inStr         text to add ANSI code to
          \param[in]  foreGround    foreground color to add to text
          \pre none
          \post Returns a string with an ANSI color code.  The end of the string includes
                an ANSI code the resets all ANSI attributes.
        */
        std::string addStyle(std::string inStr, Display::Color foreGround);

        /*!
          \brief Adds an ANSI foregound color code and an ANSI style attribiute to a string.
          
          This function adds an ANSI foregound color code and an ANSI style attribute to a string.
          
          \param[in]  inStr         text to add ANSI codes to
          \param[in]  foreGround    foreground color to add to text
          \param[in]  attribute     style attribute to add to text
          \pre none
          \post Returns a string with an ANSI color code and an ANSI style code.  The end of the 
                string includes an ANSI code the resets all ANSI attributes.
        */        
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute); 

        /*!
          \brief Adds an ANSI foregound color code and two ANSI style attribiutes to a string.
          
          This function adds an ANSI foregound color code and two ANSI style attributes to a string.
          
          \param[in]  inStr         text to add ANSI codes to
          \param[in]  foreGround    foreground color to add to text
          \param[in]  attribute1    style attribute to add to text
          \param[in]  attribute2    style attribute to add to text
          \pre none
          \post Returns a string with an ANSI color code and ANSI style codes.  The end of the 
                string includes an ANSI code the resets all ANSI attributes.
        */         
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Style attribute1, Display::Style attribute2); 

        /*!
          \brief Adds ANSI foregound and background color codes to a string.
          
          This function adds ANSI foregound and background color codes to a string.
          
          \param[in]  inStr         text to add ANSI codes to
          \param[in]  foreGround    foreground color to add to text
          \param[in]  backGround    foreground color to add to text
          \pre none
          \post Returns a string with ANSI color codes to set foreground and background colors. The end of the 
                string includes an ANSI code the resets all ANSI attributes.
        */        
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround);

        /*!
          \brief Adds ANSI foregound and background color codes and a style attribute to a string.
          
          This function adds ANSI foregound and background color codes and a style attribute to a string.
          
          \param[in]  inStr         text to add ANSI codes to
          \param[in]  foreGround    foreground color to add to text
          \param[in]  backGround    foreground color to add to text
          \param[in]  attribute     foreground color to add to text
          \pre none
          \post Returns a string with ANSI color codes to set foreground and background colors and a code
                to set a style attribute. The end of the string includes an ANSI code the resets all ANSI 
                attributes.
        */
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute);        

        /*!
          \brief Adds ANSI foregound and background color codes and two style attributes to a string.
          
          This function adds ANSI foregound and background color codes and two style attributes to a string.
          
          \param[in]  inStr         text to add ANSI codes to
          \param[in]  foreGround    foreground color to add to text
          \param[in]  backGround    foreground color to add to text
          \param[in]  attribute1     foreground color to add to text
          \param[in]  attribute2     foreground color to add to text
          \pre none
          \post Returns a string with ANSI color codes to set foreground and background colors and codes
                to set two style attributes. The end of the string includes an ANSI code the resets all ANSI 
                attributes.
        */        
        std::string addStyle(std::string inStr, Display::Color foreGround, Display::Color backGround, Display::Style attribute1, Display::Style attribute2); 
        
};

}}

#endif