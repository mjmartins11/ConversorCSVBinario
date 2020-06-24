#include "arquivo_indice.h"

typedef struct page {
    int nivel; /*!< O nível da página*/
    int keycount; /*!< Número de chaves armazenadas na página */
    int key[ORDEM-1]; /*!< As chaves de busca */
    int rrn[ORDEM-1]; /*!< O RRN do registro no arquivo de dados */
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

/**
 * Dado um arquivo de índice, um byteoffset e um valor, escreve no arquivo de índice.
 */
void escrever_cabecalho(FILE* arquivo_de_indice, int byteoffset, int valor) {
    if(arquivo_de_indice != NULL) {
        fseek(arquivo_de_indice, byteoffset, SEEK_SET);
        fwrite(&valor, sizeof(int), 1, arquivo_de_indice);
    }
}

PAGE ler_pagina(FILE* arquivo_indice, int RRN) {
    PAGE page;
    page.keycount = 0;
    if (arquivo_indice != NULL) {
        /*!< Colocando ponteiro do arquivo no local da página */
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
        fread(&(page.nivel), sizeof(int), 1, arquivo_indice);
        fread(&(page.keycount), sizeof(int), 1, arquivo_indice);
        for(int i = 0; i < ORDEM-1; i++) { /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
            fread(&(page.key[i]), sizeof(int), 1, arquivo_indice);
            fread(&(page.rrn[i]), sizeof(int), 1, arquivo_indice);
        }
        for(int i = 0; i < ORDEM; i++) /*!< Lendo os descendentes */
            fread(&(page.child[i]), sizeof(int), 1, arquivo_indice);
        
    }
    return page;
}

int buscar_chave(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        if(RRN != -1) { /*<! Página inexistente */
            PAGE page;
            int i;

            page = ler_pagina(arquivo_indice, RRN);
            for (i = 0; i < page.keycount; i++) /*<! Procurando a key (idNascimento) na página */
                if (idNascimento == page.key[i]) /*<! Achou */
                    return page.rrn[i]; /*<! Retorna referência para o registro no arquivo dados */

            for (i = 0; i < page.keycount; i++) /*<! Procurando o child para continuar a busca da key */
                if (idNascimento < page.key[i])
                    return buscar_pagina(arquivo_indice, idNascimento, page.child[i]);
                    
            return buscar_pagina(arquivo_indice, idNascimento, page.child[i+1]); /*<! Página é a última child */
        }
    }
    return -1;
}

void escrever_pagina(FILE* arquivo_indice, PAGE page, int RRN) {
    if(arquivo_indice != NULL) {
        /*!< Colocando ponteiro do arquivo no local da página */
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
        fwrite(&(page.nivel), sizeof(int), 1, arquivo_indice);
        fwrite(&(page.keycount), sizeof(int), 1, arquivo_indice);
        for(int i = 0; i < ORDEM-1; i++) { /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
            fwrite(&(page.key[i]), sizeof(int), 1, arquivo_indice);
            fwrite(&(page.rrn[i]), sizeof(int), 1, arquivo_indice);
        }
        for(int i = 0; i < ORDEM; i++) /*!< Lendo os descendentes */
            fwrite(&(page.child[i]), sizeof(int), 1, arquivo_indice);
    }
    return;
}

/**
 * O algoritmo do insertion_sort é utilizado para ordenar as páginas de disco
 * Foi escolhido esse algoritmo porque ele funciona muito bem em vetores pequenos
 * como é nosso caso (n = 5), além também de ter um bom desempenho quando 
 * o vetor está quase ordenado (melhor caso), como também é o nosso caso que quando essa função
 * é chamada, ela ordenada um vetor que apenas o último valor está desordenado,
 * o qual é o último que foi inserido.
 * complexidade no melhor caso não passa de n.
 */
void insertion_sort(int keycount, int key[], int rrn[]) {
    for (int i = 1; i < keycount; ++i) {
        int key_atual = key[i];
        int rrn_atual = rrn_atual[i];
        int j;
        for (j = i - 1; j >= 0 && key[j] > key_atual; --j) { 
            key[j+1] = key[j];
            rrn[j+1] = rrn[j]; /*!< Quando muda a chave de posição o rrn precisa acompanhar */
        }
        key[j+1] = key_atual;
        rrn[j+1] = rrn_atual;
    }
}

void particionamento(PAGE pagina1, PAGE pagina2) {
    /*!< Redistribuindo os nós da página existente (pagina1) entre a página existente (pagina1) e a nova página (pagina2) */
    for(int i = pagina1.keycount/2; i < pagina1.keycount; i++) {  
        pagina2.key[i] = pagina1.key[i]; /*!< A pagina2 deve ter a partir da metade das chaves da página1 */
        pagina1.key[i] = '$'; /*!< A pagina1 perde as chaves que passou para pagina2 */
        pagina2.keycount++; /*!< Atualizando a quantidade de chaves da pagina2 */
        pagina1.keycount--;
    }
}

/**
 * Função responsável por inserir uma chave em uma página
 * Recebe como parametro o arquivo de índice, o idNascimento do registro (key) e o RRN do registro no registro de dados
 */
void inserir_chave(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        int noRaiz = ler_cabecalho(arquivo_indice, 1);
        if(noRaiz == -1) { /*!< Inserção em árvore vazia */
            PAGE page;
            page.nivel = 1;
            page.keycount = 1;
            for(int i = 0; i < ORDEM-1; i++) {
                page.key[i] = '$';
                page.rrn[i] = '$';
            }
            page.key[0] = idNascimento;
            page.rrn[0] = RRN;
            for(int i = 0; i < ORDEM; i++)
                page.child[i] = -1;
            escrever_pagina(arquivo_indice, page, 0); /*!< Escrevendo primeira página */
            escrever_cabecalho(arquivo_indice, 5, 1); /*!< Após a criação do primeiro nó, nroNiveis = 1 */
            escrever_cabecalho(arquivo_indice, 9, 1); /*!< Após a criação do primeiro nó, proxRRN = 1 */
            escrever_cabecalho(arquivo_indice, 13, 1); /*!< Após a criação do primeiro nó, nroChaves = 1 */
        } else { /*!< Árvore possui páginas */
            PAGE page = ler_pagina(arquivo_indice, noRaiz); // page = raiz
            /*!< A página (raiz) de nó tem espaço para novas keys e só há essa página na árvore */
            if(ler_cabecalho(arquivo_indice, 5) == 1 && page.keycount < ORDEM-1) { 
                page.key[page.keycount] = idNascimento;
                page.rrn[page.keycount] = RRN;
                page.keycount++;
                insertion_sort(page.keycount, page.key, page.rrn); escrever_cabecalho(arquivo_indice, 5, 1); /*!< Ordenando a lista de chaves */
                escrever_pagina(arquivo_indice, page, noRaiz);
                escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1));
                return;
            }

            if (ler_cabecalho(arquivo_indice, 5) == 1 && page.keycount == ORDEM-1) { /*!< Nó raiz está cheio => split */
                //redistribuição
                        
                return;
            }


        }
    }
    return;
}