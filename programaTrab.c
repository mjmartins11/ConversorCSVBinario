#include <stdio.h>
#include "arquivos.h"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define SAIR 0

int main(void) {
    FILE *arquivo_entrada;
    FILE *arquivo_gerado;
    char nome_do_arquivo[100]; //Alterar o tamanho

    int operacao;

    scanf("%d", &operacao);
    while(operacao != SAIR) {
        switch(operacao) {
            case CRIAR_ARQUIVO: 
                scanf("%s", nome_do_arquivo);
                arquivo_entrada = fopen(nome_do_arquivo, "r");
                scanf("%s", nome_do_arquivo);
                arquivo_gerado = fopen(nome_do_arquivo, "w+b");
                criar_arquivo(arquivo_entrada, arquivo_gerado);
            break;
            case LER_ARQUIVO:
                scanf("%s", nome_do_arquivo);
                arquivo_gerado = fopen(nome_do_arquivo, "rb");
            break;
        }
        scanf("%d", &operacao);
    }

    fclose(arquivo_entrada);
    fclose(arquivo_gerado);

    return  0;
}