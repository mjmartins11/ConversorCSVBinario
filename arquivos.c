#include "arquivos.h"

#define TAMANHO_CABECALHO_BIN 128
#define TAMANHO_CABECALHO_CSV 89
#define TAMANHO_REGISTRO_BIN 128 //23 bytes (fixos) + 105 (variaveis)
#define TAMANHO_REGISTRO_CSV 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
#define TAMANHO_MAXIMO_REGISTRO 105 
#define LIXO "$"

typedef struct registro_cabecalho_ REGISTRO_CABECALHO;

struct registro_cabecalho_ {
    char status; //ao abrir o arquivo status = 0, ao finalizar o uso status = 1
    int RRNproxRegistro; //indica  o RRN  do  proximo  registro  ser  inserido
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
};

void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO_CSV]) {
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

void escrevar_arquivo_bin(FILE* arquivo_gerado, BEBE* bebe, int rrn_proximo_registro) {
    fseek(arquivo_gerado, (TAMANHO_REGISTRO_BIN * rrn_proximo_registro) + TAMANHO_CABECALHO_BIN, SEEK_SET); 
    // fwrite(bebe_get_idNascimento(bebe), sizeof(int), 1, arquivo_gerado);
    // fwrite(bebe_get_idadeMae(bebe), sizeof(int), 1, arquivo_gerado);
    printf("idNascimeto: %d\n", bebe_get_idNascimento(bebe));
    printf("idadeMae: %d\n", bebe_get_idadeMae(bebe));
    printf("dataNascimento: %s\n", bebe_get_dataNascimento(bebe));
    printf("sexoBebe: %c\n", bebe_get_sexoBebe(bebe));
    printf("estadoMae: %s\n", bebe_get_estadoMae(bebe));
    printf("estadoBebe: %s\n", bebe_get_estadoBebe(bebe));
    printf("cidadeMae: %s\n", bebe_get_cidadeMae(bebe));
    printf("cidadeBebe: %s\n", bebe_get_cidadeBebe(bebe));
}

void atualizar_status(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, char status) {
    (*registro_cabecalho).status = status;
    fseek(arquivo_gerado, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, arquivo_gerado);
}

void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int quantidade_de_registros) {
    (*registro_cabecalho).numeroRegistrosInseridos = quantidade_de_registros; 
    fseek(arquivo_gerado, 5, SEEK_SET);
    fwrite(&quantidade_de_registros, sizeof(int), 1, arquivo_gerado);
}

void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int rrn_proximo_registro) {
    (*registro_cabecalho).RRNproxRegistro = rrn_proximo_registro;
    fseek(arquivo_gerado, 1, SEEK_SET);
    fwrite(&rrn_proximo_registro, sizeof(int), 1, arquivo_gerado);
}

void inicializar_cabecalho(FILE *arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho) {
    int i = 0;
    (*registro_cabecalho).status = 0;
    (*registro_cabecalho).RRNproxRegistro = 0;
    (*registro_cabecalho).numeroRegistrosInseridos = 0;
    (*registro_cabecalho).numeroRegistrosRemovidos = 0;
    (*registro_cabecalho).numeroRegistrosAtualizados = 0;

    atualizar_status(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).status);
    atualizar_rrn_proximo_registro(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).RRNproxRegistro);
    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).numeroRegistrosInseridos);
    fwrite(&(*registro_cabecalho).numeroRegistrosRemovidos, sizeof(int), 1, arquivo_gerado);
    fwrite(&(*registro_cabecalho).numeroRegistrosAtualizados, sizeof(int), 1, arquivo_gerado);
    for(i = 0; i < 111; i++) 
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
}

void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado) {
    REGISTRO_CABECALHO registro_cabecalho;
    BEBE* bebe;
    char cabecalho_csv[TAMANHO_CABECALHO_CSV];
    char registro[TAMANHO_REGISTRO_CSV];
    char *retorno;
    int quantidade_de_registros = 0;

    inicializar_cabecalho(arquivo_gerado, &registro_cabecalho);

    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv
    printf("%s", cabecalho_csv);

    retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    while (retorno != NULL) {    
        printf("%s", registro);
        
        ler_arquivo_csv(&bebe, registro);   
        escrevar_arquivo_bin(arquivo_gerado, bebe, quantidade_de_registros);
        quantidade_de_registros++;
        atualizar_rrn_proximo_registro(arquivo_gerado, &registro_cabecalho, quantidade_de_registros);

        bebe_apagar(&bebe);
        retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    } 

    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, &registro_cabecalho, quantidade_de_registros);
    atualizar_status(arquivo_gerado, &registro_cabecalho, 1);
}