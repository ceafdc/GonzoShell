CC=cc
CFLAGS= -std=c11 -Wall


all: gshell

gshell: structures.o vector.o list.o refcnt.o str.o htable.o parser.o main.o
	$(CC) $(CFLAGS) *.o -o gshell

parser.o: src/parser/parser.c
	$(CC) $(CFLAGS) -c src/parser/parser.c

list.o: src/structures/list.c
	$(CC) $(CFLAGS) -c src/structures/list.c

htable.o: src/structures/htable.c
	$(CC) $(CFLAGS) -c src/structures/htable.c

refcnt.o: src/structures/refcnt.c
	$(CC) $(CFLAGS) -c src/structures/refcnt.c

str.o: src/structures/str.c
	$(CC) $(CFLAGS) -c src/structures/str.c

vector.o: src/structures/vector.c
	$(CC) $(CFLAGS) -c src/structures/vector.c

structures.o: src/structures/structures.c
	$(CC) $(CFLAGS) -c src/structures/structures.c

main.o: src/main.c
	$(CC) $(CFLAGS) -D_XOPEN_SOURCE=500 -c src/main.c

clean:
	rm -rf *.o
	rm gshell
