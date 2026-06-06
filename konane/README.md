 Jeu KONANE — Implémentation en C avec Interface Graphique SDL3

## Contexte académique
> Ce projet a été réalisé dans le cadre du module de **Techniques de Programmation**  
> à l'**École Hassania des Travaux Publics (EHTP)** — Filière **SIG** — Année **2025/2026**  
> sous la supervision de **Mme. CHERRABI**.

Ce projet est une implémentation informatique du jeu de société traditionnel hawaïen
**KONANE**, développé en **langage C** avec une **interface graphique SDL3**.

---
## Équipe

|      Étudiante      |                       Partie réalisée                               |
|---------------------|---------------------------------------------------------------------|
|   Basma EL MOUNIR   | Joueur contre Machine (IA simple) + Interface Graphique SDL3        |
|   Oumaima SAMOUL    | Joueur contre Joueur (mode console)                                 |

---

## Description du jeu

**Konane** est un jeu de stratégie abstrait traditionnel hawaïen, similaire aux dames.  
Il oppose **deux joueurs** sur un plateau **7×7**, chacun contrôlant des pierres  
de couleur **noire (B)** ou **blanche (W)**.

## Règles du jeu :
- Le plateau est rempli **alternativement** de pions noirs et blancs
- Chaque joueur **saute par-dessus** un pion adverse (horizontalement ou verticalement)
- Le pion sauté est **retiré** du plateau
- Les **captures multiples** sont obligatoires dans le même tour
- Le joueur qui **ne peut plus bouger** perd la partie

---

## Structure du projet

```
KONANE/
├── 📄 main.c                           → Point d'entrée principal
├── 📄 struct.h                         → Structures de données (Joueur, Coup)
├── 📄 menu.h / menu.c                  → Gestion des menus
├── 📄 plateau.h / plateau.c            → Plateau de jeu
├── 📄 verification.h / verification.c  → Validation des coups
├── 📄 jeu.h / jeu.c                    → Logique du jeu (HvH + HvM)
├── 📄 graphique.h / graphique.c        → Interface graphique SDL3 ✨
├── 📁 bin/                             → Exécutable compilé
└── 📁 obj/                             → Fichiers objets
```

---

## Fonctionnalités

### Mode Console
- [x] Plateau 7×7 avec affichage ASCII
- [x] Joueur contre Joueur
- [x] Joueur contre Machine (IA aléatoire)
- [x] Validation des coups
- [x] Sauts multiples obligatoires
- [x] Détection automatique de fin de partie
- [x] Menu avec options Retour / Quitter

### Interface Graphique SDL3 *(amélioration)*
- [x] Plateau coloré avec cases alternées
- [x] Pions noirs et blancs dessinés en cercles
- [x] Sélection de pion avec **bordure verte**
- [x] Cases de destination surlignées en **bleu**
- [x] Affichage du tour du joueur
- [x] Menu graphique interactif (hover sur boutons)
- [x] Écran de fin de partie avec nom du gagnant
- [x] Icône personnalisée de la fenêtre
- [x] IA Machine avec délai visuel

---

## Concepts C utilisés
```
|          Concept             |          Utilisation          |
|------------------------------|-------------------------------|
| `char M[7][7]`               | Représentation du plateau     |
| `struct`                     | Joueur, Coup                  |
| `for` / `while` / `do-while` | Parcours plateau, boucles jeu |
| `if/else` / `switch`         | Validation coups, menus       |
| `srand` / `rand`             | IA aléatoire, tirage au sort  |
| Fonctions modulaires         | Organisation du code          |
| Pointeurs                    | Passage par référence         |
| SDL3                         | Interface graphique           |
```
---

## Architecture des fonctions

### Menu
```C
void afficher_menu();
void afficher_regles();
int  lire_choix_menu();
int  lire_choix_mode();
int  valider_choix(int min, int max);
int  afficher_fin_partie(char* nom_gagnant);
```

### Plateau
```C
void creation_plateau();
void afficher_plateau();
```

