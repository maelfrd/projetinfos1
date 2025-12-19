/* affichage.h */
#ifndef AFFICHAGE_H
#define AFFICHAGE_H

#include "structures.h"

void nettoyer_ecran(void);// permet de nettoyer l'affichage de l'ecran cet a dire faire descendre le terminal 
void pause_entree(void);
void afficher_plateau(Jeu *jeu);// affichage du plateau 
void afficher_infos(Jeu *jeu);//afficher ce qui est definis dans les infos et objectifs de reussite du niveau 
void afficher_controles(Jeu *jeu);//
void afficher_jeu(Jeu *jeu);//
void afficher_fin(Jeu *jeu);//afficher le  cas de reussite du jeu 

#endif
