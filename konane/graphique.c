#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graphique.h"
#include "plateau.h"
#include "verification.h"
#include "jeu.h"
#include "struct.h"

extern Joueur J1, J2, JoueurActuel;

/* ============================================================
   Initialiser SDL3
   ============================================================ */
int initialiser_SDL(SDL_Window** fenetre, SDL_Renderer** renderer) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 0;
    }
    *fenetre = SDL_CreateWindow("Jeu KONANE",
                                 LARGEUR_FENETRE,
                                 HAUTEUR_FENETRE,
                                 SDL_WINDOW_RESIZABLE);
    if (!(*fenetre)) {
        printf("Erreur fenetre : %s\n", SDL_GetError());
        return 0;
    }
    *renderer = SDL_CreateRenderer(*fenetre, NULL);
    if (!(*renderer)) {
        printf("Erreur renderer : %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

/* ============================================================
   Fermer SDL3
   ============================================================ */
void fermer_SDL(SDL_Window* fenetre, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
}

/* ============================================================
   Dessiner un cercle rempli
   ============================================================ */
void dessiner_cercle(SDL_Renderer* renderer,
                     int cx, int cy, int r) {
    for (int dy = -r; dy <= r; dy++) {
        int dx = (int)sqrt((double)(r*r - dy*dy));
        SDL_RenderLine(renderer,
                       (float)(cx - dx), (float)(cy + dy),
                       (float)(cx + dx), (float)(cy + dy));
    }
}

/* ============================================================
   Dessiner rectangle rempli + bordure
   ============================================================ */
static void dessiner_rect(SDL_Renderer* renderer,
                           int x, int y, int w, int h,
                           int rF, int gF, int bF,
                           int rB, int gB, int bB) {
    SDL_FRect rect = {(float)x, (float)y,
                      (float)w, (float)h};
    SDL_SetRenderDrawColor(renderer, rF, gF, bF, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, rB, gB, bB, 255);
    SDL_RenderRect(renderer, &rect);
}

/* ============================================================
   Dessiner une lettre (segments)
   ============================================================ */
static void dessiner_lettre(SDL_Renderer* renderer,
                              int x, int y, char c,
                              int r, int g, int b,
                              int taille) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    int e = taille;
    int W = taille * 5;
    int H = taille * 8;

    SDL_FRect top = {(float)x,       (float)y,          (float)W, (float)e};
    SDL_FRect mid = {(float)x,       (float)(y + H/2),  (float)W, (float)e};
    SDL_FRect bot = {(float)x,       (float)(y + H - e),(float)W, (float)e};
    SDL_FRect tl  = {(float)x,       (float)y,          (float)e, (float)(H/2 + e)};
    SDL_FRect tr  = {(float)(x+W-e), (float)y,          (float)e, (float)(H/2 + e)};
    SDL_FRect bl  = {(float)x,       (float)(y + H/2),  (float)e, (float)(H/2 + e)};
    SDL_FRect br  = {(float)(x+W-e), (float)(y + H/2),  (float)e, (float)(H/2 + e)};
    SDL_FRect mc  = {(float)(x+W/2 - e/2), (float)y,   (float)e, (float)H};

    switch (c) {
        case 'A':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'B':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'C':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'D':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'E':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'F':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'G':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            { SDL_FRect mr = {(float)(x+W/2),(float)(y+H/2),(float)(W/2),(float)e};
              SDL_RenderFillRect(renderer, &mr); }
            break;
        case 'H':
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'I':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &mc);
            break;
        case 'J':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'K':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &mid);
            { SDL_FRect k1 = {(float)(x+W/2),(float)y,(float)(W/2),(float)(H/2)};
              SDL_FRect k2 = {(float)(x+W/2),(float)(y+H/2),(float)(W/2),(float)(H/2)};
              SDL_RenderFillRect(renderer, &k1);
              SDL_RenderFillRect(renderer, &k2); }
            break;
        case 'L':
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'M':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            SDL_RenderFillRect(renderer, &mc);
            break;
        case 'N':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            SDL_RenderFillRect(renderer, &top);
            break;
        case 'O':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'P':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case 'Q':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            { SDL_FRect q = {(float)(x+W/2),(float)(y+H/2),(float)(W/2),(float)(H/2)};
              SDL_RenderFillRect(renderer, &q); }
            break;
        case 'R':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            { SDL_FRect r2 = {(float)(x+W/2),(float)(y+H/2),(float)(W/2),(float)(H/2)};
              SDL_RenderFillRect(renderer, &r2); }
            break;
        case 'S':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'T':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mc);
            break;
        case 'U':
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case 'V':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            { SDL_FRect v = {(float)(x+W/4),(float)(y+H/2),(float)(W/2),(float)(H/2)};
              SDL_RenderFillRect(renderer, &v); }
            break;
        case 'W':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            SDL_RenderFillRect(renderer, &mc);
            break;
        case 'X':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            SDL_RenderFillRect(renderer, &mid);
            break;
        case 'Y':
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &mid);
            { SDL_FRect yc = {(float)(x+W/2-e/2),(float)(y+H/2),(float)e,(float)(H/2)};
              SDL_RenderFillRect(renderer, &yc); }
            break;
        case 'Z':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &mid);
            break;
        case '0':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '1':
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '2':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            break;
        case '3':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '4':
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '5':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '6':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '7':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '8':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &bl);
            SDL_RenderFillRect(renderer, &br);
            break;
        case '9':
            SDL_RenderFillRect(renderer, &top);
            SDL_RenderFillRect(renderer, &mid);
            SDL_RenderFillRect(renderer, &bot);
            SDL_RenderFillRect(renderer, &tl);
            SDL_RenderFillRect(renderer, &tr);
            SDL_RenderFillRect(renderer, &br);
            break;
        default:
            break;
    }
}

