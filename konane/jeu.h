#ifndef JEU_H
#define JEU_H

#include "struct.h"

extern Joueur J1, J2, JoueurActuel;
extern int contreMachine;

void qui_commence();
void qui_commence_IA(Joueur* JH, Joueur* JM);
void CoupEnleveHumain();
void CoupEnleveMachine(Joueur* J);
void deplacerCoupHumain();
void deplacerCoupMachine(Joueur* J);
void jouerHumainVsHumain();
void jouerHumainVsMachine();

#endif
