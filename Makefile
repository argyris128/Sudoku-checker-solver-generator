CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

all: sudoku

sudoku: src/grid.o src/sudoku.o
	$(CC) $(CFLAGS) $^ -o $@

grid: src/grid.o
	$(CC) $(CFLAGS) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-rm -f *.exe ./src/*.o