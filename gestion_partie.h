#ifndef GESTION_PARTIE_H                                   
#define GESTION_PARTIE_H                                   

#include "structures.h"                                    

void init_partie(Jeu *jeu, int lignes, int colonnes);     
void liberer_partie(Jeu *jeu);                              /* Libere la memoire du plateau */
int verifier_victoire(Jeu *jeu);                            /* il vérifie que les objectifs soit bien atteints, retourne 1/0 */
int gerer_echange(Jeu *jeu);                                /* veriffie les echanges pour le niveau 1 */
int gerer_echange_n2(Jeu *jeu);                             /* Verifie les echanges pour le niveau 2 et 3*/
void boucle_jeu(Jeu *jeu);                                  /* Boucle (etapes de fonctionnement du niveau )principale niveau 1 */
void boucle_jeu_n2(Jeu *jeu);                               /* Boucle principale niveau 2 */
void boucle_jeu_n3(Jeu *jeu);                               /* Boucle principale niveau 3 */

/* Sauvegarde */                                            
void sauvegarder_partie(Sauvegarde *sauv);                  /* Ecrit la sauvegarde dans fichier */
int charger_sauvegarde(Sauvegarde *sauv);                   /* Lit sauvegarde depuis fichier */
void supprimer_sauvegarde(void);                            /* Supprime le fichier de sauvegarde */

#endif                                                   