/* ============================================================
   Afficher une chaine de caracteres
   ============================================================ */
static void afficher_texte(SDL_Renderer* renderer,
                            int x, int y,
                            const char* texte,
                            int r, int g, int b,
                            int taille) {
    int px = x;
    for (int i = 0; texte[i] != '\0'; i++) {
        char c = texte[i];
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        if (c != ' ') {
            dessiner_lettre(renderer, px, y, c, r, g, b, taille);
        }
        px += taille * 6 + 2;
    }
}

/* ============================================================
   Dessiner un bouton avec label
   ============================================================ */
void dessiner_bouton(SDL_Renderer* renderer,
                     int x, int y, int w, int h,
                     const char* label,
                     int hover) {
    /* Ombre */
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 200);
    SDL_FRect ombre = {(float)(x+4),(float)(y+4),
                       (float)w,(float)h};
    SDL_RenderFillRect(renderer, &ombre);

    /* Corps */
    if (hover) {
        SDL_SetRenderDrawColor(renderer, 80, 130, 80, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 50, 70, 50, 255);
    }
    SDL_FRect btn = {(float)x,(float)y,(float)w,(float)h};
    SDL_RenderFillRect(renderer, &btn);

    /* Bordure */
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderRect(renderer, &btn);

    /* Texte centre */
    int len = (int)strlen(label);
    int tw  = len * (2 * 6 + 2);
    int tx  = x + (w - tw) / 2;
    int ty  = y + (h - 16) / 2;
    afficher_texte(renderer, tx, ty, label, 255, 255, 255, 2);
}

/* ============================================================
   Dessiner le plateau de jeu
   ============================================================ */
