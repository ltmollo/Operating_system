CC = gcc
CFLAGS = -Wall

################################

all: library_static library_shared

################################

library_static: library.c
	$(CC) $(CFLAGS) -c library.c -o library.o	 # compile a source file .c to object .o
	ar rcs library.a library.o					 # create static library .a

################################

library_shared: library.c
	$(CC) $(CFLAGS) -c -fPIC library.c -o library.o 	# compile the source object .c to object .o
	$(CC) $(CFLAGS) -shared -o library.so library.o		# create shared library

################################

clean:
	rm -f *.o *.a *.so