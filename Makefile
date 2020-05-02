PROGRAM_NAME=programaTrab
LIB_NAME = arquivos

all: $(PROGRAM_NAME)

run: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)

$(PROGRAM_NAME): $(LIB_NAME).o $(PROGRAM_NAME).o
	gcc $(LIB_NAME).o $(PROGRAM_NAME).o -o $(PROGRAM_NAME)

$(PROGRAM_NAME).o: $(PROGRAM_NAME).c
	gcc $(PROGRAM_NAME).c -c

$(LIB_NAME).o: $(LIB_NAME).c
	gcc $(LIB_NAME).c -c