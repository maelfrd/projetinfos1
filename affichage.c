#include <stdio.h>
#include <stddef.h>
#include <unistd.h>

#include "affichage.h"


void nettoyer_ecran(void)
    {
    for (int i = 0; i < 60; i++) {
        printf("\n");
    }
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
            const char *contenu = (*jeu).plateau[index];
            if (contenu == NULL) {
                contenu = " ";
            }


            if ((*jeu).mode_selection && x == (*jeu).selection_x && y == (*jeu).selection_y) {
                printf("[%s]", contenu);
            } else if (x == (*jeu).curseur_x && y == (*jeu).curseur_y) {
                printf("<%s>", contenu);
            } else {
                printf(" %s ", contenu);
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
        if ((*jeu).nbemoji[i] >= OBJECTIF_PAR_FRUIT) printf(" [OK]");
        printf("\n");
    }
}

void afficherControles(JeuState *jeu) {
    if (jeu == 0) return;

    printf("\n➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
    printf("🔛 Position : [%d,%d]                            🔛\n", (*jeu).curseur_x, (*jeu).curseur_y);

    if ((*jeu).mode_selection) {
        printf("🔛 MODE SÉLECTION ACTIVÉ                       🔛\n");
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
    if ((*jeu).victoire) {
        printf("\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
        printf("🔛                                               🔛\n");
        printf("🔛     🎉 FÉLICITATIONS ! 🎉                    🔛\n");
        printf("🔛                                               🔛\n");
        printf("🔛         PARTIE TERMINÉE !                     🔛\n");
        printf("🔛                                               🔛\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n\n");
        printf("Vous avez collecté tous les fruits requis !\n\n");
    } else {
        printf("\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n");
        printf("🔛                                               🔛\n");
        printf("🔛         PARTIE TERMINÉE                      🔛\n");
        printf("🔛                                               🔛\n");
        printf("➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖➖\n\n");
        printf("Vous n'avez plus de coups disponibles.\n\n");
    }

    printf("Coups utilisés : %d\n", 50 - (*jeu).nbcoups);
    printf("Coups restants : %d\n\n", (*jeu).nbcoups);
}

void afficherErreur(const char *message) {
    printf("\n[ERREUR] %s\n", message);
    
}

void afficherJeu(JeuState *jeu) {
    nettoyer_ecran();
    afficherEntete();
    afficherPlateau(jeu);
    afficherInfos(jeu);
    afficherControles(jeu);
}

