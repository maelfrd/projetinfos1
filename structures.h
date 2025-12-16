#ifndef STRUCTURES_H
#define STRUCTURES_H

#define OBJECTIF_PAR_FRUIT 25
#define NB_TYPES_BONBONS 5

/**
 * Structure représentant l'état complet du jeu
 */
typedef struct {
    int lignes;              // Nombre de lignes du plateau
    int colonnes;            // Nombre de colonnes du plateau
    char **plateau;          // Plateau de jeu (tableau dynamique)
    char *emojis[NB_TYPES_BONBONS];  // Types de bonbons disponibles
    int nbemoji[NB_TYPES_BONBONS];   // Compteurs de bonbons collectés
    int nbcoups;             // Nombre de coups restants
    int curseur_x;           // Position X du curseur
    int curseur_y;           // Position Y du curseur
    int mode_selection;      // Mode sélection activé (0 ou 1)
    int selection_x;         // Position X du bonbon sélectionné
    int selection_y;         // Position Y du bonbon sélectionné
    int continuer;           // Flag pour continuer le jeu
    int victoire;            // Flag de victoire
} JeuState;

#endif
