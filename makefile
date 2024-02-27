main.out: main.o board.o globals.o
	gcc main.o board.o globals.o -o main.out -lraylib -lGL -lm -pthread -ldl

board.o: board.c
	gcc -c board.c -o board.o

globals.o: globals.c
	gcc -c globals.c -o globals.o

main.o: main.c
	gcc -c main.c -o main.o
