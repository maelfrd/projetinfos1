/* gestion_partie.h */
#ifndef GESTION_PARTIE_H
#define GESTION_PARTIE_H

#include "structures.h"

void init_partie(Jeu *jeu, int lignes, int colonnes);
void liberer_partie(Jeu *jeu);
int verifier_victoire(Jeu *jeu);
int gerer_echange(Jeu *jeu);
int gerer_echange_n2(Jeu *jeu);
void boucle_jeu(Jeu *jeu);
void boucle_jeu_n2(Jeu *jeu);
void boucle_jeu_n3(Jeu *jeu);

/* Sauvegarde */
void sauvegarder_partie(Sauvegarde *sauv);
int charger_sauvegarde(Sauvegarde *sauv);
void supprimer_sauvegarde(void);

#endif
