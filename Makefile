#Makefile
CC=g++
LIB=-lm -ldislnc -ldiscpp -ldislin
main: main.cpp tiles.h tiles.c
	$(CC) -o bin/main.o main.cpp $(LIB)
