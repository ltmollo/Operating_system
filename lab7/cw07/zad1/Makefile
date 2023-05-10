CC=gcc
CFLAGS=-Wall
OTHER= semaphore_manager.c shared_memory.c queue_manager.c


make: simulation

simulation: client barber
	$(CC) $(CFLAGS) main.c $(OTHER) -lrt -lpthread -o main

client:
	$(CC) $(CFLAGS) client.c $(OTHER) -lrt -lpthread -o client

barber:
	$(CC) $(CFLAGS) barber.c $(OTHER) -lrt -lpthread -o barber

clean:
	rm -rf barber client main
