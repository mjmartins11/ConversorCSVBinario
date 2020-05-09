#include "arquivo_conteudo.h"

void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO_CSV]) {
    int byteoffsetArquivo = 0;
    int byteoffsetRegistro = 0;

    int idNascimento, idadeMae;
    char auxiliarParaInteiro[10];
    char sexoBebe[2];
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
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
        sexoBebe[0] = '\0';
    } else {
        sexoBebe[0] = registro[byteoffsetArquivo];
        sexoBebe[1] = '\0';
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
    if(registro[byteoffsetArquivo] == '\n') {
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
    int byteoffset_inicial = (TAMANHO_REGISTRO_BIN * rrn_proximo_registro) + BYTEOFFSET_INICIO_CONTEUDO;
    int tamanho_campo_cidadeMae = strlen(bebe_get_cidadeMae(bebe));
    int tamanho_campo_cidadeBebe = strlen(bebe_get_cidadeBebe(bebe));
    int tamanho_campos_variaveis = tamanho_campo_cidadeMae + tamanho_campo_cidadeBebe;
    int quantidade_campos_variaiveis_nao_usados = TAMANHO_MAXIMO_REGISTRO - tamanho_campos_variaveis - (2*sizeof(int));

    int idNascimento = bebe_get_idNascimento(bebe);
    int idadeMae = bebe_get_idadeMae(bebe);
    //char sexoBebe[2];
    //strcpy(sexoBebe, bebe_get_sexoBebe(bebe));

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
    fwrite(bebe_get_sexoBebe(bebe), sizeof(char), 1, arquivo_gerado);
    fwrite(bebe_get_estadoMae(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
    fwrite(bebe_get_estadoBebe(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
}

void ler_registro(FILE* arquivo, int byteoffset_inicial, BEBE** bebe) {
    int idNascimento, idadeMae, tamanho_campo_cidadeMae, tamanho_campo_cidadeBebe;
    char sexoBebe[2];
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
    char *cidadeMae;
    char *cidadeBebe;

    fseek(arquivo, byteoffset_inicial, SEEK_SET);
    fread(&tamanho_campo_cidadeMae, sizeof(int), 1, arquivo);
    fread(&tamanho_campo_cidadeBebe, sizeof(int), 1, arquivo);
    
    if (tamanho_campo_cidadeMae > 0)
        cidadeMae = (char*) malloc((tamanho_campo_cidadeMae+1) * sizeof(char));
    else 
        cidadeMae = (char*) malloc(sizeof(char));
    fread(cidadeMae, sizeof(char), tamanho_campo_cidadeMae, arquivo);
    cidadeMae[tamanho_campo_cidadeMae] = '\0';

    if (tamanho_campo_cidadeBebe > 0)
        cidadeBebe = (char*) malloc((tamanho_campo_cidadeBebe+1) * sizeof(char));
    else 
        cidadeBebe = (char*) malloc(sizeof(char));
    fread(cidadeBebe, sizeof(char), tamanho_campo_cidadeBebe, arquivo);
    cidadeBebe[tamanho_campo_cidadeBebe] = '\0';

    int byteoffset_tamanho_fixo = TAMANHO_MAXIMO_REGISTRO - tamanho_campo_cidadeBebe - tamanho_campo_cidadeMae - 2*sizeof(int);
    
    fseek(arquivo, byteoffset_tamanho_fixo, SEEK_CUR);
    
    fread(&idNascimento, sizeof(int), 1, arquivo);
    fread(&idadeMae, sizeof(int), 1, arquivo);    
    fread(dataNascimento, sizeof(char), TAMANHO_DATA_NASCIMENTO, arquivo);
    dataNascimento[TAMANHO_DATA_NASCIMENTO] = '\0';
    
    fread(sexoBebe, sizeof(char), 1, arquivo);
    sexoBebe[1] = '\0';
    fread(estadoMae, sizeof(char), TAMANHO_ESTADO, arquivo);
    estadoMae[TAMANHO_ESTADO] = '\0';

    fread(estadoBebe, sizeof(char), TAMANHO_ESTADO, arquivo);
    estadoBebe[TAMANHO_ESTADO] = '\0';

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);

    return;
}

void imprimir_registro(BEBE* bebe) {
    if (bebe != NULL) {
        // Nasceu em SAO CARLOS/SP, em 2020-04-18, um bebe de sexo FEMININO.
        // FEMININO = 2; MASCULINO = 1; IGNORADO = 0.

        printf("Nasceu em ");

        char* cidadeBebe = bebe_get_cidadeBebe(bebe);
        (cidadeBebe[0] == '\0') ? printf("-/") : printf("%s/", cidadeBebe);
        
        char estadoBebe[TAMANHO_ESTADO+1];
        strcpy(estadoBebe, bebe_get_estadoBebe(bebe));
        (estadoBebe[0] == '\0') ? printf("-, ") : printf("%s, ", estadoBebe);

        char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
        strcpy(dataNascimento, bebe_get_dataNascimento(bebe));
        (dataNascimento[0] == '\0') ? printf("em -, ") : printf("em %s, ", dataNascimento);

        char sexoBebe[2]; 
        strcpy(sexoBebe, bebe_get_sexoBebe(bebe));
        if (sexoBebe[0] == FEMININO)
            printf("um bebê de sexo FEMININO.\n");
        else if (sexoBebe[0] == MASCULINO)
            printf("um bebê de sexo MASCULINO.\n");
        else
            printf("um bebê de sexo IGNORADO.\n");        

    }
    return;
}