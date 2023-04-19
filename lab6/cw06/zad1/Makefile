CC = gcc
CFALGS = -Wall

make: start_server start_client

start_server: server.c
	$(CC) $(CFLAGS) -o server server.c

start_client: client.c
	$(CC) $(CFLAGS) -o client client.c

clean:
	rm -f server client history.txt