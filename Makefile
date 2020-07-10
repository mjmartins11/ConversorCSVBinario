all: programaTrab

run: programaTrab
	./programaTrab

programaTrab: bebe.o arquivo_indice.o binarioNaTela.o arquivo_cabecalho.o arquivo_conteudo.o arquivos.o programaTrab.o
	gcc -g bebe.o arquivo_indice.o binarioNaTela.o arquivo_cabecalho.o arquivo_conteudo.o arquivos.o programaTrab.o -o programaTrab

programaTrab.o: programaTrab.c
	gcc -g programaTrab.c -c

bebe.o: bebe.c
	gcc -g bebe.c -c

arquivos.o: arquivos.c
	gcc -g arquivos.c -c

arquivo_conteudo.o: arquivo_conteudo.c
	gcc -g arquivo_conteudo.c -c

arquivo_cabecalho.o: arquivo_cabecalho.c
	gcc -g arquivo_cabecalho.c -c

binarioNaTela.o: binarioNaTela.c
	gcc -g binarioNaTela.c -c

arquivo_indice.o: arquivo_indice.c
	gcc -g arquivo_indice.c -c