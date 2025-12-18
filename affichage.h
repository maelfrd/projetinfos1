#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"

/* Affiche l'en-tête du jeu */
void afficherEntete(void);

/* Affiche uniquement le plateau */
void afficherPlateau(JeuState *jeu);

/* Affiche les infos (coups, objectif, compteurs de bonbons) */
void afficherInfos(JeuState *jeu);

/* Affiche les contrôles et l’état de sélection */
void afficherControles(JeuState *jeu);

/* Affiche l'écran de fin de partie */
void afficherFinPartie(JeuState *jeu);

/* Affiche un message d'erreur temporaire */
void afficherErreur(const char *message);

/* Affiche l’écran complet (entête + plateau + infos + contrôles) */
void afficherJeu(JeuState *jeu);


//nettoyer l'ecran 
 void nettoyer_ecran(void);
 
#endif
