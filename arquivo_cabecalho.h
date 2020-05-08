#ifndef ARQUIVO_CABECALHO_H
    #define ARQUIVO_CABECALHO_H

    #include <stdio.h>
    #include <stdlib.h>

    #define TAMANHO_CABECALHO_BIN 128
    #define TAMANHO_CABECALHO_CSV 89
    #define LIXO "$"

    typedef struct registro_cabecalho_ REGISTRO_CABECALHO;

    struct registro_cabecalho_ {
        char status; //ao abrir o arquivo status = 0, ao finalizar o uso status = 1
        int RRNproxRegistro; //indica  o RRN  do  proximo  registro  ser  inserido
        int numeroRegistrosInseridos;
        int numeroRegistrosRemovidos;
        int numeroRegistrosAtualizados;
    };

    void inicializar_cabecalho(FILE *arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho);
    void atualizar_status(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, char status);
    void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int quantidade_de_registros);
    void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int rrn_proximo_registro);


#endif