void dessiner_plateau_graphique(SDL_Renderer* renderer,
                                 char M[7][7],
                                 Selection sel,
                                 const char* message) {
    int i, j;

    /* Fond */
    SDL_SetRenderDrawColor(renderer, 20, 20, 35, 255);
    SDL_RenderClear(renderer);

    /* Barre titre */
    dessiner_rect(renderer, 0, 0, LARGEUR_FENETRE, 50,
                  40, 40, 80, 80, 80, 160);
    afficher_texte(renderer, 220, 12,
                   "JEU KONANE",
                   255, 220, 50, 3);

    /* Message joueur */
    if (message && message[0] != '\0') {
        dessiner_rect(renderer,
                      OFFSET_X, 55,
                      TAILLE_CASE * 7, 35,
                      30, 30, 60, 60, 60, 120);
        afficher_texte(renderer,
                       OFFSET_X + 5, 62,
                       message,
                       255, 255, 100, 2);
    }

    /* Numeros colonnes */
    for (j = 0; j < 7; j++) {
        char num[3];
        SDL_itoa(j+1, num, 10);
        afficher_texte(renderer,
                       OFFSET_X + j * TAILLE_CASE + 35,
                       OFFSET_Y - 25,
                       num, 200, 200, 200, 2);
    }

    /* Numeros lignes */
    for (i = 0; i < 7; i++) {
        char num[3];
        SDL_itoa(i+1, num, 10);
        afficher_texte(renderer,
                       OFFSET_X - 28,
                       OFFSET_Y + i * TAILLE_CASE + 30,
                       num, 200, 200, 200, 2);
    }

    /* Cases et pions */
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {

            int rx = OFFSET_X + j * TAILLE_CASE;
            int ry = OFFSET_Y + i * TAILLE_CASE;

            /* Couleur case */
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer,
                                       210, 180, 140, 255);
            } else {
                SDL_SetRenderDrawColor(renderer,
                                       139, 90, 43, 255);
            }

            SDL_FRect case_rect = {
                (float)rx, (float)ry,
                (float)TAILLE_CASE, (float)TAILLE_CASE
            };
            SDL_RenderFillRect(renderer, &case_rect);

            /* Bordure case */
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderRect(renderer, &case_rect);

            /* Case selectionnee → bordure verte */
            if (sel.selectionne && sel.x == i && sel.y == j) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                for (int b = 1; b <= 4; b++) {
                    SDL_FRect bord = {
                        (float)(rx + b),
                        (float)(ry + b),
                        (float)(TAILLE_CASE - 2*b),
                        (float)(TAILLE_CASE - 2*b)
                    };
                    SDL_RenderRect(renderer, &bord);
                }
            }

            /* Coups possibles → bordure bleue */
            if (sel.selectionne) {
                Coup c;
                c.x1 = sel.x; c.y1 = sel.y;
                c.x2 = i;     c.y2 = j;
                if (coupLegal(c, M[sel.x][sel.y])) {
                    SDL_SetRenderDrawColor(renderer,
                                           0, 150, 255, 255);
                    SDL_FRect bord = {
                        (float)(rx+3), (float)(ry+3),
                        (float)(TAILLE_CASE-6),
                        (float)(TAILLE_CASE-6)
                    };
                    SDL_RenderRect(renderer, &bord);
                }
            }

            /* Centre case */
            int cx = rx + TAILLE_CASE / 2;
            int cy = ry + TAILLE_CASE / 2;

            /* Pion Noir */
            if (M[i][j] == 'B') {
                SDL_SetRenderDrawColor(renderer,
                                       10, 10, 10, 255);
                dessiner_cercle(renderer, cx+3, cy+3, 28);
                SDL_SetRenderDrawColor(renderer,
                                       25, 25, 25, 255);
                dessiner_cercle(renderer, cx, cy, 28);
                SDL_SetRenderDrawColor(renderer,
                                       80, 80, 80, 255);
                dessiner_cercle(renderer, cx-8, cy-8, 9);

            /* Pion Blanc */
            } else if (M[i][j] == 'W') {
                SDL_SetRenderDrawColor(renderer,
                                       160, 160, 160, 255);
                dessiner_cercle(renderer, cx+3, cy+3, 28);
                SDL_SetRenderDrawColor(renderer,
                                       240, 240, 240, 255);
                dessiner_cercle(renderer, cx, cy, 28);
                SDL_SetRenderDrawColor(renderer,
                                       255, 255, 255, 255);
                dessiner_cercle(renderer, cx-8, cy-8, 8);
            }
        } /* fin for j */
    } /* fin for i */

    SDL_RenderPresent(renderer);
}

/* ============================================================
   Menu principal
   ============================================================ */
