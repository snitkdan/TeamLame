objects = satelliteComs.o powerSubsystem.o consoleDisplay.o
headers = Bool.h dataStructs.h TCB.h

main: main.c $(objects) $(headers)
	gcc -Wall -g -o main main.c $(objects)

consoleDisplay.o: consoleDisplay.c
	gcc -Wall -g -c -o consoleDisplay.o consoleDisplay.c

powerSubsystem.o: powerSubsystem.c
	gcc -Wall -g -c -o powerSubsystem.o powerSubsystem.c

satelliteComs.o: satelliteComs.c
	gcc -Wall -g -c -o satelliteComs.o satelliteComs.c

test: TCB.h dataStructs.h
	gcc main.c -o test -g -Wall

clean:
	rm main *.o
