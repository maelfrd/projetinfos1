#ifndef GESTION_PARTIE_H
#define GESTION_PARTIE_H

#include "structures.h"

/**
 * Initialise une nouvelle partie
 */
void initialiserPartie(JeuState *jeu, int lignes, int colonnes);

/**
 * Vérifie si le joueur a gagné
 */
int verifierVictoire(JeuState *jeu);

/**
 * Libère la mémoire allouée pour le jeu
 */
void libererPartie(JeuState *jeu);

/**
 * Gère la sélection et la permutation des bonbons
 */
void gererPermutation(JeuState *jeu);

/**
 * Boucle principale du jeu
 */
void boucleJeu(JeuState *jeu);

#endif
