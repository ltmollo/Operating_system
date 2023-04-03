CC = gcc
CFLAGS = -Wall
REPL = repl.c

################################

all: library_static library_shared static_exe shared_exe dynamic_exe

################################

library_static: library.c
	$(CC) $(CFLAGS) -c library.c -o library.o	 # compile a source file .c to object .o
	ar rcs library.a library.o					 # create static library .a

################################

library_shared: library.c
	$(CC) $(CFLAGS) -c -fPIC library.c -o library.o 	# compile the source object .c to object .o
	$(CC) $(CFLAGS) -shared -o library.so library.o		# create shared library

################################

static_exe: library.c $(REPL) dl_manager.h
	$(CC) $(CFLAGS) $(REPL) -o repl_static -L . library.a

################################

shared_exe: library.c $(REPL) dl_manager.h
	$(CC) $(CFLAGS) $(REPL) -o repl_shared -L . ./library.so

################################

dynamic_exe: library.c $(REPL) dl_manager.h
	$(CC) $(CFLAGS) $(REPL) -o repl_dynamic -ldl -D USE_DYNAMIC_LIBRARY

################################

clean:
	rm -f *.o *.a *.so \
    repl_static repl_shared repl_dynamic