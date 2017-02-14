/*************************************************************************
 * Author:        Keith Adkins
 * Date Created:  2/12/2017
 * Last Modified: 2/13/2017
 * Course:        CS467, Winter 2017
 * Filename:      Account_Test.cpp
 *
 * Overview:
 *     Unit test for the Account class.
 ************************************************************************/

#include <Account.hpp>
#include <string>
#include <iostream>

#include <gtest/gtest.h>

namespace {

/* Sample Test1. */
TEST(AccountTest, SampleTest1) {
    legacymud::account::Account a;
    int i = 10;

    EXPECT_EQ(10, i) 
        << "Expect i equals 10.";
}


}
