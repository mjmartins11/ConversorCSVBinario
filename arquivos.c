#include "arquivos.h"

#define TAMANHO_CABECALHO_BIN 128
#define TAMANHO_CABECALHO_CSV 89
#define TAMANHO_REGISTRO_BIN 128 //23 bytes (fixos) + 105 (variaveis)
#define TAMANHO_REGISTRO_CSV 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
#define TAMANHO_MAXIMO_REGISTRO 105 
#define LIXO "$"
#define FEMININO '2'
#define MASCULINO '1'


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
    char dataNascimento[TAMANHO_DATA_NASCIMENTO + 1];
    char estadoMae[TAMANHO_ESTADO + 1];
    char estadoBebe[TAMANHO_ESTADO + 1];
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
        dataNascimento[0] = '\0';
        for(byteoffsetRegistro = 1; byteoffsetRegistro < TAMANHO_DATA_NASCIMENTO; byteoffsetRegistro++) 
            dataNascimento[byteoffsetRegistro] = '$';
    } else {
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_DATA_NASCIMENTO); byteoffsetRegistro++) { 
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
        estadoMae[0] = '\0';
        estadoMae[1] = '$';
    } else {
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_ESTADO); byteoffsetRegistro++) {
            estadoMae[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
        }
        estadoMae[byteoffsetRegistro] = '\0';
    }  

    byteoffsetRegistro = 0;
    byteoffsetArquivo++;
    if(registro[byteoffsetArquivo] == ',') {
        estadoBebe[0] = '\0';
        estadoBebe[1] = '$';
    } else {
        for(byteoffsetRegistro = 0; byteoffsetRegistro < (TAMANHO_ESTADO); byteoffsetRegistro++) {
            estadoBebe[byteoffsetRegistro] = registro[byteoffsetArquivo];
            byteoffsetArquivo++;
        }
        estadoBebe[byteoffsetRegistro] = '\0';
    }

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

