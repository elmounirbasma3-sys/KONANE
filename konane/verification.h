#ifndef VERIFICATION_H
#define VERIFICATION_H

#include "struct.h"

int coupLegal(Coup c, char pion);
int canPlay(char pion);
int fin_jeu(char pion);
int AnotherJump(int x, int y, char pion);

#endif
