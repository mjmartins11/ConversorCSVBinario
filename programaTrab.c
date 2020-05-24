/**
 * Autores:
 * Bruna Magrini da Cruz, 11218813
 * Marlon José Martins, 10249010
 */

#include <stdio.h>
#include "arquivos.h"
#include "binarioNaTela.h"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define BUSCAR_NO_ARQUIVO 3
#define SAIR 0

int main(void) {
    char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO];
    char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO];
    
    int quantidade_de_campos;

    int operacao;
    scanf("%d", &operacao);

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
        case BUSCAR_NO_ARQUIVO:
            scanf("%s", nome_do_arquivo_bin);

        break;
    }

    return  0;
}