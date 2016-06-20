CC=gcc
CFLAGS=-g -Wall -Wextra

Run: Compile
	./FrequentWords BA1B_extra_dataset.txt

Compile: FrequentWords.o vector.o
	$(CC) $(CFLAGS) FrequentWords.o vector.o -o FrequentWords

FrequentWords.o: FrequentWords.c
	$(CC) $(CFLAGS) -c FrequentWords.c

vector.o: vector.c
	$(CC) $(CFLAGS) -c vector.c

clean:
	rm -f *.o FrequentWords
