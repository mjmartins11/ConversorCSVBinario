#include "bebe.h"

struct bebe_ {
    int idNascimento;
    int idadeMae;
    char dataNascimento[TAMANHO_DATA_NASCIMENTO + 1];
    char sexoBebe;
    char estadoMae[TAMANHO_ESTADO + 1];
    char estadoBebe[TAMANHO_ESTADO + 1];
    char* cidadeMae;
    char* cidadeBebe;
};

BEBE* bebe_criar(int idNascimento, int idadeMae, char dataNascimento[11], char sexoBebe, char estadoMae[3], char estadoBebe[3], char *cidadeMae, char *cidadeBebe) {
    BEBE* b = (BEBE*) malloc(sizeof(BEBE));
    memset(b, 0, sizeof(BEBE));
    if (b != NULL) {
        b->idNascimento = idNascimento;
        b->idadeMae = idadeMae;
        strcpy(b->dataNascimento, dataNascimento);
        b->sexoBebe = sexoBebe;
        strcpy(b->estadoMae, estadoMae);
        strcpy(b->estadoBebe, estadoBebe);
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

char bebe_get_sexoBebe(BEBE* b) {
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