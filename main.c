#include <stdio.h>
#include "bebe.h"

int main(void) {
    BEBE* b;

    int idNascimento, idadeMae;
    char sexoBebe;
    char dataNascimento[11];
    char estadoMae[3];
    char estadoBebe[3];
    char *cidadeMae;
    char *cidadeBebe;

    cidadeMae = (char*) malloc(30 * sizeof(char));
    cidadeBebe = (char*) malloc(30 * sizeof(char));

    scanf("%d", &idNascimento);
    printf("idNascimento: %d\n", idNascimento);
    scanf("%d", &idadeMae);
    printf("idadeMae: %d\n", idadeMae);
    scanf("%s", dataNascimento);
    printf("dataNascimento: %s\n", dataNascimento);
    scanf("\n%c", &sexoBebe);
    printf("sexoBebe: %c\n", sexoBebe);
    scanf("%s", estadoMae);
    printf("estadoMae: %s\n", estadoMae);
    scanf("%s", estadoBebe);
    printf("estadoBebe: %s\n", estadoBebe);
    scanf("%s", cidadeMae);
    printf("cidadeMae: %s\n", cidadeMae);
    scanf("%s", cidadeBebe);
    printf("cidadeBebe: %s\n", cidadeBebe);

    b = bebe_criar(idNascimento, idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe, cidadeMae, cidadeBebe);

    printf("\nDados\n");

    printf("idNascimeto: %d\n", bebe_get_idNascimento(b));
    printf("idadeMae: %d\n", bebe_get_idadeMae(b));
    printf("dataNascimento: %s\n", bebe_get_dataNascimento(b));
    printf("sexoBebe: %c\n", bebe_get_sexoBebe(b));
    printf("estadoMae: %s\n", bebe_get_estadoMae(b));
    printf("estadoBebe: %s\n", bebe_get_estadoBebe(b));
    printf("cidadeMae: %s\n", bebe_get_cidadeMae(b));
    printf("cidadeBebe: %s\n", bebe_get_cidadeBebe(b));

    bebe_apagar(&b);

    return   0;
}