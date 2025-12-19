/* structures.h - Definitions du jeu */                     /* En-tete du fichier */
#ifndef STRUCTURES_H                                        /* Protection contre inclusion multiple */
#define STRUCTURES_H                                        /* Debut de la definition du header */

/* Nombre de fruits normaux */                              /* Commentaire de section */
#define NB_FRUITS 5                                         /* Constante: 5 types de fruits dans le jeu */

/* Bonbons speciaux */                                      /* Commentaire de section */
#define EMOJI_ARBRE     "🎄"                                /* Emoji sapin: efface une colonne */
#define EMOJI_BOMBE     "💣"                                /* Emoji bombe: efface zone 3x3 */
#define EMOJI_BOOMERANG "🥏"                                /* Emoji boomerang: efface une ligne */
#define EMOJI_ARCENCIEL "🌈"                                /* Emoji arc-en-ciel: efface tous fruits d'un type */
#define EMOJI_JOKER     "🃏"                                /* Emoji joker: remplace n'importe quel fruit */

/* Parametres de jeu */                                     /* Commentaire de section */
#define VIES_MAX 3                                          /* Nombre maximum de vies du joueur */
#define COUPS_N1 40                                         /* Nombre de coups autorises niveau 1 */
#define COUPS_N2 35                                         /* Nombre de coups autorises niveau 2 */
#define COUPS_N3 30                                         /* Nombre de coups autorises niveau 3 */
#define TEMPS_N1 240                                        /* Temps limite niveau 1 en secondes (4 min) */
#define TEMPS_N2 210                                        /* Temps limite niveau 2 en secondes (3.5 min) */
#define TEMPS_N3 180                                        /* Temps limite niveau 3 en secondes (3 min) */
#define OBJECTIF_N1 25                                      /* Fruits a collecter par type niveau 1 */
#define OBJECTIF_N2 30                                      /* Fruits a collecter par type niveau 2 */
#define OBJECTIF_N3 35                                      /* Fruits a collecter par type niveau 3 */
#define OBJECTIF_ARBRES 3                                   /* Nombre d'arbres a utiliser niveau 3 */

/* Fichier de sauvegarde */                                 /* Commentaire de section */
#define FICHIER_SAUVEGARDE "sauvegarde.txt"                 /* Nom du fichier pour sauvegarder la partie */

/* Structure principale du jeu */                           /* Commentaire de section */
typedef struct {                                            /* Debut de la structure Jeu */
    int lignes;                                             /* Nombre de lignes du plateau */
    int colonnes;                                           /* Nombre de colonnes du plateau */
    char **plateau;                                         /* Tableau 1D de pointeurs vers les emojis */
                                                            
    char *fruits[NB_FRUITS];                                /* Tableau des 5 emojis de fruits */
    int score[NB_FRUITS];                                   /* Score pour chaque type de fruit */
                                                            
    int coups;                                              /* Nombre de coups restants */
    int curseur_x, curseur_y;                               /* Position actuelle du curseur (ligne, colonne) */
    int selection;                                          /* 1 si une case est selectionnee, 0 sinon */
    int select_x, select_y;                                 /* Position de la case selectionnee */
                                                            
    int en_cours;                                           /* 1 si partie en cours, 0 si terminee */
    int victoire;                                           /* 1 si victoire, 0 si defaite */
                                                            
    int niveau;                                             /* Niveau actuel (1, 2 ou 3) */
    int vies;                                               /* Nombre de vies restantes */
    int objectif;                                           /* Nombre de fruits a collecter par type */
    int objectif_arbres;                                    /* Nombre d'arbres a utiliser (niveau 3) */
    int arbres_utilises;                                    /* Compteur d'arbres deja utilises */
                                                            
    int temps_limite;                                       /* Temps maximum en secondes */
    long temps_debut;                                       /* Timestamp du debut de partie */
    int temps_restant;                                      /* Secondes restantes */
                                                            
    /* Sauvegarde */                                        /* Commentaire de sous-section */
    char pseudo[32];                                        /* Pseudo du joueur (max 31 caracteres + '\0') */
    int niveau_max_atteint;                                 /* Plus haut niveau debloque (1, 2 ou 3) */
} Jeu;                                                      /* Fin de la structure Jeu */

/* Structure pour la sauvegarde */                          /* Commentaire de section */
typedef struct {                                            /* Debut de la structure Sauvegarde */
    char pseudo[32];                                        /* Pseudo sauvegarde du joueur */
    int vies;                                               /* Vies restantes sauvegardees */
    int niveau_en_cours;                                    /* Dernier niveau joue */
    int existe;                                             /* 1 si fichier sauvegarde existe, 0 sinon */
} Sauvegarde;                                               /* Fin de la structure Sauvegarde */

#endif                                                      /* Fin de la protection d'inclusion */

