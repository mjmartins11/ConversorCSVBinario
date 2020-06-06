#include "arquivos.h"

/**
* Recebe um arquivo, o nome dele e o tipo como vai abrir (leitura, escrita, leitura e escrita...)
* e retorna 0 caso não encontre o arquivo e 1 caso contrário.
*/
int abrir_arquivo(FILE** arquivo, char nome_do_arquivo[TAMANHO_NOME_ARQUIVO], char tipo[4]) {
    (*arquivo) = fopen(nome_do_arquivo, tipo);
    if ((*arquivo) == NULL)
        return 0;
    return 1;
}

/**
* Recebe um arquivo e o fecha.
*/
void fechar_arquivo(FILE** arquivo) {
    if ((*arquivo) != NULL) {
        fclose(*arquivo);
        arquivo = NULL;
    }
    return;
}


/**
* Recebe um arquivo e verifica se o arquivo existe, se ele é consistente e se ele possui registros.
* Retorna 0 caso o arquivo seja incosistente ou não exista (arquivo invalido);
* Retorna 1 caso não possua registros (arquivo válido, mas não há registros).
*/
int validar_procura(FILE* arquivo_entrada) {
    if (arquivo_entrada == NULL || verificar_status(arquivo_entrada) == INCONSISTENTE)
        return 0;
    
    if (numero_registros_inseridos(arquivo_entrada) == 0) {
        printf("Registro Inexistente.\n");
        return 1;        
    }

    return VALIDO;
}

/**
* Recebe o nome do arquivo csv e o nome do arquivo binário onde serão escritos os dados;
* Faz a verificação se os arquivos abrem normalmente, caso não retorna 0;
* Se os arquivos estiverem todos disponíveis para o acesso então é montado o registro de cabeçalho do arquivo
* binário e em seguida lê linha por linha do arquivo csv obtendo os campos para cada registro a ser
* escrito no arquivo binário. A cada registro transcrito é atualizado as informações no registro de cabeçalho
* por meio de funções da biblioteca "arquivo_cabecalho.h". Também é utilizado funções da biblioteca "arquivo_conteudo.h".
*/
int criar_arquivo(char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada; /*!< Arquivo csv */
    FILE* arquivo_gerado; /*!< Arquivo binário */

    if (!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_csv, "r"))
        return 0;
    
    if (!abrir_arquivo(&arquivo_gerado, nome_do_arquivo_bin, "w+b"))
        return 0;

    BEBE* bebe;
    char cabecalho_csv[TAMANHO_CABECALHO_CSV]; /*!< String utilizada ler o cabeçalho do arquivo csv*/
    char registro[TAMANHO_REGISTRO_CSV]; /*!< String utilizada para armanzenar a linha lida do arquivo csv*/
    char *retorno; /*!< É utilizada para verificar o retorno da leitura do arquivo csv*/
    int quantidade_de_registros = 0;

    inicializar_cabecalho(arquivo_gerado);

    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv

    retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    
    while (retorno != NULL) { 
        ler_arquivo_csv(&bebe, registro);   
        inserir_registro_bin(arquivo_gerado, bebe, quantidade_de_registros, 0);
        quantidade_de_registros++;
        atualizar_rrn_proximo_registro(arquivo_gerado, quantidade_de_registros);
        bebe_apagar(&bebe);
        retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    } 

    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, quantidade_de_registros);    
    atualizar_status(arquivo_gerado, '1');

    fechar_arquivo(&arquivo_entrada);
    fechar_arquivo(&arquivo_gerado);

    return 1;
}

