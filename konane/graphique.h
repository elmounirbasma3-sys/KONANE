#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <SDL3/SDL.h>
#include "struct.h"

/* Dimensions */
#define LARGEUR_FENETRE  700
#define HAUTEUR_FENETRE  800
#define TAILLE_CASE       80
#define OFFSET_X          50
#define OFFSET_Y         120

/* Structure selection */
typedef struct {
    int x;
    int y;
    int selectionne;
} Selection;

/* Fonctions */
int  initialiser_SDL(SDL_Window** fenetre, SDL_Renderer** renderer);
void definir_icone(SDL_Window* fenetre, const char* chemin_bmp);  /* NOUVEAU */
void fermer_SDL(SDL_Window* fenetre, SDL_Renderer* renderer);
void dessiner_cercle(SDL_Renderer* renderer, int cx, int cy, int r);
void dessiner_plateau_graphique(SDL_Renderer* renderer,
                                char M[7][7],
                                Selection sel,
                                const char* message);
void dessiner_bouton(SDL_Renderer* renderer,
                     int x, int y, int w, int h,
                     const char* label,
                     int hover);
int  gerer_menu(SDL_Window* fenetre, SDL_Renderer* renderer);
int  gerer_menu_mode(SDL_Window* fenetre, SDL_Renderer* renderer);
int  gerer_fin_partie(SDL_Window* fenetre,
                      SDL_Renderer* renderer,
                      const char* nom_gagnant);
void jouer_graphique_HvH(SDL_Window* fenetre,
                          SDL_Renderer* renderer);
void jouer_graphique_HvM(SDL_Window* fenetre,
                          SDL_Renderer* renderer);

#endif
