#include "arquivos.h"

#define TAMANHO_CABECALHO_CSV 89
#define TAMANHO_REGISTRO 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
#define TAMANHO_MAXIMO_REGISTRO 105 

void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO]) {
    int byteoffsetArquivo = 0;
    int byteoffsetRegistro = 0;

    int idNascimento, idadeMae;
    char auxiliarParaInteiro[10];
    char sexoBebe;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO];
    char estadoMae[TAMANHO_ESTADO];
    char estadoBebe[TAMANHO_ESTADO];
    char *cidadeMae;
    char *cidadeBebe;

    cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));

    byteoffsetRegistro = 0;
    if(registro[byteoffsetArquivo] == ',') {
        cidadeMae[byteoffsetRegistro] = '\0';
    } else {
        while(registro[byteoffsetArquivo] != ',') {
            cidadeMae[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
            byteoffsetRegistro++;
        }
        cidadeMae[byteoffsetRegistro] = '\0';
    }

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        cidadeBebe[byteoffsetRegistro] = '\0';
    } else {
        while(registro[byteoffsetArquivo] != ',') {
            cidadeBebe[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
            byteoffsetRegistro++;
        }
        cidadeBebe[byteoffsetRegistro] = '\0';
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        idNascimento = -1;
    } else {
        while(registro[byteoffsetArquivo] != ',') {
            auxiliarParaInteiro[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
            byteoffsetRegistro++;
        }
        auxiliarParaInteiro[byteoffsetRegistro] = '\0';
        idNascimento = atoi(auxiliarParaInteiro);
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        idadeMae = -1;
    } else {
        while(registro[byteoffsetArquivo] != ',') {
            auxiliarParaInteiro[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
            byteoffsetRegistro++;
        }
        auxiliarParaInteiro[byteoffsetRegistro] = '\0';
        idadeMae = atoi(auxiliarParaInteiro);
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        dataNascimento[byteoffsetRegistro] = '\0';
    } else {
        //O TAMANHO_DATA_NASCIMENTO considera o \0
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_DATA_NASCIMENTO-1); byteoffsetRegistro++) { 
            dataNascimento[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
        }
        dataNascimento[byteoffsetRegistro] = '\0';
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        sexoBebe = '\0';
    } else {
        sexoBebe = registro[byteoffsetArquivo];
        byteoffsetArquivo++;
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        estadoMae[byteoffsetRegistro] = '\0';
    } else {
        //O TAMANHO_ESTADO considera o \0 
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_ESTADO-1); byteoffsetRegistro++) {
            estadoMae[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
        }
        estadoMae[byteoffsetRegistro] = '\0';
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        estadoBebe[byteoffsetRegistro] = '\0';
    } else {
        //O TAMANHO_ESTADO considera o \0 
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_ESTADO-1); byteoffsetRegistro++) {
            estadoBebe[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
        }
        estadoBebe[byteoffsetRegistro] = '\0';
    }

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

void escrevar_arquivo_bin(FILE* arquivo_gerado, BEBE* bebe) {
    printf("idNascimeto: %d\n", bebe_get_idNascimento(bebe));
    printf("idadeMae: %d\n", bebe_get_idadeMae(bebe));
    printf("dataNascimento: %s\n", bebe_get_dataNascimento(bebe));
    printf("sexoBebe: %c\n", bebe_get_sexoBebe(bebe));
    printf("estadoMae: %s\n", bebe_get_estadoMae(bebe));
    printf("estadoBebe: %s\n", bebe_get_estadoBebe(bebe));
    printf("cidadeMae: %s\n", bebe_get_cidadeMae(bebe));
    printf("cidadeBebe: %s\n", bebe_get_cidadeBebe(bebe));
}

atualizar_status(FILE* arquivo_gerado, int status) {
    fseek(arquivo_gerado, 0, SEEK_SET);
    frwite(&status, sizeof(int), 1, arquivo_gerado);
}

void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado) {
    char cabecalho_csv[TAMANHO_CABECALHO_CSV];
    char registro[TAMANHO_REGISTRO];
    char *retorno;
    BEBE* bebe;

    atualizar_status(arquivo_gerado, 0);
        
    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv
    printf("%s", cabecalho_csv);

    retorno = fgets(registro, TAMANHO_REGISTRO, arquivo_entrada);
    while (retorno != NULL) {    
        printf("%s", registro);
        
        ler_arquivo_csv(&bebe, registro);   
        escrevar_arquivo_bin(arquivo_gerado, bebe);

        bebe_apagar(&bebe);
        retorno = fgets(registro, TAMANHO_REGISTRO, arquivo_entrada);
    } 
}