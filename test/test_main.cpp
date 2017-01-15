/*************************************************************************
 * Author:        David Rigert
 * Date Created:  1/14/2017
 * Last Modified: 1/14/2017
 * Course:        CS467, Winter 2017
 * Filename:      test_main.cpp
 *
 * Overview:
 *     This is just the main function that runs all the unit tests..
 ************************************************************************/

#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

