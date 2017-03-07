# Author:      David Rigert
# Created:     1/31/2017
# Course:      CS 467, Winter 2017
# Description: Makefile for unit tests

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread
# This gets all .cpp filenames in the current directory
# and stores them as a list of .o filenames
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
# List the relative paths to the libraries being tested here
LIB_DIRS = ../account ../display ../engine ../parser ../telnet ../gamedata
# Get the paths to the static libraries being tested
LIBS = $(foreach lib_dir, $(LIB_DIRS), $(lib_dir)/$(patsubst ../%,%,$(lib_dir)).a)
# List the relative paths to external library binaries here
EXT_BIN_DIRS = ../external/bin
# List the relative paths to external library header files here
EXT_INC_DIRS = ../external/headers
# List the names of external libraries here
EXT_LIB_NAMES = gtest
# Append the include paths for the libraries to the g++ flags
CXXFLAGS += $(foreach include_dir, $(LIB_DIRS), -I$(include_dir))
# Add the external header directory as an include path
CXXFLAGS += $(foreach ext_inc_dir, $(EXT_INC_DIRS), -I$(ext_inc_dir))
# Add the external binary directory as a search target for the linker
EXT_BIN = $(foreach ext_bin_dir, $(EXT_BIN_DIRS), -L $(ext_bin_dir))
# Tell the linker to link the external libraries
EXT_LIB = $(foreach ext_lib_name, $(EXT_LIB_NAMES), -l$(ext_lib_name))
# The name of the unit test executable
PROG = unittest

all: libdirs $(LIBS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(PROG) $(OBJS) -Wl,--start-group $(LIBS) -Wl,--end-group $(EXT_BIN) $(EXT_LIB)

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
