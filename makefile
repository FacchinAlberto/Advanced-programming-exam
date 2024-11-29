# ALBERTO FACCHIN SM3201282

CC = gcc
CFLAGS = -O0 -fopenmp -std=c17 -Wall -pedantic
HEADERS = ${wildcard *.h}
OBJECTS = pgm.o mandelbrot.o main.o

all: main

main: ${OBJECTS} ${HEADERS}
	${CC} ${CFLAGS} -o $@ ${OBJECTS} -lm

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $<

.PHONY: clean

clean:
	rm -f *.o