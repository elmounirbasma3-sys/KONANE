#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "plateau.h"
#include "verification.h"
#include "menu.h"

Joueur J1, J2, JoueurActuel;
int contreMachine = 0;

/* ============================================================ */
void qui_commence() {
    int r = rand() % 2;
    printf("APRES TIRAGE AU SORT ...\n");
    if (r == 0) {
        J1.pion = 'B'; J2.pion = 'W';
        JoueurActuel = J1;
    } else {
        J1.pion = 'W'; J2.pion = 'B';
        JoueurActuel = J2;
    }
    printf("%s va commencer avec le NOIR\n\n", JoueurActuel.nom);
}

/* ============================================================ */
void qui_commence_IA(Joueur* JH, Joueur* JM) {
    int r = rand() % 2;
    printf("APRES TIRAGE AU SORT ...\n");
    if (r == 0) {
        JH->pion = 'B';
        strcpy(JM->nom, "Machine");
        JM->pion = 'W';
        JoueurActuel = *JH;
        printf("%s va commencer avec le NOIR\n\n", JH->nom);
    } else {
        JH->pion = 'W';
        strcpy(JM->nom, "Machine");
        JM->pion = 'B';
        JoueurActuel = *JM;
        printf("La Machine va commencer avec le NOIR\n\n");
    }
}

/* ============================================================ */
void CoupEnleveHumain() {
    int x, y;
    printf("Tour de : %s\n", JoueurActuel.nom);
    printf("ENTRER LA LIGNE ET LA COLONNE DU COUP CHOISI :\n");
    printf("LIGNE > ");
    scanf("%d", &x); x--;
    printf("COLONNE > ");
    scanf("%d", &y); y--;

    while (x < 0 || x > 6 || y < 0 || y > 6
           || plateau[x][y] != JoueurActuel.pion) {
        printf("Erreur ! Ce n'est pas votre pion ou position invalide. Reessayez.\n");
        printf("LIGNE > "); scanf("%d", &x); x--;
        printf("COLONNE > "); scanf("%d", &y); y--;
    }
    plateau[x][y] = '.';
    printf("Coup enleve\n");
}

/* ============================================================ */
void CoupEnleveMachine(Joueur* J) {
    int positions[49][2], count = 0, r, i, j;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            if (plateau[i][j] == J->pion) {
                positions[count][0] = i;
                positions[count][1] = j;
                count++;
            }
    if (count > 0) {
        r = rand() % count;
        plateau[positions[r][0]][positions[r][1]] = '.';
        printf("La machine enleve un pion ...\n");
    }
}

/* ============================================================ */

void deplacerCoupHumain() {
    Coup c, nc;
    int valide = 0;
    int nx, ny;
    int mx, my;

    do {
        printf("Position initiale (ligne colonne) : ");
        scanf("%d %d", &c.x1, &c.y1);
        c.x1--; c.y1--;

        printf("Position finale   (ligne colonne) : ");
        scanf("%d %d", &c.x2, &c.y2);
        c.x2--; c.y2--;

        if (coupLegal(c, JoueurActuel.pion)) {

            /* Effectuer le premier saut */
            mx = (c.x1 + c.x2) / 2;
            my = (c.y1 + c.y2) / 2;

            plateau[c.x2][c.y2] = plateau[c.x1][c.y1];
            plateau[c.x1][c.y1] = '.';
            plateau[mx][my]     = '.';

            valide = 1;
            afficher_plateau();

            while (AnotherJump(c.x2, c.y2, JoueurActuel.pion)) {

                printf("Autre Saut possible depuis (%d,%d) !\n",
                       c.x2+1, c.y2+1);
                printf("Nouvelle position finale (ligne colonne) : ");
                scanf("%d %d", &nx, &ny);
                nx--; ny--;

                /* Construire le nouveau coup */
                nc.x1 = c.x2;
                nc.y1 = c.y2;
                nc.x2 = nx;
                nc.y2 = ny;

                if (coupLegal(nc, JoueurActuel.pion)) {

                    /* Verifier que le saut reste sur
                       la meme ligne OU la meme colonne */
                    int meme_ligne   = (nc.x1 == nc.x2);
                    int meme_colonne = (nc.y1 == nc.y2);

                    if (!meme_ligne && !meme_colonne) {
                        printf("Erreur ! Le saut doit rester sur"
                               " la meme ligne ou la meme colonne.\n");
                        continue;
                    }

                    /* Effectuer le saut suivant */
                    int mx2 = (nc.x1 + nc.x2) / 2;
                    int my2 = (nc.y1 + nc.y2) / 2;

                    plateau[nc.x2][nc.y2] = plateau[nc.x1][nc.y1];
                    plateau[nc.x1][nc.y1] = '.';
                    plateau[mx2][my2]     = '.';

                    /* Mettre a jour la position courante */
                    c.x2 = nc.x2;
                    c.y2 = nc.y2;

                    afficher_plateau();

                } else {
                    printf("Coup invalide ! Reessayez depuis (%d,%d).\n",
                           c.x2+1, c.y2+1);
                }
            }

        } else {
            printf("Coup Invalide. Recommencez!\n");
        }

    } while (!valide);
}

