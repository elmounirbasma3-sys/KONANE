#include <stdio.h>
#include <stdlib.h>
#include "verification.h"
#include "plateau.h"

/* ============================================================
   Verifie si un coup est legal selon les regles du Konane
   ============================================================ */
int coupLegal(Coup c, char pion) {
    int dx, dy, mx, my;

    if (c.x1 < 0 || c.x1 > 6 || c.y1 < 0 || c.y1 > 6 ||
        c.x2 < 0 || c.x2 > 6 || c.y2 < 0 || c.y2 > 6)
        return 0;

    if (plateau[c.x1][c.y1] != pion)
        return 0;

    if (plateau[c.x2][c.y2] != '.')
        return 0;

    dx = c.x2 - c.x1;
    dy = c.y2 - c.y1;

    if (!((abs(dx) == 2 && dy == 0) || (dx == 0 && abs(dy) == 2)))
        return 0;

    mx = (c.x1 + c.x2) / 2;
    my = (c.y1 + c.y2) / 2;

    if (plateau[mx][my] == '.' || plateau[mx][my] == pion)
        return 0;

    return 1;
}

/* ============================================================
   Verifie si le joueur peut encore jouer
   ============================================================ */
int canPlay(char pion) {
    int i, j, k;

    int directions[4][2] = {
        {-2,  0},
        { 2,  0},
        { 0, -2},
        { 0,  2}
    };

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if (plateau[i][j] == pion) {
                for (k = 0; k < 4; k++) {
                    Coup c;
                    c.x1 = i;
                    c.y1 = j;
                    c.x2 = i + directions[k][0];
                    c.y2 = j + directions[k][1];
                    if (coupLegal(c, pion))
                        return 1;
                }
            }
        }
    }
    return 0;
}

/* ============================================================
   Fin du jeu
   ============================================================ */
int fin_jeu(char pion) {
    if (canPlay(pion) == 0)
        return 1;
    return 0;
}

/* ============================================================
   Verifie si depuis la position (x, y) un autre saut est
   possible DANS LA MEME DIRECTION (meme ligne ou meme colonne)

   Logique :
   ✔ Le pion est sur (x, y)
   ✔ La case suivante (x+1 ou y+1) contient un pion ADVERSE
   ✔ La case apres (x+2 ou y+2) est VIDE

   Le saut suivant doit rester sur :
   → la meme LIGNE  (deplacement horizontal gauche/droite)
   → ou la meme COLONNE (deplacement vertical haut/bas)
   ============================================================ */
int AnotherJump(int x, int y, char pion) {
    int k;

    /* Les 4 directions possibles */
    int directions[4][2] = {
        {-2,  0},   /* haut   : meme colonne */
        { 2,  0},   /* bas    : meme colonne */
        { 0, -2},   /* gauche : meme ligne   */
        { 0,  2}    /* droite : meme ligne   */
    };

    /* Verifier que le pion du joueur est bien sur (x, y) */
    if (x < 0 || x > 6 || y < 0 || y > 6)
        return 0;

    if (plateau[x][y] != pion)
        return 0;

    /* Tester les 4 directions depuis (x, y) */
    for (k = 0; k < 4; k++) {

        /* Case intermediaire (doit contenir pion adverse) */
        int mx = x + directions[k][0] / 2;
        int my = y + directions[k][1] / 2;

        /* Case d'arrivee (doit etre vide) */
        int nx = x + directions[k][0];
        int ny = y + directions[k][1];

        /* Verifier les limites du plateau */
        if (nx < 0 || nx > 6 || ny < 0 || ny > 6)
            continue;

        if (mx < 0 || mx > 6 || my < 0 || my > 6)
            continue;

        /* ✔ Condition 1 : piece adverse au milieu */
        if (plateau[mx][my] == '.' || plateau[mx][my] == pion)
            continue;

        /* ✔ Condition 2 : case d'arrivee vide */
        if (plateau[nx][ny] != '.')
            continue;

        /* Un autre saut est possible ! */
        return 1;
    }

    return 0;
}
