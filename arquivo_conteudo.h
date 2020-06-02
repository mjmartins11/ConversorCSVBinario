#ifndef ARQUIVO_CONTEUDO_H
    #define ARQUIVO_CONTEUDO_H

    #include <stdio.h>
    #include <stdlib.h>
    #include "bebe.h"

    #define BYTEOFFSET_INICIO_CONTEUDO 128

    #define TAMANHO_REGISTRO_BIN 128 //23 bytes (fixos) + 105 (variaveis)
    #define TAMANHO_REGISTRO_CSV 136 //23 (fixos) + 105 (variaveis) + 7 (virgulas) + 1 (quebra de linha)

    #define LIXO "$"
    #define FEMININO '2'
    #define MASCULINO '1'
    #define REMOVIDO -1


    void inicializar_campos_registro(int *idNascimento, int *idadeMae, char *sexoBebe, char dataNascimento[TAMANHO_DATA_NASCIMENTO+1], char estadoMae[TAMANHO_ESTADO+1], char estadoBebe[TAMANHO_ESTADO+1], char *cidadeMae, char *cidadeBebe);

    void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO_CSV]);
    void ler_registro(FILE* arquivo, int byteoffset_inicial, BEBE** bebe);
    void inserir_registro_bin(FILE* arquivo_gerado, BEBE* bebe, int rrn_proximo_registro);
    void imprimir_registro(BEBE* bebe);
    
    int registro_removido(FILE* arquivo, int byteoffset_inicial);
    int bebe_valido_busca_combinada(FILE* arquivo_entrada, int byteoffset, BEBE* busca_combinada, BEBE** bebe);
    int marcar_como_removido(FILE* arquivo, int byteoffset);

#endif