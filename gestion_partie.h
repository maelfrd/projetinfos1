

/* gestion_partie.h */                                      /* En-tete du fichier */
#ifndef GESTION_PARTIE_H                                    /* Protection contre inclusion multiple */
#define GESTION_PARTIE_H                                    /* Debut de la definition du header */

#include "structures.h"                                     /* Inclut les structures du jeu */

void init_partie(Jeu *jeu, int lignes, int colonnes);       /* Initialise une nouvelle partie */
void liberer_partie(Jeu *jeu);                              /* Libere la memoire du plateau */
int verifier_victoire(Jeu *jeu);                            /* Verifie si objectifs atteints, retourne 1/0 */
int gerer_echange(Jeu *jeu);                                /* Gere un echange niveau 1, retourne 0/-1 */
int gerer_echange_n2(Jeu *jeu);                             /* Gere un echange niveau 2/3, retourne 0/-1 */
void boucle_jeu(Jeu *jeu);                                  /* Boucle principale niveau 1 */
void boucle_jeu_n2(Jeu *jeu);                               /* Boucle principale niveau 2 */
void boucle_jeu_n3(Jeu *jeu);                               /* Boucle principale niveau 3 */

/* Sauvegarde */                                            /* Section sauvegarde */
void sauvegarder_partie(Sauvegarde *sauv);                  /* Ecrit sauvegarde dans fichier */
int charger_sauvegarde(Sauvegarde *sauv);                   /* Lit sauvegarde depuis fichier */
void supprimer_sauvegarde(void);                            /* Supprime le fichier de sauvegarde */

#endif                                                      /* Fin de la protection d'inclusion */
