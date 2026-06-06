#include <stdio.h>
#include "plateau.h"

char plateau[7][7];

void creation_plateau() {
    int i, j;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            plateau[i][j] = ((i + j) % 2 == 0) ? 'B' : 'W';
}

void afficher_plateau() {
    int i, j;
    printf("   1 2 3 4 5 6 7\n");
    printf("  ---------------\n");
    for (i = 0; i < 7; i++) {
        printf("%d | ", i+1);
        for (j = 0; j < 7; j++) {
            /* Afficher espace si case vide */
            if (plateau[i][j] == '.')
                printf("  ");
            else
                printf("%c ", plateau[i][j]);
        }
        printf("\n");
    }
    printf("  ---------------\n\n");
}
