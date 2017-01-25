# Author:      David Rigert
# Created:     1/24/2017
# Course:      CS 467, Winter 2017
# Description: Makefile for LegacyMUD project

CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread
LIB_DIRS = account display engine parser telnet
LIBS = $(foreach lib_dir, $(LIB_DIRS), $(lib_dir)/$(lib_dir).a)
CXXFLAGS += $(foreach include_dir, $(INCLUDE_DIRS), -I$(include_dir))

all: $(LIBS)
	$(CXX) $(CXXFLAGS) $^ main.cpp -o legacymud

# Run the makefile for each library
%.a: $(dir $@)
	$(MAKE) -C $(dir $@)

# Run the clean target in each library makefile
# and then clean up main.
clean:
	$(foreach lib_dir, $(LIB_DIRS), $(MAKE) -C $(lib_dir) clean;)
	$(RM) main.o legacymud

