#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

/* ===================== NIVEAU 1 (existant) ===================== */
int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis);
int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes,
                             int x, int y, int marques[]);
int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[]);
void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes,
                             int marques[], char *emojis[], int nbemoji[], int nb_emojis);
void faireTomber(char *plateau[], int lignes, int colonnes);
void remplirCasesVides(char *plateau[], int lignes, int colonnes,
                       char *emojis[], int nb_emojis);
int permutationValide(char *plateau[], int lignes, int colonnes,
                      int x1, int y1, int x2, int y2);
void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2);
void traiterAlignementsCascade(JeuState *jeu);

/* ===================== NIVEAU 2 (nouvelles combinaisons) ===================== */
/*
    Niveau 2 ajoute :
    - Alignement long (>= 6) horizontal / vertical -> crée un bonbon spécial "V"
    - Carré 2x2 -> crée un bonbon spécial "V"
    - Croix / T / L : 3 vertical + 3 horizontal, case commune (pas forcément au centre) -> crée un bonbon spécial "V"
    - Utilisation du bonbon spécial : quand il est échangé avec un bonbon adjacent,
      il supprime toute la colonne (hauteur) et déclenche une cascade.

    IMPORTANT : pas d'ANSI, pas de gestion majuscules/minuscules ici.
*/

/* Vérifie qu'une permutation crée au moins une combinaison Niveau 2 (ou un alignement simple). */
int permutationValideNiveau2(char *plateau[], int lignes, int colonnes,
                             int x1, int y1, int x2, int y2);

/* Cascade Niveau 2 : détecte d'abord les combinaisons les plus longues, puis les plus simples. */
void traiterAlignementsCascadeNiveau2(JeuState *jeu);

/* Active l'effet du bonbon spécial (V) : supprime toute la colonne. */
void effetSpecialColonne(JeuState *jeu, int x, int y);

#endif
