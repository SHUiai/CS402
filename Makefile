warmup1: warmup1.o my402list.o transaction.o
	gcc -o warmup1 -g warmup1.o my402list.o transaction.o

warmup1.o: warmup1.c my402list.h transaction.h
	gcc -g -c -Wall warmup1.c

my402list.o: my402list.c my402list.h
	gcc -g -c -Wall my402list.c

transaction.o: my402list.h transaction.c transaction.h
	gcc -g -c -Wall transaction.c 

clean:
	rm -f *.o warmup1
