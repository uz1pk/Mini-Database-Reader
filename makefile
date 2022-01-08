all: reader

clean:
	rm *.o

reader: reader.o hash.o
	clang -Wall -pedantic -std=c99 reader.o hash.o -o reader
reader.o: reader.c
	clang -Wall -pedantic -std=c99 -c reader.c
hash.o: hash.c 
	clang -Wall -pedantic -std=c99 -c hash.c
