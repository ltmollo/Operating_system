CC = -gcc
CFLAGS = -Wall

all: start start2

start: rev.c
	$(CC) $(CFLAGS) -o reverse rev.c main.c

start2: block.c
	$(CC) $(CFLAGS) -o reverse2 block.c main.c

test: reverse2 reverse
	echo "reverse result:" > compare.txt ; echo "reverse time results: " > pomiar_zad_2.txt ; \
	./reverse text reverse_res.txt >> pomiar_zad_2.txt ; tac < text | rev > tmp.txt ; diff -s tmp.txt reverse_res.txt >> compare.txt ; \
	echo "reverse2 result:" >> compare.txt ; echo "reverse2 results: " >> pomiar_zad_2.txt ;
	./reverse2 text reverse2_res.txt >> pomiar_zad_2.txt ; diff -s tmp.txt reverse2_res.txt >> compare.txt

clean:
	rm -f reverse reverse2 *.txt