/**
* Recebe o nome do arquivo binário para fazer a leitura.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo é inconsistente (status == 0);
* Com todas as condições favoráveis para a execução da função então ela lê o arquivo binário utilizando funções
* da biblioteca "arquivo_conteudo.h".
*/
int ler_arquivo(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada; /*!< Arquivo binário */

    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "rb"))
        return 0;

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }

    int quantidade_de_registros = quantidade_total_de_registros(arquivo_entrada);
    int i, byteoffset_inicial_linha; /*!< É utilizado para indicar o início do registro atual */
    for (i = 0; i < quantidade_de_registros; i++) {
        BEBE *bebe;
        byteoffset_inicial_linha = (i * TAMANHO_REGISTRO_BIN) + TAMANHO_CABECALHO_BIN;
        if (!registro_removido(arquivo_entrada, byteoffset_inicial_linha)) {
            ler_registro(arquivo_entrada, byteoffset_inicial_linha, &bebe);
            imprimir_registro(bebe);
            bebe_apagar(&bebe);
        }
    }

    fechar_arquivo(&arquivo_entrada);

    return 1;
}

/**
* Recebe o nome do arquivo binário e uma estrutura bebe.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo está consistente;
* Com todas as condições favoráveis para a execução da função, realiza a inserção.
* Na inserção, realiza a busca no arquivo do proxRRN  e insere no local;
* Ao fim da inserção, atualiza o proxRRN e a quantidade de registros. Além do status.
* Retorna se a operação foi um sucesso.
*/
int inserir_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* bebe) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    
    if (!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r+b"))
        return 0;

    if (verificar_status(arquivo_entrada) == INCONSISTENTE) {
        fechar_arquivo(&arquivo_entrada);
        return 0;
    }

    atualizar_status(arquivo_entrada, '0');

    int rrn = rrn_prox_registro(arquivo_entrada);
    int registros_inseridos = numero_registros_inseridos(arquivo_entrada);
    
    inserir_registro_bin(arquivo_entrada, bebe, rrn, 0);

    atualizar_rrn_proximo_registro(arquivo_entrada, rrn + 1);
    atualizar_quantidade_de_registros_inseridos(arquivo_entrada, registros_inseridos + 1);
    
    atualizar_status(arquivo_entrada, '1');     
    fechar_arquivo(&arquivo_entrada);

    return 1;
}

/**
* Recebe o nome do arquivo binário, o RRN do bebe a ser atualizado e uma estrutura bebe que contém as alterações a serem realizadas.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo está consistente e possui registros;
* Com todas as condições favoráveis para a execução da função, realiza a atualização.
* Na atualização, calcula o byteoffset a ser atualizado, verifica se não foi removido ou se é valido;
* Cria um novo bebe que será o bebe final. Este contém os novos campos e os campos que não foram alterados;
* Atualiza o bebe.
* Retorna se a operação foi um sucesso.
*/
int atualizar_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], int rrn, BEBE* bebe_alteracao) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    
    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r+b"))
        return 0;

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }
    
    int byteoffset = rrn * TAMANHO_REGISTRO_BIN + TAMANHO_CABECALHO_BIN;    

    if (registro_removido(arquivo_entrada, byteoffset) || rrn_prox_registro(arquivo_entrada) <= rrn) {
        fechar_arquivo(&arquivo_entrada);
        return 1;
    }

    BEBE *bebe;

    int inicio_campo_fixo = 0;

    if(atualizar_dados_registro(arquivo_entrada, byteoffset, bebe_alteracao, &bebe, &inicio_campo_fixo) == 0) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro Inexistente.\n");
        return 1;
    }

    atualizar_status(arquivo_entrada, '0');
    inserir_registro_bin(arquivo_entrada, bebe, rrn, inicio_campo_fixo);
    atualizar_numero_registros_atualizados(arquivo_entrada, (numero_registros_atualizados(arquivo_entrada) + 1));
    atualizar_status(arquivo_entrada, '1');

    fechar_arquivo(&arquivo_entrada);

    return 1;
}

