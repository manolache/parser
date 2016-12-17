#==========================================================#
# A makefile for small and medium c/c++ projects     
# Author: Manolache Ionut
#==========================================================#


#==========================================================#
# Software Deployment Variables
#==========================================================#
DIR_INSTALL := /usr/local/bin
#==========================================================#


#==========================================================#
# Software Build Variables
#==========================================================#

# out directory - holds the deliverable (app or library)
OUTPUT := parser		
DIR_OUTPUT := ./out
LINK_TARGET := $(DIR_OUTPUT)/$(OUTPUT)

# inc directory - holds header files
DIR_INCLUDE := ./inc
INCLUDES := -I$(DIR_INCLUDE)
ADDITIONAL_INCLUDES :=

# src directory - holds source files
DIR_SOURCES := ./src
CPPSOURCES := $(wildcard $(DIR_SOURCES)/*.cpp)

# temporary directory - holds objects and dependencies 
DIR_TMP := ./tmp
OBJ := $(CPPSOURCES:.cpp=.o)
OBJECTS := $(patsubst $(DIR_SOURCES)%, $(DIR_TMP)%, $(OBJ))
DEPS := $(OBJECTS:.o=.d)
#==========================================================#

#==========================================================#
# Compiler variables
#==========================================================#
CXX := g++

#compile flags
CXXFLAGS := -std=c++14 -Wall -g #-fcolor-diagnostics 

#link flags
LDFLAGS = -lm
#==========================================================#

#==========================================================#
# Rules
#==========================================================#

all: dirs deps build

build: $(LINK_TARGET)

deps: $(DEPS)

# creates the directory structure
dirs: 
	mkdir -p $(DIR_INCLUDE)
	mkdir -p $(DIR_SOURCES)
	mkdir -p $(DIR_TMP)
	mkdir -p $(DIR_OUTPUT)

# link - generate the executable
$(LINK_TARGET): $(OBJECTS)
	@echo Linking $(LINK_TARGET)
	$(CXX) $(LDFLAGS) $^ -o $@

-include $(DEPS)

# generate dependencies using the c++ preprocessor
$(DIR_TMP)/%.d : $(DIR_SOURCES)/%.cpp
	$(CXX) $(INCLUDES) $(ADDITIONAL_INCLUDES) -MM -MP -MT $(@:.d=.o) $< -MF $@ 

# generate objects from c++ sources
$(DIR_TMP)/%.o : $(DIR_SOURCES)/%.cpp
	@echo Compiling $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ADDITIONAL_INCLUDES) -c $< -o $@

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(DEPS) $(LINK_TARGET)

.PHONY : install
install : $(LINK_TARGET)
	cp $< $(DIR_INSTALL)

.PHONY : uninstall
uninstall:
	rm -f $(DIR_INSTALL)/$(OUTPUT)

#==========================================================#