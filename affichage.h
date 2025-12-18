#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"

/* Nettoie l'écran (version débutant : imprime des sauts de ligne). */
void nettoyer_ecran(void);

/* Pause simple : attend que l'utilisateur appuie sur Entrée. */
void pause_entree(void);

/* Affichages du jeu (niveau 1 existant). */
void afficherEntete(void);
void afficherPlateau(JeuState *jeu);
void afficherInfos(JeuState *jeu);
void afficherControles(JeuState *jeu);
void afficherErreur(const char *message);
void afficherJeu(JeuState *jeu);
void afficherFinPartie(JeuState *jeu);

#endif
