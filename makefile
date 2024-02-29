main.out: main.o board.o globals.o gridcell.o piece.o
	gcc main.o board.o globals.o gridcell.o piece.o -o main.out -lraylib -lGL -lm -pthread -ldl

board.o: board.c
	gcc -c board.c -o board.o

globals.o: globals.c
	gcc -c globals.c -o globals.o

gridcell.o: gridcell.c
	gcc -c gridcell.c -o gridcell.o

piece.o: piece.c
	gcc -c piece.c -o piece.o

main.o: main.c
	gcc -c main.c -o main.o
