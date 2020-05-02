#include "arquivos.h"

#define TAMANHO_CABECALHO_CSV 89
#define TAMANHO_REGISTRO 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
#define TAMANHO_MAXIMO_REGISTRO 105 

void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado) {
    char cabecalho_csv[TAMANHO_CABECALHO_CSV];
    char registro[TAMANHO_REGISTRO];
    char *retorno;
    BEBE* bebe;

    int i = 0;

    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv
    printf("%s", cabecalho_csv);

    do {
        retorno = fgets(registro, TAMANHO_REGISTRO, arquivo_entrada);
        printf("%s", registro);

        if(registro[i] == ','){
            printf("cidadeMae: \n");
        } else {
            printf("cidadeMae: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("cidadeBebe: \n");
        } else {
            printf("cidadeBebe: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("idNascimento: \n");
        } else {
            printf("idNascimento: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("idadeMae: \n");
        } else {
            printf("idadeMae: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("dataNascimento: \n");
        } else {
            printf("dataNascimento: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("sexoBebe: \n");
        } else {
            printf("sexoBebe: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

                i++;
        if(registro[i] == ','){
            printf("estadoMae: \n");
        } else {
            printf("estadoMae: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }

        i++;
        if(registro[i] == ','){
            printf("estadoBebe: \n");
        } else {
            printf("estadoBebe: ");
            while(registro[i] != ',') {
                printf("%c", registro[i]);
                i++;
            }
            printf("\n");
        }


        // token = strtok(registro, ",");  //cidadeMae
        // printf("cidadeMae: %s\n", token);
        // token = strtok(NULL, ","); //cidadeBebe
        // printf("cidadeBebe: %s\n", token);
        // token = strtok(NULL, ","); //idNascimento
        // printf("idNascimento: %s\n", token);
        // token = strtok(NULL, ","); //idadeMae
        // printf("idadeMae: %s\n", token);
        // token = strtok(NULL, ","); //dataNascimento
        // printf("dataNascimento: %s\n", token);
        // token = strtok(NULL, ","); //sexoBebe
        // printf("sexoBebe: %s\n", token);
        // token = strtok(NULL, ","); //estadoMae
        // printf("estadoMae: %s\n", token);
        // token = strtok(NULL, ","); //estadoBebe
        // printf("estadoBebe: %s\n", token);
        // printf("\n----------------\n"); 

    } while (retorno != NULL);

}

/*
CACHOEIRA DO SUL,CACHOEIRA DO SUL,1,33,2016-01-01,2,RS,RS
IPAMERI,IPAMERI,2,25,2016-01-01,1,GO,GO
COLINAS DO TOCANTINS,ARAGUAINA,3,19,2016-01-02,1,TO,TO
SANHARO,SANHARO,4,29,2016-01-02,1,PE,PE
MANAUS,MANAUS,5,25,2016-01-03,2,AM,AM
UBERLANDIA,UBERLANDIA,6,17,2016-01-03,2,,MG
RIO DE JANEIRO,RIO DE JANEIRO,7,21,2016-01-03,1,RJ,RJ
PALHOCA,FLORIANOPOLIS,8,17,2016-01-05,1,SC,SC
*/