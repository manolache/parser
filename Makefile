#==========================================================#
# makefile to build the ini parser project     
#==========================================================#

#==========================================================#
# build Variables
#==========================================================#

# output 
APP_NAME := iniparser
LIB_NAME := libiniparser.so
DIR_INSTALL := /usr/local/bin

# app root directory
DIR_APP_ROOT = ./app
# out directory - holds the app
DIR_OUTPUT := $(DIR_APP_ROOT)/out
# inc directory - holds header files
DIR_INCLUDE := $(DIR_APP_ROOT)/inc
# src directory - holds source files
DIR_SOURCES := $(DIR_APP_ROOT)/src
# dir resources
DIR_RESOURCES := $(DIR_APP_ROOT)/res
# temporary directory - holds objects and dependencies 
DIR_TMP := $(DIR_APP_ROOT)/tmp
# libraries directory - tells the compiler where to look after shared libraries
DIR_LIBRARIES := 

# convenient variables
LINK_TARGET := $(DIR_OUTPUT)/$(APP_NAME)
LINK_TARGET_LIB := $(DIR_OUTPUT)/$(LIB_NAME)
INCLUDES := -I$(DIR_INCLUDE)
ADDITIONAL_INCLUDES :=
CPPSOURCES := $(wildcard $(DIR_SOURCES)/*.cpp)
OBJ := $(CPPSOURCES:.cpp=.o)
OBJECTS := $(patsubst $(DIR_SOURCES)%, $(DIR_TMP)%, $(OBJ))
DEPS := $(OBJECTS:.o=.d)
#==========================================================#


#==========================================================#
# test variables
#==========================================================#

# output 
TEST_APP_NAME := test-iniparser

# test root directory
DIR_TEST_ROOT = ./test
# out directory - holds the app
DIR_TEST_OUTPUT := $(DIR_TEST_ROOT)/out
# inc directory - holds header files
DIR_TEST_INCLUDE := $(DIR_TEST_ROOT)/inc
# src directory - holds source files
DIR_TEST_SOURCES := $(DIR_TEST_ROOT)/src
# dir resources
DIR_TEST_RESOURCES := $(DIR_TEST_ROOT)/res
# temporary directory - holds objects and dependencies 
DIR_TEST_TMP := $(DIR_TEST_ROOT)/tmp
# libraries directory - tells the compiler where to look after shared libraries
DIR_TEST_LIBRARIES := $(DIR_OUTPUT) 

# convenient variables
TEST_LINK_TARGET := $(DIR_TEST_OUTPUT)/$(TEST_APP_NAME)
TEST_INCLUDES := -I$(DIR_TEST_INCLUDE)
TEST_ADDITIONAL_INCLUDES := -I$(DIR_INCLUDE)
TEST_CPPSOURCES := $(wildcard $(DIR_TEST_SOURCES)/*.cpp)
TEST_OBJ := $(TEST_CPPSOURCES:.cpp=.o)
TEST_OBJECTS := $(patsubst $(DIR_TEST_SOURCES)%, $(DIR_TEST_TMP)%, $(TEST_OBJ))
TEST_DEPS := $(TEST_OBJECTS:.o=.d)
#==========================================================#


#==========================================================#
# compiler & linker
#==========================================================#
CXX := g++

#compile flags
CXXFLAGS := -std=c++14 -Wall -g -DDEBUG -fpic

#link flags
LDFLAGS = -lm
TEST_LDFLAGS = -lm -l$(APP_NAME)
#==========================================================#


#==========================================================#
# rules
#==========================================================#

all: deps deps-test build build-test test

dirs:
	mkdir -p $(DIR_INCLUDE)
	mkdir -p $(DIR_SOURCES)
	mkdir -p $(DIR_TMP)
	mkdir -p $(DIR_OUTPUT)
	mkdir -p $(DIR_RESOURCES)

dirs-test:
	mkdir -p $(DIR_TEST_INCLUDE)
	mkdir -p $(DIR_TEST_SOURCES)
	mkdir -p $(DIR_TEST_TMP)
	mkdir -p $(DIR_TEST_OUTPUT)
	mkdir -p $(DIR_TEST_RESOURCES)

deps: $(DEPS)
deps-test: $(TEST_DEPS)
-include $(DEPS) $(TEST_DEPS)

build: $(LINK_TARGET)

build-test: $(LINK_TARGET_LIB) $(TEST_LINK_TARGET)

test: build-test
	@echo Running the unit tests...
	@LD_LIBRARY_PATH=./app/out/ $(TEST_LINK_TARGET) ./test/res/empty.ini ./test/res/first.ini ./test/res/update.ini

# generate app dependencies using the c++ preprocessor
$(DIR_TMP)/%.d : $(DIR_SOURCES)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ADDITIONAL_INCLUDES) -MM -MP -MT $(@:.d=.o) $< -MF $@ 

# compile the application
$(DIR_TMP)/%.o : $(DIR_SOURCES)/%.cpp
	@echo Compiling $< ...
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(ADDITIONAL_INCLUDES) -c $< -o $@

# link the application in executable format
$(LINK_TARGET): $(OBJECTS)
	@echo Linking $< ...
	$(CXX) $(DIR_LIBRARIES) $(LDFLAGS) $^ -o $@

# generate test dependencies using the c++ preprocessor
$(DIR_TEST_TMP)/%.d : $(DIR_TEST_SOURCES)/%.cpp
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_ADDITIONAL_INCLUDES) -MM -MP -MT $(@:.d=.o) $< -MF $@ 

# compile the test application
$(DIR_TEST_TMP)/%.o : $(DIR_TEST_SOURCES)/%.cpp
	@echo Compiling $< ...
	$(CXX) $(CXXFLAGS) $(TEST_INCLUDES) $(TEST_ADDITIONAL_INCLUDES) -c $< -o $@

# link the application in shared library format
$(LINK_TARGET_LIB): $(OBJECTS)
	@echo Linking $< ...
	$(CXX) $(DIR_LIBRARIES) $(LDFLAGS) -shared $^ -o $@

# link the test application
$(TEST_LINK_TARGET): $(TEST_OBJECTS)
	@echo Linking $< ...
	$(CXX) -L$(DIR_TEST_LIBRARIES) $(TEST_LDFLAGS) $^ -o $@

#==========================================================#
# PHONY targets
#==========================================================#
.PHONY : install
install : $(LINK_TARGET)
	@echo Installing $< ...
	cp $< $(DIR_INSTALL)

.PHONY : uninstall
uninstall:
	@echo Uninstalling ...
	rm -f $(DIR_INSTALL)/$(APP_NAME)

.PHONY : clean
clean :
	rm -f $(OBJECTS) $(DEPS) $(LINK_TARGET) $(TEST_OBJECTS) $(TEST_DEPS) $(TEST_LINK_TARGET)
