#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

/*
    MOTEUR DU JEU (logique)
    - détecter les alignements (3+ identiques)
    - supprimer les bonbons alignés
    - faire tomber les bonbons
    - remplir les cases vides
    - gérer les cascades après une permutation

    IMPORTANT :
    - Aucune couleur / séquence ANSI ici (pas de \033[...]).
    - Aucune gestion majuscule/minuscule ici (c'est la saisie).
*/

/* Retourne l'index d'un emoji dans le tableau emojis[], ou -1 si absent. */
int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis);

/*
    Vérifie les alignements autour de la case (x,y).
    - marques[] (taille lignes*colonnes) est rempli avec 1 pour les cases à supprimer.
    - Retourne 1 si au moins un alignement (horizontal ou vertical) est trouvé, sinon 0.

    Variables internes typiques :
    - compte_h : nombre de bonbons identiques alignés horizontalement (gauche+centre+droite)
    - debut_h / fin_h : colonnes extrêmes de cet alignement horizontal
    - compte_v : idem en vertical (haut+centre+bas)
    - debut_v / fin_v : lignes extrêmes de cet alignement vertical
*/
int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes,
                             int x, int y, int marques[]);

/* Parcourt tout le plateau et remplit marques[] pour tous les alignements. */
int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[]);

/*
    Supprime (met à NULL) toutes les cases marquées dans marques[].
    Met à jour les compteurs nbemoji[] en fonction des emojis supprimés.
*/
void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes,
                             int marques[], char *emojis[], int nbemoji[], int nb_emojis);

/* Fait tomber les bonbons (gravité) : les NULL remontent, les bonbons descendent. */
void faireTomber(char *plateau[], int lignes, int colonnes);

/* Remplit les cases vides (NULL) avec des bonbons aléatoires. */
void remplirCasesVides(char *plateau[], int lignes, int colonnes,
                       char *emojis[], int nb_emojis);

/* Vérifie qu'une permutation (x1,y1) <-> (x2,y2) crée au moins un alignement. */
int permutationValide(char *plateau[], int lignes, int colonnes,
                      int x1, int y1, int x2, int y2);

/* Permute deux bonbons sur le plateau. */
void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2);

/* Lance la suppression + gravité + remplissage tant qu'il y a des alignements (cascade). */
void traiterAlignementsCascade(JeuState *jeu);

#endif