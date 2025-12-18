#include <stdlib.h>
#include <string.h>

#include "moteur.h"

/* Bonbon spécial du niveau 2 : "V" = efface toute la colonne (vertical) */
static char BONBON_SPECIAL_COLONNE[] = "V";

static int estBonbonSpecial(char *c)
{
    if (c == NULL) return 0;
    return (strcmp(c, BONBON_SPECIAL_COLONNE) == 0);
}

static int estBonbonNormal(char *c)
{
    if (c == NULL) return 0;
    if (estBonbonSpecial(c)) return 0;
    return 1;
}

int trouverIndexEmoji(char *emoji, char *emojis[], int nb_emojis)
{
    int i;

    if (emoji == NULL) return -1;

    for (i = 0; i < nb_emojis; i++) {
        if (strcmp(emoji, emojis[i]) == 0) return i;
    }
    return -1;
}

/* ===================== Détection alignements simples (niveau 1) ===================== */

int verifierAlignementDepuis(char *plateau[], int lignes, int colonnes,
                             int x, int y, int marques[])
{
    int alignements_trouves = 0;
    char *bonbon = plateau[x * colonnes + y];

    if (!estBonbonNormal(bonbon)) return 0;

    /* Horizontal */
    {
        int compte_h = 1;
        int debut_h = y;
        int fin_h   = y;
        int j;

        for (j = y - 1; j >= 0; j--) {
            char *c = plateau[x * colonnes + j];
            if (estBonbonNormal(c) && strcmp(c, bonbon) == 0) { compte_h++; debut_h = j; }
            else break;
        }
        for (j = y + 1; j < colonnes; j++) {
            char *c = plateau[x * colonnes + j];
            if (estBonbonNormal(c) && strcmp(c, bonbon) == 0) { compte_h++; fin_h = j; }
            else break;
        }

        if (compte_h >= 3) {
            for (j = debut_h; j <= fin_h; j++) marques[x * colonnes + j] = 1;
            alignements_trouves = 1;
        }
    }

    /* Vertical */
    {
        int compte_v = 1;
        int debut_v = x;
        int fin_v   = x;
        int i;

        for (i = x - 1; i >= 0; i--) {
            char *c = plateau[i * colonnes + y];
            if (estBonbonNormal(c) && strcmp(c, bonbon) == 0) { compte_v++; debut_v = i; }
            else break;
        }
        for (i = x + 1; i < lignes; i++) {
            char *c = plateau[i * colonnes + y];
            if (estBonbonNormal(c) && strcmp(c, bonbon) == 0) { compte_v++; fin_v = i; }
            else break;
        }

        if (compte_v >= 3) {
            for (i = debut_v; i <= fin_v; i++) marques[i * colonnes + y] = 1;
            alignements_trouves = 1;
        }
    }

    return alignements_trouves;
}

int verifierTousAlignements(char *plateau[], int lignes, int colonnes, int marques[])
{
    int i, x, y;

    for (i = 0; i < lignes * colonnes; i++) marques[i] = 0;

    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            if (verifierAlignementDepuis(plateau, lignes, colonnes, x, y, marques)) {
                return 1;
            }
        }
    }
    return 0;
}

void supprimerBonbonsMarques(char *plateau[], int lignes, int colonnes,
                             int marques[], char *emojis[], int nbemoji[], int nb_emojis)
{
    int x, y;

    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            int index = x * colonnes + y;

            if (marques[index] == 1) {
                if (plateau[index] != NULL) {
                    int idx = trouverIndexEmoji(plateau[index], emojis, nb_emojis);
                    if (idx >= 0) nbemoji[idx]++;
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
                if (ligneEcriture != ligne) plateau[index] = NULL;

                ligneEcriture--;
            }
        }
    }
}

void remplirCasesVides(char *plateau[], int lignes, int colonnes,
                       char *emojis[], int nb_emojis)
{
    int x, y;

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

    char *tmp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = tmp;

    {
        int total = lignes * colonnes;
        int *marques = (int*)calloc((size_t)total, sizeof(int));
        int valide = 0;

        if (marques != NULL) {
            valide = verifierAlignementDepuis(plateau, lignes, colonnes, x1, y1, marques) ||
                     verifierAlignementDepuis(plateau, lignes, colonnes, x2, y2, marques);
            free(marques);
        }

        tmp = plateau[idx1];
        plateau[idx1] = plateau[idx2];
        plateau[idx2] = tmp;

        return valide;
    }
}

