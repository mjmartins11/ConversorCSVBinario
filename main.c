#include <stdio.h>
#include <stdlib.h>
#include "arquivo_cabecalho.h"

int main(void) {
    FILE* arquivo = fopen("arquivoGerado.bin", "w+b");

    int zero = 0;
    int valor = 14;

    fwrite(&zero, sizeof(char), 1, arquivo);
    fwrite(&valor, sizeof(int), 1, arquivo);
    fwrite(&valor, sizeof(int), 1, arquivo);

    fseek(arquivo, 0, SEEK_SET);
    fread(&valor, sizeof(char), 1, arquivo);
    printf("status: %d\n", valor);
    fread(&valor, sizeof(int), 1, arquivo);
    printf("rrn: %d\n", valor);
    fread(&valor, sizeof(int), 1, arquivo);
    printf("qtd: %d\n", valor);

    return 0;
}