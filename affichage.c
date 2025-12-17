#include <stdio.h>
#include <unistd.h>

#include "affichage.h"

static const char* safeCell(char *cell) {
    return (cell == 0) ? " " : cell;
}

void afficherEntete(void) {
    printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
    printf("🔛          CANDY CRUSH                       🔛\n");
    printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n\n");
}

void afficherPlateau(JeuState *jeu) {
    if (jeu == 0) return;

    for (int y = 0; y < (*jeu).colonnes; y++) printf("───");
    printf("─\n");

    for (int x = 0; x < (*jeu).lignes; x++) {
        printf("│ ");
        for (int y = 0; y < (*jeu).colonnes; y++) {
            int index = x * (*jeu).colonnes + y;
            const char *cell = safeCell((*jeu).plateau[index]);

            if ((*jeu).mode_selection && x == (*jeu).selection_x && y == (*jeu).selection_y) {
                printf("\033[1;34m[%s]\033[0m", cell);
            } else if (x == (*jeu).curseur_x && y == (*jeu).curseur_y) {
                printf("\033[1;32m<%s>\033[0m", cell);
            } else {
                printf(" %s ", cell);
            }
        }
        printf("│\n");
    }

    for (int y = 0; y < (*jeu).colonnes; y++) printf("───");
    printf("─\n");
}

void afficherInfos(JeuState *jeu) {
    if (jeu == 0) return;

    const char *noms[NB_TYPES_BONBONS] = { "Mangue", "Citron", "Pomme", "Raisin", "Tomate" };

    printf("\n➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
    printf("🔛 Coups restants : %-18d         🔛\n", (*jeu).nbcoups);
    printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");

    printf("\n➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
    printf("🔛 OBJECTIF : %d de chaque fruit                🔛\n", OBJECTIF_PAR_FRUIT);
    printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");

    printf("\nRécolte :\n");
    for (int i = 0; i < NB_TYPES_BONBONS; i++) {
        printf("  %s %-7s : %2d/%d", (*jeu).emojis[i], noms[i], (*jeu).nbemoji[i], OBJECTIF_PAR_FRUIT);
        if ((*jeu).nbemoji[i] >= OBJECTIF_PAR_FRUIT) printf(" \033[1;32m✓\033[0m");
        printf("\n");
    }
}

void afficherControles(JeuState *jeu) {
    if (jeu == 0) return;

    printf("\n➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
    printf("🔛 Position : [%d,%d]                            🔛\n", (*jeu).curseur_x, (*jeu).curseur_y);

    if ((*jeu).mode_selection) {
        printf("🔛 \033[1;33mMODE SÉLECTION ACTIVÉ\033[0m                     🔛\n");
        printf("🔛 Bonbon sélectionné : [%d,%d]                 🔛\n", (*jeu).selection_x, (*jeu).selection_y);
        printf("🔛 → Déplacez vers un bonbon adjacent          🔛\n");
        printf("🔛 → Entrée pour permuter                      🔛\n");
        printf("🔛 → P pour annuler la sélection               🔛\n");
    } else {
        printf("🔛 → Déplacer : Z Q S D                         🔛\n");
        printf("🔛 → Sélection : P                              🔛\n");
        printf("🔛 → Valider permutation : Entrée               🔛\n");
    }

    printf("🔛 → Quitter : X                               🔛\n");
    printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
}

void afficherFinPartie(JeuState *jeu) {
    if (jeu == 0) return;

    printf("\033[2J\033[H");

    if ((*jeu).victoire) {
        printf("\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
        printf("🔛                                               🔛\n");
        printf("🔛     \033[1;32m🎉 FÉLICITATIONS ! 🎉\033[0m                   🔛\n");
        printf("🔛                                               🔛\n");
        printf("🔛         PARTIE TERMINÉE !                     🔛\n");
        printf("🔛                                               🔛\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n\n");
        printf("Vous avez collecté tous les fruits requis !\n\n");
    } else {
        printf("\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
        printf("🔛                                               🔛\n");
        printf("🔛         \033[1;31mPARTIE TERMINÉE\033[0m                      🔛\n");
        printf("🔛                                               🔛\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n\n");
        printf("Vous n'avez plus de coups disponibles.\n\n");
    }

    printf("Coups utilisés : %d\n", 50 - (*jeu).nbcoups);
    printf("Coups restants : %d\n\n", (*jeu).nbcoups);
}

void afficherErreur(const char *message) {
    printf("\n\033[1;31m✗ %s\033[0m\n", message);
    sleep(1);
}

void afficherJeu(JeuState *jeu) {
    printf("\033[2J\033[H");
    afficherEntete();
    afficherPlateau(jeu);
    afficherInfos(jeu);
    afficherControles(jeu);
}
