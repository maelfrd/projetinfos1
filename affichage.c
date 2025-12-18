#include <stdio.h>
#include "affichage.h"

void nettoyer_ecran(void)
{
    int i;
    for (i = 0; i < 60; i++) {
        printf("\n");
    }
}

void pause_entree(void)
{
    int c;
    printf("\nAppuie sur Entrée pour continuer...");
    fflush(stdout);

    /* on vide la fin de ligne si besoin */
    while (1) {
        c = getchar();
        if (c == '\n' || c == EOF) break;
    }
}

void afficherEntete(void)
{
    printf("============================================\n");
    printf("                 CANDY CRUSH                \n");
    printf("============================================\n\n");
}

/* Affiche une case : si NULL, on affiche un espace (évite printf(\"%s\", NULL)). */
static const char *contenu_case(char *contenu)
{
    if (contenu == NULL) return " ";
    return contenu;
}

void afficherPlateau(JeuState *jeu)
{
    int x, y;

    if (jeu == NULL) return;

    /* ligne du haut */
    for (y = 0; y < jeu->colonnes; y++) printf("---");
    printf("-\n");

    for (x = 0; x < jeu->lignes; x++) {
        printf("| ");
        for (y = 0; y < jeu->colonnes; y++) {
            int index = x * jeu->colonnes + y;
            const char *contenu = contenu_case(jeu->plateau[index]);

            if (jeu->mode_selection && x == jeu->selection_x && y == jeu->selection_y) {
                printf("[%s]", contenu);
            } else if (x == jeu->curseur_x && y == jeu->curseur_y) {
                printf("<%s>", contenu);
            } else {
                printf(" %s ", contenu);
            }
        }
        printf("|\n");
    }

    /* ligne du bas */
    for (y = 0; y < jeu->colonnes; y++) printf("---");
    printf("-\n");
}

void afficherInfos(JeuState *jeu)
{
    int i;

    const char *noms[NB_TYPES_BONBONS] = { "Mangue", "Citron", "Pomme", "Raisin", "Tomate" };

    if (jeu == NULL) return;

    printf("\nCoups restants : %d\n", jeu->nbcoups);
    printf("Objectif : %d de chaque fruit\n", OBJECTIF_PAR_FRUIT);

    printf("\nRecolte :\n");
    for (i = 0; i < NB_TYPES_BONBONS; i++) {
        printf("  %s %-7s : %2d/%d",
               jeu->emojis[i], noms[i], jeu->nbemoji[i], OBJECTIF_PAR_FRUIT);

        if (jeu->nbemoji[i] >= OBJECTIF_PAR_FRUIT) {
            printf(" [OK]");
        }
        printf("\n");
    }
}

void afficherControles(JeuState *jeu)
{
    if (jeu == NULL) return;

    printf("\nPosition curseur : [%d,%d]\n", jeu->curseur_x, jeu->curseur_y);

    if (jeu->mode_selection) {
        printf("Mode selection : ACTIF\n");
        printf("Selection : [%d,%d]\n", jeu->selection_x, jeu->selection_y);
        printf("Deplace Z Q S D puis Entrée pour permuter\n");
        printf("P : annuler selection\n");
    } else {
        printf("Z Q S D : deplacer\n");
        printf("P : selectionner\n");
        printf("Entrée : valider permutation (quand selection active)\n");
    }

    printf("X : quitter la partie\n");
}

void afficherErreur(const char *message)
{
    printf("\n[ERREUR] %s\n", message);
    pause_entree();
}

void afficherJeu(JeuState *jeu)
{
    nettoyer_ecran();
    afficherEntete();
    afficherPlateau(jeu);
    afficherInfos(jeu);
    afficherControles(jeu);
}

void afficherFinPartie(JeuState *jeu)
{
    nettoyer_ecran();

    if (jeu == NULL) return;

    if (jeu->victoire) {
        printf("============================================\n");
        printf("              VICTOIRE !                     \n");
        printf("============================================\n\n");
        printf("Tu as atteint tous les objectifs.\n");
    } else {
        printf("============================================\n");
        printf("             FIN DE PARTIE                   \n");
        printf("============================================\n\n");
        printf("Plus de coups disponibles ou partie quittée.\n");
    }

    printf("\nCoups restants : %d\n", jeu->nbcoups);
    pause_entree();
}
