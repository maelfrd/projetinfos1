#include <stdio.h>
#include <unistd.h>
#include "affichage.h"

void afficherEntete(void) {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║       CANDY CRUSH - NIVEAU 1          ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
}

void afficherPlateau(JeuState *jeu) {
    // Bordure supérieure
    for(int y = 0; y < jeu->colonnes; y++)
        printf("───");
    printf("─\n");
    
    // Affichage ligne par ligne
    for(int x = 0; x < jeu->lignes; x++) {
        printf("│ ");
        for(int y = 0; y < jeu->colonnes; y++) {
            int index = x * jeu->colonnes + y;
            
            // Case sélectionnée en bleu
            if(jeu->mode_selection && x == jeu->selection_x && y == jeu->selection_y) {
                printf("\033[1;34m[%s]\033[0m", jeu->plateau[index]);
            }
            // Curseur en vert
            else if(x == jeu->curseur_x && y == jeu->curseur_y) {
                printf("\033[1;32m<%s>\033[0m", jeu->plateau[index]);
            }
            // Affichage normal
            else {
                printf(" %s ", jeu->plateau[index]);
            }
        }
        printf("│\n");
    }
    
    // Bordure inférieure
    for(int y = 0; y < jeu->colonnes; y++)
        printf("───");
    printf("─\n");
}

void afficherInfos(JeuState *jeu) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│ Coups restants : %-18d │\n", jeu->nbcoups);
    printf("└─────────────────────────────────────┘\n");
    
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│ OBJECTIF : %d de chaque fruit        │\n", OBJECTIF_PAR_FRUIT);
    printf("└─────────────────────────────────────┘\n");
    
    printf("\nProgression :\n");
    for(int i = 0; i < NB_TYPES_BONBONS; i++) {
        printf("  %s : %2d/%d ", jeu->emojis[i], jeu->nbemoji[i], OBJECTIF_PAR_FRUIT);
        
        // Barre de progression
        printf("[");
        int progression = (jeu->nbemoji[i] * 20) / OBJECTIF_PAR_FRUIT;
        for(int j = 0; j < 20; j++) {
            if(j < progression) printf("█");
            else printf("░");
        }
        printf("]");
        
        // Indicateur de complétion
        if(jeu->nbemoji[i] >= OBJECTIF_PAR_FRUIT) {
            printf(" \033[1;32m✓\033[0m");
        }
        printf("\n");
    }
}

void afficherControles(JeuState *jeu) {
    printf("\n┌─────────────────────────────────────┐\n");
    printf("│ Position : [%d,%d]                    │\n", jeu->curseur_x, jeu->curseur_y);
    
    if(jeu->mode_selection) {
        printf("│ \033[1;33mMODE SÉLECTION ACTIVÉ\033[0m             │\n");
        printf("│ Bonbon sélectionné : [%d,%d]         │\n", jeu->selection_x, jeu->selection_y);
        printf("│ → Déplacez vers un bonbon adjacent  │\n");
        printf("│ → Appuyez sur P pour permuter       │\n");
    } else {
        printf("│ → Déplacer : Flèches ↑↓←→           │\n");
        printf("│ → Sélectionner : P                  │\n");
    }
    
    printf("│ → Quitter : Q                       │\n");
    printf("└─────────────────────────────────────┘\n");
}

void afficherFinPartie(JeuState *jeu) {
    printf("\033[2J\033[H");  // Efface l'écran
    
    if(jeu->victoire) {
        printf("\n");
        printf("╔═══════════════════════════════════════╗\n");
        printf("║                                       ║\n");
        printf("║     \033[1;32m🎉 FÉLICITATIONS ! 🎉\033[0m           ║\n");
        printf("║                                       ║\n");
        printf("║         PARTIE TERMINÉE !             ║\n");
        printf("║                                       ║\n");
        printf("╚═══════════════════════════════════════╝\n\n");
        printf("Vous avez collecté tous les fruits requis !\n\n");
    } else {
        printf("\n");
        printf("╔═══════════════════════════════════════╗\n");
        printf("║          PARTIE TERMINÉE              ║\n");
        printf("╚═══════════════════════════════════════╝\n\n");
    }
    
    printf("┌─────────────────────────────────────┐\n");
    printf("│ STATISTIQUES FINALES                │\n");
    printf("└─────────────────────────────────────┘\n\n");
    
    printf("Fruits collectés :\n");
    for(int i = 0; i < NB_TYPES_BONBONS; i++) {
        printf("  %s : %2d/%d ", jeu->emojis[i], jeu->nbemoji[i], OBJECTIF_PAR_FRUIT);
        if(jeu->nbemoji[i] >= OBJECTIF_PAR_FRUIT) {
            printf("\033[1;32m✓\033[0m");
        }
        printf("\n");
    }
    
    printf("\nCoups utilisés : %d\n", 50 - jeu->nbcoups);
    printf("Coups restants : %d\n\n", jeu->nbcoups);
}

void afficherErreur(const char *message) {
    printf("\n\033[1;31m✗ %s\033[0m\n", message);
    usleep(1500000);
}
