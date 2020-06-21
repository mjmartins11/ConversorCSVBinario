#ifndef ARQUIVO_INDICE_H
    #define ARQUIVO_INDICE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include "bebe.h"

    #define TAMANHO_CABECALHO 72
    #define TAMANHO_PAGINA 72

    #define ORDEM 6
    #define LIXO '$'

    void inicializar_cabecalho_indice(FILE* arquivo_de_indice);
    void inserir_pagina(FILE* arquivo_indice, BEBE* bebe, int RRN);

#endif