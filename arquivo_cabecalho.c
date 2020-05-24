#include "arquivo_cabecalho.h"

/**
 * Recebe um arquivo .bin e uma struct REGISTRO_CABECALHO
 * Inicializa a struct REGISTRO_CABECALHO e escreve o cabeçalho no arquivo .bin
 */
void inicializar_cabecalho(FILE *arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho) {
    int i = 0;
    (*registro_cabecalho).status[0] = '0';
    (*registro_cabecalho).RRNproxRegistro = 0;
    (*registro_cabecalho).numeroRegistrosInseridos = 0;
    (*registro_cabecalho).numeroRegistrosRemovidos = 0;
    (*registro_cabecalho).numeroRegistrosAtualizados = 0;

    atualizar_status(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).status);
    atualizar_rrn_proximo_registro(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).RRNproxRegistro);
    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).numeroRegistrosInseridos);
    fwrite(&(*registro_cabecalho).numeroRegistrosRemovidos, sizeof(int), 1, arquivo_gerado);
    fwrite(&(*registro_cabecalho).numeroRegistrosAtualizados, sizeof(int), 1, arquivo_gerado);
    for(i = 0; i < 111; i++) /*!< O cabeçalho contém 111 bytes preenchidos com lixo ($). Eles são escritos aqui */
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor do status
 * Atualiza o valor de status na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento status
 */
void atualizar_status(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, char status[1]) {
    (*registro_cabecalho).status[0] = status[0]; /*!< Atualizando struct */
    fseek(arquivo_gerado, 0, SEEK_SET); 
    fwrite(status, sizeof(char), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor do RRNproxRegistro
 * Atualiza o valor de RRNproxRegistro na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento RRNproxRegistro
 */
void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int rrn_proximo_registro) {
    (*registro_cabecalho).RRNproxRegistro = rrn_proximo_registro; /*!< Atualizando struct */
    fseek(arquivo_gerado, 1, SEEK_SET);
    fwrite(&rrn_proximo_registro, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor da quantidade de registros
 * Atualiza o valor da quantidade de registros na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento quantidade de registros
 */
void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int quantidade_de_registros) {
    (*registro_cabecalho).numeroRegistrosInseridos = quantidade_de_registros; /*!< Atualizando struct */
    fseek(arquivo_gerado, 5, SEEK_SET);
    fwrite(&quantidade_de_registros, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}


int verificar_status(FILE* arquivo) {
    if (arquivo == NULL)
        return 0;

    char status[1];
    fread(status, sizeof(char), 1, arquivo);
    if (status[0] == '0')
        return INCONSISTENTE;
    
    return 1;
}

int quantidade_registro(FILE* arquivo) {
    if (arquivo == NULL)
        return 0;

    int byteoffset_numeroRegistroInseridos = 5; /*!< Posição do dado "numeroRegistroInseridos" no cabeçalho do arquivo */
    int numeroRegistrosInseridos, numeroRegistrosRemovidos;
    int quantidade_de_registros;
    
    fseek(arquivo, byteoffset_numeroRegistroInseridos, SEEK_SET);
    fread(&numeroRegistrosInseridos, sizeof(int), 1, arquivo);
    fread(&numeroRegistrosRemovidos,  sizeof(int), 1, arquivo);

    quantidade_de_registros = numeroRegistrosInseridos - numeroRegistrosRemovidos;

    return quantidade_de_registros;
}

int rrn_prox_registro(FILE* arquivo) {
    if (arquivo == NULL)
        return -1;

    int proximo_rrn;
    fseek(arquivo, 1, SEEK_SET);
    fread(&proximo_rrn, sizeof(char), 1, arquivo);

    return proximo_rrn;
}