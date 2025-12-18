#include <stdlib.h>
#include <string.h>

#include "moteur.h"

int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis)
{
    int i;

    if (emoji == NULL) {
        return -1;
    }

    for (i = 0; i < nb_emojis; i++) {
        if (strcmp(emoji, emojis[i]) == 0) {
            return i;
        }
    }

    return -1;
}

int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes,
                             int x, int y, int marques[])
{
    int alignements_trouves = 0;

    char *bonbon = plateau[x * colonnes + y];
    if (bonbon == NULL) {
        return 0;
    }

    /* ===== Alignement horizontal =====
       compte_h : nombre de bonbons identiques collés à (x,y) sur la ligne
       debut_h / fin_h : colonnes extrêmes de la séquence
    */
    {
        int compte_h = 1;
        int debut_h = y;
        int fin_h   = y;

        int j;

        /* à gauche */
        for (j = y - 1; j >= 0; j--) {
            char *c = plateau[x * colonnes + j];
            if (c != NULL && strcmp(c, bonbon) == 0) {
                compte_h++;
                debut_h = j;
            } else {
                break;
            }
        }

        /* à droite */
        for (j = y + 1; j < colonnes; j++) {
            char *c = plateau[x * colonnes + j];
            if (c != NULL && strcmp(c, bonbon) == 0) {
                compte_h++;
                fin_h = j;
            } else {
                break;
            }
        }

        /* si 3+ alors on marque toutes les cases */
        if (compte_h >= 3) {
            for (j = debut_h; j <= fin_h; j++) {
                marques[x * colonnes + j] = 1;
            }
            alignements_trouves = 1;
        }
    }

    /* ===== Alignement vertical ===== */
    {
        int compte_v = 1;
        int debut_v = x;
        int fin_v   = x;

        int i;

        /* en haut */
        for (i = x - 1; i >= 0; i--) {
            char *c = plateau[i * colonnes + y];
            if (c != NULL && strcmp(c, bonbon) == 0) {
                compte_v++;
                debut_v = i;
            } else {
                break;
            }
        }

        /* en bas */
        for (i = x + 1; i < lignes; i++) {
            char *c = plateau[i * colonnes + y];
            if (c != NULL && strcmp(c, bonbon) == 0) {
                compte_v++;
                fin_v = i;
            } else {
                break;
            }
        }

        if (compte_v >= 3) {
            for (i = debut_v; i <= fin_v; i++) {
                marques[i * colonnes + y] = 1;
            }
            alignements_trouves = 1;
        }
    }

    return alignements_trouves;
}

int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[])
{
    int i;
    int x;
    int y;

    /* remise à zéro */
    for (i = 0; i < lignes * colonnes; i++) {
        marques[i] = 0;
    }

    /* on cherche au moins un alignement */
    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            if (verifierAlignementDepuis(plateau, lignes, colonnes, x, y, marques)) {
                return 1; /* dès qu'on en trouve un, c'est suffisant */
            }
        }
    }

    return 0;
}

void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes,
                             int marques[], char *emojis[], int nbemoji[], int nb_emojis)
{
    int x;
    int y;

    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            int index = x * colonnes + y;

            if (marques[index] == 1) {
                if (plateau[index] != NULL) {
                    int emoji_idx = trouverIndexEmoji(plateau[index], emojis, nb_emojis);
                    if (emoji_idx >= 0) {
                        nbemoji[emoji_idx]++;
                    }
                }
                plateau[index] = NULL;
            }
        }
    }
}

void faireTomber(char *plateau[], int lignes, int colonnes)
{
    int col;

    for (col = 0; col < colonnes; col++) {
        int ligneEcriture = lignes - 1;
        int ligne;

        for (ligne = lignes - 1; ligne >= 0; ligne--) {
            int index = ligne * colonnes + col;

            if (plateau[index] != NULL) {
                int indexEcriture = ligneEcriture * colonnes + col;

                plateau[indexEcriture] = plateau[index];

                if (ligneEcriture != ligne) {
                    plateau[index] = NULL;
                }

                ligneEcriture--;
            }
        }
    }
}

void remplirCasesVides(char *plateau[], int lignes, int colonnes,
                       char *emojis[], int nb_emojis)
{
    int x;
    int y;

    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            int index = x * colonnes + y;

            if (plateau[index] == NULL) {
                int r = rand() % nb_emojis;
                plateau[index] = emojis[r];
            }
        }
    }
}

int permutationValide(char *plateau[], int lignes, int colonnes,
                      int x1, int y1, int x2, int y2)
{
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;

    /* on échange temporairement */
    char *temp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = temp;

    /* tableau de marques pour tester l'alignement */
    {
        int total = lignes * colonnes;
        int *marques = (int*)calloc((size_t)total, sizeof(int));
        int valide = 0;

        if (marques != NULL) {
            /* il suffit qu'une des deux positions participe à un alignement */
            valide = verifierAlignementDepuis(plateau, lignes, colonnes, x1, y1, marques) ||
                     verifierAlignementDepuis(plateau, lignes, colonnes, x2, y2, marques);
            free(marques);
        }

        /* on remet le plateau comme avant */
        temp = plateau[idx1];
        plateau[idx1] = plateau[idx2];
        plateau[idx2] = temp;

        return valide;
    }
}

void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2)
{
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;

    char *temp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = temp;
}

void traiterAlignementsCascade(JeuState *jeu)
{
    int total;
    int *marques;

    if (jeu == NULL) {
        return;
    }

    total = jeu->lignes * jeu->colonnes;
    marques = (int*)calloc((size_t)total, sizeof(int));
    if (marques == NULL) {
        return;
    }

    while (1) {
        int alignements_trouves = verifierTousAlignements(
            jeu->plateau, jeu->lignes, jeu->colonnes, marques
        );

        if (!alignements_trouves) {
            break;
        }

        supprimerBonbonsMarques(
            jeu->plateau, jeu->lignes, jeu->colonnes,
            marques, jeu->emojis, jeu->nbemoji, NB_TYPES_BONBONS
        );
        faireTomber(jeu->plateau, jeu->lignes, jeu->colonnes);
        remplirCasesVides(jeu->plateau, jeu->lignes, jeu->colonnes, jeu->emojis, NB_TYPES_BONBONS);
    }

    free(marques);
}