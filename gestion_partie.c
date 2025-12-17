#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"

static int iabs(int v) { return (v < 0) ? -v : v; }

static int estBlanc(char c) {
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '\r' || c == '\v' || c == '\f');
}

static char enMinusculeASCII(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
    return c;
}

static char lireCommandeLigne(void) {
    char buf[64];

    if (fgets(buf, sizeof(buf), stdin) == 0) {
        return '\0';
    }

    for (int i = 0; buf[i] != '\0'; i++) {
        if (buf[i] == '\n') return '\n';
        if (estBlanc(buf[i])) continue;
        return enMinusculeASCII(buf[i]);
    }

    return '\n';
}

void initialiserPartie(JeuState *jeu, int lignes, int colonnes) {
    if (jeu == 0) return;

    if (lignes <= 0) lignes = 10;
    if (colonnes <= 0) colonnes = 10;

    (*jeu).lignes = lignes;
    (*jeu).colonnes = colonnes;
    (*jeu).nbcoups = 50;

    (*jeu).curseur_x = 0;
    (*jeu).curseur_y = 0;

    (*jeu).mode_selection = 0;
    (*jeu).selection_x = -1;
    (*jeu).selection_y = -1;

    (*jeu).continuer = 1;
    (*jeu).victoire = 0;

    (*jeu).emojis[0] = "🥭";
    (*jeu).emojis[1] = "🍋";
    (*jeu).emojis[2] = "🍏";
    (*jeu).emojis[3] = "🍇";
    (*jeu).emojis[4] = "🍅";

    for (int i = 0; i < NB_TYPES_BONBONS; i++) {
        (*jeu).nbemoji[i] = 0;
    }

    size_t total_cases = (size_t)lignes * (size_t)colonnes;
    (*jeu).plateau = (char**)malloc(total_cases * sizeof(char*));
    if ((*jeu).plateau == 0) {
        (*jeu).continuer = 0;
        return;
    }

    srand((unsigned int)time(0));
    for (int x = 0; x < lignes; x++) {
        for (int y = 0; y < colonnes; y++) {
            int r = rand() % NB_TYPES_BONBONS;
            (*jeu).plateau[x * colonnes + y] = (*jeu).emojis[r];
        }
    }
}

int verifierVictoire(JeuState *jeu) {
    if (jeu == 0) return 0;
    for (int i = 0; i < NB_TYPES_BONBONS; i++) {
        if ((*jeu).nbemoji[i] < OBJECTIF_PAR_FRUIT) return 0;
    }
    return 1;
}

void libererPartie(JeuState *jeu) {
    if (jeu == 0) return;
    free((*jeu).plateau);
    (*jeu).plateau = 0;
}

int gererPermutation(JeuState *jeu) {
    if (jeu == 0) return -2;

    int dx = iabs((*jeu).curseur_x - (*jeu).selection_x);
    int dy = iabs((*jeu).curseur_y - (*jeu).selection_y);

    int code = 0;

    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        code = -1;
    } else {
        int ok = permutationValide(
            (*jeu).plateau, (*jeu).lignes, (*jeu).colonnes,
            (*jeu).selection_x, (*jeu).selection_y,
            (*jeu).curseur_x, (*jeu).curseur_y
        );

        if (!ok) {
            code = -2;
        } else {
            permuterBonbons(
                (*jeu).plateau, (*jeu).colonnes,
                (*jeu).selection_x, (*jeu).selection_y,
                (*jeu).curseur_x, (*jeu).curseur_y
            );
            (*jeu).nbcoups--;

            traiterAlignementsCascade(jeu);
            code = 0;
        }
    }

    (*jeu).mode_selection = 0;
    (*jeu).selection_x = -1;
    (*jeu).selection_y = -1;

    return code;
}

void boucleJeu(JeuState *jeu) {
    if (jeu == 0) return;

    while ((*jeu).continuer) {

        if ((*jeu).nbcoups <= 0) break;

        (*jeu).victoire = verifierVictoire(jeu);
        if ((*jeu).victoire) break;

        afficherJeu(jeu);

        printf("Commande (z=haut, s=bas, q=gauche, d=droite, P=selection, Entrée=valider, x=quitter) : ");
        fflush(stdout);

        char c = lireCommandeLigne();

        if (c == '\0') {
            (*jeu).continuer = 0;
            break;
        }

        if (c == '\n') {
            if ((*jeu).mode_selection) {
                int r = gererPermutation(jeu);
                if (r == -1) afficherErreur("Les bonbons doivent etre adjacents !");
                if (r == -2) afficherErreur("Permutation invalide (aucun alignement) !");
            }
            continue;
        }

        appliquerCommande(jeu, c);
    }

    (*jeu).victoire = verifierVictoire(jeu);
}