void permuterBonbons(char *plateau[], int colonnes, int x1, int y1, int x2, int y2)
{
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;

    char *tmp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = tmp;
}

void traiterAlignementsCascade(JeuState *jeu)
{
    int total;
    int *marques;

    if (jeu == NULL) return;

    total = jeu->lignes * jeu->colonnes;
    marques = (int*)calloc((size_t)total, sizeof(int));
    if (marques == NULL) return;

    while (1) {
        int ok = verifierTousAlignements(jeu->plateau, jeu->lignes, jeu->colonnes, marques);
        if (!ok) break;

        supprimerBonbonsMarques(
            jeu->plateau, jeu->lignes, jeu->colonnes,
            marques, jeu->emojis, jeu->nbemoji, NB_TYPES_BONBONS
        );

        faireTomber(jeu->plateau, jeu->lignes, jeu->colonnes);
        remplirCasesVides(jeu->plateau, jeu->lignes, jeu->colonnes, jeu->emojis, NB_TYPES_BONBONS);
    }

    free(marques);
}

/* ===================== Niveau 2 : combinaisons avancées ===================== */

/* Marque une suite horizontale à partir d'un début, renvoie la longueur */
static int longueurSuiteH(char *plateau[], int lignes, int colonnes, int x, int y_debut)
{
    int y = y_debut;
    char *b = plateau[x * colonnes + y_debut];

    if (!estBonbonNormal(b)) return 0;

    while (y < colonnes) {
        char *c = plateau[x * colonnes + y];
        if (estBonbonNormal(c) && strcmp(c, b) == 0) y++;
        else break;
    }
    return y - y_debut;
}

static int longueurSuiteV(char *plateau[], int lignes, int colonnes, int x_debut, int y)
{
    int x = x_debut;
    char *b = plateau[x_debut * colonnes + y];

    if (!estBonbonNormal(b)) return 0;

    while (x < lignes) {
        char *c = plateau[x * colonnes + y];
        if (estBonbonNormal(c) && strcmp(c, b) == 0) x++;
        else break;
    }
    return x - x_debut;
}