### Vérification
```C
int coupLegal(Coup c, char pion);
int canPlay(char pion);
int fin_jeu(char pion);
int AnotherJump(int x, int y, char pion);
```

### Jeu
```C
void qui_commence();
void qui_commence_IA(Joueur* JH, Joueur* JM);
void CoupEnleveHumain();
void CoupEnleveMachine(Joueur* J);
void deplacerCoupHumain();
void deplacerCoupMachine(Joueur* J);
void jouerHumainVsHumain();
void jouerHumainVsMachine();
```

### Graphique (SDL3)
```C
int  initialiser_SDL(SDL_Window** f, SDL_Renderer** r);
void definir_icone(SDL_Window* f, const char* chemin);
void dessiner_plateau_graphique(...);
void dessiner_bouton(...);
int  gerer_menu(...);
int  gerer_menu_mode(...);
int  gerer_fin_partie(...);
void jouer_graphique_HvH(...);
void jouer_graphique_HvM(...);
```

---

## Installation et Compilation

### Prérequis
- **GCC** (MinGW 64-bit recommandé)
- **SDL3** : [Télécharger SDL3](https://github.com/libsdl-org/SDL/releases)

### Compilation (mode console uniquement)
```bash
gcc -o KONANE main.c menu.c plateau.c verification.c jeu.c
```

### Compilation (avec interface graphique SDL3)
```bash
gcc -o KONANE main.c menu.c plateau.c verification.c jeu.c graphique.c \
    -IC:\SDL3-3.4.10\x86_64-w64-mingw32\include \
    -LC:\SDL3-3.4.10\x86_64-w64-mingw32\lib \
    -lSDL3 -lm
```

### Copier la DLL (Windows)
```bash
copy C:\SDL3-3.4.10\x86_64-w64-mingw32\bin\SDL3.dll C:\konane\
```

### Lancer le jeu
```bash
KONANE.exe
```

---

## Comment jouer

### Mode Console
```
1. Lancer KONANE.exe
2. Choisir : JOUER → Joueur vs Joueur ou Joueur vs Machine
3. Entrer les noms des joueurs
4. Le tirage au sort détermine qui commence avec les NOIRS
5. Tour 1 : Noir retire un pion B
6. Tour 2 : Blanc retire un pion W
7. Ensuite : saisir position initiale et finale (ligne colonne)
```

### Mode Graphique
```
1. Cliquer sur un pion (bordure verte = sélectionné)
2. Les cases bleues = destinations possibles
3. Cliquer sur une case bleue pour effectuer le saut
4. Si multi-saut possible → continuer depuis la même pièce
```

---

## Aperçu

### Menu Principal
```
        Bienvenue dans le jeu KONANE !!!

1. JOUER
2. Regles de jeu
3. Quitter

VOTRE CHOIX >>>
```

### Plateau de jeu
```
   1 2 3 4 5 6 7
  ---------------
1 | B W B W B W B
2 | W B W B W B W
3 | B W B W B W B
4 | W B W B W B W
5 | B W B W B W B
6 | W B W B W B W
7 | B W B W B W B
  ---------------
```

### Fin de partie
```
========================================
   FIN DE LA PARTIE !
   Le joueur Basma a gagne !!!
========================================

1. Retour au menu principal
2. Quitter
```

---

## 🔧 Difficultés rencontrées

- Gestion des **sauts multiples** obligatoires
- Implémentation de la **détection de fin de jeu**
- Adaptation de l'API **SDL3** (différente de SDL2)
- Gestion des **variables globales** entre plusieurs fichiers
- Affichage du **texte sans SDL_ttf** (segments dessinés manuellement)

---

## Références

- [Documentation SDL3](https://wiki.libsdl.org/SDL3/)
- Règles du jeu Konane : [Wikipedia](https://en.wikipedia.org/wiki/Konane)
- Cours de Techniques de Programmation — EHTP 2025/2026

---

## 📜 Licence

Projet académique — **EHTP / Filière SIG 2025/2026**  
Tous droits réservés © Basma EL MOUNIR & Oumaima SAMOUL

---