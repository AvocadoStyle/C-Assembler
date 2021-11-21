MAIN: MAIN.o
	gcc -g -ansi -Wall MAIN.o -o MAIN -lm
MAIN.o: MAIN.c Data_structer.h
	gcc -c -ansi -Wall MAIN.c -o MAIN.o
