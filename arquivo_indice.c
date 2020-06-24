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

PAGE *ler_pagina(FILE* arquivo_indice, int RRN) {
    PAGE *page = NULL;
    (*page).keycount = 0;
    if (arquivo_indice != NULL && RRN != -1) { /*!< Se o arquivo existe e o RRN não é -1 */
        /*!< Colocando ponteiro do arquivo no local da página */
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
        fread(&((*page).nivel), sizeof(int), 1, arquivo_indice);
        fread(&((*page).keycount), sizeof(int), 1, arquivo_indice);
        for(int i = 0; i < ORDEM-1; i++) { /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
            fread(&((*page).key[i]), sizeof(int), 1, arquivo_indice);
            fread(&((*page).rrn[i]), sizeof(int), 1, arquivo_indice);
        }
        for(int i = 0; i < ORDEM; i++) /*!< Lendo os descendentes */
            fread(&((*page).child[i]), sizeof(int), 1, arquivo_indice);
        
    }
    return page;
}

int buscar_chave(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        if(RRN != -1) { /*<! Página inexistente */
            PAGE *page;
            int i;

            page = ler_pagina(arquivo_indice, RRN);
            for (i = 0; i < (*page).keycount; i++) /*<! Procurando a key (idNascimento) na página */
                if (idNascimento == (*page).key[i]) /*<! Achou */
                    return (*page).rrn[i]; /*<! Retorna referência para o registro no arquivo dados */

            for (i = 0; i < (*page).keycount; i++) /*<! Procurando o child para continuar a busca da key */
                if (idNascimento < (*page).key[i])
                    return buscar_pagina(arquivo_indice, idNascimento, (*page).child[i]);
                    
            return buscar_pagina(arquivo_indice, idNascimento, (*page).child[i+1]); /*<! Página é a última child */
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

// void inserir_chave_antiga(FILE* arquivo_indice, int idNascimento, int RRN) {
//     if(arquivo_indice != NULL) {
//         int noRaiz = ler_cabecalho(arquivo_indice, 1);
//         if(noRaiz == -1) { /*!< Inserção em árvore vazia */
//             PAGE page;
//             page.nivel = 1;
//             page.keycount = 1;
//             for(int i = 0; i < ORDEM-1; i++) {
//                 page.key[i] = '$';
//                 page.rrn[i] = '$';
//             }
//             page.key[0] = idNascimento;
//             page.rrn[0] = RRN;
//             for(int i = 0; i < ORDEM; i++)
//                 page.child[i] = -1;
//             escrever_pagina(arquivo_indice, page, 0); /*!< Escrevendo primeira página */
//             escrever_cabecalho(arquivo_indice, 5, 1); /*!< Após a criação do primeiro nó, nroNiveis = 1 */
//             escrever_cabecalho(arquivo_indice, 9, 1); /*!< Após a criação do primeiro nó, proxRRN = 1 */
//             escrever_cabecalho(arquivo_indice, 13, 1); /*!< Após a criação do primeiro nó, nroChaves = 1 */
//         } else { /*!< Árvore possui páginas */
//             PAGE page = ler_pagina(arquivo_indice, noRaiz); // page = raiz
//             /*!< A página (raiz) de nó tem espaço para novas keys e só há essa página na árvore */
//             if(ler_cabecalho(arquivo_indice, 5) == 1 && page.keycount < ORDEM-1) { 
//                 page.key[page.keycount] = idNascimento;
//                 page.rrn[page.keycount] = RRN;
//                 page.keycount++;
//                 insertion_sort(page.keycount, page.key, page.rrn); escrever_cabecalho(arquivo_indice, 5, 1); /*!< Ordenando a lista de chaves */
//                 escrever_pagina(arquivo_indice, page, noRaiz);
//                 escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1));
//                 return;
//             }

//             if (ler_cabecalho(arquivo_indice, 5) == 1 && page.keycount == ORDEM-1) { /*!< Nó raiz está cheio => split */
//                 //redistribuição
                        
//                 return;
//             }


//         }
//     }
//     return;
// }

void inicializar_pagina(PAGE *pagina) {
    if(pagina != NULL) {
        pagina->nivel = -1;
        pagina->keycount = 0;
        for(int i = 0; i < ORDEM-1; i++) {
            pagina->key[i] = '$';
            pagina->rrn[i] = '$';
        }
    }
}

/**
 * Função responsável por verificar a posição que a nova chave deve ser inserida.
 * Recebe como parâmetro o vetor de chaves a ser analisado, o tamanho do vetor e o novo idNascimento.
*/
int procura_posicao(int key[], int keycount, int idNascimento) {
    int posicao = 0;
    /**
     * Enquanto a posição for menor que o tamanho e o idNascimento menor que a chave de cada posição, 
     * a posição de inserção do novo idNascimento é maior.
    */
    while (posicao < keycount && idNascimento > key[posicao])
            posicao++;
    return posicao;
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
        int rrn_atual = rrn[i];
        int j;
        for (j = i - 1; j >= 0 && key[j] > key_atual; --j) { 
            key[j+1] = key[j];
            rrn[j+1] = rrn[j]; /*!< Quando muda a chave de posição o rrn precisa acompanhar */
        }
        key[j+1] = key_atual;
        rrn[j+1] = rrn_atual;
    }
}

/**
 * 
 * Retorna 1 caso deva ser criada uma nova raiz ou 0 caso contrário.
*/
int inserir(FILE* arquivo_indice, PAGE *pagina, int idNascimento, int *nova_chave_raiz, int *upRRN) {
    if(pagina == NULL) { /*!< Inserção em árvore vazia */
        (*upRRN) = -1;
        (*nova_chave_raiz) = idNascimento;
        return 1;
    }

    int keycount;
    int posicao_nova_chave;

    keycount = pagina->keycount;
    posicao_nova_chave = procura_posicao(pagina->key, keycount, idNascimento);

    if(posicao_nova_chave < keycount && idNascimento == pagina->key[posicao_nova_chave]) {
        /*!< Valor duplicado*/
        return 0;
    }

    int nova_chave;
    int rrn_nova_pagina;

    /*!< Se a nova chamada não precisar criar uma nova raiz, retorna */
    if(!inserir(arquivo_indice, ler_pagina(arquivo_indice, pagina->child[posicao_nova_chave]), idNascimento, &nova_chave, &rrn_nova_pagina))
        return 0;
    
    /*!< Ainda há espaço na página */
    if(keycount < ORDEM-1) { 
        pagina->key[keycount] = nova_chave;
        pagina->keycount++;
        insertion_sort(pagina->keycount, pagina->key, pagina->rrn);
        return 0; /*!< Como foi inserido na página que tem espaço, não precisa criar um novo nó raiz */
    }

    /*!< A quantidade de chaves na página é maxima */

    int ultima_chave;
    int rrn_ultima_pagina;

    if(posicao_nova_chave == ORDEM-1) { /*!< A posição que a chave deve ser inserida é a última da lista */
        ultima_chave = nova_chave;
        rrn_ultima_pagina = rrn_nova_pagina;
    } else {
        ultima_chave = pagina->key[ORDEM-2];
        rrn_ultima_pagina = pagina->rrn[ORDEM-1];

        pagina->key[ORDEM-2] = nova_chave;
        pagina->rrn[ORDEM-1] = rrn_nova_pagina;

        insertion_sort(pagina->keycount, pagina->key, pagina->rrn);
    }

    int posicao_de_split = (ORDEM - 1)/2;
    (*nova_chave_raiz) = pagina->key[posicao_de_split];

    PAGE nova_pagina_direita; /*!< Página que ficará a direita depois do split */
    inicializar_pagina(&nova_pagina_direita);

    pagina->keycount = posicao_de_split; /*!< A página da esquerda terá a quantidade de nós "splitados" */
    nova_pagina_direita.keycount = ORDEM-1-posicao_de_split; /*!< As chaves que não ficarem no nó esquerdo, ficam no nó direito */

    for(int i = 0; i < nova_pagina_direita.keycount; i++) {
        nova_pagina_direita.child[i] = pagina->child[i + posicao_de_split + 1];
        if(i < nova_pagina_direita.keycount - 1)
            nova_pagina_direita.key[i] = pagina->key[i + posicao_de_split + 1];
        else
             nova_pagina_direita.key[i] = ultima_chave;
    }
    
    nova_pagina_direita.child[nova_pagina_direita.keycount] = rrn_ultima_pagina;

    return 1;
}

/**
 * Função responsável por inserir uma chave em uma página
 * Recebe como parametro o arquivo de índice, o idNascimento do registro (key) e o RRN do registro no registro de dados
 */
void inserir_chave(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        int rrn_raiz = ler_cabecalho(arquivo_indice, 1);
        PAGE *raiz = ler_pagina(arquivo_indice, rrn_raiz);
        int nova_chave_raiz, rrn_nova_pagina;
        if(inserir(arquivo_indice, raiz, idNascimento, &nova_chave_raiz, &rrn_nova_pagina)) { 
            /*!< Criando um novo nó raiz */
            raiz->keycount = 1;
            raiz->key[0] = nova_chave_raiz;
            raiz->child[0] = rrn_raiz;
            raiz->child[1] = rrn_nova_pagina;
            //IMPORTANTE: Verificar a atualização do RRN do nó raiz
        }

    }
}