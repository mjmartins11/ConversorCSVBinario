/**
 * Autores:
 *  Bruna Magrini da Cruz, 11218813
 *  Marlon José Martins, 10249010
 */

#include <stdio.h>
#include <string.h>

#include "arquivos.h"
#include "bebe.h"
#include "binarioNaTela.h"

#define CRIAR_ARQUIVO 1
#define LER_ARQUIVO 2
#define BUSCAR_NO_ARQUIVO 3
#define BUSCAR_POR_RRN 4
#define REMOVER_REGISTRO 5
#define INSERIR_REGISTRO 6
#define ATUALIZAR_REGISTRO 7
#define CRIAR_ARVOREB 8
#define BUSCAR_ARVOREB 9 
#define INSERIR_ARVOREB 10
#define SAIR 0

/**
 * Função responsável por fazer a leitura dos campos incluindo a verificação "NULO".
 * Retorna uma estrutura BEBE preenchido com os valores de entrada e os valores padrão caso o campo seja nulo.
*/
BEBE* leitura() {
    char verificarEntrada[TAMANHO_MAXIMO_REGISTRO];
    int idNascimento;
    int idadeMae;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char sexoBebe;
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
    char *cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    char *cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    
    inicializar_campos_registro(&idNascimento, &idadeMae, &sexoBebe, dataNascimento, estadoMae, estadoBebe, cidadeMae, cidadeBebe);

    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0) 
        strcpy(cidadeMae, verificarEntrada);
    
    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0)
        strcpy(cidadeBebe, verificarEntrada);

    scanf("%s", verificarEntrada);
    if(strcmp("NULO", verificarEntrada) != 0) 
        idNascimento = atoi(verificarEntrada);
    
    scanf("%s", verificarEntrada);
    if(strcmp("NULO", verificarEntrada) != 0)
        idadeMae = atoi(verificarEntrada);

    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0)
        strcpy(dataNascimento, verificarEntrada);

    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0)
        sexoBebe = verificarEntrada[0];

    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0)
        strcpy(estadoMae, verificarEntrada);

    scan_quote_string(verificarEntrada);
    if(strcmp("", verificarEntrada) != 0)
        strcpy(estadoBebe, verificarEntrada);
    
    return bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

/**
 * Está função é responsável por fazer a leitura em caso de busca combinada.
 * Se o campo não precisar ser alterado, é mantido as flags indicativas definadas na inicialização dos campos;
 * Se o valor for nulo, preenche com o valor padrão do campo;
 * Caso contrário, atribui o valor de entrada.
 * Retorna uma estrutura BEBE preenchida com os valores de entradas, valores padrões (em caso NULO) e flag
 *  indicativa caso o valor não precise ser comparado.
 */
BEBE* leitura_busca_combinada() {
    char verificarEntrada[TAMANHO_MAXIMO_REGISTRO];
    char nome_do_campo[15];
    int i, j, quantidade_de_campos;

    int idNascimento = 0;
    int idadeMae = 0;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1] = "$\0";
    char sexoBebe = '$';
    char sexoBebeAuxiliar = '$';
    char estadoMae[TAMANHO_ESTADO+1] = "$\0";
    char estadoBebe[TAMANHO_ESTADO+1] = "$\0";
    char *cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    char *cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    cidadeMae[0] = '$';
    cidadeMae[1] = '\0';
    cidadeBebe[0] = '$';
    cidadeBebe[1] = '\0';
    
    scanf("%d", &quantidade_de_campos);
    for(i = 0; i < quantidade_de_campos; i++) {
        scanf("%s", nome_do_campo);
        if(strcmp("sexoBebe", nome_do_campo) == 0) {
            /**
             * A leitura para o campo bebe é feita de forma manual sem utilizar
             * a função scan_quote_string.
             * Isto devido ao problema de entrada quando o valor é NULO
             * Está decisão foi tomada com o monitor Vinicius durante o atendimento
            */
            scanf("%*c%c", &sexoBebeAuxiliar); /*!< Pulando espaço e aspas */
            if(sexoBebeAuxiliar == '\"') {
                scanf("%c%*c", &sexoBebe);
            } else {
                scanf("%*s"); 
                sexoBebe = '0';
            }
        } else {
            scan_quote_string(verificarEntrada);
            if(strcmp("idNascimento", nome_do_campo) == 0) 
                if(strcmp("", verificarEntrada) != 0) 
                    idNascimento = atoi(verificarEntrada);
                else 
                    idNascimento = -1;
            else if(strcmp("idadeMae", nome_do_campo) == 0) 
                if(strcmp("", verificarEntrada) != 0)
                    idadeMae = atoi(verificarEntrada);
                else
                    idadeMae = -1;
            else if(strcmp("dataNascimento", nome_do_campo) == 0) 
                if(strcmp("", verificarEntrada) != 0) {
                    strcpy(dataNascimento, verificarEntrada);
                } else {
                    dataNascimento[0] = '\0'; 
                    for(int j = 1; j < TAMANHO_DATA_NASCIMENTO; j++) 
                        dataNascimento[j] = '$'; /*!< Atribuindo lixo ($) */
                }
            else if(strcmp("estadoBebe", nome_do_campo) == 0)
                if(strcmp("", verificarEntrada) != 0) {
                    strcpy(estadoBebe, verificarEntrada);
                } else {
                    estadoBebe[0] = '\0';
                    estadoBebe[1] = '$';
                }
            else if(strcmp("estadoMae", nome_do_campo) == 0) 
                if(strcmp("", verificarEntrada) != 0) {
                    strcpy(estadoMae, verificarEntrada);
                } else {
                    estadoMae[0] = '\0'; 
                    estadoMae[1] = '$';
                }
            else if(strcmp("cidadeMae", nome_do_campo) == 0)
                if(strcmp("", verificarEntrada) != 0)
                    strcpy(cidadeMae, verificarEntrada);
                else 
                    cidadeMae[0] = '\0'; 
            else if(strcmp("cidadeBebe", nome_do_campo) == 0) 
                if(strcmp("", verificarEntrada) != 0) 
                    strcpy(cidadeBebe, verificarEntrada);
                else 
                    cidadeBebe[0] = '\0'; 
            else 
                printf("Campo inválido.\n");
        }
    }
    return bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

