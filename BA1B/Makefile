CC=gcc
CFLAGS=-g -Wall -Wextra

FrequentWords: FrequentWords.o vector.o
	$(CC) $(CFLAGS) FrequentWords.o vector.o -o FrequentWords

FrequentWords.o: FrequentWords.c
	$(CC) $(CFLAGS) -c FrequentWords.c

vector.o: vector.c
	$(CC) $(CFLAGS) -c vector.c

clean:
	rm -f *.o lab9
