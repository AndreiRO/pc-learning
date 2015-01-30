.PHONY: build cleanrun

CC = gcc
CFLGS = -Oo -g -Wall -Werror

build: anagrams

anagrams.o: anagrams.c
	$(CC) $(CFLAGS) -c $^ -o $@

angrams: anagrams.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -v anagrams.o anagrams

run: clean build
	./anagrams input.txt
