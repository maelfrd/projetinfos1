
#ifndef AFFICHAGE_H                                         /* Protection contre inclusion multiple */
#define AFFICHAGE_H                                         /* Debut de la definition du header */

#include "structures.h"                                     /* Inclut les structures du jeu */

/* ========== FONCTIONS DE BASE ========== */               /* Section fonctions de base */
void nettoyer_ecran(void);                                  /* Efface l'ecran (50 lignes vides) */
void pause_entree(void);                                    /* Attend appui sur Entree */

/* ========== AFFICHAGE DU JEU ========== */                /* Section affichage jeu */
void afficher_plateau(Jeu *jeu);                            /* Affiche la grille de jeu */
void afficher_infos(Jeu *jeu);                              /* Affiche vies, temps, score */
void afficher_controles(Jeu *jeu);                          /* Affiche les controles disponibles */
void afficher_jeu(Jeu *jeu);                                /* Affiche tout l'ecran de jeu */
void afficher_fin(Jeu *jeu);                                /* Affiche ecran de fin de niveau */

/* ========== AFFICHAGE DES MENUS ========== */             /* Section affichage menus */
void afficher_titre(void);                                  /* Affiche le titre du jeu */
void afficher_menu_avec_sauvegarde(int selection, Sauvegarde *sauv);  /* Menu avec option reprendre */
void afficher_menu_sans_sauvegarde(int selection);          /* Menu sans option reprendre */
void afficher_parametres(void);                             /* Affiche ecran parametres */

/* ========== MESSAGES DE PROGRESSION ========== */         /* Section messages progression */
void afficher_niveau_debloque(const char *pseudo, int niveau);  /* Message niveau debloque */
void afficher_jeu_termine(const char *pseudo);              /* Message jeu termine (victoire finale) */
void afficher_game_over(const char *pseudo);                /* Message game over (plus de vies) */
void afficher_niveau_echoue(int vies_restantes);            /* Message echec niveau */
void afficher_nouvelle_partie_warning(const char *ancien_pseudo);  /* Warning ecrasement sauvegarde */
void afficher_nouvelle_partie(void);                        /* Ecran nouvelle partie */
void afficher_au_revoir(void);                              /* Message de sortie du jeu */

/* ========== SAISIE LIEE A L'AFFICHAGE ========== */       /* Section saisie affichage */
char lire_menu(void);                                       /* Lit une touche dans le menu */
void lire_pseudo(char *pseudo);                             /* Demande et lit le pseudo joueur */

#endif                                                      /* Fin de la protection d'inclusion */
