powerSubsystem.o: powerSubsystem.c
	gcc -Wall -g -c -o powerSubsystem.o powerSubsystem.c

powerSubsystemTest.o: powerSubsystemTest.cc powerSubsystemTest.h
	g++ -Wall -g -c -o powerSubsystemTest.o powerSubsystemTest.cc

test_suite: powerSubsystem.o powerSubsystemTest.o test_suite.cc
	g++ -Wall -g -o test_suite test_suite.cc powerSubsystem.o powerSubsystemTest.o
