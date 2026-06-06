#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "plateau.h"
#include "jeu.h"
#include "graphique.h"
#include "struct.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    srand((unsigned int)time(NULL));

    SDL_Window*   fenetre  = NULL;
    SDL_Renderer* renderer = NULL;

    if (!initialiser_SDL(&fenetre, &renderer)) {
        printf("Erreur SDL3\n");
        return 1;
    }

    /* ✅ Definir l'icone de la fenetre */
    definir_icone(fenetre, "logo.bmp");

    strcpy(J1.nom, "Joueur1");
    strcpy(J2.nom, "Joueur2");

    int continuer = 1;
    while (continuer) {
        int choix = gerer_menu(fenetre, renderer);

        switch (choix) {
            case 1: {
                int mode = gerer_menu_mode(fenetre, renderer);
                if (mode == 1) {
                    int r = rand() % 2;
                    J1.pion = (r == 0) ? 'B' : 'W';
                    J2.pion = (J1.pion == 'B') ? 'W' : 'B';
                    jouer_graphique_HvH(fenetre, renderer);
                }
                else if (mode == 2) {
                    jouer_graphique_HvM(fenetre, renderer);
                }
                break;
            }
            case 3:
                continuer = 0;
                break;
        }
    }

    fermer_SDL(fenetre, renderer);
    return 0;
}
