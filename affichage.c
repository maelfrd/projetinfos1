/* affichage.c - Fonctions d'affichage */
#include <stdio.h>
#include "affichage.h"

void nettoyer_ecran(void)
{
    int i;
    for (i = 0; i < 50; i++) printf("\n");
}

void pause_entree(void)
{
    int c;
    printf("\nAppuie sur Entree...");
    while ((c = getchar()) != '\n' && c != EOF);
}

void afficher_plateau(Jeu *jeu)
{
    int x, y, idx;
    char *c;
    
    /* Bordure haute */
    for (y = 0; y < jeu->colonnes; y++) printf("---");
    printf("-\n");
    
    /* Lignes du plateau */
    for (x = 0; x < jeu->lignes; x++) {
        printf("| ");
        for (y = 0; y < jeu->colonnes; y++) {
            idx = x * jeu->colonnes + y;
            c = jeu->plateau[idx];
            if (c == NULL) c = " ";
            
            if (jeu->selection && x == jeu->select_x && y == jeu->select_y)
                printf("[%s]", c);
            else if (x == jeu->curseur_x && y == jeu->curseur_y)
                printf("<%s>", c);
            else
                printf(" %s ", c);
        }
        printf("|\n");
    }
    
    /* Bordure basse */
    for (y = 0; y < jeu->colonnes; y++) printf("---");
    printf("-\n");
}

void afficher_infos(Jeu *jeu)
{
    int i;
    const char *noms[5] = {"Pomme", "Citron", "Raisin", "Orange", "Cerise"};
    
    /* Vies */
    printf("\nVies: ");
    for (i = 0; i < jeu->vies; i++) printf("❤️ ");
    printf("(%d/%d)\n", jeu->vies, VIES_MAX);
    
    /* Temps */
    if (jeu->temps_limite > 0)
        printf("Temps: %d s\n", jeu->temps_restant);
    
    /* Coups et objectif */
    printf("Coups: %d\n", jeu->coups);
    printf("Objectif: %d de chaque fruit\n", jeu->objectif);
    
    /* Arbres niveau 3 */
    if (jeu->objectif_arbres > 0)
        printf("Arbres 🎄: %d/%d\n", jeu->arbres_utilises, jeu->objectif_arbres);
    
    /* Score par fruit */
    printf("\nRecolte:\n");
    for (i = 0; i < NB_FRUITS; i++) {
        printf("  %s %-7s: %2d/%d", jeu->fruits[i], noms[i], jeu->score[i], jeu->objectif);
        if (jeu->score[i] >= jeu->objectif) printf(" [OK]");
        printf("\n");
    }
}

void afficher_controles(Jeu *jeu)
{
    printf("\nCurseur: [%d,%d]\n", jeu->curseur_x, jeu->curseur_y);
    
    if (jeu->selection) {
        printf("Selection: [%d,%d] - ACTIVE\n", jeu->select_x, jeu->select_y);
        printf("Deplace puis P pour echanger, ou P pour annuler\n");
    } else {
        printf("Z/Q/S/D: deplacer | P: selectionner | X: quitter\n");
    }
}

void afficher_jeu(Jeu *jeu)
{
    nettoyer_ecran();
    printf("============ CANDY CRUSH ============\n");
    afficher_plateau(jeu);
    afficher_infos(jeu);
    afficher_controles(jeu);
}

void afficher_fin(Jeu *jeu)
{
    nettoyer_ecran();
    printf("============================================\n");
    if (jeu->victoire)
        printf("            VICTOIRE !\n");
    else
        printf("            PARTIE TERMINEE\n");
    printf("============================================\n");
    printf("Coups restants: %d\n", jeu->coups);
    pause_entree();
}
