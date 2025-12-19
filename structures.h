/* structures.h - Definitions du jeu */
#ifndef STRUCTURES_H
#define STRUCTURES_H

/* Nombre de fruits normaux */
#define NB_FRUITS 5

/* Bonbons speciaux */
#define EMOJI_ARBRE     "🎄"
#define EMOJI_BOMBE     "💣"
#define EMOJI_BOOMERANG "🪃"
#define EMOJI_ARCENCIEL "🌈"
#define EMOJI_JOKER     "🃏"

/* Parametres de jeu */
#define VIES_MAX 3
#define COUPS_N1 40
#define COUPS_N2 35
#define COUPS_N3 30
#define TEMPS_N1 240
#define TEMPS_N2 210
#define TEMPS_N3 180
#define OBJECTIF_N1 25
#define OBJECTIF_N2 30
#define OBJECTIF_N3 35
#define OBJECTIF_ARBRES 3

/* Fichier de sauvegarde */
#define FICHIER_SAUVEGARDE "sauvegarde.txt"

/* Structure principale du jeu */
typedef struct {
    int lignes;
    int colonnes;
    char **plateau;
    
    char *fruits[NB_FRUITS];
    int score[NB_FRUITS];
    
    int coups;
    int curseur_x, curseur_y;
    int selection;
    int select_x, select_y;
    
    int en_cours;
    int victoire;
    
    int niveau;
    int vies;
    int objectif;
    int objectif_arbres;
    int arbres_utilises;
    
    int temps_limite;
    long temps_debut;
    int temps_restant;
    
    /* Sauvegarde */
    char pseudo[32];
    int niveau_max_atteint;  /* 1, 2 ou 3 */
} Jeu;

/* Structure pour la sauvegarde */
typedef struct {
    char pseudo[32];
    int vies;
    int niveau_en_cours;
    int existe;  /* 1 si sauvegarde existe */
} Sauvegarde;

#endif
