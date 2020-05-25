#include "arquivo_conteudo.h"

/**
 * Recebe uma estrutura de dados BEBE e uma linha (registro) do arquivo .csv
 * Organiza e separa as informações do registro e insere em BEBE
 */
void ler_arquivo_csv(BEBE** bebe, char registro[TAMANHO_REGISTRO_CSV]) {
    int byteoffsetRegistro = 0; /*!< Representa o byteoffset na linha que estamos trabalhando. Ou seja, posição atual no registro */
    int byteoffsetCampo = 0; /*!< Representa o byteoffset do campo que estamos trabalhando. Ou seja, posição no campo */

    int idNascimento, idadeMae;
    char auxiliarParaInteiro[10];
    char sexoBebe[1];
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
    char *cidadeMae;
    char *cidadeBebe;

    cidadeMae = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));
    cidadeBebe = (char*) malloc(TAMANHO_MAXIMO_REGISTRO * sizeof(char));

    byteoffsetCampo = 0; /*!< Posição 0 no campo cidadeMae */
    if(registro[byteoffsetRegistro] == ',') {
        cidadeMae[byteoffsetCampo] = '\0'; /*!< Se o campo não contém dados, recebe \0 */
    } else {
        /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        while(registro[byteoffsetRegistro] != ',') {
            cidadeMae[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++; /*!< Como estamos lendo o registro, atualizamos o byteoffset do registro atual */
            byteoffsetCampo++;
        }
        cidadeMae[byteoffsetCampo] = '\0';
    }

    byteoffsetCampo = 0; /*!< Posição 0 no campo cidadeBebe */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        cidadeBebe[byteoffsetCampo] = '\0'; /*!< Se o campo não contém dados, recebe \0 */
    } else {
        /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        while(registro[byteoffsetRegistro] != ',') {
            cidadeBebe[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++;
            byteoffsetCampo++;
        }
        cidadeBebe[byteoffsetCampo] = '\0';
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo idNascimento */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        idNascimento = -1; /*!< Se o campo não contém dados, recebe -1 */
    } else {
        /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        while(registro[byteoffsetRegistro] != ',') {
            auxiliarParaInteiro[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++; 
            byteoffsetCampo++;
        }
         /*!< É necessário transformar o valor lido de char para inteiro */
        auxiliarParaInteiro[byteoffsetCampo] = '\0';
        idNascimento = atoi(auxiliarParaInteiro);
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo idadeMae */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        idadeMae = -1; /*!< Se o campo não contém dados, recebe -1 */
    } else {
        /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        while(registro[byteoffsetRegistro] != ',') {
            auxiliarParaInteiro[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++; 
            byteoffsetCampo++;
        }
        /*!< É necessário transformar o valor lido de char para inteiro */
        auxiliarParaInteiro[byteoffsetCampo] = '\0';
        idadeMae = atoi(auxiliarParaInteiro);
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo dataNascimento  */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        /*!< Se o campo não contém dados, recebe \0$$$$$$$$$ */
        dataNascimento[0] = '\0';
        for(byteoffsetCampo = 1; byteoffsetCampo < TAMANHO_DATA_NASCIMENTO; byteoffsetCampo++) 
            dataNascimento[byteoffsetCampo] = '$'; /*!< Atribuindo lixo ($) */
    } else {
         /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        for(byteoffsetCampo = 0; byteoffsetCampo < (TAMANHO_DATA_NASCIMENTO); byteoffsetCampo++) { 
            dataNascimento[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++;
        }
         /*!< Inserindo na última posição um finalizador de string */
        dataNascimento[byteoffsetCampo] = '\0';
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo sexoBebe */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        /*!< Se o campo não contém dados, recebe 0 (IGNORADO) */
        sexoBebe[0] = '0';
    } else {
        /*!< Se o campo contém dados, recebe o valor */
        sexoBebe[0] = registro[byteoffsetRegistro];
        byteoffsetRegistro++;
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo estadoMae */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == ',') {
        /*!< Se o campo não contém dados, recebe \0$ */
        estadoMae[0] = '\0';
        estadoMae[1] = '$';
    } else {
         /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        for(byteoffsetCampo = 0; byteoffsetCampo < (TAMANHO_ESTADO); byteoffsetCampo++) {
            estadoMae[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++;
        }
        /*!< Inserindo na última posição um finalizador de string */
        estadoMae[byteoffsetCampo] = '\0';
    }  

    byteoffsetCampo = 0; /*!< Posição 0 no campo estadoBebe */
    byteoffsetRegistro++;
    if(registro[byteoffsetRegistro] == '\n') { /*!< Verifica se é o final da linha */
        /*!< Se o campo não contém dados, recebe \0$ */
        estadoBebe[0] = '\0';
        estadoBebe[1] = '$';
    } else {
         /*!< Se o campo contém dados, seu valor é até a próxima aparição da vírgula */
        for(byteoffsetCampo = 0; byteoffsetCampo < (TAMANHO_ESTADO); byteoffsetCampo++) {
            estadoBebe[byteoffsetCampo] = registro[byteoffsetRegistro];
            byteoffsetRegistro++;
        }
        /*!< Inserindo na última posição um finalizador de string */
        estadoBebe[byteoffsetCampo] = '\0';
    }

    /*!< Cria uma estrutura de dados BEBE com os valores obtidos */
    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);
}

/**
 * Recebe um arquivo .bin, uma estrutura de dados BEBE e o RRNproxRegistro
 * Escreve os valores do registro (contidos em BEBE) no arquivo .bin
 */
void escrevar_arquivo_bin(FILE* arquivo_gerado, BEBE* bebe, int rrn_proximo_registro) {
    int i = 0;
    /*!< O byteoffset de inicio do registro no arquivo .bin é dado pelo tamanho do registro * RRN somado ao tamanho do cabeçalho */
    int byteoffset_inicial = (TAMANHO_REGISTRO_BIN * rrn_proximo_registro) + BYTEOFFSET_INICIO_CONTEUDO; 
    int tamanho_campo_cidadeMae = strlen(bebe_get_cidadeMae(bebe));
    int tamanho_campo_cidadeBebe = strlen(bebe_get_cidadeBebe(bebe));
    int tamanho_campos_variaveis = tamanho_campo_cidadeMae + tamanho_campo_cidadeBebe; /*!< Armazena a quantidade de bytes utilizado por cidadeMae e cidadeBebe */
    int quantidade_campos_variaveis_nao_usados = TAMANHO_MAXIMO_REGISTRO - tamanho_campos_variaveis - (2*sizeof(int)); /*!<  Armazena a quantidade de campos variaveis não usados (serão preenchidos com lixo posteriormente) */

    int idNascimento = bebe_get_idNascimento(bebe);
    int idadeMae = bebe_get_idadeMae(bebe);

    fseek(arquivo_gerado, byteoffset_inicial, SEEK_SET);
    fwrite(&tamanho_campo_cidadeMae, sizeof(int), 1, arquivo_gerado);
    fwrite(&tamanho_campo_cidadeBebe, sizeof(int), 1, arquivo_gerado);
    if(tamanho_campo_cidadeMae > 0)
        fwrite(bebe_get_cidadeMae(bebe), sizeof(char), tamanho_campo_cidadeMae, arquivo_gerado);
    if(tamanho_campo_cidadeBebe > 0)
        fwrite(bebe_get_cidadeBebe(bebe), sizeof(char), tamanho_campo_cidadeBebe, arquivo_gerado);
    for(i = 0; i < quantidade_campos_variaveis_nao_usados; i++) /*!< Preenchendo os espaços não usados pelos campos de tamanhos variaveis com lixo ($) */
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
    fwrite(&idNascimento, sizeof(int), 1, arquivo_gerado);
    fwrite(&idadeMae, sizeof(int), 1, arquivo_gerado);
    fwrite(bebe_get_dataNascimento(bebe), sizeof(char), TAMANHO_DATA_NASCIMENTO, arquivo_gerado);
    fwrite(bebe_get_sexoBebe(bebe), sizeof(char), 1, arquivo_gerado);
    fwrite(bebe_get_estadoMae(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
    fwrite(bebe_get_estadoBebe(bebe), sizeof(char), TAMANHO_ESTADO, arquivo_gerado);
}


int registro_removido(FILE* arquivo, int byteoffset_inicial) {
    if (arquivo == NULL)
        return 1;

    int registro_removido;
    fseek(arquivo, byteoffset_inicial, SEEK_SET);
    fread(&registro_removido, sizeof(int), 1, arquivo);
    
    if (registro_removido == REMOVIDO)
        return 1;

    return 0;
}

/**
* Recebe um arquivo binário, o início da linha a ser lida e uma variável do tipo bebê para armazenr os campos lidos.
* Se o arquivo não for nulo então é executado a função;
* Posiciona o seek no início da linha a ser linha e obtém os valores conforme antes escritos pela função "escrever_arquivo_bin".
*/
void ler_registro(FILE* arquivo, int byteoffset_inicial, BEBE** bebe) {
    if (arquivo == NULL) 
        return;

    int idNascimento, idadeMae, tamanho_campo_cidadeMae, tamanho_campo_cidadeBebe;
    char sexoBebe[1];
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
    char *cidadeMae;
    char *cidadeBebe;

    fseek(arquivo, byteoffset_inicial, SEEK_SET);
    fread(&tamanho_campo_cidadeMae, sizeof(int), 1, arquivo);
    fread(&tamanho_campo_cidadeBebe, sizeof(int), 1, arquivo);
    
    if (tamanho_campo_cidadeMae > 0)
        cidadeMae = (char*) malloc((tamanho_campo_cidadeMae+1) * sizeof(char));
    else 
        cidadeMae = (char*) malloc(sizeof(char));
    fread(cidadeMae, sizeof(char), tamanho_campo_cidadeMae, arquivo);
    cidadeMae[tamanho_campo_cidadeMae] = '\0';

    if (tamanho_campo_cidadeBebe > 0)
        cidadeBebe = (char*) malloc((tamanho_campo_cidadeBebe+1) * sizeof(char));
    else 
        cidadeBebe = (char*) malloc(sizeof(char));
    fread(cidadeBebe, sizeof(char), tamanho_campo_cidadeBebe, arquivo);
    cidadeBebe[tamanho_campo_cidadeBebe] = '\0';

    int byteoffset_tamanho_fixo = TAMANHO_MAXIMO_REGISTRO - tamanho_campo_cidadeBebe - tamanho_campo_cidadeMae - 2*sizeof(int);
    
    fseek(arquivo, byteoffset_tamanho_fixo, SEEK_CUR);
    
    fread(&idNascimento, sizeof(int), 1, arquivo);
    fread(&idadeMae, sizeof(int), 1, arquivo);    
    fread(dataNascimento, sizeof(char), TAMANHO_DATA_NASCIMENTO, arquivo);
    dataNascimento[TAMANHO_DATA_NASCIMENTO] = '\0';
    
    fread(sexoBebe, sizeof(char), 1, arquivo);
    fread(estadoMae, sizeof(char), TAMANHO_ESTADO, arquivo);
    estadoMae[TAMANHO_ESTADO] = '\0';

    fread(estadoBebe, sizeof(char), TAMANHO_ESTADO, arquivo);
    estadoBebe[TAMANHO_ESTADO] = '\0';

    (*bebe) = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);

    return;
}

/**
* Recebe uma variável do tipo BEBE e caso não seja nula então imprime os valores dos seus elementos
* utilizando as funções da biblioteca "bebe.h"
*/
void imprimir_registro(BEBE* bebe) {
    if (bebe != NULL) {
        // Nasceu em SAO CARLOS/SP, em 2020-04-18, um bebê de sexo FEMININO.
        // FEMININO = 2; MASCULINO = 1; IGNORADO = 0.

        printf("Nasceu em ");

        char* cidadeBebe = bebe_get_cidadeBebe(bebe);
        (cidadeBebe[0] == '\0') ? printf("-/") : printf("%s/", cidadeBebe);
        
        char estadoBebe[TAMANHO_ESTADO+1];
        strcpy(estadoBebe, bebe_get_estadoBebe(bebe));
        (estadoBebe[0] == '\0') ? printf("-, ") : printf("%s, ", estadoBebe);

        char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
        strcpy(dataNascimento, bebe_get_dataNascimento(bebe));
        (dataNascimento[0] == '\0') ? printf("em -, ") : printf("em %s, ", dataNascimento);

        char sexoBebe[1];
        strcpy(sexoBebe, bebe_get_sexoBebe(bebe));
        if (sexoBebe[0] == FEMININO)
            printf("um bebê de sexo FEMININO.\n");
        else if (sexoBebe[0] == MASCULINO)
            printf("um bebê de sexo MASCULINO.\n");
        else
            printf("um bebê de sexo IGNORADO.\n");        

    }
    return;
}

int bebe_valido_busca_combinada(FILE* arquivo_entrada, int byteoffset, BEBE* busca_combinada, BEBE** bebe) {
    if(registro_removido(arquivo_entrada, byteoffset)) return 0;
    ler_registro(arquivo_entrada, byteoffset, bebe);

    if(bebe_get_idNascimento(busca_combinada) != -1) 
        if(bebe_get_idNascimento(busca_combinada) != bebe_get_idNascimento(*bebe)) return 0;

    if(bebe_get_idadeMae(busca_combinada) != -1) 
        if(bebe_get_idadeMae(busca_combinada) != bebe_get_idadeMae(*bebe)) return 0;

    if(strcmp(bebe_get_dataNascimento(busca_combinada), "$") != 0)
        if(strcmp(bebe_get_dataNascimento(busca_combinada), bebe_get_dataNascimento(*bebe)) != 0) return 0;   

    if(bebe_get_sexoBebe(busca_combinada)[0] != '$') 
        if(bebe_get_sexoBebe(busca_combinada)[0] != bebe_get_sexoBebe(*bebe)[0]) return 0;     

    if(strcmp(bebe_get_estadoMae(busca_combinada), "$") != 0)
        if(strcmp(bebe_get_estadoMae(busca_combinada), bebe_get_estadoMae(*bebe)) != 0) return 0;   
    
    if(strcmp(bebe_get_estadoBebe(busca_combinada), "$") != 0)
        if(strcmp(bebe_get_estadoBebe(busca_combinada), bebe_get_estadoBebe(*bebe)) != 0) return 0;   
    
    if(strcmp(bebe_get_cidadeMae(busca_combinada), "$") != 0)
        if(strcmp(bebe_get_cidadeMae(busca_combinada), bebe_get_cidadeMae(*bebe)) != 0) return 0;   

    if(strcmp(bebe_get_cidadeBebe(busca_combinada), "$") != 0)
        if(strcmp(bebe_get_cidadeBebe(busca_combinada), bebe_get_cidadeBebe(*bebe)) != 0) return 0;

    return 1;   
}