/* Détecte les combinaisons "difficiles" et remplit marques[] + specialCreate[].
   Ordre :
   1) lignes/colonnes >= 6
   2) croix 3+3
   3) carré 2x2
   4) alignements simples (3+) via verifierTousAlignements (fait après)
*/
static int detecterCombinaisonsNiveau2(char *plateau[], int lignes, int colonnes,
                                       int marques[], int specialCreate[])
{
    int trouve = 0;
    int x, y;

    /* remise à zéro */
    for (x = 0; x < lignes * colonnes; x++) {
        marques[x] = 0;
        specialCreate[x] = 0;
    }

    /* 1) Alignements longs (>=6) horizontaux */
    for (x = 0; x < lignes; x++) {
        y = 0;
        while (y < colonnes) {
            int len = longueurSuiteH(plateau, lignes, colonnes, x, y);
            if (len >= 6) {
                int j;
                int milieu = y + (len / 2);
                int specialIndex = x * colonnes + milieu;

                for (j = y; j < y + len; j++) {
                    marques[x * colonnes + j] = 1;
                }
                marques[specialIndex] = 0; /* on garde le spécial */
                specialCreate[specialIndex] = 1;

                trouve = 1;
                y = y + len;
            } else if (len > 0) {
                y = y + len;
            } else {
                y++;
            }
        }
    }

    /* 1) Alignements longs (>=6) verticaux */
    for (y = 0; y < colonnes; y++) {
        x = 0;
        while (x < lignes) {
            int len = longueurSuiteV(plateau, lignes, colonnes, x, y);
            if (len >= 6) {
                int i;
                int milieu = x + (len / 2);
                int specialIndex = milieu * colonnes + y;

                for (i = x; i < x + len; i++) {
                    marques[i * colonnes + y] = 1;
                }
                marques[specialIndex] = 0;
                specialCreate[specialIndex] = 1;

                trouve = 1;
                x = x + len;
            } else if (len > 0) {
                x = x + len;
            } else {
                x++;
            }
        }
    }

    /* 2) Croix / T / L : une ligne verticale (>=3) et une ligne horizontale (>=3)
          qui se croisent sur UNE même case.
          IMPORTANT : la case commune n'a pas besoin d'être le centre des lignes.
          Exemple accepté : la ligne verticale peut être 'au-dessus' du point commun,
          et la ligne horizontale peut être à gauche/droite du point commun.
    */
    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            char *centre = plateau[x * colonnes + y];

            int haut = 0;
            int bas = 0;
            int gauche = 0;
            int droite = 0;

            int i, j;

            if (!estBonbonNormal(centre)) continue;
            if (specialCreate[x * colonnes + y] == 1) continue; /* ne pas écraser un spécial déjà créé */

            /* compter vers le haut */
            for (i = x - 1; i >= 0; i--) {
                char *c = plateau[i * colonnes + y];
                if (estBonbonNormal(c) && strcmp(c, centre) == 0) haut++;
                else break;
            }

            /* compter vers le bas */
            for (i = x + 1; i < lignes; i++) {
                char *c = plateau[i * colonnes + y];
                if (estBonbonNormal(c) && strcmp(c, centre) == 0) bas++;
                else break;
            }

            /* compter vers la gauche */
            for (j = y - 1; j >= 0; j--) {
                char *c = plateau[x * colonnes + j];
                if (estBonbonNormal(c) && strcmp(c, centre) == 0) gauche++;
                else break;
            }

            /* compter vers la droite */
            for (j = y + 1; j < colonnes; j++) {
                char *c = plateau[x * colonnes + j];
                if (estBonbonNormal(c) && strcmp(c, centre) == 0) droite++;
                else break;
            }

            /* longueur totale (incluant la case commune) */
            if ((1 + haut + bas) >= 3 && (1 + gauche + droite) >= 3) {
                int centerIndex = x * colonnes + y;

                /* marquer toute la verticale autour du centre */
                for (i = x - haut; i <= x + bas; i++) {
                    marques[i * colonnes + y] = 1;
                }

                /* marquer toute l'horizontale autour du centre */
                for (j = y - gauche; j <= y + droite; j++) {
                    marques[x * colonnes + j] = 1;
                }

                /* créer un spécial sur la case commune */
                specialCreate[centerIndex] = 1;
                marques[centerIndex] = 0;

                trouve = 1;
            }
        }
    }

    /* 3) Carré 2x2 */
    for (x = 0; x < lignes - 1; x++) {
        for (y = 0; y < colonnes - 1; y++) {
            char *a = plateau[x * colonnes + y];
            char *b = plateau[x * colonnes + (y+1)];
            char *c = plateau[(x+1) * colonnes + y];
            char *d = plateau[(x+1) * colonnes + (y+1)];

            if (!estBonbonNormal(a)) continue;

            if (estBonbonNormal(b) && estBonbonNormal(c) && estBonbonNormal(d) &&
                strcmp(a, b) == 0 && strcmp(a, c) == 0 && strcmp(a, d) == 0) {

                int specialIndex = x * colonnes + y;

                marques[x * colonnes + (y+1)] = 1;
                marques[(x+1) * colonnes + y] = 1;
                marques[(x+1) * colonnes + (y+1)] = 1;

                /* on garde le coin haut-gauche comme spécial */
                specialCreate[specialIndex] = 1;
                marques[specialIndex] = 0;

                trouve = 1;
            }
        }
    }

    return trouve;
}