void escrevar_arquivo_bin(FILE* arquivo_gerado, BEBE* bebe, int rrn_proximo_registro) {
    int i = 0;
    int byteoffset_inicial = (TAMANHO_REGISTRO_BIN * rrn_proximo_registro) + TAMANHO_CABECALHO_BIN;
    int tamanho_campo_cidadeMae = strlen(bebe_get_cidadeMae(bebe));
    int tamanho_campo_cidadeBebe = strlen(bebe_get_cidadeBebe(bebe));
    int tamanho_campos_variaveis = tamanho_campo_cidadeMae + tamanho_campo_cidadeBebe;
    int quantidade_campos_variaiveis_nao_usados = TAMANHO_MAXIMO_REGISTRO - tamanho_campos_variaveis;

    int idNascimento = bebe_get_idNascimento(bebe);
    int idadeMae = bebe_get_idadeMae(bebe);
    char sexoBebe = bebe_get_sexoBebe(bebe);

    fseek(arquivo_gerado, byteoffset_inicial, SEEK_SET);
    fwrite(&tamanho_campo_cidadeMae, sizeof(int), 1, arquivo_gerado);
    fwrite(&tamanho_campo_cidadeBebe, sizeof(int), 1, arquivo_gerado);
    if(tamanho_campo_cidadeMae > 0)
        fwrite(bebe_get_cidadeMae(bebe), sizeof(char), tamanho_campo_cidadeMae, arquivo_gerado);
    if(tamanho_campo_cidadeBebe > 0)
        fwrite(bebe_get_cidadeBebe(bebe), sizeof(char), tamanho_campo_cidadeBebe, arquivo_gerado);
    for(i = 0; i < quantidade_campos_variaiveis_nao_usados; i++) 
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
    fwrite(&idNascimento, sizeof(int), 1, arquivo_gerado);
    fwrite(&idadeMae, sizeof(int), 1, arquivo_gerado);
    fwrite(bebe_get_dataNascimento(bebe), sizeof(char), TAMANHO_DATA_NASCIMENTO, arquivo_gerado);
    fwrite(&sexoBebe, sizeof(char), 1, arquivo_gerado);
    fwrite(bebe_get_estadoMae(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
    fwrite(bebe_get_estadoBebe(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
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

    retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    while (retorno != NULL) {    
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

void ler_linha_arquivo(FILE* arquivo, int byteoffset_inicial, BEBE** bebe) {
    int idNascimento, idadeMae, tamanho_campo_cidadeMae, tamanho_campo_cidadeBebe;
    char sexoBebe;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO + 1];
    char estadoMae[TAMANHO_ESTADO + 1];
    char estadoBebe[TAMANHO_ESTADO + 1];
    char *cidadeMae;
    char *cidadeBebe;

    fseek(arquivo, byteoffset_inicial, SEEK_SET);
    fread(&tamanho_campo_cidadeMae, sizeof(int), 1, arquivo);
    printf("tcdm = %d\n", tamanho_campo_cidadeMae);
    if (tamanho_campo_cidadeMae > 0) 
        cidadeMae = (char*) malloc(tamanho_campo_cidadeMae * sizeof(char));
    else
        cidadeMae = NULL;

    fread(&tamanho_campo_cidadeBebe, sizeof(int), 1, arquivo);
    printf("tcdb = %d\n", tamanho_campo_cidadeBebe);
    if (tamanho_campo_cidadeBebe > 0)
        cidadeBebe = (char*) malloc(tamanho_campo_cidadeBebe * sizeof(char));
    else 
        cidadeBebe = NULL;

    fread(cidadeMae, sizeof(char), tamanho_campo_cidadeMae, arquivo);
    cidadeMae[tamanho_campo_cidadeMae] = '\0';
    printf("cidadeMae = %s\n", cidadeMae);
    fread(cidadeBebe, sizeof(char), tamanho_campo_cidadeBebe, arquivo);
    cidadeBebe[tamanho_campo_cidadeBebe] = '\0';

    int byteoffset_tamanho_fixo = (2*sizeof(int)) + byteoffset_inicial + TAMANHO_MAXIMO_REGISTRO - tamanho_campo_cidadeMae - tamanho_campo_cidadeBebe;

    printf("bostf = %d\n", byteoffset_tamanho_fixo);

    fseek(arquivo, byteoffset_tamanho_fixo, SEEK_SET);
    fread(&idNascimento, sizeof(int), 1, arquivo);
    printf("idNasc = %d\n", idNascimento);
    fread(&idadeMae, sizeof(int), 1, arquivo);
    fread(&sexoBebe, sizeof(char), 1, arquivo);
    fread(estadoMae, sizeof(char), TAMANHO_ESTADO, arquivo);
    fread(estadoBebe, sizeof(char), TAMANHO_ESTADO, arquivo);

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);

    return;
}

void imprimir_registro(BEBE* bebe) {
    if (bebe != NULL) {
        // Nasceu em SAO CARLOS/SP, em 2020-04-18, um bebe de sexo FEMININO.
        // FEMININO = 2; MASCULINO = 1; IGNORADO = 0.

        printf("Nasceu em ");

        char* cidadeBebe = bebe_get_cidadeBebe(bebe);
        (cidadeBebe == NULL) ? printf("-/") : printf("%s/", cidadeBebe);
        
        char estadoBebe[TAMANHO_ESTADO];
        strcpy(estadoBebe, bebe_get_estadoBebe(bebe));
        (estadoBebe[0] == '\0') ? printf("-, ") : printf("%s, ", estadoBebe);

        char dataNascimento[TAMANHO_DATA_NASCIMENTO];
        strcpy(dataNascimento, bebe_get_dataNascimento(bebe));
        (dataNascimento[0] == '\0') ? printf("em -, ") : printf("em %s, ", dataNascimento);

        char sexoBebe = bebe_get_sexoBebe(bebe);
        if (sexoBebe == FEMININO)
            printf("um bebe de sexo FEMININO.\n");
        else if (sexoBebe == MASCULINO)
            printf("um bebe de sexo MASCULINO.\n");
        else
            printf("um bebe de sexo IGNORADO.\n");        

    }
    return;
}

void ler_arquivo(FILE* arquivo_entrada) {
    if (arquivo_entrada != NULL) {
        int byteoffset_numeroRegistroInseridos = 5;
        int byteoffset_numeroRegistrosRemovidos = 9;
        int numeroRegistrosInseridos, numeroRegistrosRemovidos;
        int quantidade_de_registros;
        
        fseek(arquivo_entrada, byteoffset_numeroRegistroInseridos, SEEK_SET);
        fread(&numeroRegistrosInseridos, sizeof(int), 1, arquivo_entrada);
        fread(&numeroRegistrosRemovidos,  sizeof(int), 1, arquivo_entrada);

        //printf("nri = %d\nnrr = %d\n", numeroRegistrosInseridos, numeroRegistrosRemovidos);

        quantidade_de_registros = numeroRegistrosInseridos - numeroRegistrosRemovidos;

        //printf("qtdr = %d\n", quantidade_de_registros);

        int i, byteoffset_inicial_linha;
        for (i = 0; i < quantidade_de_registros; i++) {
            BEBE *bebe;
            byteoffset_inicial_linha = (i * TAMANHO_REGISTRO_BIN) + TAMANHO_CABECALHO_BIN;
            //printf("bosil = %d\n", byteoffset_inicial_linha);
            ler_linha_arquivo(arquivo_entrada, byteoffset_inicial_linha, &bebe);
            //printf("bosildeucerto\n");
            imprimir_registro(bebe);
            bebe_apagar(&bebe);
        }
    }
    return;
}