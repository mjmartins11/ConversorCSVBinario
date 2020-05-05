#include <stdio.h>
#include "arquivos.h"
#include "binarioNaTela.c"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define SAIR 0

FILE* abrir_arquivo(char nome_do_arquivo[100], char tipo[4]) {
    FILE* arq = fopen(nome_do_arquivo, tipo);
    if (arq == NULL) {
        printf("Falha no processamento do arquivo.");
        exit(1);
    }
    return arq;
}

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
                fclose(arquivo_entrada);
                fclose(arquivo_gerado);
                binarioNaTela(nome_do_arquivo);       
            break;
            case LER_ARQUIVO:
                scanf("%s", nome_do_arquivo);
                arquivo_gerado = abrir_arquivo(nome_do_arquivo, "rb");
                ler_arquivo(arquivo_gerado);
                fclose(arquivo_gerado);
            break;
        }
        scanf("%d", &operacao);
    }

    return  0;
}