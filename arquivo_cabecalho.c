#include "arquivo_cabecalho.h"

/**
 * Recebe um arquivo .bin e uma struct REGISTRO_CABECALHO
 * Inicializa a struct REGISTRO_CABECALHO e escreve o cabeçalho no arquivo .bin
 */
void inicializar_cabecalho(FILE *arquivo_gerado) {
    int i = 0;
    atualizar_status(arquivo_gerado, '0');
    atualizar_rrn_proximo_registro(arquivo_gerado, 0);
    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, 0);
    atualizar_numero_registros_removidos(arquivo_gerado, 0);
    atualizar_numero_registros_atualizados(arquivo_gerado, 0);
    for(i = 0; i < 111; i++) /*!< O cabeçalho contém 111 bytes preenchidos com lixo ($). Eles são escritos aqui */
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor do status
 * Atualiza o valor de status na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento status
 */
void atualizar_status(FILE* arquivo_gerado, char status) {
    fseek(arquivo_gerado, 0, SEEK_SET); 
    fwrite(&status, sizeof(char), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor do RRNproxRegistro
 * Atualiza o valor de RRNproxRegistro na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento RRNproxRegistro
 */
void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, int rrn_proximo_registro) {
    fseek(arquivo_gerado, 1, SEEK_SET);
    fwrite(&rrn_proximo_registro, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin, uma struct REGISTRO_CABECALHO e o novo valor da quantidade de registros
 * Atualiza o valor da quantidade de registros na struct REGISTRO_CABECALHO e no arquivo .bin para o valor do argumento quantidade de registros
 */
void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, int quantidade_de_registros) {
    fseek(arquivo_gerado, 5, SEEK_SET);
    fwrite(&quantidade_de_registros, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

void atualizar_numero_registros_removidos(FILE* arquivo, int numero_registros_removidos) {
    fseek(arquivo, 9, SEEK_SET);
    fwrite(&numero_registros_removidos, sizeof(int), 1, arquivo);
}

void atualizar_numero_registros_atualizados(FILE* arquivo, int numero_registros_atualizados) {
    fseek(arquivo, 13, SEEK_SET);
    fwrite(&numero_registros_atualizados, sizeof(int), 1, arquivo);
}

int buscar_no_cabecalho(FILE* arquivo, int seek) {
    if (arquivo != NULL) {
        int valor_buscado;
        fseek(arquivo, seek, SEEK_SET);
        fread(&valor_buscado, sizeof(int), 1, arquivo);
        return valor_buscado;
    }
    return -1;
}

int rrn_prox_registro(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 1);
}

int numero_registros_inseridos(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 5);
}

int numero_registros_removidos(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 9);
}

int numero_registros_atualizados(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 13);
}

int verificar_status(FILE* arquivo) {
    if (arquivo == NULL)
        return 0;

    char status;
    fseek(arquivo, 0, SEEK_SET);
    fread(&status, sizeof(char), 1, arquivo);
    if (status == '0')
        return INCONSISTENTE;
    
    return 1;
}

int quantidade_total_de_registros(FILE * arquivo) {
    if (arquivo == NULL)
        return 0;

    int byteoffset_numeroRegistroInseridos = 5; /*!< Posição do dado "numeroRegistroInseridos" no cabeçalho do arquivo */
    int numeroRegistrosInseridos, numeroRegistrosRemovidos;
    int quantidade_de_registros;
    
    fseek(arquivo, byteoffset_numeroRegistroInseridos, SEEK_SET);
    fread(&numeroRegistrosInseridos, sizeof(int), 1, arquivo);
    fread(&numeroRegistrosRemovidos,  sizeof(int), 1, arquivo);

    quantidade_de_registros = numeroRegistrosInseridos + numeroRegistrosRemovidos;

    return quantidade_de_registros;
}