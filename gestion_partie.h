#ifndef GESTION_PARTIE_H
#define GESTION_PARTIE_H

#include "structures.h"

void initialiserPartie(JeuState *jeu, int lignes, int colonnes);
int verifierVictoire(JeuState *jeu);
void libererPartie(JeuState *jeu);

/*
  0  : permutation exécutée
 -1  : pas adjacent
 -2  : permutation invalide (pas d’alignement)
*/
int gererPermutation(JeuState *jeu);

void boucleJeu(JeuState *jeu);

#endif