/* Test rapide : existe-t-il AU MOINS UNE combinaison niveau 2 sur le plateau ? */
static int existeCombinaisonNiveau2(char *plateau[], int lignes, int colonnes)
{
    int total = lignes * colonnes;
    int *marques = (int*)calloc((size_t)total, sizeof(int));
    int *specialCreate = (int*)calloc((size_t)total, sizeof(int));
    int ok = 0;

    if (marques == NULL || specialCreate == NULL) {
        if (marques) free(marques);
        if (specialCreate) free(specialCreate);
        return 0;
    }

    /* d'abord les combinaisons avancées */
    if (detecterCombinaisonsNiveau2(plateau, lignes, colonnes, marques, specialCreate)) {
        ok = 1;
    } else {
        /* sinon alignement simple */
        ok = verifierTousAlignements(plateau, lignes, colonnes, marques);
    }

    free(marques);
    free(specialCreate);
    return ok;
}

int permutationValideNiveau2(char *plateau[], int lignes, int colonnes,
                             int x1, int y1, int x2, int y2)
{
    int idx1 = x1 * colonnes + y1;
    int idx2 = x2 * colonnes + y2;

    char *tmp = plateau[idx1];
    plateau[idx1] = plateau[idx2];
    plateau[idx2] = tmp;

    {
        int ok = existeCombinaisonNiveau2(plateau, lignes, colonnes);

        /* remettre comme avant */
        tmp = plateau[idx1];
        plateau[idx1] = plateau[idx2];
        plateau[idx2] = tmp;

        return ok;
    }
}

void effetSpecialColonne(JeuState *jeu, int x, int y)
{
    int i;

    if (jeu == NULL) return;
    if (y < 0 || y >= jeu->colonnes) return;

    for (i = 0; i < jeu->lignes; i++) {
        int idx = i * jeu->colonnes + y;

        if (jeu->plateau[idx] != NULL) {
            int emo = trouverIndexEmoji(jeu->plateau[idx], jeu->emojis, NB_TYPES_BONBONS);
            if (emo >= 0) jeu->nbemoji[emo]++;
        }
        jeu->plateau[idx] = NULL;
    }
}

void traiterAlignementsCascadeNiveau2(JeuState *jeu)
{
    int total;
    int *marques;
    int *specialCreate;

    if (jeu == NULL) return;

    total = jeu->lignes * jeu->colonnes;

    marques = (int*)calloc((size_t)total, sizeof(int));
    specialCreate = (int*)calloc((size_t)total, sizeof(int));

    if (marques == NULL || specialCreate == NULL) {
        if (marques) free(marques);
        if (specialCreate) free(specialCreate);
        return;
    }

    while (1) {
        int trouveComplexe;
        int trouveSimple;

        /* Étape 1 : combinaisons avancées (ordre long -> simple) */
        trouveComplexe = detecterCombinaisonsNiveau2(
            jeu->plateau, jeu->lignes, jeu->colonnes, marques, specialCreate
        );

        /* Étape 2 : alignements simples (3+) en dernier */
        {
            int *marquesSimple = (int*)calloc((size_t)total, sizeof(int));
            if (marquesSimple != NULL) {
                trouveSimple = verifierTousAlignements(
                    jeu->plateau, jeu->lignes, jeu->colonnes, marquesSimple
                );

                if (trouveSimple) {
                    int i;
                    for (i = 0; i < total; i++) {
                        if (specialCreate[i] == 0 && marquesSimple[i] == 1) {
                            marques[i] = 1;
                        }
                    }
                }
                free(marquesSimple);
            } else {
                trouveSimple = 0;
            }
        }

        if (!trouveComplexe && !trouveSimple) {
            break;
        }

        /* Création des spéciaux d'abord (ils restent sur le plateau) */
        {
            int i;
            for (i = 0; i < total; i++) {
                if (specialCreate[i] == 1) {
                    jeu->plateau[i] = BONBON_SPECIAL_COLONNE;
                    marques[i] = 0;
                }
            }
        }

        /* Suppression des marqués */
        supprimerBonbonsMarques(
            jeu->plateau, jeu->lignes, jeu->colonnes,
            marques, jeu->emojis, jeu->nbemoji, NB_TYPES_BONBONS
        );

        /* Chute + remplissage */
        faireTomber(jeu->plateau, jeu->lignes, jeu->colonnes);
        remplirCasesVides(jeu->plateau, jeu->lignes, jeu->colonnes, jeu->emojis, NB_TYPES_BONBONS);
    }

    free(marques);
    free(specialCreate);
}
