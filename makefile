# Author:		David Rigert
# Created:		2/16/2017
# Course:		CS 467, Winter 2017
# Description:	Makefile for main.cpp

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread 
# This gets all .cpp filenames in the current directory
# and stores them as a list of .o filenames
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
# List the relative paths to the libraries being tested here
LIB_DIRS = account display engine telnet parser
# Get the paths to the static libraries being tested
LIBS = $(foreach lib_dir, $(LIB_DIRS), $(lib_dir)/$(patsubst %,%,$(lib_dir)).a)
# Append the include paths for the libraries to the g++ flags
CXXFLAGS += $(foreach include_dir, $(LIB_DIRS), -I$(include_dir))
# Program executable name
PROG = legacymud

all: libdirs $(LIBS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) -Wl,--start-group $(LIBS) -Wl,--end-group

# Rules to force the library makefiles to always run
.PHONY: libdirs $(LIB_DIRS)

libdirs: $(LIB_DIRS)

# Run the makefile for each library
$(LIB_DIRS):
	$(MAKE) -C $@

# Generic rule for compiling all .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@

# Run the clean target in each library makefile
# and then clean up this directory.
clean:
	$(foreach lib_dir, $(LIB_DIRS), $(MAKE) -C $(lib_dir) clean;)
	$(RM) $(OBJS) $(PROG)
