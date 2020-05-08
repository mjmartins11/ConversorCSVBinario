#include "arquivo_cabecalho.h"

void inicializar_cabecalho(FILE *arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho) {
    int i = 0;
    (*registro_cabecalho).status = 0;
    (*registro_cabecalho).RRNproxRegistro = 0;
    (*registro_cabecalho).numeroRegistrosInseridos = 0;
    (*registro_cabecalho).numeroRegistrosRemovidos = 0;
    (*registro_cabecalho).numeroRegistrosAtualizados = 0;

    atualizar_status(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).status);
    atualizar_rrn_proximo_registro(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).RRNproxRegistro);
    atualizar_quantidade_de_registros_inseridos(arquivo_gerado, registro_cabecalho, (*registro_cabecalho).numeroRegistrosInseridos);
    fwrite(&(*registro_cabecalho).numeroRegistrosRemovidos, sizeof(int), 1, arquivo_gerado);
    fwrite(&(*registro_cabecalho).numeroRegistrosAtualizados, sizeof(int), 1, arquivo_gerado);
    for(i = 0; i < 111; i++) 
        fwrite(&LIXO, sizeof(char), 1, arquivo_gerado);   
}

void atualizar_status(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, char status) {
    (*registro_cabecalho).status = status;
    fseek(arquivo_gerado, 0, SEEK_SET);
    fwrite(&status, sizeof(char), 1, arquivo_gerado);
}

void atualizar_quantidade_de_registros_inseridos(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int quantidade_de_registros) {
    (*registro_cabecalho).numeroRegistrosInseridos = quantidade_de_registros; 
    fseek(arquivo_gerado, 5, SEEK_SET);
    fwrite(&quantidade_de_registros, sizeof(int), 1, arquivo_gerado);
}

void atualizar_rrn_proximo_registro(FILE* arquivo_gerado, REGISTRO_CABECALHO *registro_cabecalho, int rrn_proximo_registro) {
    (*registro_cabecalho).RRNproxRegistro = rrn_proximo_registro;
    fseek(arquivo_gerado, 1, SEEK_SET);
    fwrite(&rrn_proximo_registro, sizeof(int), 1, arquivo_gerado);
}