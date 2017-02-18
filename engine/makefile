# Author:      David Rigert
# Created:     1/24/2017
# Course:      CS 467, Winter 2017
# Description: Makefile for engine library in LegacyMUD project

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread
# Enter the name of the library here
LIBNAME = engine
# This gets all .cpp filenames in the current directory
# and stores them as a list of .o filenames
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
# List the relative paths to any include directories here
INCLUDE_DIRS = ../engine ../parser ../telnet ../account
# Append the include paths to the g++ flags
CXXFLAGS += $(foreach include_dir, $(INCLUDE_DIRS), -I$(include_dir))

all: $(OBJS)
	$(AR) r $(LIBNAME).a $^

# Generic rule for compiling all .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@

# Clean up all objects and libraries in current directory
clean:
	$(RM) $(OBJS) $(LIBNAME).a
