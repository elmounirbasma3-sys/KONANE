#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

/* ============================================================
   Valide que le choix est entre min et max
   ============================================================ */
int valider_choix(int min, int max) {
    int choix;
    scanf("%d", &choix);
    while (choix < min || choix > max) {
        printf("Choix invalide ! Veuillez choisir entre %d et %d : ", min, max);
        scanf("%d", &choix);
    }
    return choix;
}

/* ============================================================
   Affiche le menu principal
   ============================================================ */
void afficher_menu() {
    printf("\n\tBienvenue dans le jeu KONANE !!!\n\n");
    printf("1. JOUER\n");
    printf("2. Regles de jeu\n");
    printf("3. Quitter\n\n");
    printf("VOTRE CHOIX >>> ");
}

/* ============================================================
   Affiche les regles avec option Retour / Quitter
   ============================================================ */
void afficher_regles() {
    int choix;
    printf("\n\t<<< Regles de jeu >>>\n\n");
    printf("Le plateau est rempli alternativement de pions noirs (B) et blancs (W).\n");
    printf("Chaque joueur deplace ses pions en sautant par-dessus un pion adverse\n");
    printf("horizontalement ou verticalement (Pas en diagonale).\n");
    printf("Le pion saute est retire du plateau.\n");
    printf("Les captures multiples sont obligatoires dans le meme tour.\n");
    printf("Le joueur qui ne peut plus bouger perd la partie.\n\n");

    printf("\t<<< Regles de jeu >>>\n\n");
    printf("2. RETOUR\n");
    printf("3. Quitter\n");
    printf("VOTRE CHOIX > > > ");
    choix = valider_choix(2, 3);
    if (choix == 3) {
        printf("Fermeture du jeu...\n");
        exit(0);
    }
}

/* ============================================================
   Affiche le menu de fin de partie
   Retourne 1 si retour au menu, 0 si quitter
   ============================================================ */
int afficher_fin_partie(char* nom_gagnant) {
    int choix;
    printf("\n========================================\n");
    printf("   FIN DE LA PARTIE !\n");
    printf("   Le joueur %s a gagne !!!\n", nom_gagnant);
    printf("========================================\n\n");
    printf("1. Retour au menu principal\n");
    printf("2. Quitter\n\n");
    printf("VOTRE CHOIX >>> ");
    choix = valider_choix(1, 2);
    return choix;
}

/* ============================================================ */
int lire_choix_menu() {
    int choix;
    printf("VOTRE CHOIX >>> ");
    choix = valider_choix(1, 3);
    return choix;
}

/* ============================================================ */
int lire_choix_mode() {
    int choix;
    printf("\nChoisissez le mode de jeu :\n\n");
    printf("      >>> 1 - Joueur contre Joueur\n");
    printf("      >>> 2 - Joueur contre Machine\n\n");
    printf("                              3. RETOUR\n");
    printf("                              4. Quitter\n\n");
    printf("VOTRE CHOIX >>> ");
    choix = valider_choix(1, 4);
    return choix;
}
