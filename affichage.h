#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"




/**
 * Affiche l'en-tête du jeu
 */
void afficherEntete(void);

/**
 * Affiche le plateau de jeu
 */
void afficherPlateau(JeuState *jeu);

/**
 * Affiche les informations de jeu (coups, objectifs, progression)
 */
void afficherInfos(JeuState *jeu);

/**
 * Affiche les contrôles disponibles
 */
void afficherControles(JeuState *jeu);

/**
 * Affiche l'écran de fin de partie
 */
void afficherFinPartie(JeuState *jeu);

/**
 * Affiche un message d'erreur temporaire
 */
void afficherErreur(const char *message);


#endif
