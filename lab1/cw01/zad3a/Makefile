CC = gcc
CFLAGS = -Wall

################################

default:
	$(info ----------------)
	$(info Please run: `make test -B`)
	$(info ----------------)

.PHONY: library_static library_shared static_exe shared_exe dynamic_exe

################################

library_static.o: library.c
	$(CC) $(CFLAGS) -c library.c -o library_static.o		# compile a source file ".c" to object ".o"

liblibrary.a: library_static.o
	ar rcs liblibrary.a library_static.o					# create static library ".a" (with "lib" prefix)

library_static: liblibrary.a

################################

library_shared.o: library.c
	$(CC) $(CFLAGS) -c -fPIC library.c -o library_shared.o		# compile the source object ".c" to object ".o" (Position Independent Code)

liblibrary.so: library_shared.o
	$(CC) -shared -o liblibrary.so library_shared.o			# create shared library (with "lib" prefix)

library_shared: liblibrary.so

################################

repl_static: repl.c library_static
	$(CC) $(CFLAGS) repl.c -o repl_static -L. liblibrary.a

static_exe: repl_static

################################

repl_shared: repl.c library_shared
	$(CC) $(CFLAGS) repl.c -o repl_shared -L. liblibrary.so -Wl,-rpath=`pwd`

shared_exe: repl_shared

################################

repl_dynamic: repl.c library_shared
	$(CC) $(CFLAGS) repl.c  -o repl_dynamic -ldl -D USE_DYNAMIC_LIBRARY

dynamic_exe: repl_dynamic

################################

test: static_exe shared_exe dynamic_exe
	./repl_static  < commands_test.txt > results_static.txt
	./repl_shared  < commands_test.txt > results_shared.txt
	./repl_dynamic < commands_test.txt > results_dynamic.txt

################################

clean:
	rm -f *.o *.a *.so \
    repl_static repl_shared repl_dynamic \
    results_*.txt

################################
