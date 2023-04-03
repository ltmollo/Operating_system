CC = gcc
CFLAGS = -Wall

################################

default:
	$(info ----------------)
	$(info Please run: `make test -B; make test_O0 -B; make test_O1 -B;`)
	$(info `make test_O2 -B; make test_O3 -B; make test_Os -B;`)
	$(info ----------------)

.PHONY: \
    library_static library_shared static_exe shared_exe dynamic_exe \
    library_static_O0 library_shared_O0 static_exe_O0 shared_exe_O0 dynamic_exe_O0 test_O0 \
    library_static_O1 library_shared_O1 static_exe_O1 shared_exe_O1 dynamic_exe_O1 test_O1 \
    library_static_O2 library_shared_O2 static_exe_O2 shared_exe_O2 dynamic_exe_O2 test_O2 \
    library_static_O3 library_shared_O3 static_exe_O3 shared_exe_O3 dynamic_exe_O3 test_O3 \
    library_static_Os library_shared_Os static_exe_Os shared_exe_Os dynamic_exe_Os test_Os \
    test test_O0 test_O1 test_O2 test_O3 test_Os

################################

library_static.o: library.c
	$(CC) $(CFLAGS) -c library.c -o library_static.o		# compile a source file ".c" to object ".o"

liblibrary.a: library_static.o

	ar rcs liblibrary.a library_static.o					# create static library ".a" (with "lib" prefix)

library_static: liblibrary.a

library_static_O0: CFLAGS += -O0
library_static_O0: liblibrary.a

library_static_O1: CFLAGS += -O1
library_static_O1: liblibrary.a

library_static_O2: CFLAGS += -O2
library_static_O2: liblibrary.a

library_static_O3: CFLAGS += -O3
library_static_O3: liblibrary.a

library_static_Os: CFLAGS += -Os
library_static_Os: liblibrary.a

################################

library_shared.o: library.c
	$(CC) $(CFLAGS) -c -fPIC library.c -o library_shared.o		# compile the source object ".c" to object ".o" (Position Independent Code)

liblibrary.so: library_shared.o
	$(CC) -shared -o liblibrary.so library_shared.o			# create shared library (with "lib" prefix)

library_shared: liblibrary.so

library_shared_O0: CFLAGS += -O0
library_shared_O0: liblibrary.so

library_shared_O1: CFLAGS += -O1
library_shared_O1: liblibrary.so

library_shared_O2: CFLAGS += -O2
library_shared_O2: liblibrary.so

library_shared_O3: CFLAGS += -O3
library_shared_O3: liblibrary.so

library_shared_Os: CFLAGS += -Os
library_shared_Os: liblibrary.so

################################

repl_static: repl.c library_static
	$(CC) $(CFLAGS) repl.c -o repl_static -L. liblibrary.a

static_exe: repl_static

static_exe_O0: CFLAGS += -O0
static_exe_O0: repl_static

static_exe_O1: CFLAGS += -O1
static_exe_O1: repl_static

static_exe_O2: CFLAGS += -O2
static_exe_O2: repl_static

static_exe_O3: CFLAGS += -O3
static_exe_O3: repl_static

static_exe_Os: CFLAGS += -Os
static_exe_Os: repl_static

################################

repl_shared: repl.c library_shared
	$(CC) $(CFLAGS) repl.c -o repl_shared -L. liblibrary.so -Wl,-rpath=`pwd`

shared_exe: repl_shared

shared_exe_O0: CFLAGS += -O0
shared_exe_O0: repl_shared

shared_exe_O1: CFLAGS += -O1
shared_exe_O1: repl_shared

shared_exe_O2: CFLAGS += -O2
shared_exe_O2: repl_shared

shared_exe_O3: CFLAGS += -O3
shared_exe_O3: repl_shared

shared_exe_Os: CFLAGS += -Os
shared_exe_Os: repl_shared

################################

repl_dynamic: repl.c library_shared
	$(CC) $(CFLAGS) repl.c  -o repl_dynamic -ldl -D USE_DYNAMIC_LIBRARY

dynamic_exe: repl_dynamic

dynamic_exe_O0: CFLAGS += -O0
dynamic_exe_O0: repl_dynamic

dynamic_exe_O1: CFLAGS += -O1
dynamic_exe_O1: repl_dynamic

dynamic_exe_O2: CFLAGS += -O2
dynamic_exe_O2: repl_dynamic

dynamic_exe_O3: CFLAGS += -O3
dynamic_exe_O3: repl_dynamic

dynamic_exe_Os: CFLAGS += -Os
dynamic_exe_Os: repl_dynamic

################################

test: static_exe shared_exe dynamic_exe
	./repl_static  < commands_test.txt > results_static.txt
	./repl_shared  < commands_test.txt > results_shared.txt
	./repl_dynamic < commands_test.txt > results_dynamic.txt

test_O0: static_exe_O0 shared_exe_O0 dynamic_exe_O0
	./repl_static  < commands_test.txt > results_static_O0.txt
	./repl_shared  < commands_test.txt > results_shared_O0.txt
	./repl_dynamic < commands_test.txt > results_dynamic_O0.txt

test_O1: static_exe_O1 shared_exe_O1 dynamic_exe_O1
	./repl_static  < commands_test.txt > results_static_O1.txt
	./repl_shared  < commands_test.txt > results_shared_O1.txt
	./repl_dynamic < commands_test.txt > results_dynamic_O1.txt

test_O2: static_exe_O2 shared_exe_O2 dynamic_exe_O2
	./repl_static  < commands_test.txt > results_static_O2.txt
	./repl_shared  < commands_test.txt > results_shared_O2.txt
	./repl_dynamic < commands_test.txt > results_dynamic_O2.txt

test_O3: static_exe_O3 shared_exe_O3 dynamic_exe_O3
	./repl_static  < commands_test.txt > results_static_O3.txt
	./repl_shared  < commands_test.txt > results_shared_O3.txt
	./repl_dynamic < commands_test.txt > results_dynamic_O3.txt

test_Os: static_exe_Os shared_exe_Os dynamic_exe_Os
	./repl_static  < commands_test.txt > results_static_Os.txt
	./repl_shared  < commands_test.txt > results_shared_Os.txt
	./repl_dynamic < commands_test.txt > results_dynamic_Os.txt

################################

clean:
	rm -f *.o *.a *.so \
    repl_static repl_shared repl_dynamic \
    results_*.txt

################################
