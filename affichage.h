/* affichage.h */
#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"

void nettoyer_ecran(void);
void pause_entree(void);
void afficher_plateau(Jeu *jeu);
void afficher_infos(Jeu *jeu);
void afficher_controles(Jeu *jeu);
void afficher_jeu(Jeu *jeu);
void afficher_fin(Jeu *jeu);

#endif
