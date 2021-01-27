testlib : main.o bitmap.o hash.o list.o hex_dump.o
	gcc -g -o testlib main.o bitmap.o hash.o list.o hex_dump.o

main.o : main.c bitmap.h hash.h list.h
	gcc -c main.c

hash.o : hash.c hash.h debug.h
	gcc -c hash.c

list.o : list.c list.h debug.h
	gcc -c list.c

hex_dump.o : hex_dump.c hex_dump.h
	gcc -c hex_dump.c

bitmap.o : bitmap.c bitmap.h limits.h round.h hex_dump.c hex_dump.h
	gcc -c bitmap.c

clean:
	rm -f main.o bitmap.o hash.o list.o hex_dump.o testlib
