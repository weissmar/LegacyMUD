/*!
  \file     InteractiveNounStub.cpp
  \author   David Rigert
  \created  02/26/2017
  \modified 02/26/2017
  \course   CS467, Winter 2017
 
  \details This file contains the implementation of the InteractiveNounStub class.
*/
#include "InteractiveNounStub.hpp"

namespace legacymud { namespace test {

std::string InteractiveNounStub::getName() const {
    return "";
}

engine::ObjectType InteractiveNounStub::getObjectType() const {
    return engine::ObjectType::NONE;
}

engine::InteractiveNoun* InteractiveNounStub::copy() {
    return nullptr;
}

bool InteractiveNounStub::editAttribute(engine::Player*, std::string) {
    return false;
}

bool InteractiveNounStub::editWizard(engine::Player*) {
    return false;
}

std::string InteractiveNounStub::serialize() {
    return "";
}

bool InteractiveNounStub::deserialize(std::string) {
    return false;
}

}}
