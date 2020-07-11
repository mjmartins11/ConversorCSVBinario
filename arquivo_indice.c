#include "arquivo_indice.h"

#define DEBUG 0

typedef struct page {
    int nivel; /*!< O nível da página*/
    int keycount; /*!< Número de chaves armazenadas na página */
    int key[ORDEM-1]; /*!< As chaves de busca */
    int rrn[ORDEM-1]; /*!< O RRN do registro no arquivo de dados */
    int child[ORDEM]; /* O RRN dos nós descendentes */
} PAGE;

/**
 * Cabeçalho
 *              Status | noRaiz | nroNiveis | proxRRN | nroChaves
 * Byteoffset:     0       1         5          9          13
*/

/**
 * Recebe como parametro o arquivo de índices.
 * Cria o cabeçalho no arquivo de índices com os valores padrões.
 */
void inicializar_cabecalho_indice(FILE* arquivo_de_indice) {
    if(arquivo_de_indice != NULL) {
        char status = '0';
        int noRaiz = -1, nroNiveis = 0, proxRRN = 0, nroChaves = 0;
        fseek(arquivo_de_indice, 0, SEEK_SET);
        fwrite(&status, sizeof(char), 1, arquivo_de_indice);
        fwrite(&noRaiz, sizeof(int), 1, arquivo_de_indice);
        fwrite(&nroNiveis, sizeof(int), 1, arquivo_de_indice);
        fwrite(&proxRRN, sizeof(int), 1, arquivo_de_indice);
        fwrite(&nroChaves, sizeof(int), 1, arquivo_de_indice);
        char lixo = '$';
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
    return;
}

PAGE *ler_pagina(FILE* arquivo_indice, int RRN) {
    if(RRN == -1)
        return NULL;
        
    PAGE *page = (PAGE*) malloc (sizeof(PAGE));
    if(page != NULL) {
        page->keycount = 0;
        if (arquivo_indice != NULL && RRN != -1) { /*!< Se o arquivo existe e o RRN não é -1 */
            /*!< Colocando ponteiro do arquivo no local da página */
            fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
            fread(&(page->nivel), sizeof(int), 1, arquivo_indice);
            fread(&(page->keycount), sizeof(int), 1, arquivo_indice);
            for(int i = 0; i < ORDEM-1; i++) { /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
                fread(&(page->key[i]), sizeof(int), 1, arquivo_indice);
                fread(&(page->rrn[i]), sizeof(int), 1, arquivo_indice);
            }
            for(int i = 0; i < ORDEM; i++) /*!< Lendo os descendentes */
                fread(&(page->child[i]), sizeof(int), 1, arquivo_indice);
        }
    }
    return page;
}

int buscar_chave(FILE* arquivo_indice, int idNascimento, int RRN, int *quantidade_de_paginas) {
    if(arquivo_indice != NULL) {
        if(RRN != -1) { /*<! Página inexistente */
            PAGE *page;
            page = ler_pagina(arquivo_indice, RRN);
            if(page != NULL) {
                (*quantidade_de_paginas)++;
                int i;
                for (i = 0; i < page->keycount; i++) /*<! Procurando a key (idNascimento) na página */
                    if (idNascimento == page->key[i]) /*<! Achou */
                        return page->rrn[i]; /*<! Retorna referência para o registro no arquivo dados */

                for (i = 0; i < page->keycount; i++) /*<! Procurando a child para continuar a busca da key */
                    if (idNascimento < page->key[i]) 
                        return buscar_chave(arquivo_indice, idNascimento, page->child[i], quantidade_de_paginas);
                        
                return buscar_chave(arquivo_indice, idNascimento, page->child[i], quantidade_de_paginas); /*<! Página é a última child */
            }
        }
    }
    return -1;
}

void escrever_pagina(FILE* arquivo_indice, PAGE page, int RRN) {
    if(arquivo_indice != NULL) {
        int i, j;
        int vazio = -1;
        /*!< Colocando ponteiro do arquivo no local da página */
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
        fwrite(&(page.nivel), sizeof(int), 1, arquivo_indice);
        fwrite(&(page.keycount), sizeof(int), 1, arquivo_indice);
        for(i = 0; i < page.keycount; i++) { /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
            fwrite(&(page.key[i]), sizeof(int), 1, arquivo_indice);
            fwrite(&(page.rrn[i]), sizeof(int), 1, arquivo_indice);
        }

        for (j = i; j < ORDEM-1; j++) {
            fwrite(&vazio, sizeof(int), 1, arquivo_indice);
            fwrite(&vazio, sizeof(int), 1, arquivo_indice);
        }

        for(i = 0; i < (page.keycount+1); i++) /*!< Lendo os descendentes */
            fwrite(&(page.child[i]), sizeof(int), 1, arquivo_indice);

        for (j = i; j < ORDEM; j++)
            fwrite(&vazio, sizeof(int), 1, arquivo_indice);

    }
    return;
}

/**
 * Função responsável por colocar páginas com os valores de quando estiver vazia.
 * Recebe o ponteiro para uma página e preenche ela com valores de inicialização.
*/
void inicializar_pagina(PAGE *pagina) {
    if(pagina != NULL) {
        pagina->nivel = -1;
        pagina->keycount = 0;
        for(int i = 0; i < ORDEM-1; i++) {
            pagina->key[i] = -1;
            pagina->rrn[i] = -1;
            pagina->child[i] = -1;
        }
        pagina->child[ORDEM - 1] = -1;
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

   //printf("\nkeycount: %d\n", keycount);

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
int inserir(FILE* arquivo_indice, PAGE *pagina, int rrn_pagina, int idNascimento, int rrn_idNascimento, int *nova_chave_raiz, int *upRRN, int *rrn_da_nova_chave, PAGE** nova_pagina) {
    
    //printf("\nENTRO\n");
    
    if(pagina == NULL) { /*!< Inserção em árvore vazia */
        (*upRRN) = -1;
        (*nova_chave_raiz) = idNascimento;
        (*rrn_da_nova_chave) = rrn_idNascimento;
        (*nova_pagina) = NULL;

        if(DEBUG) printf("\nVAZIO\n");
        return 1;
    }

    int keycount;
    int posicao_nova_chave;

    keycount = pagina->keycount;
    posicao_nova_chave = procura_posicao(pagina->key, keycount, idNascimento);

    if(DEBUG) ("\n pos:  %d\n", posicao_nova_chave);
    
    if(posicao_nova_chave < keycount && idNascimento == pagina->key[posicao_nova_chave]) {
        if(DEBUG) ("\nSAIU; DUPLICADO\n");
        /*!< Valor duplicado*/
        return 0;
    }

    int nova_chave;
    int rrn_nova_chave;
    int rrn_nova_pagina;

    PAGE* page_child = ler_pagina(arquivo_indice, pagina->child[posicao_nova_chave]);

    /*!< Se a nova chamada não precisar criar uma nova raiz, retorna */
    // printf("pagina-child: %d\n", pagina->child[posicao_nova_chave]);
    int retorno = inserir(arquivo_indice, page_child, pagina->child[posicao_nova_chave], idNascimento, rrn_idNascimento, &nova_chave, &rrn_nova_pagina, &rrn_nova_chave, nova_pagina);
    printf("retorno: %d\n", retorno);
    if(!retorno)
        return 0;
    
    if(keycount < ORDEM-1) { /*!< Ainda há espaço na página */

       // printf("\nSAME\n");
        //printf("\n pos:  %d\n", posicao_nova_chave);
        //necessario ?
        //posicao_nova_chave = procura_posicao(pagina->key, keycount, nova_chave);
        
        for (int i = keycount; i > posicao_nova_chave; i--) {
            pagina->key[i] = pagina->key[i - 1];
            pagina->rrn[i] = pagina->rrn[i - 1];
            pagina->child[i + 1] = pagina->child[i];
        }
        
        pagina->key[posicao_nova_chave] = nova_chave;
        pagina->rrn[posicao_nova_chave] = rrn_nova_chave;
        pagina->child[posicao_nova_chave + 1] = rrn_nova_pagina;
        pagina->keycount++;

        escrever_pagina(arquivo_indice, *pagina, rrn_pagina);
        free(pagina);
        // escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++

        if(DEBUG) ("\nsaida; inser na mesma pag \n");

        return 0; /*!< Como foi inserido na página que tem espaço, não precisa criar um novo nó raiz */
    }

    /*!< A quantidade de chaves na página é maxima */

    int ultima_chave;
    int rrn_ultima_chave;
    int rrn_ultima_pagina;

    if(DEBUG) ("\nSPLIT\n");

    if(posicao_nova_chave == ORDEM-1) { /*!< A posição que a chave deve ser inserida é a última da lista */
        if(DEBUG) ("\n pos: ORDEM - 1\n");
        ultima_chave = nova_chave;
        rrn_ultima_chave = rrn_nova_chave;
        rrn_ultima_pagina = rrn_nova_pagina;
    } else {
        if(DEBUG) ("\n pos qualquer\n");
        ultima_chave = pagina->key[ORDEM-2];
        rrn_ultima_chave = pagina->rrn[ORDEM-2];
        rrn_ultima_pagina = pagina->child[ORDEM-1];

        for (int i = ORDEM-2; i > posicao_nova_chave; i--) {
            pagina->key[i] = pagina->key[i - 1];
            pagina->rrn[i] = pagina->rrn[i - 1];
            pagina->child[i + 1] = pagina->child[i];
        }

        pagina->key[posicao_nova_chave] = nova_chave;
        pagina->rrn[posicao_nova_chave] = rrn_nova_chave;
        pagina->child[posicao_nova_chave+1] = rrn_nova_pagina;

    }

    /**
     * key:   [1, 2, 3, 4, 5]
     * rrn:   [0, 1, 2, 3, 4]
     * child: [-1, -1, -1, -1, -1, -1]
     * 
     * 
     * 
     * nova_chave_raiz = 3
     * rrn_da_nova_chave = 2
     * 
     *          esquerda: 
     * keycount = 2
     * key:   [1, 2, 3, -1, -1]
     * rrn:   [0, 1, 2, -1, -1]
     * child: [-1, -1, -1, -1, -1, -1]
     * 
     *          direita: 
     * keycount = 3
     * key:   [4, 5, 6, -1, -1]
     * rrn:   [3, 4, 5, -1, -1]
     * child: [-1, -1, -1, -1, -1, -1]
     * 
     * 
    */

    int posicao_de_split = (ORDEM - 1)/2 + 1;//! aqui tava errado, antes estava ordem - 1 / 2 faltando o +1

    if(DEBUG) ("\nSplit pos: %d\n",posicao_de_split);
    (*nova_chave_raiz) = pagina->key[posicao_de_split];
    (*rrn_da_nova_chave) = pagina->rrn[posicao_de_split];

    //PAGE nova_pagina_direita; /*!< Página que ficará a direita depois do split */
    (*nova_pagina) = (PAGE*) malloc(sizeof(PAGE)); //nova_pagina_direita
    inicializar_pagina(*nova_pagina);

    pagina->keycount = posicao_de_split; /*!< A página da esquerda terá a quantidade de nós "splitados" */
    (*nova_pagina)->keycount = ORDEM - 1 - posicao_de_split; /*!< As chaves que não ficarem no nó esquerdo, ficarão no nó direito */

    //printf("nova pagina direita keycount = %d\n", nova_pagina->keycount);

    for(int i = 0; i < (*nova_pagina)->keycount; i++) {
        (*nova_pagina)->child[i] = pagina->child[i + posicao_de_split + 1];
        if(i < (*nova_pagina)->keycount - 1) {
            (*nova_pagina)->key[i] = pagina->key[i + posicao_de_split + 1];
            //pagina->key[i + posicao_de_split + 1] = -1;
            (*nova_pagina)->rrn[i] = pagina->rrn[i + posicao_de_split + 1];
            //pagina->rrn[i + posicao_de_split + 1] = -1;
        } else {
            (*nova_pagina)->key[i] = ultima_chave;
            (*nova_pagina)->rrn[i] = rrn_ultima_chave;
        }
    }
    
    (*nova_pagina)->child[(*nova_pagina)->keycount] = rrn_ultima_pagina;

    if(DEBUG) ("\nChaves:\n");
    for (size_t i = 0; i < (*nova_pagina)->keycount; i++)
    {
        if(DEBUG) ("\n%d\n",(*nova_pagina)->key[i]);
    }
    

    int proxRRN = ler_cabecalho(arquivo_indice, 9);
    escrever_cabecalho(arquivo_indice, 9, proxRRN + 1);
    (*upRRN) = proxRRN;

    /*!< A página esquerda continuará com o mesmo RRN e como perdeu metade das chaves (split) deverá ser atualizada no arquivo */
    escrever_pagina(arquivo_indice, *pagina, rrn_pagina);
    free(pagina);

    /*!< A página direita é uma nova página a ser escrita no arquivo */
    // int proxRRN = ler_cabecalho(arquivo_indice, 9);
    escrever_pagina(arquivo_indice, **nova_pagina, proxRRN);

    /*!< Enviando o RRN do nó direito para criação do novo nó na função inserir_chave(...) */
    // escrever_cabecalho(arquivo_indice, 9, proxRRN + 1);
    // escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++


    if(DEBUG) ("\nFoi ate o fim\n");
    return 1;
}

void nivelar(FILE* arquivo_indice, int RRN, int nivel) {
    if(arquivo_indice != NULL) { 
        if (RRN == -1)
            return; /*!< Página não existe */
            
        PAGE* page = ler_pagina(arquivo_indice, RRN);
        page->nivel = nivel;
        escrever_pagina(arquivo_indice, *page, RRN);
        for(int i = 0; i < (page->keycount + 1); i++) 
            nivelar(arquivo_indice, page->child[i], (nivel+1));
    }
    return;
}

/**
 * Função responsável por inserir uma chave em uma página
 * Recebe como parametro o arquivo de índice, o idNascimento do registro (key) e o RRN do registro no registro de dados
 */
void inserir_chave(FILE* arquivo_indice, int idNascimento, int RRN) {
    if(arquivo_indice != NULL) {
        int rrn_raiz = ler_cabecalho(arquivo_indice, 1);
        if(DEBUG) ("\nrrnRaiz: %d\n", rrn_raiz);
        
        PAGE *raiz = ler_pagina(arquivo_indice, rrn_raiz);
        int nova_chave_raiz, rrn_nova_pagina, rrn_da_nova_chave;

        PAGE* nova_pagina;

        if(inserir(arquivo_indice, raiz, rrn_raiz, idNascimento, RRN, &nova_chave_raiz, &rrn_nova_pagina, &rrn_da_nova_chave, &nova_pagina) == 1 ) {
           
            if(DEBUG) ("\nCRIANDO NOVO  NO\n");
            /*!< Criando um novo nó raiz */
            PAGE* up_raiz = raiz;

            //if(raiz == NULL) {
                raiz = (PAGE*) malloc(sizeof(PAGE));
                inicializar_pagina(raiz); /*!< Preenchendo os campos com -1 (valor inicial) */
            //}
            raiz->keycount = 1;
            raiz->key[0] = nova_chave_raiz;
            raiz->rrn[0] = rrn_da_nova_chave; /*!< Campo de referência do arquivo de dados */
            raiz->child[0] = rrn_raiz;
            raiz->child[1] = rrn_nova_pagina;

            int proxRRN = ler_cabecalho(arquivo_indice, 9);

            if(DEBUG) ("\nproxrrn: %d\n", proxRRN);
            if(DEBUG) ("ESCREVENDO A NOVA RAIZ:");
            escrever_cabecalho(arquivo_indice, 1, proxRRN); /*!< Atualizando o RRN do nó raiz */
            
            // fseek(arquivo_indice,1, SEEK_SET);
                // fwrite(&proxRRN, 1,sizeof(int), arquivo_indice);

            escrever_pagina(arquivo_indice, *raiz, proxRRN);
            // if(up_raiz != NULL) { /*!< Nó da esquerda */
            //     escrever_pagina(arquivo_indice, *up_raiz, rrn_raiz);
            //     free(up_raiz);
            // }
            
            // if(nova_pagina != NULL) { /*!< Nó da direita */
            //     escrever_pagina(arquivo_indice, *nova_pagina, rrn_nova_pagina);
            //     free(nova_pagina);
            // } 

            nivelar(arquivo_indice, proxRRN, 1);

            free(raiz);

            if(DEBUG) ("\nFOI ATUALIZADO\n");
            escrever_cabecalho(arquivo_indice, 9, (proxRRN + 1));
            escrever_cabecalho(arquivo_indice, 5, (ler_cabecalho(arquivo_indice, 5) + 1)); //nroNiveis++
        }
        printf("att numero de chave\n");

        // int noRaiz = ler_cabecalho(arquivo_indice, 1);
        // nivelar(arquivo_indice, rrn_raiz, 1);
        escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++
    }
    return;
}