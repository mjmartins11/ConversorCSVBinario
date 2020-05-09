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
        b->idNascimento = idNascimento;
        b->idadeMae = idadeMae;
        b->dataNascimento[0] = dataNascimento[0];
        b->dataNascimento[1] = dataNascimento[1];
        b->dataNascimento[2] = dataNascimento[2];
        b->dataNascimento[3] = dataNascimento[3];
        b->dataNascimento[4] = dataNascimento[4];
        b->dataNascimento[5] = dataNascimento[5];
        b->dataNascimento[6] = dataNascimento[6];
        b->dataNascimento[7] = dataNascimento[7];
        b->dataNascimento[8] = dataNascimento[8];
        b->dataNascimento[9] = dataNascimento[9];
        b->dataNascimento[10] = dataNascimento[10];
        // strcpy(b->dataNascimento, dataNascimento);
        //strcpy(b->sexoBebe, sexoBebe);
        b->sexoBebe[0] = sexoBebe[0];
        // printf("estadoMae: %s |", estadoMae);
        // printf("%c | %c \n", estadoMae[0], estadoMae[1]);
        b->estadoMae[0] = estadoMae[0];
        b->estadoMae[1] = estadoMae[1];
        b->estadoMae[2] = estadoMae[2];
        // strcpy(b->estadoMae, estadoMae);
        // strcpy(b->estadoBebe, estadoBebe);
        b->estadoBebe[0] = estadoBebe[0];
        b->estadoBebe[1] = estadoBebe[1];
        b->estadoBebe[2] = estadoBebe[2];
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