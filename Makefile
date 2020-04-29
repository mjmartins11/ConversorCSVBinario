all: programaTrab

run: programaTrab
	./programaTrab

programaTrab: arquivos.o programaTrab.o
	gcc arquivos.o programaTrab.o -o programaTrab

programaTrab.o: programaTrab.c
	gcc programaTrab.c -c

arquivos.o: arquivos.c
	gcc arquivos.c -c