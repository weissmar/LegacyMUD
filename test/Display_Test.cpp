/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  1/30/2017
 * Last Modified: 1/30/2017
 * Course:        CS467, Winter 2017
 * Filename:      Display_Test.hpp
 *
 * Overview:
 *     Unit test for the Display class.
 ************************************************************************/

#include "Display.h"
#include <string>
#include <iostream>

#include <gtest/gtest.h>

namespace {

// Add red to a string.
TEST(DisplayTest, AddRed) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be red.", d.RED);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[31mThis should be red.\033\[0m", outStr) 
        << "Expect ANSI code 31 for red.";
}

// Add green to a string.
TEST(DisplayTest, AddGreen) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be green.", d.GREEN);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[32mThis should be green.\033\[0m", outStr) 
        << "Expect ANSI code 32 for green.";
}

// Add yellow to a string.
TEST(DisplayTest, AddYellow) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be yellow.", d.YELLOW);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[33mThis should be yellow.\033\[0m", outStr) 
        << "Expect ANSI code 33 for yellow.";
}

// Add blue to a string.
TEST(DisplayTest, AddBlue) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be blue.", d.BLUE);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[34mThis should be blue.\033\[0m", outStr) 
        << "Expect ANSI code 34 for blue.";
}

// Add magenta to a string.
TEST(DisplayTest, AddMagenta) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be magenta.", d.MAGENTA);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[35mThis should be magenta.\033\[0m", outStr) 
        << "Expect ANSI code 35 for magenta.";
}

// Add cyan to a string.
TEST(DisplayTest, AddCyan) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be cyan.", d.CYAN);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[36mThis should be cyan.\033\[0m", outStr) 
        << "Expect ANSI code 36 for cyan.";
}

// Add green to a string.  Add bright.
TEST(DisplayTest, AddGreenBright) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be green and bright.", d.GREEN, d.BRIGHT);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[32;1mThis should be green and bright.\033\[0m", outStr) 
        << "Expect ANSI code 32 for green and 1 for bright.";
}

// Add green to a string.  Add underscore.
TEST(DisplayTest, AddGreenUnderscore) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be green and underscored.", d.GREEN, d.UNDERSCORE);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[32;4mThis should be green and underscored.\033\[0m", outStr) 
        << "Expect ANSI code 32 for green and 4 for underscore.";
}

// Add white to a string.  Add bright and underscore.
TEST(DisplayTest, AddWhiteBrightUnderscore) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be white, bright and underscored.", d.WHITE, d.BRIGHT, d.UNDERSCORE);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[37;1;4mThis should be white, bright and underscored.\033\[0m", outStr) 
        << "Expect ANSI code 37 for white, 1 for bright, and 4 for underscore.";
}

// Add red to a string.  Add a white background.
TEST(DisplayTest, AddRedOnWhite) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be red on white.", d.RED, d.WHITE);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[31;47mThis should be red on white.\033\[0m", outStr) 
        << "Expect ANSI code 31 for red. ANSI code 47 for white background";
}

// Add red to a string.  Add a cyan background. Add bright.
TEST(DisplayTest, AddRedOnCyanBright) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be red on cyan and bright.", d.RED, d.CYAN, d.BRIGHT);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[31;46;1mThis should be red on cyan and bright.\033\[0m", outStr) 
        << "Expect ANSI code 31 for red. ANSI code 46 for cyan background, ANSI code 1 for bright.";
}

// Add yellow to a string.  Add a black background. Add bright and underscore.
TEST(DisplayTest, AddYellowOnBlackBrightUnderscore) {
    legacymud::display::Display d;
    std::string outStr;
    outStr = d.addStyle("This should be yellow on black, bright, and underscored.", d.YELLOW, d.BLACK, d.BRIGHT, d.UNDERSCORE);
    std::cout << outStr << std::endl;
    EXPECT_EQ("\033\[33;40;1;4mThis should be yellow on black, bright, and underscored.\033\[0m", outStr) 
        << "Expect ANSI code 33 for yellow. ANSI code 40 for black background, ANSI code 1 for bright and 4 for underscore.";
}

}
