#include <stdio.h>
#include "arquivos.h"
#include "binarioNaTela.h"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define SAIR 0

int main(void) {
    char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO];
    char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO];

    int operacao;

    scanf("%d", &operacao);
    while(operacao != SAIR) {
        switch(operacao) {
            case CRIAR_ARQUIVO: 
                scanf("%s %s", nome_do_arquivo_csv, nome_do_arquivo_bin);
                if(!criar_arquivo(nome_do_arquivo_csv, nome_do_arquivo_bin)) {
                    printf("Falha no carregamento do arquivo.\n");
                    return 0;
                }
                binarioNaTela(nome_do_arquivo_bin);       
            break;
            case LER_ARQUIVO:
                scanf("%s", nome_do_arquivo_bin);
                if(!ler_arquivo(nome_do_arquivo_bin)){
                    printf("Falha no processamento do arquivo.\n");
                    return 0;
                }
            break;
        }
        scanf("%d", &operacao);
    }

    return  0;
}