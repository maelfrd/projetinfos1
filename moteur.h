
/* moteur.h - Logique du jeu */                             /* En-tete du fichier */
#ifndef MOTEUR_H                                            /* Protection contre inclusion multiple */
#define MOTEUR_H                                            /* Debut de la definition du header */

#include "structures.h"                                     /* Inclut les structures du jeu */

/* Fonctions de base */                                     /* Section fonctions de base */
int est_special(char *c);                                   /* Verifie si emoji est special (bombe, etc.) */
int est_joker(char *c);                                     /* Verifie si emoji est un joker */
int est_fruit(char *c);                                     /* Verifie si emoji est un fruit normal */
int index_fruit(Jeu *jeu, char *f);                         /* Retourne index du fruit (0-4) ou -1 */

/* Menu */                                                  /* Section menu */
int menu(Sauvegarde *sauv);                                 /* Affiche menu et retourne choix utilisateur */

/* Alignements */                                           /* Section alignements */
int chercher_alignements(Jeu *jeu, int marques[]);          /* Trouve alignements de 3+, remplit marques[] */
void supprimer_marques(Jeu *jeu, int marques[]);            /* Supprime cases marquees, compte points */
void faire_tomber(Jeu *jeu);                                /* Applique gravite (fruits tombent) */
void remplir_vides(Jeu *jeu, int niveau2);                  /* Remplit cases vides avec nouveaux fruits */

/* Permutations */                                          /* Section permutations */
void echanger(Jeu *jeu, int x1, int y1, int x2, int y2);    /* Echange deux cases adjacentes */
int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2);  /* Verifie si echange cree alignement */
void cascade(Jeu *jeu);                                     /* Boucle suppression/gravite/remplissage (N1) */
void cascade_niveau2(Jeu *jeu);                             /* Cascade avec detection speciaux (N2/N3) */

/* Combinaisons speciales niveau 2 */                       /* Section combinaisons speciales */
int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[]);  /* Detecte formations speciales */
int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2);  /* Validation echange niveau 2 */

/* Effets des bonbons speciaux */                           /* Section effets speciaux */
void effet_colonne(Jeu *jeu, int y);                        /* Arbre: supprime colonne entiere */
void effet_ligne(Jeu *jeu, int x);                          /* Boomerang: supprime ligne entiere */
void effet_bombe(Jeu *jeu, int x, int y);                   /* Bombe: supprime zone 3x3 */
void effet_arcenciel(Jeu *jeu, char *cible);                /* Arc-en-ciel: supprime tous fruits d'un type */

#endif                                                      /* Fin de la protection d'inclusion */
