#include "arquivos.h"

void criar_arquivo(FILE* arquivo_entrada, FILE* arquivo_gerado) {
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
}

void ler_arquivo(FILE* arquivo_entrada) {
    if (arquivo_entrada != NULL) {
        int byteoffset_numeroRegistroInseridos = 5;
        int numeroRegistrosInseridos, numeroRegistrosRemovidos;
        int quantidade_de_registros;
        
        fseek(arquivo_entrada, byteoffset_numeroRegistroInseridos, SEEK_SET);
        fread(&numeroRegistrosInseridos, sizeof(int), 1, arquivo_entrada);
        fread(&numeroRegistrosRemovidos,  sizeof(int), 1, arquivo_entrada);

        quantidade_de_registros = numeroRegistrosInseridos - numeroRegistrosRemovidos;

        if (quantidade_de_registros == 0) {
            printf("Registro inexistente.\n");
            return;
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
    }
    return;
}