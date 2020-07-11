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
 * Lê o status do arquivo. 
 * Define-se 0 se estiver inconsistente e 1 se estiver consistente.
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
 * Lê o registro (inteiro) no byteoffset e retorna o valor.
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

/**
 * Recebe como parametro um arquivo de indice e o RRN.
 * Lê e retorna a página do arquivo de indice que possui o RRN passado.
 **/
PAGE *ler_pagina(FILE* arquivo_indice, int RRN) {
    if(RRN == -1)
        return NULL;
    PAGE *page = (PAGE*) malloc(sizeof(PAGE));
    if(page != NULL) {
        page->keycount = 0;
        /*!< Se o arquivo existe e o RRN não é -1 */
        if (arquivo_indice != NULL && RRN != -1) { 
            /*!< Colocando ponteiro do arquivo no local da página */
            fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
            fread(&(page->nivel), sizeof(int), 1, arquivo_indice);
            fread(&(page->keycount), sizeof(int), 1, arquivo_indice);
            /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
            for(int i = 0; i < ORDEM-1; i++) { 
                fread(&(page->key[i]), sizeof(int), 1, arquivo_indice);
                fread(&(page->rrn[i]), sizeof(int), 1, arquivo_indice);
            }
            /*!< Lendo os descendentes */
            for(int i = 0; i < ORDEM; i++) 
                fread(&(page->child[i]), sizeof(int), 1, arquivo_indice);
        }
    }
    return page;
}

/**
 * Recebe como parametro o arquivo de indice, um idNascimento, um RRN e a quantidade de páginas.
 * Realiza a busca na árvore-B do idNascimento recursivamente no nó com RRN do valor passado.
 * A cada nova página percorrida, armazena em quantidade de páginas. 
 * Retorna o RRN do idNascimento no arquivo de dados.
 **/
int buscar_chave(FILE* arquivo_indice, int idNascimento, int RRN, int *quantidade_de_paginas) {
    if(arquivo_indice != NULL) {
        if(RRN != -1) { /*<! Página inexistente */
            PAGE *pagina;
            pagina = ler_pagina(arquivo_indice, RRN);
            int rrnArquivoDeDados = -1;
            if(pagina != NULL) {
                (*quantidade_de_paginas)++;
                int i;
                for (i = 0; i < pagina->keycount; i++) /*<! Procurando a key (idNascimento) na página */
                    if (idNascimento == pagina->key[i]) { /*<! Achou */
                        rrnArquivoDeDados = pagina->rrn[i]; /*<! Retorna referência para o registro no arquivo dados */
                        free(pagina);
                        pagina = NULL;
                        return rrnArquivoDeDados;
                    }

                for (i = 0; i < pagina->keycount; i++) /*<! Procurando a child para continuar a busca da key */
                    if (idNascimento < pagina->key[i]) {
                        rrnArquivoDeDados = buscar_chave(arquivo_indice, idNascimento, pagina->child[i], quantidade_de_paginas);
                        free(pagina);
                        pagina = NULL;
                        return rrnArquivoDeDados;
                    }
                        
                rrnArquivoDeDados = buscar_chave(arquivo_indice, idNascimento, pagina->child[i], quantidade_de_paginas); /*<! Página é a última child */
                free(pagina);
                pagina = NULL;
                return rrnArquivoDeDados;
            }
        }
    }
    return -1;
}

/**
 * Função responsável por escrever uma página no arquivo de índices.
 * Recebe como parametro o arquivo de indice, a página a ser escrita e o RRN de onde deve ser escrito. 
 **/
