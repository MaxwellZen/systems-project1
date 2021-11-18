all: main.o methods.h methods.o
	gcc -o main main.o

run:
	./main

clean:
	rm *.o

main.o: main.c methods.h methods.o
	gcc -c main.c

methods.o: methods.h methods.c
	gcc -c methods.c