int main(void) {
    int i;
    char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO];
    char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO];
    char nome_do_arquivo_indice[TAMANHO_NOME_ARQUIVO];
    int rrn_busca, quantidade, idNascimento;

    BEBE* bebe = NULL;

    int operacao;
    scanf("%d", &operacao);
    switch(operacao) {
        case CRIAR_ARQUIVO:
            scanf("%s %s", nome_do_arquivo_csv, nome_do_arquivo_bin);
            if(!criar_arquivo(nome_do_arquivo_csv, nome_do_arquivo_bin)) {
                printf("Falha no carregamento do arquivo.\n");
                return 0;
            }
            binarioNaTela(nome_do_arquivo_bin);
        break;

        case LER_ARQUIVO:
            scanf("%s", nome_do_arquivo_bin);
            if(!ler_arquivo(nome_do_arquivo_bin))
                printf("Falha no processamento do arquivo.\n");
        break;

        case BUSCAR_NO_ARQUIVO:
            scanf("%s", nome_do_arquivo_bin);
            bebe = leitura_busca_combinada();
            if (!busca_por_campos(nome_do_arquivo_bin, bebe)) 
                printf("Falha no processamento do arquivo.\n");
            bebe_apagar(&bebe);
        break;

        case BUSCAR_POR_RRN:
            scanf("%s %d", nome_do_arquivo_bin, &rrn_busca);
            if (!busca_rrn(nome_do_arquivo_bin, rrn_busca))
                printf("Falha no processamento do arquivo.\n");
        break;

        case REMOVER_REGISTRO:
            scanf("%s", nome_do_arquivo_bin);
            scanf("%d", &quantidade);
            for (i = 0; i < quantidade; i++) {
                bebe = leitura_busca_combinada();
                if(!remover_registro(nome_do_arquivo_bin, bebe)) {
                    printf("Falha no processamento do arquivo.\n");
                    bebe_apagar(&bebe);
                    return 0;
                }
                bebe_apagar(&bebe);
            }
            binarioNaTela(nome_do_arquivo_bin);
        break;

        case INSERIR_REGISTRO:
            scanf("%s", nome_do_arquivo_bin);
            scanf("%d", &quantidade);
            for(i = 0; i < quantidade; i++) {
                bebe = leitura();
                if(!inserir_registro(nome_do_arquivo_bin, bebe)) {
                    printf("Falha no processamento do arquivo.\n");
                    bebe_apagar(&bebe);
                    return 0;
                }
                bebe_apagar(&bebe);
            }
            binarioNaTela(nome_do_arquivo_bin);
        break;

        case ATUALIZAR_REGISTRO:
            scanf("%s", nome_do_arquivo_bin);
            scanf("%d", &quantidade);
            for (i = 0; i < quantidade; i++) {
                scanf("%d", &rrn_busca);
                bebe = leitura_busca_combinada();
                if(!atualizar_registro(nome_do_arquivo_bin, rrn_busca, bebe)) {
                    printf("Falha no processamento do arquivo.\n");
                    bebe_apagar(&bebe);
                    return 0;
                }
                bebe_apagar(&bebe);
            }
            binarioNaTela(nome_do_arquivo_bin);
        break;

        case CRIAR_ARVOREB:
            scanf("%s %s", nome_do_arquivo_bin, nome_do_arquivo_indice);
            int criar_arvore = criar_arvore_b(nome_do_arquivo_bin, nome_do_arquivo_indice);
            if(!criar_arvore) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }
            binarioNaTela(nome_do_arquivo_indice);
        break;

        case BUSCAR_ARVOREB:
            scanf("%s %s %*s", nome_do_arquivo_bin, nome_do_arquivo_indice);
            scanf("%d", &idNascimento);
            if(!buscar_arvore_b(nome_do_arquivo_bin, nome_do_arquivo_indice, idNascimento)) {
                printf("Falha no processamento do arquivo.\n");
                return 0;
            }
        break;

        case INSERIR_ARVOREB:
            scanf("%s %s", nome_do_arquivo_bin, nome_do_arquivo_indice);
            scanf("%d", &quantidade);
            for (i = 0; i < quantidade; i++) {
                scanf("%d", &rrn_busca);
                bebe = leitura_busca_combinada();
                if(!inserir_registro_arvore_b(nome_do_arquivo_bin, nome_do_arquivo_indice, bebe)) {
                    printf("Falha no processamento do arquivo.\n");
                    bebe_apagar(&bebe);
                    return 0;
                }
                bebe_apagar(&bebe);
            }
            binarioNaTela(nome_do_arquivo_indice);
        break;
    }

    return 0;
}