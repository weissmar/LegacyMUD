/*************************************************************************
 * Author:        David Rigert
 * Date Created:  1/14/2017
 * Last Modified: 2/5/2017
 * Course:        CS467, Winter 2017
 * Filename:      Sample_Test.cpp
 *
 * Overview:
 *     Unit test for the Sample class.
 ************************************************************************/

#include "Sample.hpp"

#include <gtest/gtest.h>

namespace {

// Example of testing whether a constructor initializes values correctly.
TEST(SampleTest, DefaultConstructor) {
    legacymud::test::Sample samp;
    EXPECT_EQ(0, samp.getValue()) 
        << "Default constructor should set value to 0";
}

// Example of testing whether accessor and mutator functions work correctly.
TEST(SampleTest, SetValueGetValue) {
    legacymud::test::Sample samp;
    samp.setValue(123);
    EXPECT_EQ(123, samp.getValue()); 
}

// Example of a death test. 
// The empty string is a regex statement that must match the error message.
// Leave it blank for test for any crash.
TEST(SampleTest, DivideByZeroDeath) {
    legacymud::test::Sample samp;
    EXPECT_DEATH(samp.divideByZero(), "");
}

// Example of what happens when a test fails.
TEST(SampleTest, FailedTest) {
    EXPECT_FALSE(true) << "Expected false, got true";
}

// Example of what happens when a death test fails.
TEST(SampleTest, FailedDeathTest) {
    legacymud::test::Sample samp;
    EXPECT_DEATH(samp.getValue(), "");
}

}