int gerer_menu(SDL_Window* fenetre, SDL_Renderer* renderer) {
    SDL_Event event;
    int hover = 0;
    (void)fenetre;

    while (1) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        hover = 0;
        if (mx>=200&&mx<=500&&my>=250&&my<=310) hover=1;
        if (mx>=200&&mx<=500&&my>=330&&my<=390) hover=2;
        if (mx>=200&&mx<=500&&my>=410&&my<=470) hover=3;

        /* Fond */
        SDL_SetRenderDrawColor(renderer, 15, 15, 30, 255);
        SDL_RenderClear(renderer);

        /* Titre */
        dessiner_rect(renderer, 80, 50, 540, 100,
                      40, 40, 90, 100, 100, 200);
        afficher_texte(renderer, 120, 70,
                       "BIENVENUE KONANE",
                       255, 220, 50, 3);
        afficher_texte(renderer, 200, 115,
                       "JEU HAWAIEN",
                       180, 180, 220, 2);

        /* Pions decoration */
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        dessiner_cercle(renderer, 120, 400, 25);
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        dessiner_cercle(renderer, 580, 400, 25);

        /* Boutons */
        dessiner_bouton(renderer, 200, 250, 300, 60,
                        "1  JOUER", hover==1);
        dessiner_bouton(renderer, 200, 330, 300, 60,
                        "2  REGLES", hover==2);
        dessiner_bouton(renderer, 200, 410, 300, 60,
                        "3  QUITTER", hover==3);

        /* Legende */
        dessiner_rect(renderer, 50, 550, 600, 70,
                      30, 30, 50, 60, 60, 100);
        SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
        dessiner_cercle(renderer, 130, 585, 20);
        afficher_texte(renderer, 160, 577,
                       "NOIR B", 200, 200, 200, 2);
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        dessiner_cercle(renderer, 430, 585, 20);
        afficher_texte(renderer, 460, 577,
                       "BLANC W", 200, 200, 200, 2);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 3;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                if (hover==1) return 1;
                if (hover==2) return 2;
                if (hover==3) return 3;
            }
        }
        SDL_Delay(16);
    }
}

/* ============================================================
   Menu mode de jeu
   ============================================================ */
int gerer_menu_mode(SDL_Window* fenetre,
                     SDL_Renderer* renderer) {
    SDL_Event event;
    int hover = 0;
    (void)fenetre;

    while (1) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        hover = 0;
        if (mx>=150&&mx<=550&&my>=200&&my<=280) hover=1;
        if (mx>=150&&mx<=550&&my>=310&&my<=390) hover=2;
        if (mx>=150&&mx<=550&&my>=420&&my<=500) hover=3;

        SDL_SetRenderDrawColor(renderer, 15, 15, 30, 255);
        SDL_RenderClear(renderer);

        dessiner_rect(renderer, 100, 50, 500, 80,
                      40, 40, 90, 100, 100, 200);
        afficher_texte(renderer, 150, 75,
                       "MODE DE JEU",
                       255, 220, 50, 3);

        dessiner_bouton(renderer, 150, 200, 400, 80,
                        "1  JOUEUR VS JOUEUR", hover==1);
        dessiner_bouton(renderer, 150, 310, 400, 80,
                        "2  JOUEUR VS MACHINE", hover==2);
        dessiner_bouton(renderer, 150, 420, 400, 80,
                        "3  RETOUR", hover==3);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 0;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                if (hover>=1 && hover<=3) return hover;
            }
        }
        SDL_Delay(16);
    }
}

/* ============================================================
   Ecran fin de partie
   ============================================================ */
int gerer_fin_partie(SDL_Window* fenetre,
                      SDL_Renderer* renderer,
                      const char* nom_gagnant) {
    SDL_Event event;
    int hover = 0;
    (void)fenetre;

    char msg[100] = "GAGNANT : ";
    int k = (int)strlen(msg);
    for (int i = 0; nom_gagnant[i] && k < 99; i++, k++) {
        char c = nom_gagnant[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        msg[k] = c;
    }
    msg[k] = '\0';

    while (1) {
        float mx, my;
        SDL_GetMouseState(&mx, &my);

        hover = 0;
        if (mx>=120&&mx<=330&&my>=460&&my<=530) hover=1;
        if (mx>=370&&mx<=580&&my>=460&&my<=530) hover=2;

        SDL_SetRenderDrawColor(renderer, 10, 10, 20, 255);
        SDL_RenderClear(renderer);

        /* Panneau victoire */
        dessiner_rect(renderer, 80, 130, 540, 120,
                      60, 50, 10, 220, 180, 0);
        afficher_texte(renderer, 180, 155,
                       "FIN DE PARTIE",
                       255, 50, 50, 3);
        afficher_texte(renderer, 100, 215,
                       msg,
                       255, 220, 50, 2);

        /* Felicitations */
        dessiner_rect(renderer, 80, 280, 540, 100,
                      30, 30, 60, 80, 80, 150);
        afficher_texte(renderer, 130, 315,
                       "FELICITATIONS",
                       100, 255, 100, 2);

        /* Boutons */
        dessiner_bouton(renderer, 120, 460, 210, 70,
                        "1  MENU", hover==1);
        dessiner_bouton(renderer, 370, 460, 210, 70,
                        "2  QUITTER", hover==2);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) return 2;
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                if (hover==1) return 1;
                if (hover==2) return 2;
            }
        }
        SDL_Delay(16);
    }
}

