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


int validar_procura(FILE* arquivo_entrada) {
    if (arquivo_entrada == NULL)
        return 0;

    if (verificar_status(arquivo_entrada) == INCONSISTENTE)
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

    inicializar_cabecalho(arquivo_gerado); //, &registro_cabecalho);

    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv

    retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    
    while (retorno != NULL) { 
        ler_arquivo_csv(&bebe, registro);   
        inserir_registro_bin(arquivo_gerado, bebe, quantidade_de_registros);
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
    
    inserir_registro_bin(arquivo_entrada, bebe, rrn);

    atualizar_rrn_proximo_registro(arquivo_entrada, rrn + 1);
    atualizar_quantidade_de_registros_inseridos(arquivo_entrada, registros_inseridos + 1);
    
    atualizar_status(arquivo_entrada, '1');     
    fechar_arquivo(&arquivo_entrada);

    return 1;
}

int atualizar_registro(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO], int rrn_busca, BEBE* bebe) {
    FILE* arquivo_entrada; /*!< Arquivo binário */
    
    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r+b"))
        return 0;

    int esta_valido = validar_procura(arquivo_entrada);
    if (esta_valido != VALIDO) {
        fechar_arquivo(&arquivo_entrada);
        return esta_valido;
    }
    
    int byteoffset = rrn_busca * TAMANHO_REGISTRO_BIN + TAMANHO_CABECALHO_BIN;    
    if (registro_removido(arquivo_entrada, byteoffset)) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro Inexistente.\n");
        return 1;
    }

    // atualizar_dados_registro();

    fechar_arquivo(&arquivo_entrada);

    return 1;
}

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
    if (registro_removido(arquivo_entrada, byteoffset)) {
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