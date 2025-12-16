#ifndef SAISIE_H
#define SAISIE_H

#include <termios.h>

// Variables globales pour la configuration du terminal
extern struct termios orig_termios;

/**
 * Désactive le mode canonique du terminal
 */
void desactiver_mode_canonique(void);

/**
 * Réactive le mode canonique du terminal
 */
void reactiver_mode_canonique(void);

/**
 * Vérifie si une touche a été pressée
 */
int kbhit(void);

/**
 * Déplace le curseur selon les touches fléchées
 */
char deplacerCurseur(int *curseur_x, int *curseur_y, int lignes, int colonnes);

#endif
