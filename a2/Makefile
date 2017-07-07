objects = satelliteComs.o powerSubsystem.o consoleDisplay.o \
          thrusterSubsystem.o warningAlarm.o
headers = Bool.h dataStructs.h TCB.h

all: main 

main: main.c $(objects) $(headers)
	gcc -Wall -g -o main main.c $(objects)

warningAlarm.o: warningAlarm.c
	gcc -Wall -g -c -o warningAlarm.o warningAlarm.c

thrusterSubsystem.o: thrusterSubsystem.c
	gcc -Wall -g -c -o thrusterSubsystem.o thrusterSubsystem.c

consoleDisplay.o: consoleDisplay.c
	gcc -Wall -g -c -o consoleDisplay.o consoleDisplay.c

powerSubsystem.o: powerSubsystem.c
	gcc -Wall -g -c -o powerSubsystem.o powerSubsystem.c

satelliteComs.o: satelliteComs.c
	gcc -Wall -g -c -o satelliteComs.o satelliteComs.c

clean:
	rm main *.o
