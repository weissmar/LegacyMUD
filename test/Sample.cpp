/*************************************************************************
 * Author:        David Rigert
 * Date Created:  1/14/2017
 * Last Modified: 2/5/2017
 * Course:        CS467, Winter 2017
 * Filename:      Sample.cpp
 *
 * Overview:
 *     Implementation file for unit test example.
 ************************************************************************/

#include "Sample.hpp"

namespace legacymud { namespace test {

Sample::Sample() {
    _value = 0;
}

Sample::~Sample() {
}

int Sample::getValue() {
    return _value;
}

void Sample::setValue(int value) {
    _value = value;
}

int Sample::divideByZero() {
    return _value / 0;
}

}}

