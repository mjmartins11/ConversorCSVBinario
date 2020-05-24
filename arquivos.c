#include "arquivos.h"

/**
* Recebe um arquivo, o nome dele e o tipo como vai abrir (leitura, escrita, leitura e escrita...)
* e retorna 0 caso não encontre o arquivo e 1 caso contrário.
*/
int abrir_arquivo(FILE** arquivo, char nome_do_arquivo[TAMANHO_NOME_ARQUIVO], char tipo[4]) {
    (*arquivo) = fopen(nome_do_arquivo, tipo);
    if ((*arquivo) == NULL)
        return 0;
    return 1;
}

/**
* Recebe um arquivo e o fecha.
*/
void fechar_arquivo(FILE** arquivo) {
    if ((*arquivo) != NULL)
        fclose(*arquivo);
    return;
}

/**
* Recebe o nome do arquivo csv e o nome do arquivo binário onde serão escritos os dados;
* Faz a verificação se os arquivos abrem normalmente, caso não retorna 0;
* Se os arquivos estiverem todos disponíveis para o acesso então é montado o registro de cabeçalho do arquivo
* binário e em seguida lê linha por linha do arquivo csv obtendo os campos para cada registro a ser
* escrito no arquivo binário. A cada registro transcrito é atualizado as informações no registro de cabeçalho
* por meio de funções da biblioteca "arquivo_cabecalho.h". Também é utilizado funções da biblioteca "arquivo_conteudo.h".
*/
int criar_arquivo(char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada; /*!< Arquivo csv */
    FILE* arquivo_gerado; /*!< Arquivo binário */

    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_csv, "r"))
        return 0;
    
    if(!abrir_arquivo(&arquivo_gerado, nome_do_arquivo_bin, "w+b"))
        return 0;

    REGISTRO_CABECALHO registro_cabecalho;
    BEBE* bebe;
    char cabecalho_csv[TAMANHO_CABECALHO_CSV]; /*!< String utilizada ler o cabeçalho do arquivo csv*/
    char registro[TAMANHO_REGISTRO_CSV]; /*!< String utilizada para armanzenar a linha lida do arquivo csv*/
    char *retorno; /*!< É utilizada para verificar o retorno da leitura do arquivo csv*/
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
    char status[1];
    status[0] = '1';
    atualizar_status(arquivo_gerado, &registro_cabecalho, status);

    fechar_arquivo(&arquivo_entrada);
    fechar_arquivo(&arquivo_gerado);

    return 1;
}

/**
* Recebe o nome do arquivo binário para fazer a leitura.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo é inconsistente (status == 0);
* Com todas as condições favoráveis para a execução da função então ela lê o arquivo binário utilizando funções
* da biblioteca "arquivo_conteudo.h".
*/
int ler_arquivo(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada; /*!< Arquivo binário */

    if (!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r"))
        return 0;
    
    if (verificar_status(arquivo_entrada) == INCONSISTENTE) {
        fechar_arquivo(&arquivo_entrada);
        return 0;
    }

    int quantidade_de_registros = quantidade_registro(arquivo_entrada);
    if (quantidade_de_registros == 0) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro inexistente.\n");
        return 1;        
    }

    int i, byteoffset_inicial_linha; /*!< É utilizado para indicar o início do registro atual */
    for (i = 0; i < quantidade_de_registros; i++) {
        BEBE *bebe;
        byteoffset_inicial_linha = (i * TAMANHO_REGISTRO_BIN) + TAMANHO_CABECALHO_BIN;
        if (!registro_removido(arquivo_entrada, byteoffset_inicial_linha)) {
            ler_registro(arquivo_entrada, byteoffset_inicial_linha, &bebe);
            imprimir_registro(bebe);
            bebe_apagar(&bebe);
        }
    }

    fechar_arquivo(&arquivo_entrada);

    return 1;
}

