all: main

run: main 
	./main

main:
	gcc bebe.o main.o -o main

main.o: main.c
	gcc main.c -c

bebe.o: bebe.c
	gcc bebe.c -c
