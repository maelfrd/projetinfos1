#include <stdlib.h>
#include <string.h>

#include "moteur.h"

int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis) {
    if (emoji == 0) return -1;
    for (int i = 0; i < nb_emojis; i++) {
        if (strcmp(emoji, emojis[i]) == 0) return i;
    }
    return -1;
}

int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes,
                             int x, int y, int marques[]) {
    int alignements_trouves = 0;

    char *bonbon = plateau[x * colonnes + y];
    if (bonbon == 0) return 0;

    int compte_h = 1;
    int debut_h = y, fin_h = y;

    for (int j = y - 1; j >= 0; j--) {
        char *c = plateau[x * colonnes + j];
        if (c != 0 && strcmp(c, bonbon) == 0) { compte_h++; debut_h = j; }
        else break;
    }
    for (int j = y + 1; j < colonnes; j++) {
        char *c = plateau[x * colonnes + j];
        if (c != 0 && strcmp(c, bonbon) == 0) { compte_h++; fin_h = j; }
        else break;
    }
    if (compte_h >= 3) {
        for (int j = debut_h; j <= fin_h; j++) marques[x * colonnes + j] = 1;
        alignements_trouves = 1;
    }

    int compte_v = 1;
    int debut_v = x, fin_v = x;

    for (int i = x - 1; i >= 0; i--) {
        char *c = plateau[i * colonnes + y];
        if (c != 0 && strcmp(c, bonbon) == 0) { compte_v++; debut_v = i; }
        else break;
    }
    for (int i = x + 1; i < lignes; i++) {
        char *c = plateau[i * colonnes + y];
        if (c != 0 && strcmp(c, bonbon) == 0) { compte_v++; fin_v = i; }
        else break;
    }
    if (compte_v >= 3) {
        for (int i = debut_v; i <= fin_v; i++) marques[i * colonnes + y] = 1;
        alignements_trouves = 1;
    }

    return alignements_trouves;
}

int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[]) {
    for (int i = 0; i < lignes * colonnes; i++) marques[i] = 0;

    int total_alignements = 0;
    for (int x = 0; x < lignes; x++) {
        for (int y = 0; y < colonnes; y++) {
            total_alignements += verifierAlignementDepuis(plateau, lignes, colonnes, x, y, marques);
        }
    }
    return total_alignements > 0;
}

void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes,
                             int marques[], char *emojis[], int nbemoji[], int nb_emojis) {
    for (int x = 0; x < lignes; x++) {
        for (int y = 0; y < colonnes; y++) {
            int index = x * colonnes + y;
            if (marques[index] == 1) {
                if (plateau[index] != 0) {
                    int emoji_idx = trouverIndexEmoji(plateau[index], emojis, nb_emojis);
                    if (emoji_idx >= 0) nbemoji[emoji_idx]++;
                }
                plateau[index] = 0;
            }
        }
    }
}

void faireTomber(char *plateau[], int lignes, int colonnes) {
    for (int col = 0; col < colonnes; col++) {
        int ligneEcriture = lignes - 1;
        for (int ligne = lignes - 1; ligne >= 0; ligne--) {
            int index = ligne * colonnes + col;
            if (plateau[index] != 0) {
                int indexEcriture = ligneEcriture * colonnes + col;
                plateau[indexEcriture] = plateau[index];
                if (ligneEcriture != ligne) plateau[index] = 0;
                ligneEcriture--;
            }
        }
    }
}

void remplirCasesVides(char *plateau[], int lignes, int colonnes,
                       char *emojis[], int nb_emojis) {
    for (int x = 0; x < lignes; x++) {
        for (int y = 0; y < colonnes; y++) {
            int index = x * colonnes + y;
            if (plateau[index] == 0) {
                int r = rand() % nb_emojis;
                plateau[index] = emojis[r];
            }
        }
    }
}

int permutationValide(char *plateau[], int lignes, int colonnes,
                      int x1, int y1, int x2, int y2) {
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;

    char *temp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = temp;

    size_t total = (size_t)lignes * (size_t)colonnes;
    int *marques = (int*)calloc(total, sizeof(int));
    int valide =
        verifierAlignementDepuis(plateau, lignes, colonnes, x1, y1, marques) ||
        verifierAlignementDepuis(plateau, lignes, colonnes, x2, y2, marques);
    free(marques);

    temp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = temp;

    return valide;
}

void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2) {
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;
    char *temp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = temp;
}

void traiterAlignementsCascade(JeuState *jeu) {
    if (jeu == 0) return;

    int continuer_traitement = 1;

    while (continuer_traitement) {
        size_t total = (size_t)(*jeu).lignes * (size_t)(*jeu).colonnes;
        int *marques = (int*)calloc(total, sizeof(int));

        int alignements_trouves = verifierTousAlignements(
            (*jeu).plateau, (*jeu).lignes, (*jeu).colonnes, marques
        );

        if (alignements_trouves) {
            supprimerBonbonsMarques(
                (*jeu).plateau, (*jeu).lignes, (*jeu).colonnes,
                marques, (*jeu).emojis, (*jeu).nbemoji, NB_TYPES_BONBONS
            );
            faireTomber((*jeu).plateau, (*jeu).lignes, (*jeu).colonnes);
            remplirCasesVides((*jeu).plateau, (*jeu).lignes, (*jeu).colonnes, (*jeu).emojis, NB_TYPES_BONBONS);
        } else {
            continuer_traitement = 0;
        }

        free(marques);
    }
}
