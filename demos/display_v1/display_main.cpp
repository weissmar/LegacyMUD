/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/31/2017
 * Last Modified: 2/7/2017
 * Course:        CS467, Winter 2017
 * Filename:      display_main.cpp
 *
 * Overview:
 *     This main function for for local testing of the display class.
 ************************************************************************/

#include "Display.hpp"
#include <string>
#include <iostream>

int main() {   
    legacymud::display::Display d;
    
    std::string str1 = "Hello";
    std::cout << d.addStyle(str1,d.YELLOW) << std::endl;
    
    std::string str2 = "This is a string.";
    std::cout << d.addStyle(str2,d.CYAN, d.BRIGHT) << std::endl;
    
    std::string str3 = "This is a string.";
    std::cout << d.addStyle(str3,d.CYAN, d.UNDERSCORE) << std::endl;
    
    std::string str4 = "This is a string.";
    std::cout << d.addStyle(str4,d.WHITE, d.UNDERSCORE, d.BRIGHT) << std::endl;  

    std::string str5 = "This is a string.";
    std::cout << d.addStyle(str5,d.RED, d.WHITE) << std::endl; 

    std::string str6 = "This is a string.";
    std::cout << d.addStyle(str5,d.RED, d.WHITE, d.BRIGHT) << std::endl;   

    std::string str7 = "This is a string.";
    std::cout << d.addStyle(str7,d.RED, d.BLACK, d.UNDERSCORE, d.BRIGHT) << std::endl;    

    return 0;
}
