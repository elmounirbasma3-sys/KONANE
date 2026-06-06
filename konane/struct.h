#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED

typedef struct {
    char nom[50];
    char pion;
} Joueur;

typedef struct {
    int x1, y1, x2, y2;
} Coup;

#endif // STRUCT_H_INCLUDED
