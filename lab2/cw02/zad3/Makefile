CC = -gcc
CFLAGS = -Wall

make: start

start:
	$(CC) $(CFLAGS) -o size main.c

test: size
	echo "Program output: " > result.txt ; ./size >> result.txt ;
	echo "wc --bytes * output:" >> result.txt ; wc --bytes * >> result.txt

clean:
	rm -f size *.txt