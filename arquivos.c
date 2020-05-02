#include "arquivos.h"

#define TAMANHO_CABECALHO_CSV 89
#define TAMANHO_REGISTRO 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
#define TAMANHO_MAXIMO_REGISTRO 105 

void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO]) {
    int i = 0;
    int j = 0;

    int idNascimento, idadeMae;
    char auxiliarParaInteiro[100];
    char sexoBebe;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO];
    char estadoMae[TAMANHO_ESTADO];
    char estadoBebe[TAMANHO_ESTADO];
    char *cidadeMae;
    char *cidadeBebe;

    cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));

    j = 0;
    if(registro[i] == ',') {
        cidadeMae[j] = '\0';
    } else {
        while(registro[i] != ',') {
            cidadeMae[j] = registro[i];
            i++;
            j++;
        }
        cidadeMae[j] = '\0';
    }

    j = 0;
    i++;
    if(registro[i] == ',') {
        cidadeBebe[j] = '\0';
    } else {
        while(registro[i] != ',') {
            cidadeBebe[j] = registro[i];
            i++;
            j++;
        }
        cidadeBebe[j] = '\0';
    }  

    j = 0;
    i++;
    if(registro[j] == ',') {
    } else {
        while(registro[i] != ',') {
            auxiliarParaInteiro[j] = registro[i];
            i++;
            j++;
        }
        auxiliarParaInteiro[j] = '\0';
    }  
    idNascimento = atoi(auxiliarParaInteiro);

    j = 0;
    i++;
    if(registro[j] == ',') {
    } else {
        while(registro[i] != ',') {
            auxiliarParaInteiro[j] = registro[i];
            i++;
            j++;
        }
        auxiliarParaInteiro[j] = '\0';
    }  
    idadeMae = atoi(auxiliarParaInteiro);

    j = 0;
    i++;
    if(registro[i] == ',') {
        dataNascimento[j] = '\0';
    } else {
        //O TAMANHO_DATA_NASCIMENTO considera o \0
        for(j = 0; j < (TAMANHO_DATA_NASCIMENTO-1); j++) { 
            dataNascimento[j] = registro[i];
            i++;
        }
        dataNascimento[j] = '\0';
    }  

    j = 0;
    i++;
    if(registro[i] == ',') {
        sexoBebe = '\0';
    } else {
        sexoBebe = registro[i];
        i++;
    }  

    j = 0;
    i++;
    if(registro[i] == ',') {
        estadoMae[j] = '\0';
    } else {
        //O TAMANHO_ESTADO considera o \0 
        for(j = 0; j < (TAMANHO_ESTADO-1); j++) {
            estadoMae[j] = registro[i];
            i++;
        }
        estadoMae[j] = '\0';
    }  

    j = 0;
    i++;
    if(registro[i] == ',') {
        estadoBebe[j] = '\0';
    } else {
        //O TAMANHO_ESTADO considera o \0 
        for(j = 0; j < (TAMANHO_ESTADO-1); j++) {
            estadoBebe[j] = registro[i];
            i++;
        }
        estadoBebe[j] = '\0';
    }

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado) {
    char cabecalho_csv[TAMANHO_CABECALHO_CSV];
    char registro[TAMANHO_REGISTRO];
    char *retorno;

    BEBE* b;
        
    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv
    printf("%s", cabecalho_csv);

    retorno = fgets(registro, TAMANHO_REGISTRO, arquivo_entrada);
    while (retorno != NULL) {    
        printf("%s", registro);
        
        ler_arquivo_csv(&b, registro);   
        printf("idNascimeto: %d\n", bebe_get_idNascimento(b));
        printf("idadeMae: %d\n", bebe_get_idadeMae(b));
        printf("dataNascimento: %s\n", bebe_get_dataNascimento(b));
        printf("sexoBebe: %c\n", bebe_get_sexoBebe(b));
        printf("estadoMae: %s\n", bebe_get_estadoMae(b));
        printf("estadoBebe: %s\n", bebe_get_estadoBebe(b));
        printf("cidadeMae: %s\n", bebe_get_cidadeMae(b));
        printf("cidadeBebe: %s\n", bebe_get_cidadeBebe(b));

        bebe_apagar(&b);
        retorno = fgets(registro, TAMANHO_REGISTRO, arquivo_entrada);
    } 
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