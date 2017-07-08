# Define Compilers
AR = ar
ARFLAGS = rcs
CC = gcc
CXX = g++

# Define useful flags
CFLAGS += -g -Wall -Wpedantic -I. -I.. -O0
LDFLAGS += -L. -la2
CPPUNITFLAGS = -L../gtest -lgtest

# Define common dependencies
OBJS = satelliteComs.o powerSubsystem.o consoleDisplay.o \
          thrusterSubsystem.o warningAlarm.o
HEADERS = dataStructs.h TCB.h powerSubsystem.h
TESTOBJS = powerSubsystemTest.o test_suite.o

# Default
all: test_suite main

main: main.c $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) -o main $(OBJS)

test_suite: $(TESTOBJS)
	$(CXX) $(CFLAGS) -o test_suite $(TESTOBJS)\
	$(CPPUNITFLAGS) $(LDFLAGS) -lpthread $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -std=c11 -c $<

%.o: %.cc $(HEADERS)
	$(CXX) $(CFLAGS) -std=c++11 -c $<

clean:
	rm main test_suite *.o
