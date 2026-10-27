CC=gcc
CFLAGS=-g -Werror -std=c11


ifeq ($(shell uname), Darwin)
	LEAKTEST ?= leaks --atExit --
else
	LEAKTEST ?= valgrind --leak-check=full
endif

.PHONY: all valgrind clean test

all: tokenize

valgrind: tokenize
	$(LEAKTEST) ./tokenize

tokenize-tests : %-tests: %
	env python3 tests/$*_tests.py

test: tokenize-tests

clean: 
	rm -rf *.o
	rm -rf tests/__pycache__
	rm -f tokenize
	rm -f ../vector/vect.o

tokenize: tokenize.o tokens.o ../vector/vect.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

