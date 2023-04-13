CC = gcc
CFLAGS = -Wall

make: start test_fork test_exec

start:
	$(CC) $(CFALGS) -o main_fork main.c
	$(CC) $(CFALGS) -o main_exec main.c -D EXEC

test_fork: main_fork
	echo "Fork test" > fork.txt ; echo "" >> fork.txt ;
	echo "Ignore:" >> fork.txt ; ./main_fork ignore >> fork.txt; echo "" >> fork.txt ;
	echo "Handler:" >> fork.txt ; ./main_fork handler >> fork.txt; echo "" >> fork.txt ;
	echo "Mask:" >> fork.txt ; ./main_fork mask >> fork.txt; echo "" >> fork.txt ;
	echo "Pending:" >> fork.txt ; ./main_fork pending >> fork.txt; echo "" >> fork.txt ;

test_exec: main_exec
	echo "Exec test" > exec.txt ; echo "" >> exec.txt ;
	echo "Ignore:" >> exec.txt; ./main_exec ignore >> exec.txt; echo "" >> exec.txt ;
	echo "Mask:" >> exec.txt ; ./main_exec mask >> exec.txt; echo "" >> exec.txt ;
	echo "Pending:" >> exec.txt ; ./main_exec pending >> exec.txt; echo "" >> exec.txt ;
clean:
	rm -f main_fork main_exec fork.txt exec.txt