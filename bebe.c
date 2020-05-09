#include "bebe.h"
#include <stdio.h>

struct bebe_ {
    int idNascimento;
    int idadeMae;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO+1];
    char sexoBebe[1];
    char estadoMae[TAMANHO_ESTADO+1];
    char estadoBebe[TAMANHO_ESTADO+1];
    char* cidadeMae;
    char* cidadeBebe;
};

BEBE* bebe_criar(int idNascimento, int idadeMae, char dataNascimento[TAMANHO_DATA_NASCIMENTO+1], char sexoBebe[1], char estadoMae[TAMANHO_ESTADO+1], char estadoBebe[TAMANHO_ESTADO+1], char *cidadeMae, char *cidadeBebe) {
    BEBE* b = (BEBE*) malloc(sizeof(BEBE));
    memset(b, 0, sizeof(BEBE));
    if (b != NULL) {
        int i = 0;
        b->idNascimento = idNascimento;
        b->idadeMae = idadeMae;
        /*
            Caso fosse utilizado uma função como strcpy para atribuir os valores
            de dataNascimento, estadoMae ou estadoBebe, quando o primeiro valor
            é \0, a função ignora os caracteres finais ($). Por isso, os
            valores foram passados campo a campo
        */
        for(i = 0; i < TAMANHO_DATA_NASCIMENTO+1; i++) 
            b->dataNascimento[i] = dataNascimento[i];
        b->sexoBebe[0] = sexoBebe[0];
        for(i = 0; i < TAMANHO_ESTADO+1; i++) 
            b->estadoMae[i] = estadoMae[i];
        for(i = 0; i < TAMANHO_ESTADO+1; i++) 
            b->estadoBebe[i] = estadoBebe[i];
        b->cidadeMae = cidadeMae;
        b->cidadeBebe = cidadeBebe;
    }
    return b;
}

int bebe_get_idNascimento(BEBE* b) {
    if (b != NULL) {
        return b->idNascimento;
    }
}

int bebe_get_idadeMae(BEBE* b) {
    if (b != NULL) {
        return b->idadeMae;
    }
}

char* bebe_get_dataNascimento(BEBE* b) {
    if (b != NULL) {
        return b->dataNascimento;
    }
}

char* bebe_get_sexoBebe(BEBE* b) {
    if (b != NULL) {
        return b->sexoBebe;
    }
}

char* bebe_get_estadoMae(BEBE* b) {
    if (b != NULL) {
        return b->estadoMae;
    }
}

char* bebe_get_estadoBebe(BEBE* b) {
    if (b != NULL) {
        return b->estadoBebe;
    }
}

char* bebe_get_cidadeMae(BEBE* b) {
    if (b != NULL) {
        return b->cidadeMae;
    }
}

char* bebe_get_cidadeBebe(BEBE* b) {
    if (b != NULL) {
        return b->cidadeBebe;
    }
}

void bebe_apagar(BEBE** b) {
    if ((*b) != NULL) {
        free((*b)->cidadeMae);
        free((*b)->cidadeBebe);
        free(*b);
        (*b) = NULL;
    }
    return;
}