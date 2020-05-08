#ifndef ARQUIVO_CONTEUDO_H
    #define ARQUIVO_CONTEUDO_H

    #include <stdio.h>
    #include <stdlib.h>
    #include "bebe.h"

    #define BYTEOFFSET_INICIO_CONTEUDO 128

    #define TAMANHO_REGISTRO_BIN 128 //23 bytes (fixos) + 105 (variaveis)
    #define TAMANHO_REGISTRO_CSV 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)
    #define TAMANHO_MAXIMO_REGISTRO 105 

    #define LIXO "$"
    #define FEMININO '2'
    #define MASCULINO '1'
    #define REMOVIDO '*'

    void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO_CSV]);
    void escrevar_arquivo_bin(FILE* arquivo_gerado, BEBE* bebe, int rrn_proximo_registro);
    void ler_registro(FILE* arquivo, int byteoffset_inicial, BEBE** bebe);
    void imprimir_registro(BEBE* bebe);

#endif