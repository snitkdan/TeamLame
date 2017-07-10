# Builds tests & main files for Project 2

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR = ../gtest

# Where to find user code.
USER_DIR = .

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -Wall -Wextra -pthread

# Compilers
CC = gcc
CXX = g++

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = test_suite

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.

all : $(TESTS) main

clean :
	rm -f $(TESTS) main gtest.a gtest_main.a *.o

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^


# Builds tests
warningSubsystem.o : $(USER_DIR)/warningSubsystem.c $(USER_DIR)/warningSubsystem.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/warningSubsystem.c

warningSubsystemTest.o : $(USER_DIR)/warningSubsystemTest.cc \
                     $(USER_DIR)/warningSubsystem.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/warningSubsystemTest.cc

thrusterSubsystem.o : $(USER_DIR)/thrusterSubsystem.c $(USER_DIR)/thrusterSubsystem.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/thrusterSubsystem.c

thrusterSubsystemTest.o : $(USER_DIR)/thrusterSubsystemTest.cc \
                     $(USER_DIR)/thrusterSubsystem.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/thrusterSubsystemTest.cc

satelliteComs.o : $(USER_DIR)/satelliteComs.c $(USER_DIR)/satelliteComs.h $(USER_DIR)/TCB.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/satelliteComs.c $(USER_DIR)/rand2.c

rand2.o : $(USER_DIR)/rand2.c $(USER_DIR)/TCB.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/rand2.c

satelliteComsTest.o : $(USER_DIR)/satelliteComsTest.cc \
                     $(USER_DIR)/satelliteComs.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/satelliteComsTest.cc

powerSubsystem.o : $(USER_DIR)/powerSubsystem.c $(USER_DIR)/powerSubsystem.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/powerSubsystem.c

powerSubsystemTest.o : $(USER_DIR)/powerSubsystemTest.cc \
                     $(USER_DIR)/powerSubsystem.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/powerSubsystemTest.cc

test_suite : thrusterSubsystemTest.o thrusterSubsystem.o powerSubsystemTest.o powerSubsystem.o gtest_main.a \
             satelliteComs.o satelliteComsTest.o rand2.o warningSubsystem.o warningSubsystemTest.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# Build main
main: #powerSubsystem.o main.c
	#$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/main.c powerSubsystem.o