/* ============================================================ */
void deplacerCoupMachine(Joueur* J) {
    Coup coups[200], c;
    int count = 0, r, i, j, k;
    int d[4][2] = {{-2,0},{2,0},{0,-2},{0,2}};

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            if (plateau[i][j] == J->pion) {
                for (k = 0; k < 4; k++) {
                    c.x1 = i; c.y1 = j;
                    c.x2 = i + d[k][0];
                    c.y2 = j + d[k][1];
                    if (coupLegal(c, J->pion))
                        coups[count++] = c;
                }
            }
        }
    }

    if (count == 0) {
        printf("Pas de deplacement possible pour la Machine.\n\n");
        return;
    }

    r = rand() % count;
    c = coups[r];
    int mx = (c.x1 + c.x2) / 2;
    int my = (c.y1 + c.y2) / 2;
    plateau[c.x2][c.y2] = plateau[c.x1][c.y1];
    plateau[c.x1][c.y1] = '.';
    plateau[mx][my] = '.';
    printf("Machine joue : (%d,%d) -> (%d,%d)\n",
           c.x1+1, c.y1+1, c.x2+1, c.y2+1);
}

/* ============================================================
   Joueur vs Joueur
   ============================================================ */
void jouerHumainVsHumain() {
    int choix_fin;
    char nom_gagnant[50];
    Joueur joueur_noir, joueur_blanc;

    printf("JOUEUR 1, ENTRER VOTRE NOM : "); scanf("%s", J1.nom);
    printf("JOUEUR 2, ENTRER VOTRE NOM : "); scanf("%s", J2.nom);

    qui_commence();
    afficher_plateau();

    /* Identifier Noir et Blanc */
    if (J1.pion == 'B') {
        joueur_noir  = J1;
        joueur_blanc = J2;
    } else {
        joueur_noir  = J2;
        joueur_blanc = J1;
    }

    /* ===  Suppression initiale ===
       Noir (B) retire un pion B en premier
       Blanc (W) retire un pion W ensuite     */

    JoueurActuel = joueur_noir;
    CoupEnleveHumain();
    afficher_plateau();

    JoueurActuel = joueur_blanc;
    CoupEnleveHumain();
    afficher_plateau();

    /* === Phase principale : Noir commence === */
    JoueurActuel = joueur_noir;

    while (1) {
        /* Verifier si le joueur actuel peut jouer */
        if (!canPlay(JoueurActuel.pion)) {
            /* L'adversaire gagne */
            Joueur gagnant = (JoueurActuel.pion == joueur_noir.pion)
                             ? joueur_blanc : joueur_noir;
            strcpy(nom_gagnant, gagnant.nom);

            choix_fin = afficher_fin_partie(nom_gagnant);
            if (choix_fin == 2) {
                printf("Fermeture du jeu...\n");
                exit(0);
            }
            return;
        }

        /* Afficher uniquement le nom du joueur */
        printf("\nTour de : %s\n", JoueurActuel.nom);
        deplacerCoupHumain();
        afficher_plateau();

        /* Alterner */
        JoueurActuel = (JoueurActuel.pion == joueur_noir.pion)
                       ? joueur_blanc : joueur_noir;
    }
}

/* ============================================================
   Joueur vs Machine
   ============================================================ */
void jouerHumainVsMachine() {
    Joueur JH, JM;
    int choix_fin;
    char nom_gagnant[50];

    printf("Joueur contre machine\n");
    printf("ENTRER VOTRE NOM : "); scanf("%s", JH.nom);
    printf("\n\t<<<< %s VS Machine >>>\n\n", JH.nom);

    qui_commence_IA(&JH, &JM);
    afficher_plateau();

    /* Identifier Noir et Blanc */
    Joueur joueur_noir  = (JH.pion == 'B') ? JH : JM;
    Joueur joueur_blanc = (JH.pion == 'W') ? JH : JM;

    /* Noir enleve en premier */
    JoueurActuel = joueur_noir;
    if (JoueurActuel.pion == JH.pion) {
        CoupEnleveHumain();
    } else {
        CoupEnleveMachine(&JM);
    }
    afficher_plateau();

    /* Blanc enleve ensuite */
    JoueurActuel = joueur_blanc;
    if (JoueurActuel.pion == JH.pion) {
        CoupEnleveHumain();
    } else {
        CoupEnleveMachine(&JM);
    }
    afficher_plateau();

    /* Phase principale : Noir commence */
    JoueurActuel = joueur_noir;

    while (1) {
        /* Tour Humain */
        if (JoueurActuel.pion == JH.pion) {
            if (!canPlay(JH.pion)) {
                printf("\nVous ne pouvez plus jouer !\n");
                strcpy(nom_gagnant, JM.nom);
                choix_fin = afficher_fin_partie(nom_gagnant);
                if (choix_fin == 2) {
                    printf("Fermeture du jeu...\n");
                    exit(0);
                }
                return;
            }
            printf("\nTour de : %s\n", JH.nom);
            deplacerCoupHumain();
            afficher_plateau();
            JoueurActuel = JM;

        /* Tour Machine */
        } else {
            if (!canPlay(JM.pion)) {
                printf("\nLa Machine ne peut plus jouer !\n");
                strcpy(nom_gagnant, JH.nom);
                choix_fin = afficher_fin_partie(nom_gagnant);
                if (choix_fin == 2) {
                    printf("Fermeture du jeu...\n");
                    exit(0);
                }
                return;
            }
            printf("\nTour de : Machine\n");
            deplacerCoupMachine(&JM);
            afficher_plateau();
            JoueurActuel = JH;
        }
    }
}
