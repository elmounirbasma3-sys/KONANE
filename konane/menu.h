#ifndef MENU_H
#define MENU_H

void afficher_menu();
void afficher_regles();
int lire_choix_menu();
int lire_choix_mode();
int valider_choix(int min, int max);
int afficher_fin_partie(char* nom_gagnant);

#endif
