CC = gcc
CFALGS = -Wall

make: start

start: main.c portion.c
	$(CC) $(CFLAGS) -o main main.c
	$(CC) $(CFALGS) -o portion portion.c

test: start
	echo "./main 0.00002 125" > report.txt ; ./main 0.00002 125 >> report.txt ; echo " " >> report.txt;
	echo "./main 0.0000002 125" >> report.txt ; ./main 0.0000002 125 >> report.txt ; echo " " >> report.txt;
	echo "./main 0.000000002 125" >> report.txt ; ./main 0.000000002 125 >> report.txt ; echo " " >> report.txt;

	echo "./main 0.0003 125" >> report.txt; ./main 0.0003 125 >> report.txt ; echo " " >> report.txt;
	echo "./main 0.000003 1250" >> report.txt; ./main 0.000003 1250 >> report.txt ; echo " " >> report.txt;
	echo "./main 0.00000003 1250" >> report.txt; ./main 0.00000003 1250 >> report.txt ; echo " " >> report.txt;

	echo "./main 0.0000004 1250" >> report.txt ; ./main 0.0000004 1250 >> report.txt; echo " " >> report.txt;
	echo "./main 0.00000004 12500" >> report.txt ; ./main 0.00000004 12500 >> report.txt ; echo " " >> report.txt
	echo "./main 0.00000002 12500" >> report.txt ; ./main 0.00000002 12500 >> report.txt ; echo " " >> report.txt;


	echo "./main 0.0000009 1250" >> report.txt ; ./main 0.0000009 1250 >> report.txt; echo " " >> report.txt;
	echo "./main 0.00000009 12500" >> report.txt ; ./main 0.00000009 12500 >> report.txt ; echo " " >> report.txt
	echo "./main 0.00000009 12500" >> report.txt ; ./main 0.00000009 12500 >> report.txt ; echo " " >> report.txt;



	echo "./main 0.000000009 1250" >> report.txt ; ./main 0.000000009 1250 >> report.txt; echo " " >> report.txt;
	echo "./main 0.0000000009 12500" >> report.txt ; ./main 0.0000000009 12500 >> report.txt ; echo " " >> report.txt
	echo "./main 0.0000000009 12500" >> report.txt ; ./main 0.0000000009 12500 >> report.txt ; echo " " >> report.txt;



	echo "./main 0.000000009 2250" >> report.txt ; ./main 0.000000009 2250 >> report.txt; echo " " >> report.txt;
	echo "./main 0.000000000009 12500" >> report.txt ; ./main 0.00000000009 12500 >> report.txt ; echo " " >> report.txt
	echo "./main 0.000000000005 12500" >> report.txt ; ./main 0.00000000005 12500 >> report.txt ; echo " " >> report.txt;

clean:
	rm -f main portion