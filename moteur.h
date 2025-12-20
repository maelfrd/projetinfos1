
                       
#ifndef MOTEUR_H                                            /* Protection contre inclusion multiple */
#define MOTEUR_H                                            /* Debut de la definition du header */

#include "structures.h"                                     /* Inclut les structures du jeu */

/* Fonctions de base */                                  
int est_special(char *c);                                   /* Verifie si l'emoji est special (bombe, etc.) */
int est_joker(char *c);                                     /* Verifie si l'emoji est un joker */
int est_fruit(char *c);                                     /* Verifie si l'emoji est un fruit normal */
int index_fruit(Jeu *jeu, char *f);                         /* Retourne index du fruit (0-4) ou -1 */

/* Menu */                                                  
int menu(Sauvegarde *sauv);                                 /* Affichage du menu et retourne choix utilisateur */

/* Alignements */                                           
int chercher_alignements(Jeu *jeu, int marques[]);          /* Trouve les alignements de 3+, remplit marques[] */
void supprimer_marques(Jeu *jeu, int marques[]);            /* Supprime les cases marquees, on compte les points */
void faire_tomber(Jeu *jeu);                                /* Applique la gravite (les fruits tombent) */
void remplir_vides(Jeu *jeu, int niveau2);                  /* Remplit les cases vides avec des nouveaux fruits */

/* Permutations */                                         
void echanger(Jeu *jeu, int x1, int y1, int x2, int y2);    /* Echange entre les deux cases adjacentes */
int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2);  /* Verifie si l'echange cree un alignement */
void cascade(Jeu *jeu);                                     /* Boucle suppression/gravite/remplissage (N1) */
void cascade_niveau2(Jeu *jeu);                             /* Cascade avec detection speciaux (N2/N3) */

/* Combinaisons speciales niveau 2 */                      
int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[]);  /* Detecte les formations speciales */
int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2);  /* Validation echange niveau 2 */

/* Effets des bonbons speciaux */                           
void effet_colonne(Jeu *jeu, int y);                        /* Arbre: supprime une colonne entiere */
void effet_ligne(Jeu *jeu, int x);                          /* Boomerang: supprime une ligne entiere */
void effet_bombe(Jeu *jeu, int x, int y);                   /* Bombe: supprime une zone 3x3 */
void effet_arcenciel(Jeu *jeu, char *cible);                /* Arc-en-ciel: supprime tous fruits d'un type dans tout le plateau */

#endif                                                      /* Fin de la protection d'inclusion */
