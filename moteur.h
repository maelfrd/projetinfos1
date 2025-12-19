/* moteur.h - Logique du jeu */
#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

/* Fonctions de base */
int est_special(char *c);
int est_joker(char *c);
int est_fruit(char *c);
int index_fruit(Jeu *jeu, char *f);

/* Alignements */
int chercher_alignements(Jeu *jeu, int marques[]);
void supprimer_marques(Jeu *jeu, int marques[]);
void faire_tomber(Jeu *jeu);
void remplir_vides(Jeu *jeu, int niveau2);

/* Permutations */
void echanger(Jeu *jeu, int x1, int y1, int x2, int y2);
int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2);
void cascade(Jeu *jeu);
void cascade_niveau2(Jeu *jeu);

/* Combinaisons speciales niveau 2 */
int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[]);
int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2);

/* Effets des bonbons speciaux */
void effet_colonne(Jeu *jeu, int y);
void effet_ligne(Jeu *jeu, int x);
void effet_bombe(Jeu *jeu, int x, int y);
void effet_arcenciel(Jeu *jeu, char *cible);

#endif
