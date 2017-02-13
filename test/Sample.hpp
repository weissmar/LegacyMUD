/*************************************************************************
 * Author:        David Rigert
 * Date Created:  1/14/2017
 * Last Modified: 2/5/2017
 * Course:        CS467, Winter 2017
 * Filename:      Sample.hpp
 *
 * Overview:
 *     This file contains a sample class to demonstrate how to use gtest.
 ************************************************************************/
#ifndef LEGACYMUD_SAMPLE_HPP
#define LEGACYMUD_SAMPLE_HPP

namespace legacymud { namespace test {

/*!
  \brief A sample class to demonstrate how to use gtest.
*/
class Sample {
    public:
        // Constructor
        Sample();

        // Destructor
        ~Sample();

        // Gets the value
        int getValue();

        // Sets the value
        void setValue(int value);

        // Attempts to divide the value by 0
        int divideByZero();

    private:
        int _value;
};

}}

#endif

