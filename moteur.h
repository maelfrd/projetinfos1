#ifndef MOTEUR_H
#define MOTEUR_H

#include "structures.h"

/**
 * Trouve l'index d'un emoji dans le tableau
 */
int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis);

/**
 * Vérifie s'il y a un alignement depuis une position donnée
 */
int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes, 
                              int x, int y, int marques[]);

/**
 * Vérifie tous les alignements sur le plateau
 */
int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[]);

/**
 * Supprime les bonbons marqués
 */
void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes, 
                              int marques[], char *emojis[], int nbemoji[], int nb_emojis);

/**
 * Applique la gravité sur le plateau
 */
void faireTomber(char *plateau[], int lignes, int colonnes);

/**
 * Remplit les cases vides avec de nouveaux bonbons
 */
void remplirCasesVides(char *plateau[], int lignes, int colonnes, 
                       char *emojis[], int nb_emojis);

/**
 * Vérifie si une permutation est valide
 */
int permutationValide(char *plateau[], int lignes, int colonnes, 
                      int x1, int y1, int x2, int y2);

/**
 * Permute deux bonbons
 */
void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2);

/**
 * Traite les alignements en cascade après une permutation
 */
void traiterAlignementsCascade(JeuState *jeu);

#endif