void escrever_pagina(FILE* arquivo_indice, PAGE page, int RRN) {
    if(arquivo_indice != NULL) {
        int i, j;
        int vazio = -1;
        /*!< Colocando ponteiro do arquivo no local da página */
        fseek(arquivo_indice, ((RRN * TAMANHO_PAGINA) + TAMANHO_CABECALHO), SEEK_SET); 
        fwrite(&(page.nivel), sizeof(int), 1, arquivo_indice);
        fwrite(&(page.keycount), sizeof(int), 1, arquivo_indice);
        /*!< No arquivo aparece uma Ci (chave) e um Pri (RRN correspondente) */
        for(i = 0; i < page.keycount; i++) { 
            fwrite(&(page.key[i]), sizeof(int), 1, arquivo_indice);
            fwrite(&(page.rrn[i]), sizeof(int), 1, arquivo_indice);
        }
        /*!< Preenchendo os espaços restantes com vazio */
        for (j = i; j < ORDEM-1; j++) {
            fwrite(&vazio, sizeof(int), 1, arquivo_indice);
            fwrite(&vazio, sizeof(int), 1, arquivo_indice);
        }
        /*!< Escrevendo os descendentes */
        for(i = 0; i < (page.keycount+1); i++) 
            fwrite(&(page.child[i]), sizeof(int), 1, arquivo_indice);
        /*!< Preenchendo os espaços restantes com vazio */
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
    return;
}

/**
 * Função responsável por verificar a posição que a nova chave deve ser inserida.
 * Recebe como parâmetro o vetor de chaves a ser analisado, o tamanho do vetor e o novo idNascimento.
*/
int procura_posicao(int key[], int keycount, int idNascimento) {
    int posicao = 0;
    /*!< Enquanto a posição for menor que o tamanho e o idNascimento menor que a chave de cada posição, a posição de inserção do novo idNascimento é maior. */
    while (posicao < keycount && idNascimento > key[posicao]) posicao++;
    
    return posicao;
}

/**
 * Recebe como parâmetro
 *    arquivo_indice: o arquivo com a árvore-B
 *    pagina: uma struct do tipo PAGE que representa o registro do nó raiz na primeira execução e depois a página atual do arquivo de índice
 *    rrn_pagina: o rrn no arquivo de índice do nó raiz na primeira execução e depois o rrn da página atual
 *    idNascimento: nova chave a ser inserida na árvore-B
 *    rrn_idNascimento: rrn do registro que será inserido na árvore-B no arquivo de dados (RRN correspondente ao idNascimento)
 *  Caso seja necessário criar um novo nó raiz,
 *    nova_chave_raiz: a chave que irá para a posição [0] do novo nó raiz
 *    rrn_nova_chave_raiz: o rrn da chave que irá para a posição [0] do novo nó raiz 
 *    rrn_no_direito: o rrn da página que foi criada (nó direito)
 * Retorna 1 caso deva ser criada uma nova raiz ou 0 caso contrário.
*/
int inserir(FILE* arquivo_indice, PAGE **ponteiro_pagina, int rrn_pagina, int idNascimento, int rrn_idNascimento, int *nova_chave_raiz, int *rrn_nova_chave_raiz, int *rrn_no_direito) {
    PAGE *pagina = (*ponteiro_pagina);
    if(pagina == NULL) { /*!< Inserção em página vazia */
        (*nova_chave_raiz) = idNascimento;
        (*rrn_nova_chave_raiz) = rrn_idNascimento;
        (*rrn_no_direito) = -1;
        return 1;
    }

    int keycount;
    int posicao_nova_chave;

    keycount = pagina->keycount;
    posicao_nova_chave = procura_posicao(pagina->key, keycount, idNascimento);

    /*!< Valor duplicado*/
    if(posicao_nova_chave < keycount && idNascimento == pagina->key[posicao_nova_chave])
        return 0;

    int nova_chave;
    int rrn_nova_chave;
    int rrn_nova_pagina;

    PAGE* pagina_descendente = ler_pagina(arquivo_indice, pagina->child[posicao_nova_chave]);

    /*!< Se a nova chamada não precisar criar uma nova raiz, retorna */
    if(!inserir(arquivo_indice, &pagina_descendente, pagina->child[posicao_nova_chave], idNascimento, rrn_idNascimento, &nova_chave, &rrn_nova_chave, &rrn_nova_pagina)) {
        free(pagina_descendente);
        return 0;
    }

    free(pagina_descendente);
    
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
        *ponteiro_pagina = NULL;

        return 0; /*!< Como foi inserido na página que tem espaço, não precisa criar um novo nó raiz */
    }

    /*!< A quantidade de chaves na página é maxima */

    int ultima_chave;
    int rrn_ultima_chave;
    int rrn_ultima_pagina;

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

    int posicao_de_split = ORDEM/2;

    (*nova_chave_raiz) = pagina->key[posicao_de_split];
    (*rrn_nova_chave_raiz) = pagina->rrn[posicao_de_split];

    /*!< Página que ficará a direita depois do split */
    PAGE *nova_pagina = (PAGE*) malloc(sizeof(PAGE));
    inicializar_pagina(nova_pagina);

    pagina->keycount = posicao_de_split; /*!< A página da esquerda terá a quantidade de nós "splitados" */
    nova_pagina->keycount = ORDEM - 1 - posicao_de_split; /*!< As chaves que não ficarem no nó esquerdo, ficarão no nó direito */
    nova_pagina->nivel = pagina->nivel;

    for(int i = 0; i < nova_pagina->keycount; i++) {
        nova_pagina->child[i] = pagina->child[i + posicao_de_split + 1];
        if(i < nova_pagina->keycount - 1) {
            nova_pagina->key[i] = pagina->key[i + posicao_de_split + 1];
            nova_pagina->rrn[i] = pagina->rrn[i + posicao_de_split + 1];
        } else {
            nova_pagina->key[i] = ultima_chave;
            nova_pagina->rrn[i] = rrn_ultima_chave;
        }
    }

    nova_pagina->child[nova_pagina->keycount] = rrn_ultima_pagina;
    
    int proxRRN = ler_cabecalho(arquivo_indice, 9);
    escrever_cabecalho(arquivo_indice, 9, proxRRN + 1);
    (*rrn_no_direito) = proxRRN;

    /*!< A página esquerda continuará com o mesmo RRN e como perdeu metade das chaves (split) deverá ser atualizada no arquivo */
    escrever_pagina(arquivo_indice, *pagina, rrn_pagina);
    free(pagina);
    *ponteiro_pagina = NULL;

    /*!< A página direita é uma nova página a ser escrita no arquivo */
    escrever_pagina(arquivo_indice, *nova_pagina, proxRRN);
    free(nova_pagina);
    nova_pagina = NULL;

    return 1;
}

