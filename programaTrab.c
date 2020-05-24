/**
 * Autores:
 * Bruna Magrini da Cruz, 11218813
 * Marlon José Martins, 10249010
 */

#include <stdio.h>
#include <string.h>
#include "arquivos.h"
#include "bebe.h"
#include "binarioNaTela.h"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define BUSCAR_NO_ARQUIVO 3
#define SAIR 0

int main(void) {
    int i;
    char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO];
    char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO];
    int quantidade_de_campos;
    char nome_do_campo[15];
    
    BEBE* bebe = NULL;
    int idNascimento = -1;
    int idadeMae = -1;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1] = "\0";
    char sexoBebe[1] = "\0";
    char estadoMae[TAMANHO_ESTADO+1] = "\0";
    char estadoBebe[TAMANHO_ESTADO+1] = "\0";
    char* cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    char* cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    cidadeMae = "\0";
    cidadeBebe = "\0"; 

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
            scanf("%d", &quantidade_de_campos);
            printf("quantidade: %d\n", quantidade_de_campos);
            for(i = 0; i < quantidade_de_campos; i++) {
                scanf("%s", nome_do_campo);
                if(strcmp("idNascimento", nome_do_campo) == 0) 
                    scanf("%d", &idNascimento);
                else if(strcmp("idadeMae", nome_do_campo) == 0) 
                    scanf("%d", &idadeMae);
                else if(strcmp("dataNascimento", nome_do_campo) == 0) 
                    scanf("%s", dataNascimento);
                else if(strcmp("sexoBebe", nome_do_campo) == 0) 
                    scanf("%c", sexoBebe);
                else if(strcmp("estadoMae", nome_do_campo) == 0) 
                    scanf("%s", estadoMae);
                else if(strcmp("estadoBebe", nome_do_campo) == 0)
                    scanf("%s", estadoBebe);
                else if(strcmp("cidadeMae", nome_do_campo) == 0)
                    scanf("%s", cidadeMae);
                else if(strcmp("cidadeBebe", nome_do_campo) == 0)
                    scanf("%s", cidadeBebe);
                else 
                    printf("Campo inválido.\n");
            }
            bebe = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
            busca_por_campos(nome_do_arquivo_bin, bebe);
            // bebe_apagar(&bebe);
        break;
    }

    return 0;
}