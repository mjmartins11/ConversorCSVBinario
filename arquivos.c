#include "arquivos.h"

int abrir_arquivo(FILE** arquivo, char nome_do_arquivo[TAMANHO_NOME_ARQUIVO], char tipo[4]) {
    (*arquivo) = fopen(nome_do_arquivo, tipo);
    if ((*arquivo) == NULL)
        return 0;
    return 1;
}

void fechar_arquivo(FILE** arquivo) {
    if ((*arquivo) != NULL)
        fclose(*arquivo);
    return;
}

int criar_arquivo(char nome_do_arquivo_csv[TAMANHO_NOME_ARQUIVO], char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada;
    FILE* arquivo_gerado;

    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_csv, "r"))
        return 0;
    
    if(!abrir_arquivo(&arquivo_gerado, nome_do_arquivo_bin, "w+b"))
        return 0;

    REGISTRO_CABECALHO registro_cabecalho;
    BEBE* bebe;
    char cabecalho_csv[TAMANHO_CABECALHO_CSV];
    char registro[TAMANHO_REGISTRO_CSV];
    char *retorno;
    int quantidade_de_registros = 0;

    inicializar_cabecalho(arquivo_gerado, &registro_cabecalho);

    fgets(cabecalho_csv, TAMANHO_CABECALHO_CSV, arquivo_entrada); //Pulando linha de cabeçalho do arquivo .csv

    retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    while (retorno != NULL) {    
        ler_arquivo_csv(&bebe, registro);   
        escrevar_arquivo_bin(arquivo_gerado, bebe, quantidade_de_registros);
        quantidade_de_registros++;
        atualizar_rrn_proximo_registro(arquivo_gerado, &registro_cabecalho, quantidade_de_registros);

        bebe_apagar(&bebe);
        retorno = fgets(registro, TAMANHO_REGISTRO_CSV, arquivo_entrada);
    } 

    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, &registro_cabecalho, quantidade_de_registros);
    atualizar_status(arquivo_gerado, &registro_cabecalho, 1);

    fechar_arquivo(&arquivo_entrada);
    fechar_arquivo(&arquivo_gerado);

    return 1;
}

int ler_arquivo(char nome_do_arquivo_bin[TAMANHO_NOME_ARQUIVO]) {
    FILE* arquivo_entrada;

    if(!abrir_arquivo(&arquivo_entrada, nome_do_arquivo_bin, "r"))
        return 0;
    
    int status;
    fread(&status, sizeof(int), 1, arquivo_entrada);
    if (status == INCONSISTENTE) {
        fechar_arquivo(&arquivo_entrada);
        return 0;
    }

    int byteoffset_numeroRegistroInseridos = 5;
    int numeroRegistrosInseridos, numeroRegistrosRemovidos;
    int quantidade_de_registros;
    
    fseek(arquivo_entrada, byteoffset_numeroRegistroInseridos, SEEK_SET);
    fread(&numeroRegistrosInseridos, sizeof(int), 1, arquivo_entrada);
    fread(&numeroRegistrosRemovidos,  sizeof(int), 1, arquivo_entrada);

    quantidade_de_registros = numeroRegistrosInseridos - numeroRegistrosRemovidos;

    if (quantidade_de_registros == 0) {
        fechar_arquivo(&arquivo_entrada);
        printf("Registro inexistente.\n");
        return 1;
    }

    char caracter_de_removido;
    int i, byteoffset_inicial_linha;
    for (i = 0; i < quantidade_de_registros; i++) {
        BEBE *bebe;
        byteoffset_inicial_linha = (i * TAMANHO_REGISTRO_BIN) + TAMANHO_CABECALHO_BIN;
        fseek(arquivo_entrada, byteoffset_inicial_linha, SEEK_SET);
        fread(&caracter_de_removido, sizeof(char), 1, arquivo_entrada);
        if (caracter_de_removido != REMOVIDO) {
            ler_registro(arquivo_entrada, byteoffset_inicial_linha, &bebe);
            imprimir_registro(bebe);
            bebe_apagar(&bebe);
        }
    }

    fechar_arquivo(&arquivo_entrada);

    return 1;
}