/**
 * Função responsável por inserir uma chave em uma página
 * Recebe como parametro o arquivo de índice, o idNascimento do registro (key) e o RRN do registro no registro de dados
 */
void inserir_chave(FILE* arquivo_indice, int idNascimento, int rrn_idNascimento) {
    if(arquivo_indice != NULL) {
        int rrn_raiz = ler_cabecalho(arquivo_indice, 1);
        PAGE *raiz = ler_pagina(arquivo_indice, rrn_raiz);

        int nova_chave_raiz, rrn_nova_chave_raiz, rrn_nova_pagina;
        if(inserir(arquivo_indice, &raiz, rrn_raiz, idNascimento, rrn_idNascimento, &nova_chave_raiz, &rrn_nova_chave_raiz, &rrn_nova_pagina)) {            
            int nroNiveis = ler_cabecalho(arquivo_indice, 5); /*!< nroNiveis */
            escrever_cabecalho(arquivo_indice, 5, nroNiveis + 1); /*!< Incrementando o nroNiveis */
            nroNiveis++;

            /*!< Criando um novo nó raiz */
            raiz = (PAGE*) malloc(sizeof(PAGE));
            inicializar_pagina(raiz); /*!< Preenchendo os campos com o valor inicial */
            raiz->keycount = 1;
            raiz->key[0] = nova_chave_raiz; 
            raiz->rrn[0] = rrn_nova_chave_raiz; /*!< Campo de referência do arquivo de dados */
            raiz->child[0] = rrn_raiz; /*!< Filho esquerdo do nó raiz */
            raiz->child[1] = rrn_nova_pagina; /*!< Filho direito do nó raiz */
            raiz->nivel = nroNiveis; /*!< A quantidade de níveis é o nível do raiz */

            int proxRRN = ler_cabecalho(arquivo_indice, 9); /*!< proxRRN */
            escrever_cabecalho(arquivo_indice, 9, (proxRRN + 1)); /*!< Incrementando o proxRRN */

            escrever_pagina(arquivo_indice, *raiz, proxRRN); /*!< Escrevendo a página do novo raiz */
            escrever_cabecalho(arquivo_indice, 1, proxRRN); /*!< Atualizando o RRN do nó raiz no cabeçalho */
    
            free(raiz);
            raiz = NULL;
        }
        
        if(raiz != NULL) 
            free(raiz); 
    
        escrever_cabecalho(arquivo_indice, 13, (ler_cabecalho(arquivo_indice, 13) + 1)); /*!< Incrementando o nroChaves */
    }
    return;
}