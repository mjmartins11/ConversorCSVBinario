#ifndef ARQUIVO_INDICE_H
    #define ARQUIVO_INDICE_H

    #include <stdio.h>
    #include <stdlib.h>

    #define TAMANHO_CABECALHO 72
    #define TAMANHO_PAGINA 72

    #define ORDEM 6

    void inicializar_cabecalho_indice(FILE* arquivo_de_indice);
    int ler_cabecalho(FILE* arquivo_de_indice, int byteoffset);

    void inserir_chave(FILE* arquivo_indice, int idNascimento, int RRN);
    int buscar_chave(FILE* arquivo_indice, int idNascimento, int RRN, int *quantidade_de_paginas);

#endif