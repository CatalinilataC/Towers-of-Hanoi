CC=gcc
CFLAGS=-Wall -Wextra -c
EXE=tema2
build: tema2.o
	$(CC) $^ -o $(EXE)
tema2.o: tema2.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf tema2.o $(EXE)