/* ============================================================
   Jeu Humain vs Humain (graphique SDL3)
   ============================================================ */
void jouer_graphique_HvH(SDL_Window* fenetre,
                           SDL_Renderer* renderer) {
    Selection sel = {0, 0, 0};
    SDL_Event event;
    char nom_gagnant[50];
    char message[150];
    int phase = 0;
    int premier_click = 1;
    Coup coup_en_cours;
    coup_en_cours.x1 = -1;
    coup_en_cours.y1 = -1;
    coup_en_cours.x2 = -1;
    coup_en_cours.y2 = -1;

    Joueur joueur_noir  = (J1.pion == 'B') ? J1 : J2;
    Joueur joueur_blanc = (J1.pion == 'W') ? J1 : J2;
    JoueurActuel = joueur_noir;

    creation_plateau();

    while (1) {
        /* Message */
        if (phase == 0) {
            snprintf(message, sizeof(message),
                     "TOUR DE : %s  CLIQUER UN PION NOIR A ENLEVER",
                     joueur_noir.nom);
        } else if (phase == 1) {
            snprintf(message, sizeof(message),
                     "TOUR DE : %s  CLIQUER UN PION BLANC A ENLEVER",
                     joueur_blanc.nom);
        } else {
            snprintf(message, sizeof(message),
                     "TOUR DE : %s  (%s)",
                     JoueurActuel.nom,
                     JoueurActuel.pion == 'B' ? "NOIR" : "BLANC");
        }

        /* Fin de jeu */
        if (phase == 2 && fin_jeu(JoueurActuel.pion)) {
            Joueur gagnant = (JoueurActuel.pion == 'B')
                             ? joueur_blanc : joueur_noir;
            strcpy(nom_gagnant, gagnant.nom);
            int res = gerer_fin_partie(fenetre, renderer,
                                        nom_gagnant);
            if (res == 2) {
                fermer_SDL(fenetre, renderer);
                exit(0);
            }
            return;
        }

        dessiner_plateau_graphique(renderer, plateau,
                                    sel, message);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                fermer_SDL(fenetre, renderer);
                exit(0);
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                float mx = event.button.x;
                float my = event.button.y;
                int col = (int)((mx - OFFSET_X) / TAILLE_CASE);
                int lig = (int)((my - OFFSET_Y) / TAILLE_CASE);

                if (col<0||col>6||lig<0||lig>6) continue;

                /* Phase 0 : Noir enleve B */
                if (phase == 0) {
                    if (plateau[lig][col] == 'B') {
                        plateau[lig][col] = '.';
                        JoueurActuel = joueur_blanc;
                        phase = 1;
                    }
                }
                /* Phase 1 : Blanc enleve W */
                else if (phase == 1) {
                    if (plateau[lig][col] == 'W') {
                        plateau[lig][col] = '.';
                        JoueurActuel = joueur_noir;
                        phase = 2;
                        premier_click = 1;
                    }
                }
                /* Phase 2 : Sauts */
                else if (phase == 2) {
                    if (premier_click) {
                        /* Selectionner pion */
                        if (plateau[lig][col] ==
                            JoueurActuel.pion) {
                            sel.x = lig;
                            sel.y = col;
                            sel.selectionne = 1;
                            coup_en_cours.x1 = lig;
                            coup_en_cours.y1 = col;
                            premier_click = 0;
                        }
                    } else {
                        /* Destination */
                        coup_en_cours.x2 = lig;
                        coup_en_cours.y2 = col;

                        if (coupLegal(coup_en_cours,
                                      JoueurActuel.pion)) {
                            int mx2 = (coup_en_cours.x1 +
                                       coup_en_cours.x2) / 2;
                            int my2 = (coup_en_cours.y1 +
                                       coup_en_cours.y2) / 2;

                            plateau[coup_en_cours.x2]
                                   [coup_en_cours.y2] =
                            plateau[coup_en_cours.x1]
                                   [coup_en_cours.y1];
                            plateau[coup_en_cours.x1]
                                   [coup_en_cours.y1] = '.';
                            plateau[mx2][my2] = '.';

                            /* Multi-saut ? */
                            if (AnotherJump(coup_en_cours.x2,
                                            coup_en_cours.y2,
                                            JoueurActuel.pion)) {
                                coup_en_cours.x1 =
                                    coup_en_cours.x2;
                                coup_en_cours.y1 =
                                    coup_en_cours.y2;
                                sel.x = coup_en_cours.x1;
                                sel.y = coup_en_cours.y1;
                                sel.selectionne = 1;
                            } else {
                                sel.selectionne = 0;
                                JoueurActuel =
                                    (JoueurActuel.pion == 'B')
                                    ? joueur_blanc
                                    : joueur_noir;
                                premier_click = 1;
                            }
                        } else {
                            /* Re-selectionner */
                            if (plateau[lig][col] ==
                                JoueurActuel.pion) {
                                sel.x = lig;
                                sel.y = col;
                                sel.selectionne = 1;
                                coup_en_cours.x1 = lig;
                                coup_en_cours.y1 = col;
                            } else {
                                sel.selectionne = 0;
                                premier_click = 1;
                            }
                        }
                    }
                }
            }
        }
        SDL_Delay(16);
    }
}

