CC = -gcc
CFLAGS = -Wall

make: start

start:
	$(CC) $(CFLAGS) -o size main.c

test: size
	echo "program result:" > result.txt ; ./size . >> result.txt ;
	echo "wc result:" >> result.txt ; find . -type f | xargs wc --bytes >> result.txt
clean:
	rm -f size *.txt