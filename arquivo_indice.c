#include "arquivo_indice.h"

typedef struct page {
    int nivel; /*!< O nível da página*/
    int keycount; /*!< Número de chaves armazenadas na página */
    int rrn[ORDEM-1]; /*!< O RRN do registro no arquivo de dados */
    char key[ORDEM-1]; /*!< As chaves de busca */
    int child[ORDEM]; /* O RRN dos nós descendentes */
} PAGE;

/**
 * Recebe como parametro o arquivo de índices.
 * Cria o cabeçalho no arquivo de índices com os valores padrões.
 */
void inicializar_cabecalho_indice(FILE* arquivo_de_indice) {
    if(arquivo_de_indice != NULL) {
        char status = '0', lixo = LIXO;
        int noRaiz = -1, nroNiveis = 0, proxRRN = 0, nroChaves = 0;
        fseek(arquivo_de_indice, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, arquivo_de_indice);
        fwrite(&noRaiz, sizeof(int), 1, arquivo_de_indice);
        fwrite(&nroNiveis, sizeof(int), 1, arquivo_de_indice);
        fwrite(&proxRRN, sizeof(int), 1, arquivo_de_indice);
        fwrite(&nroChaves, sizeof(int), 1, arquivo_de_indice);
        for(int i = 0; i < 55; i++) /*!< Insere lixo no restante do espaço do cabeçalho */
            fwrite(&lixo, sizeof(char), 1, arquivo_de_indice);
    }
    return;
}

/**
 * Recebe como parametro o arquivo de índices.
 * Lê o status do arquivo. 0 se estiver inconsistente e 1 se estiver consistente.
 */
char ler_status(FILE* arquivo_de_indice) {
    char status = '0';
    if(arquivo_de_indice != NULL) {
        fseek(arquivo_de_indice, 0, SEEK_SET);
        fread(&status, sizeof(char), 1, arquivo_de_indice);
    }
    return status;
}

/**
 * Recebe como parametro o arquivo de índices e o byteoffset do registro a ser lido.
 * Lê o registro (inteiro) no byteoffset.
 */
int ler_cabecalho(FILE* arquivo_de_indice, int byteoffset) {
    int valor = -1;
    if(arquivo_de_indice != NULL) {
        fseek(arquivo_de_indice, byteoffset, SEEK_SET);
        fread(&valor, sizeof(int), 1, arquivo_de_indice);
    }
    return valor;
}

PAGE* ler_pagina(FILE* arquivo_indice, int RRN) {
    if (arquivo_indice != NULL) {
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET);
        
        PAGE* page;
        fread(&(page.nivel), sizeof(int), 1, arquivo_indice);
        fread(&(page.keycount), sizeof(int), 1, arquivo_indice);
        for(int i = 0; i < ORDEM-1; i++) {
            fread(&(page.key[i]), sizeof(int), 1, arquivo_indice);
            fread(&(page.rrn[i]), sizeof(int), 1, arquivo_indice);
        }
        for(int i  0; i < ORDEM; i++) 
            fread(&(page.child[i]), sizeof(int), 1, arquivo_indice);
        
        return page;
    }
    return NULL;
}

int buscar_pagina(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        if(RRN != -1) { /*<! Página inexistente */
            PAGE* page;
            int i;

            page = ler_pagina(arquivo_indice, RRN);
            for (i = 0; i < page.keycount; i++) /*<! Procurando a key (idNascimento) na página */
                if (idNascimento == page.key[i]) /*<! Achou */
                    return rrn[i]; /*<! Retorna referência para o registro no arquivo dados */

            for (i = 0; i < page.keycount; i++) /*<! Procurando o child para continuar a busca da key */
                if (idNascimento < page.key[i])
                    return buscar_pagina(arquivo_indice, idNascimento, page.child[i]);
                    
            return buscar_pagina(arquivo_indice, idNascimento, page.child[i+1]); /*<! Página é a última child */
        }
    }
    return -1;
}

void inserir_pagina(FILE* arquivo_indice, BEBE* bebe, int RRN) {
    if(arquivo_indice != NULL) {
        int noRaiz = ler_cabecalho(arquivo_indice, 1);
         = buscar_pagina(arquivo_indice, bebe_get_idNascimento(bebe), noRaiz);

    }
    return;
}