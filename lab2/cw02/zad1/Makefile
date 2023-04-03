CC = -gcc
CFLAGS = -Wall

all: zamien zamien2

zamien: lib_change.c
	$(CC) $(CFLAGS) -o zamien lib_change.c main.c

zamien2: sys_change.c
	$(CC) $(CFLAGS) -o zamien2 sys_change.c main.c

test: zamien2 zamien
	echo "sys commands result:" > compare.txt ; echo "sys time results: " > pomiar_zad_1.txt ; \
	./zamien2 a c text sys_res.txt >> pomiar_zad_1.txt ; tr a c < text > tmp.txt ; diff -s tmp.txt sys_res.txt >> compare.txt ; \
	echo "lib commands result:" >> compare.txt ; echo "lib time results: " >> pomiar_zad_1.txt ;
	./zamien a c text lib_res.txt >> pomiar_zad_1.txt ; diff -s tmp.txt lib_res.txt >> compare.txt

clean:
	rm -f zamien zamien2 sys_res lib_res tmp *.txt