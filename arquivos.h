#ifndef ARQUIVOS_H
    #define ARQUIVOS_H

    #include <stdio.h>
    #include <string.h> 
    #include "bebe.h"
    #include "arquivo_cabecalho.h"
    #include "arquivo_conteudo.h"

    void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado);
    void ler_arquivo(FILE* arquivo_entrada);

#endif