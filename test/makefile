# Author:      David Rigert
# Created:     1/14/2017
# Course:      CS 467, Winter 2017
# Description: Makefile for unit tests

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread
SOURCES = Sample.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TESTSOURCES = *_Test.cpp
EXTLIB=-L ../external/bin
EXTINC=-I../external/headers
LIB=-lgtest

all: $(TESTSOURCES) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(TESTSOURCES) $(OBJECTS) test_main.cpp -o unittest $(EXTINC) $(EXTLIB) $(LIB) 

$(OBJECTS) : $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(OBJECTS) unittest

