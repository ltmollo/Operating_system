CC = gcc
CFLAGS = -Wall

make: client server

client: client.c
	$(CC) $(CFLAGS) client.c -o client -lpthread

server: server.c
	$(CC) $(CFLAGS) server.c -o server -lpthread

clean:
	rm -f client server history.txt