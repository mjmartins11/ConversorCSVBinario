#ifndef ARQUIVO_CABECALHO_H
    #define ARQUIVO_CABECALHO_H

    #include <stdio.h>
    #include <stdlib.h>

    #define TAMANHO_CABECALHO_BIN 128
    #define TAMANHO_CABECALHO_CSV 89
    #define LIXO "$"

    #define INCONSISTENTE 0

/*
    typedef struct registro_cabecalho_ REGISTRO_CABECALHO;

    struct registro_cabecalho_ {
        char status; //ao abrir o arquivo status = 0, ao finalizar o uso status = 1
        int RRNproxRegistro; //indica  o RRN  do  proximo  registro  ser  inserido
        int numeroRegistrosInseridos;
        int numeroRegistrosRemovidos;
        int numeroRegistrosAtualizados;
    };
*/
    //void inicializar_cabecalho(FILE *arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho);
    void inicializar_cabecalho(FILE *arquivo_gerado);
    //void atualizar_status(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, char status);
    void atualizar_status(FILE* arquivo_gerado, char status);
    //void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int quantidade_de_registros);
    void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, int quantidade_de_registros);
    //void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int rrn_proximo_registro);
    void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, int rrn_proximo_registro);
    void atualizar_numero_registros_removidos(FILE* arquivo, int numero_registros_removidos);
    void atualizar_numero_registros_atualizados(FILE* arquivo, int numero_registros_atualizados);

    int rrn_prox_registro(FILE* arquivo);
    int numero_registros_inseridos(FILE* arquivo);
    int numero_registros_removidos(FILE* arquivo);
    int numero_registros_atualizados(FILE* arquivo);

    int verificar_status(FILE* arquivo);
    int quantidade_total_de_registros(FILE * arquivo);

#endif