/* ============================================================
   Jeu Humain vs Machine (graphique SDL3)
   ============================================================ */
void jouer_graphique_HvM(SDL_Window* fenetre,
                           SDL_Renderer* renderer) {
    Selection sel = {0, 0, 0};
    SDL_Event event;
    char nom_gagnant[50];
    char message[150];
    int phase = 0;
    int premier_click = 1;
    Coup coup_en_cours;
    coup_en_cours.x1 = -1;
    coup_en_cours.y1 = -1;
    coup_en_cours.x2 = -1;
    coup_en_cours.y2 = -1;

    Joueur JH, JM;
    strcpy(JH.nom, J1.nom);

    int r = rand() % 2;
    JH.pion = (r == 0) ? 'B' : 'W';
    strcpy(JM.nom, "MACHINE");
    JM.pion = (JH.pion == 'B') ? 'W' : 'B';

    Joueur joueur_noir  = (JH.pion == 'B') ? JH : JM;
    Joueur joueur_blanc = (JH.pion == 'W') ? JH : JM;
    JoueurActuel = joueur_noir;

    creation_plateau();

    while (1) {
        /* Message */
        if (phase == 0) {
            snprintf(message, sizeof(message),
                     "ENLEVER UN PION NOIR  TOUR DE : %s",
                     joueur_noir.nom);
        } else if (phase == 1) {
            snprintf(message, sizeof(message),
                     "ENLEVER UN PION BLANC  TOUR DE : %s",
                     joueur_blanc.nom);
        } else {
            snprintf(message, sizeof(message),
                     "TOUR DE : %s  (%s)",
                     JoueurActuel.nom,
                     JoueurActuel.pion == 'B' ? "NOIR" : "BLANC");
        }

        /* Fin de jeu */
        if (phase == 2 && fin_jeu(JoueurActuel.pion)) {
            Joueur gagnant = (JoueurActuel.pion == JH.pion)
                             ? JM : JH;
            strcpy(nom_gagnant, gagnant.nom);
            int res = gerer_fin_partie(fenetre, renderer,
                                        nom_gagnant);
            if (res == 2) {
                fermer_SDL(fenetre, renderer);
                exit(0);
            }
            return;
        }

        dessiner_plateau_graphique(renderer, plateau,
                                    sel, message);

        /* Machine : enlever phase 0 */
        if (phase == 0 && JoueurActuel.pion == JM.pion) {
            SDL_Delay(600);
            CoupEnleveMachine(&JM);
            JoueurActuel = joueur_blanc;
            phase = 1;
            continue;
        }
        /* Machine : enlever phase 1 */
        if (phase == 1 && JoueurActuel.pion == JM.pion) {
            SDL_Delay(600);
            CoupEnleveMachine(&JM);
            JoueurActuel = joueur_noir;
            phase = 2;
            continue;
        }
        /* Machine : sauts */
        if (phase == 2 && JoueurActuel.pion == JM.pion) {
            SDL_Delay(700);
            deplacerCoupMachine(&JM);
            JoueurActuel = JH;
            premier_click = 1;
            sel.selectionne = 0;
            continue;
        }

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                fermer_SDL(fenetre, renderer);
                exit(0);
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {

                float mx = event.button.x;
                float my = event.button.y;
                int col = (int)((mx - OFFSET_X) / TAILLE_CASE);
                int lig = (int)((my - OFFSET_Y) / TAILLE_CASE);

                if (col<0||col>6||lig<0||lig>6) continue;

                /* Phase 0 humain */
                if (phase == 0 &&
                    JoueurActuel.pion == JH.pion) {
                    if (plateau[lig][col] == JH.pion) {
                        plateau[lig][col] = '.';
                        JoueurActuel = joueur_blanc;
                        phase = 1;
                    }
                }
                /* Phase 1 humain */
                else if (phase == 1 &&
                         JoueurActuel.pion == JH.pion) {
                    if (plateau[lig][col] == JH.pion) {
                        plateau[lig][col] = '.';
                        JoueurActuel = joueur_noir;
                        phase = 2;
                    }
                }
                /* Phase 2 humain sauts */
                else if (phase == 2 &&
                         JoueurActuel.pion == JH.pion) {
                    if (premier_click) {
                        if (plateau[lig][col] == JH.pion) {
                            sel.x = lig;
                            sel.y = col;
                            sel.selectionne = 1;
                            coup_en_cours.x1 = lig;
                            coup_en_cours.y1 = col;
                            premier_click = 0;
                        }
                    } else {
                        coup_en_cours.x2 = lig;
                        coup_en_cours.y2 = col;

                        if (coupLegal(coup_en_cours, JH.pion)) {
                            int mx2 = (coup_en_cours.x1 +
                                       coup_en_cours.x2) / 2;
                            int my2 = (coup_en_cours.y1 +
                                       coup_en_cours.y2) / 2;

                            plateau[coup_en_cours.x2]
                                   [coup_en_cours.y2] =
                            plateau[coup_en_cours.x1]
                                   [coup_en_cours.y1];
                            plateau[coup_en_cours.x1]
                                   [coup_en_cours.y1] = '.';
                            plateau[mx2][my2] = '.';

                            if (AnotherJump(coup_en_cours.x2,
                                            coup_en_cours.y2,
                                            JH.pion)) {
                                coup_en_cours.x1 =
                                    coup_en_cours.x2;
                                coup_en_cours.y1 =
                                    coup_en_cours.y2;
                                sel.x = coup_en_cours.x1;
                                sel.y = coup_en_cours.y1;
                                sel.selectionne = 1;
                            } else {
                                sel.selectionne = 0;
                                JoueurActuel = JM;
                                premier_click = 1;
                            }
                        } else {
                            if (plateau[lig][col] == JH.pion) {
                                sel.x = lig;
                                sel.y = col;
                                sel.selectionne = 1;
                                coup_en_cours.x1 = lig;
                                coup_en_cours.y1 = col;
                            } else {
                                sel.selectionne = 0;
                                premier_click = 1;
                            }
                        }
                    }
                }
            }
        }
        SDL_Delay(16);
    }
}

/* ============================================================
   Definir l'icone de la fenetre depuis un fichier BMP
   ============================================================ */
void definir_icone(SDL_Window* fenetre, const char* chemin_bmp) {
    SDL_Surface* icone = SDL_LoadBMP(chemin_bmp);
    if (icone) {
        SDL_SetWindowIcon(fenetre, icone);
        SDL_DestroySurface(icone);
        printf("Icone chargee avec succes !\n");
    } else {
        printf("Erreur chargement icone : %s\n", SDL_GetError());
    }
}
