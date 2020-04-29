all: programaTrab

run: programaTrab
	./programaTrab

programaTrab: programaTrab.o
	gcc programaTrab.o -o programaTrab

programaTrab.o: programaTrab.c
	gcc programaTrab.c -c