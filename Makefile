PROGRAM_NAME = programaTrab
LIB_ARQUIVO = arquivos
LIB_BINARIO = binarioNaTela
LIB_CABECALHO = arquivo_cabecalho
LIB_CONTEUDO = arquivo_conteudo

TAD_NAME = bebe

all: $(PROGRAM_NAME)

run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)

$(PROGRAM_NAME): $(TAD_NAME).o $(LIB_ARQUIVO).o $(PROGRAM_NAME).o
	gcc $(TAD_NAME).o $(LIB_BINARIO).o $(LIB_CABECALHO).o $(LIB_CONTEUDO).o $(LIB_ARQUIVO).o $(PROGRAM_NAME).o -o $(PROGRAM_NAME)

$(PROGRAM_NAME).o: $(PROGRAM_NAME).c
	gcc $(PROGRAM_NAME).c -c

$(TAD_NAME).o: $(TAD_NAME).c
	gcc $(TAD_NAME).c -c

$(LIB_ARQUIVO).o: $(LIB_ARQUIVO).c
	gcc $(LIB_ARQUIVO).c -c

$(LIB_CONTEUDO).o: $(LIB_CONTEUDO).c
	gcc $(LIB_CONTEUDO).c -c

$(LIB_CABECALHO).o: $(LIB_CABECALHO).c
	gcc $(LIB_CABECALHO).c -c

$(LIB_BINARIO).o: $(LIB_BINARIO).c
	gcc $(LIB_BINARIO).c -c