
/* saisie.h */                                              /* En-tete du fichier */
#ifndef SAISIE_H                                            /* Protection contre inclusion multiple */
#define SAISIE_H                                            /* Debut de la definition du header */

#include "structures.h"                                     /* Inclut les structures du jeu */

char lire_commande(void);                                   /* Lit une touche clavier, retourne caractere */
void appliquer_commande(Jeu *jeu, char cmd);                /* Applique commande au jeu (deplacement/selection) */
int appliquer_commande_menu(char cmd);                      /* Applique commande menu, retourne action */

#endif                                                      /* Fin de la protection d'inclusion */
