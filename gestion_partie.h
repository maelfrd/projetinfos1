#ifndef GESTION_PARTIE_H
#define GESTION_PARTIE_H

#include "structures.h"

/* Niveau 1 (inchangé) */
void initialiserPartie(JeuState *jeu, int lignes, int colonnes);
int verifierVictoire(JeuState *jeu);
void libererPartie(JeuState *jeu);
int gererPermutation(JeuState *jeu);
void boucleJeu(JeuState *jeu);

/* Niveau 2 (nouvelles combinaisons et bonbon spécial) */
int gererPermutationNiveau2(JeuState *jeu);
void boucleJeuNiveau2(JeuState *jeu);

#endif
