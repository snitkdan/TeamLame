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
CXXFLAGS += -g -Wall -Wextra

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
# OBJ = satelliteComs.o powerSubsystem.o consoleDisplay.o \
#       thrusterSubsystem.o warningAlarm.o rand2.o terminalComs.o \

OBJ =	startup.o scheduler.o solarPanelControl.o

# TEST_OBJ = thrusterSubsystemTest.o satelliteComsTest.o powerSubsystemTest.o \

TEST_OBJ	=	startupTest.o schedulerTest.o solarPanelControlTest.o gtest_main.a

PROJ_HEADERS = dataStructs.h TCB.h

# Default make command
all: $(TESTS) main

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
thrusterSubsystemTest.o : $(USER_DIR)/thrusterSubsystemTest.cc \
                     $(USER_DIR)/thrusterSubsystem.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/thrusterSubsystemTest.cc

satelliteComsTest.o : $(USER_DIR)/satelliteComsTest.cc \
                     $(USER_DIR)/satelliteComs.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/satelliteComsTest.cc

powerSubsystemTest.o : $(USER_DIR)/powerSubsystemTest.cc \
                     $(USER_DIR)/powerSubsystem.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/powerSubsystemTest.cc

startupTest.o : $(USER_DIR)/startupTest.cc \
                     $(USER_DIR)/startup.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/startupTest.cc

schedulerTest.o : $(USER_DIR)/schedulerTest.cc \
                     $(USER_DIR)/scheduler.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/schedulerTest.cc

solarPanelControlTest.o : $(USER_DIR)/solarPanelControlTest.cc \
                     $(USER_DIR)/solarPanelControl.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/solarPanelControlTest.cc

test_suite : $(OBJ) $(TEST_OBJ)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread

# Builds primary files
rand2.o : $(USER_DIR)/rand2.c $(PROJ_HEADERS) $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/rand2.c

terminalComs.o: $(USER_DIR)/terminalComs.c $(PROJ_HEADERS) $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/terminalComs.c

warningAlarm.o : $(USER_DIR)/warningAlarm.c $(USER_DIR)/warningAlarm.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/warningAlarm.c

thrusterSubsystem.o : $(USER_DIR)/thrusterSubsystem.c $(USER_DIR)/thrusterSubsystem.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/thrusterSubsystem.c

satelliteComs.o : $(USER_DIR)/satelliteComs.c $(USER_DIR)/satelliteComs.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/satelliteComs.c $(USER_DIR)/rand2.c

powerSubsystem.o : $(USER_DIR)/powerSubsystem.c $(USER_DIR)/powerSubsystem.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/powerSubsystem.c

consoleDisplay.o: $(USER_DIR)/consoleDisplay.c $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/consoleDisplay.c

startup.o : $(USER_DIR)/startup.c $(USER_DIR)/startup.h $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/startup.c

scheduler.o: $(USER_DIR)/scheduler.c $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/scheduler.c

solarPanelControl.o: $(USER_DIR)/solarPanelControl.c $(GTEST_HEADERS)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/solarPanelControl.c

main : $(OBJ) $(PROJ_HEADERS) $(USER_DIR)/main.c
	$(CC) -o main $(USER_DIR)/main.c $(OBJ)

# Builds GPIO experiment code
gpio: $(USER_DIR)/gpio.c $(OBJ) $(PROJ_HEADERS)
	$(CC) -o gpio $(USER_DIR)/gpio.c $(OBJ)

# Convenience Commands
copy:
	scp -r ../a2 root@192.168.7.2:/dev

clean:
	rm -f $(TESTS) main gpio gtest.a gtest_main.a *.o