/**
* Recebe o nome do arquivo binário e uma estrutura bebe que contém os campos da busca combinada.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo está consistente e possui registros;
* Com todas as condições favoráveis para a execução da função, realiza a busca combinada.
* Na busca combinada, percorre o arquivo imprimindo registros que atenderem aos requisitos da busca combinada.
* Retorna se a operação foi um sucesso.
*/
int busca_por_campos(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* busca_combinada) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    
    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "rb")) 
        return 0;
    

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }

    int byteoffset;
    int quantidade_de_registros = quantidade_total_de_registros(arquivo_entrada) * TAMANHO_REGISTRO_BIN;
    int encontrou_registro = 0;
    BEBE* bebe;
    for(byteoffset = BYTEOFFSET_INICIO_CONTEUDO; byteoffset < quantidade_de_registros; byteoffset += TAMANHO_REGISTRO_BIN) 
        if(bebe_valido_busca_combinada(arquivo_entrada, byteoffset, busca_combinada, &bebe)) {
            encontrou_registro = 1;
            imprimir_registro(bebe);
        }

    if(!encontrou_registro) 
        printf("Registro Inexistente.\n");
     
    fechar_arquivo(&arquivo_entrada);
    
    return 1;
}

/**
* Recebe o nome do arquivo binário e o RRN do registro a ser procurado.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo está consistente e possui registros;
* Verifica se o registro não foi removido e se o RRN é valido;
* Com todas as condições favoráveis para a execução da função, realiza a busca por RRN.
* Na busca por RRN, lê o registro e imprime.
* Retorna se a operação foi um sucesso.
*/
int busca_rrn(char nome_arquivo[TAMANHO_NOME_ARQUIVO], int rrn) {
    FILE* arquivo_entrada; /*!< Arquivo binário */

    if (!abrir_arquivo(&arquivo_entrada, nome_arquivo, "rb"))
        return 0;

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }

    int byteoffset = rrn * TAMANHO_REGISTRO_BIN + TAMANHO_CABECALHO_BIN;    
    if (registro_removido(arquivo_entrada, byteoffset) || rrn_prox_registro(arquivo_entrada) <= rrn) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro Inexistente.\n");
        return 1;
    }

    BEBE* bebe;
    ler_registro(arquivo_entrada, byteoffset, &bebe);
    imprimir_registro(bebe);
    bebe_apagar(&bebe);

    fechar_arquivo(&arquivo_entrada);
    
    return 1;
}

/**
* Recebe o nome do arquivo binário e uma estrutura bebe que contém os campos que devem ser validados na busca para remoção.
* Inicialmente verifica se o arquivo abre normalmente, caso não retorna 0;
* Logo após verifica se o arquivo está consistente e possui registros;
* Com todas as condições favoráveis para a execução da função, realiza a remoção.
* Na remoção, o arquivo é percorrido e os campos que atenderem os requisitos da estrutura, são removidos.
* Retorna se a operação foi um sucesso.
*/
int remover_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], BEBE* busca_combinada) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    
    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r+b"))
        return 0;

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }

    int byteoffset;
    int quantidade_de_registros = quantidade_total_de_registros(arquivo_entrada) * TAMANHO_REGISTRO_BIN;
    BEBE* bebe;
    /*!< Percorre o arquivo */
    for(byteoffset = TAMANHO_CABECALHO_BIN; byteoffset < quantidade_de_registros; byteoffset += TAMANHO_REGISTRO_BIN) {
        if(bebe_valido_busca_combinada(arquivo_entrada, byteoffset, busca_combinada, &bebe)) {
            atualizar_status(arquivo_entrada, '0');
            marcar_como_removido(arquivo_entrada, byteoffset);
            atualizar_numero_registros_removidos(arquivo_entrada, (numero_registros_removidos(arquivo_entrada)+1));
            atualizar_quantidade_de_registros_inseridos(arquivo_entrada, (numero_registros_inseridos(arquivo_entrada)-1));
            atualizar_status(arquivo_entrada, '1');
        }
    }

    fechar_arquivo(&arquivo_entrada);

    return 1;
}