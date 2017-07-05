main: main.c powerSubsystem.o satelliteComs.o Bool.h dataStructs.h TCB.h
	gcc -Wall -g -o main main.c powerSubsystem.o satelliteComs.o

powerSubsystem.o: powerSubsystem.c
	gcc -Wall -g -c -o powerSubsystem.o powerSubsystem.c

satelliteComs.o: satelliteComs.c
	gcc -Wall -g -c -o satelliteComs.o satelliteComs.c

test: TCB.h dataStructs.h
	gcc main.c -o test -g -Wall

clean:
	rm main *.o