int busca_por_campos(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* busca_combinada) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "rb"))
        return 0;

    if (verificar_status(arquivo_entrada) == INCONSISTENTE) {
        fechar_arquivo(&arquivo_entrada);
        return 0;
    }
    
    if (quantidade_registro(arquivo_entrada) == 0) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro inexistente.\n");
        return 1;        
    }

    int byteoffset;
    BEBE* bebe;
    for(byteoffset = TAMANHO_CABECALHO_BIN; byteoffset < quantidade_total_de_registros(arquivo_entrada); byteoffset += TAMANHO_REGISTRO_BIN) {
        if(registro_removido(arquivo_entrada, byteoffset)) continue;
        ler_registro(arquivo_entrada, byteoffset, &bebe);

        if(bebe_get_idNascimento(busca_combinada) != -1) 
            if(bebe_get_idNascimento(busca_combinada) != bebe_get_idNascimento(bebe)) continue;
    
        if(bebe_get_idadeMae(busca_combinada) != -1) 
            if(bebe_get_idadeMae(busca_combinada) != bebe_get_idadeMae(bebe)) continue;

        if(strcmp(bebe_get_dataNascimento(busca_combinada), "$") != 0)
            if(strcmp(bebe_get_dataNascimento(busca_combinada), bebe_get_dataNascimento(bebe)) != 0) continue;   

        if(bebe_get_sexoBebe(busca_combinada)[0] != '$') 
            if(strcmp(bebe_get_sexoBebe(busca_combinada), bebe_get_sexoBebe(bebe)) != 0) continue;     

        if(strcmp(bebe_get_estadoMae(busca_combinada), "$") != 0)
            if(strcmp(bebe_get_estadoMae(busca_combinada), bebe_get_estadoMae(bebe)) != 0) continue;   
        
        if(strcmp(bebe_get_estadoBebe(busca_combinada), "$") != 0)
            if(strcmp(bebe_get_estadoBebe(busca_combinada), bebe_get_estadoBebe(bebe)) != 0) continue;   
        
        if(strcmp(bebe_get_cidadeMae(busca_combinada), "$") != 0)
            if(strcmp(bebe_get_cidadeMae(busca_combinada), bebe_get_cidadeMae(bebe)) != 0) continue;   

        if(strcmp(bebe_get_cidadeBebe(busca_combinada), "$") != 0)
            if(strcmp(bebe_get_cidadeBebe(busca_combinada), bebe_get_cidadeBebe(bebe)) != 0) continue;   

        imprimir_registro(bebe);
    }
    return 1;
}


int busca_rrn(char nome_arquivo[TAMANHO_NOME_ARQUIVO], int rrn) {

}

/*
int busca_rrn(char nome_arquivo[TAMANHO_NOME_ARQUIVO], int rrn) {
    FILE* arquivo_entrada; /*!< Arquivo binário  

    if (!abrir_arquivo(&arquivo_entrada, nome_arquivo, "r"))
        return 0;
    
    if (verificar_status(arquivo_entrada) == INCONSISTENTE) {
        fechar_arquivo(&arquivo_entrada);
        return 0;
    }

    if (quantidade_registro(arquivo_entrada) == 0 || rrn >= rrn_prox_registro(arquivo_entrada)) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro inexistente.\n");
        return 1;        
    }
    
    int byteoffset = rrn * TAMANHO_REGISTRO_BIN + TAMANHO_CABECALHO_BIN;    
    if (registro_removido(arquivo_entrada, byteoffset)) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro inexistente.\n");
        return 1;
    }

    BEBE* bebe;
    ler_registro(arquivo_entrada, byteoffset, &bebe);
    imprimir_registro(bebe);
    bebe_apagar(&bebe);

    for (i = 0; i < quantidade_de_registros; i++) {
        byteoffset_inicial_linha = (i * TAMANHO_REGISTRO_BIN) + TAMANHO_CABECALHO_BIN;
        fseek(arquivo_entrada, byteoffset_inicial_linha, SEEK_SET);
        fread(&registro_removido, sizeof(int), 1, arquivo_entrada);
        if (registro_removido == REMOVIDO) {
            if ()
            ler_registro(arquivo_entrada, byteoffset_inicial_linha, &bebe);
            imprimir_registro(bebe);
            bebe_apagar(&bebe);
        }
    } 
}
    fechar_arquivo(&arquivo_entrada);
    
    return 1;
}
*/