#ifndef ARQUIVOS_H
    #define ARQUIVOS_H

    #include <stdio.h>
    #include <string.h> 
    #include "bebe.h"
    #include "arquivo_cabecalho.h"
    #include "arquivo_conteudo.h"

    #define TAMANHO_NOME_ARQUIVO 100

    int criar_arquivo(char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]);
    int ler_arquivo(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]);
    int busca_rrn(char nome_arquivo[TAMANHO_NOME_ARQUIVO], int rrn);
    int busca_por_campos(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* bebe_validacao);

#endif