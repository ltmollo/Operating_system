CC = gcc
CFALGS = -Wall

make: start

start:
	$(CC) $(CFLAGS) -o program main.c

test: program
	echo "Input: ./program . \#include" > test.txt ; ./program . \#include >> test.txt ;
	echo "\nInput: ./program .. \#include" >> test.txt ; ./program .. \#include >> test.txt ;

clean:
	rm -f program test.txt