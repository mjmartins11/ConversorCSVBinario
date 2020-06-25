#ifndef ARQUIVOS_H
    #define ARQUIVOS_H

    #include <stdio.h>
    #include <string.h> 
    #include "bebe.h"
    #include "arquivo_cabecalho.h"
    #include "arquivo_conteudo.h"
    #include "arquivo_indice.h"

    #define TAMANHO_NOME_ARQUIVO 100

    #define VALIDO 2

    int criar_arquivo(char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]);
    int ler_arquivo(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]);
    int busca_rrn(char nome_arquivo[TAMANHO_NOME_ARQUIVO], int rrn);
    int busca_por_campos(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* bebe_validacao);
    int remover_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* busca_combinada);
    int inserir_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* bebe);
    int atualizar_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], int rrn_busca, BEBE* bebe);
    int criar_arvore_b(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_indice[TAMANHO_NOME_ARQUIVO]);

#endif