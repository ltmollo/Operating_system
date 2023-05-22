CC = -gcc
CFLAGS = -Wall

make: start test

start:
	$(CC) $(CFLAGS) -o main main.c -lpthread

test: start
	./main testing > result.txt; cat result.txt

clean:
	rm -f main result.txt