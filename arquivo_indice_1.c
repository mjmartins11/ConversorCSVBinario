#include "arquivo_indice.h"

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

        //Teste
        page.nivel = 0;

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
 * Recebe como parametro
 *    arquivo_indice: o arquivo com a árvore-B
 *    pagina: uma struct do tipo página que representa o registro do nó raiz na primeira execução e depois a página atual do arquivo de índice
 *    rrn_pagina: o rrn no arquivo de índice do nó raiz na primeira execução e depois o rrn da página atual
 *    idNascimento: nova chave a ser inserida na árvore-B
 *    rrn_idNascimento: rrn do registro que será inserido na árvore-B no arquivo de dados (RRN correspondente ao idNascimento)
 *  Caso seja necessário criar um novo nó raiz,
 *    nova_chave_raiz: a chave que irá para a posição [0] do novo nó raiz
 *    rrn_da_nova_chave: o rrn da chave que irá para a posição [0] do novo nó raiz 
 *    rrn_no_direito: o rrn da página que foi criada (nó direito)
 *    nova_pagina: a página direita criada 
 * Retorna 1 caso deva ser criada uma nova raiz ou 0 caso contrário.
*/
int inserir(FILE* arquivo_indice, PAGE *pagina, int rrn_pagina, int idNascimento, int rrn_idNascimento, int *nova_chave_raiz, int *rrn_da_nova_chave, int *rrn_no_direito,  PAGE** nova_pagina) {
    if(pagina == NULL) { /*!< A árvore está vazia */
        (*rrn_no_direito) = -1; 
        (*nova_chave_raiz) = idNascimento;
        (*rrn_da_nova_chave) = rrn_idNascimento;
        (*nova_pagina) = NULL;
        return 1;
    }


    int keycount; 
    int posicao_nova_chave; /*!< Posição da nova chave no vetor de keys da página */

    keycount = pagina->keycount;
    posicao_nova_chave = procura_posicao(pagina->key, keycount, idNascimento);

    /*!< Verificando se o valor está duplicado*/
    if(posicao_nova_chave < keycount && idNascimento == pagina->key[posicao_nova_chave]) 
        return 0;
    
    int nova_chave;
    int rrn_nova_chave;
    int rrn_nova_pagina;

    PAGE* page_child = ler_pagina(arquivo_indice, pagina->child[posicao_nova_chave]);

    if(!inserir(arquivo_indice, page_child, pagina->child[posicao_nova_chave], idNascimento, rrn_idNascimento, &nova_chave, &rrn_nova_pagina, &rrn_nova_chave, nova_pagina))
        return 0; /*!< Se a nova chamada não precisar criar uma nova raiz, retorna */
    
    posicao_nova_chave = procura_posicao(pagina->key, keycount, nova_chave);

    if(keycount < ORDEM-1) { /*!< Ainda há espaço na página */

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
        escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++

        return 0; /*!< Como foi inserido na página que tem espaço, não precisa criar um novo nó raiz */
    }

    /*!< A quantidade de chaves na página é maxima */

    int ultima_chave;
    int rrn_ultima_chave;
    int rrn_ultima_pagina; /*!< rrn da última child */

    if(posicao_nova_chave == ORDEM-1) { /*!< A posição que a chave deve ser inserida é a última da lista */
        ultima_chave = nova_chave;
        rrn_ultima_chave = rrn_nova_chave;
        rrn_ultima_pagina = rrn_nova_pagina;
    } else {
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

    int posicao_de_split = (ORDEM - 1)/2;
    (*nova_chave_raiz) = pagina->key[posicao_de_split];
    (*rrn_da_nova_chave) = pagina->rrn[posicao_de_split];

    //PAGE nova_pagina_direita; /*!< Página que ficará a direita depois do split */
    (*nova_pagina) = (PAGE*) malloc(sizeof(PAGE)); //nova_pagina_direita
    inicializar_pagina(*nova_pagina);

    pagina->keycount = posicao_de_split; /*!< A página da esquerda terá a quantidade de nós "splitados" */
    (*nova_pagina)->keycount = (ORDEM - 1) - posicao_de_split; /*!< As chaves que não ficarem no nó esquerdo, ficarão no nó direito */

    //printf("nova pagina direita keycount = %d\n", nova_pagina->keycount);

    int i;
    for(i = 0; i < (*nova_pagina)->keycount - 1; i++) {
        (*nova_pagina)->child[i] = pagina->child[i + posicao_de_split + 1];
        (*nova_pagina)->key[i] = pagina->key[i + posicao_de_split + 1];
        //pagina->key[i + posicao_de_split + 1] = -1;
        (*nova_pagina)->rrn[i] = pagina->rrn[i + posicao_de_split + 1];
        //pagina->rrn[i + posicao_de_split + 1] = -1;
    }

    (*nova_pagina)->key[i] = ultima_chave;
    (*nova_pagina)->rrn[i] = rrn_ultima_chave;    
    (*nova_pagina)->child[(*nova_pagina)->keycount] = rrn_ultima_pagina;

    int proxRRN = ler_cabecalho(arquivo_indice, 9);
    escrever_cabecalho(arquivo_indice, 9, proxRRN + 1);
    (*rrn_no_direito) = proxRRN;

    /*!< A página esquerda continuará com o mesmo RRN e como perdeu metade das chaves (split) deverá ser atualizada no arquivo */
    escrever_pagina(arquivo_indice, *pagina, rrn_pagina);
    free(pagina);

    /*!< A página direita é uma nova página a ser escrita no arquivo */
    // int proxRRN = ler_cabecalho(arquivo_indice, 9);
    escrever_pagina(arquivo_indice, **nova_pagina, proxRRN);

    /*!< Enviando o RRN do nó direito para criação do novo nó na função inserir_chave(...) */
    escrever_cabecalho(arquivo_indice, 9, proxRRN + 1);
    escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++

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
        int nova_chave_raiz, rrn_da_nova_chave;
        int rrn_nova_pagina; /*!< Se existir, será o filho da direita */

        PAGE* nova_pagina;

        escrever_cabecalho(arquivo_indice, 0, '0'); /*!< Atualizando o valor do status para inconsistente */

        if(inserir(arquivo_indice, raiz, rrn_raiz, idNascimento, RRN, &nova_chave_raiz, &rrn_da_nova_chave, &rrn_nova_pagina, &nova_pagina)) {
            /*!< Criando um novo nó raiz */
            //PAGE* up_raiz = raiz;

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
            escrever_cabecalho(arquivo_indice, 1, proxRRN); /*!< Atualizando o RRN do nó raiz */
            
            escrever_pagina(arquivo_indice, *raiz, proxRRN);
            // if(up_raiz != NULL) { /*!< Nó da esquerda */
            //     escrever_pagina(arquivo_indice, *up_raiz, rrn_raiz);
            //     free(up_raiz);
            // }
            
            // if(nova_pagina != NULL) { /*!< Nó da direita */
            //     escrever_pagina(arquivo_indice, *nova_pagina, rrn_nova_pagina);
            //     free(nova_pagina);
            // } 
            
            free(raiz);

            escrever_cabecalho(arquivo_indice, 9, (proxRRN + 1));
            escrever_cabecalho(arquivo_indice, 5, (ler_cabecalho(arquivo_indice, 5) + 1)); //nroNiveis++
            escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); //nroChaves++
        }

        escrever_cabecalho(arquivo_indice, 0, '1');
    }
    return;
}