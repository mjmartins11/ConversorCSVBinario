#include "arquivo_cabecalho.h"

/**
 * Recebe um arquivo .bin e inicializa o cabeçalho no arquivo.
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
 * Recebe um arquivo .bin e o novo status.
 * Atualiza o status no arquivo.
 */
void atualizar_status(FILE* arquivo_gerado, char status) {
    if(arquivo_gerado != NULL) { 
        fseek(arquivo_gerado, 0, SEEK_SET); 
        fwrite(&status, sizeof(char), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
    }
}

/**
 * Recebe um arquivo .bin e o novo rrnProxRegistro.
 * Atualiza o rrnProxRegistro no arquivo.
 */
void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, int rrn_proximo_registro) {
    fseek(arquivo_gerado, 1, SEEK_SET);
    fwrite(&rrn_proximo_registro, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin e a nova quantidade de registros inseridos.
 * Atualiza a quantidade de registros inseridos no arquivo.
 */
void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, int quantidade_de_registros) {
    fseek(arquivo_gerado, 5, SEEK_SET);
    fwrite(&quantidade_de_registros, sizeof(int), 1, arquivo_gerado); /*!< Atualizando arquivo .bin */
}

/**
 * Recebe um arquivo .bin e a nova quantidade de registros removidos.
 * Atualiza a quantidade de registros removidos no arquivo.
 */
void atualizar_numero_registros_removidos(FILE* arquivo, int numero_registros_removidos) {
    fseek(arquivo, 9, SEEK_SET);
    fwrite(&numero_registros_removidos, sizeof(int), 1, arquivo);
}

/**
 * Recebe um arquivo .bin e a nova quantidade de registros atualizados.
 * Atualiza a quantidade de registros atualizados no arquivo.
 */
void atualizar_numero_registros_atualizados(FILE* arquivo, int numero_registros_atualizados) {
    fseek(arquivo, 13, SEEK_SET);
    fwrite(&numero_registros_atualizados, sizeof(int), 1, arquivo);
}

/**
 * Recebe um arquivo .bin e a posição para onde deve ser feito o seek.
 * Retorna o valor inteiro localizado na posição de seek ou -1.
 */
int buscar_no_cabecalho(FILE* arquivo, int seek) {
    if (arquivo != NULL) {
        int valor_buscado;
        fseek(arquivo, seek, SEEK_SET);
        fread(&valor_buscado, sizeof(int), 1, arquivo);
        return valor_buscado;
    }
    return -1;
}

/**
 * Recebe um arquivo .bin e busca o rrnProxRegistro.
 */
int rrn_prox_registro(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 1);
}

/**
 * Recebe um arquivo .bin e busca o rrnProxRegistro.
 */
int numero_registros_inseridos(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 5);
}

/**
 * Recebe um arquivo .bin e busca a quantidade de registros removidos.
 */
int numero_registros_removidos(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 9);
}

/**
 * Recebe um arquivo .bin e busca a quantidade de registros atualizados.
 */
int numero_registros_atualizados(FILE* arquivo) {
    return buscar_no_cabecalho(arquivo, 13);
}

/**
 * Recebe um arquivo .bin e verifica se o arquivo está consistente.
 */
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

/**
 * Recebe um arquivo .bin e a quantidade total de registros (removidos ou não) do arquivo.
 */
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