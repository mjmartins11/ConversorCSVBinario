#ifndef BEBE_H
    #define BEBE_H

    #include <stdlib.h>
    #include <string.h>

    typedef struct bebe_ BEBE;

    BEBE* bebe_criar(int idNascimento, int idadeMae, char dataNascimento[11], char sexoBebe, char estadoMae[3], char estadoBebe[3], char *cidadeMae, char *cidadeBebe);
    int bebe_get_idNascimento(BEBE *b);
    int bebe_get_idadeMae(BEBE *b);
    char* bebe_get_dataNascimento(BEBE *b);
    char bebe_get_sexoBebe(BEBE *b);
    char* bebe_get_estadoMae(BEBE *b);
    char* bebe_get_estadoBebe(BEBE *b);
    char* bebe_get_cidadeMae(BEBE *b);
    char* bebe_get_cidadeBebe(BEBE *b);
    void bebe_apagar(BEBE** b);

#endif