#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"


/* valeur absolue : simple et lisible */
static int valeur_absolue(int v)
{
    if (v < 0) return -v;
    return v;
}

/* est-ce un espace / separateur ? */
static int estBlanc(char c)
{
    if (c == ' ') return 1;
    if (c == '\t') return 1;
    if (c == '\n') return 1;
    if (c == '\r') return 1;
    return 0;
}

/* Lit une commande : z/q/s/d/p/x, ou '\n' (Entrée). Ignore les espaces. */
static char lireCommandeLigne(void)
{
    char buf[128];
    int i;

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 'x'; /* sécurité */
    }

    /* si l'utilisateur appuie juste sur Entrée */
    if (buf[0] == '\n' || buf[0] == '\0') {
        return '\n';
    }

    for (i = 0; buf[i] != '\0'; i++) {
        if (!estBlanc(buf[i])) {
            return buf[i]; /* pas de conversion : minuscules uniquement */
        }
    }

    return '\n';
}

void initialiserPartie(JeuState *jeu, int lignes, int colonnes)
{
    int i;
    int total;

    if (jeu == NULL) return;

    srand((unsigned int)time(NULL));

    jeu->lignes = lignes;
    jeu->colonnes = colonnes;
    jeu->emojis[0] = "🥭";
    jeu->emojis[1] = "🍋";
    jeu->emojis[2] = "🍏";
    jeu->emojis[3] = "🍇";
    jeu->emojis[4] = "🍅";

    jeu->nbcoups = 50;

    jeu->curseur_x = 0;
    jeu->curseur_y = 0;

    jeu->mode_selection = 0;
    jeu->selection_x = -1;
    jeu->selection_y = -1;

    jeu->victoire = 0;
    jeu->continuer = 1;

    total = lignes * colonnes;

    jeu->plateau = (char**)malloc((size_t)total * sizeof(char*));
    if (jeu->plateau == NULL) {
        jeu->continuer = 0;
        return;
    }

    for (i = 0; i < NB_TYPES_BONBONS; i++) {
        jeu->nbemoji[i] = 0;
    }

    for (i = 0; i < total; i++) {
        int r = rand() % NB_TYPES_BONBONS;
        jeu->plateau[i] = jeu->emojis[r];
    }
}

int verifierVictoire(JeuState *jeu)
{
    int i;

    if (jeu == NULL) return 0;

    for (i = 0; i < NB_TYPES_BONBONS; i++) {
        if (jeu->nbemoji[i] < OBJECTIF_PAR_FRUIT) {
            return 0;
        }
    }
    return 1;
}

void libererPartie(JeuState *jeu)
{
    if (jeu == NULL) return;

    if (jeu->plateau != NULL) {
        free(jeu->plateau);
        jeu->plateau = NULL;
    }
}

int gererPermutation(JeuState *jeu)
{
    int dx, dy;
    int ok;

    if (jeu == NULL) return -2;

    dx = valeur_absolue(jeu->curseur_x - jeu->selection_x);
    dy = valeur_absolue(jeu->curseur_y - jeu->selection_y);

    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;
        return -1;
    }

    ok = permutationValide(
        jeu->plateau, jeu->lignes, jeu->colonnes,
        jeu->selection_x, jeu->selection_y,
        jeu->curseur_x, jeu->curseur_y
    );

    if (!ok) {
        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;
        return -2;
    }

    permuterBonbons(
        jeu->plateau, jeu->colonnes,
        jeu->selection_x, jeu->selection_y,
        jeu->curseur_x, jeu->curseur_y
    );

    jeu->nbcoups--;

    traiterAlignementsCascade(jeu);

    jeu->mode_selection = 0;
    jeu->selection_x = -1;
    jeu->selection_y = -1;

    return 0;
}

void boucleJeu(JeuState *jeu)
{
    if (jeu == NULL) return;

    while (jeu->continuer) {

        if (jeu->nbcoups <= 0) {
            break;
        }

        if (verifierVictoire(jeu)) {
            jeu->victoire = 1;
            break;
        }

        afficherJeu(jeu);

        {
            char c = lireCommandeLigne();

            if (c == '\n') {
                if (jeu->mode_selection) {
                    int r = gererPermutation(jeu);
                    if (r == -1) afficherErreur("Les bonbons doivent etre adjacents !");
                    if (r == -2) afficherErreur("Permutation invalide (aucun alignement) !");
                }
                continue;
            }

            appliquerCommande(jeu, c);
        }
    }

    jeu->victoire = verifierVictoire(jeu);
}


/* ===================== NIVEAU 2 ===================== */
/* ===================== NIVEAU 2 ===================== */
/* Même règles de déplacement, mais :
   - combinaisons avancées (niveau 2)
   - bonbon spécial "V" : lorsqu'il est échangé, il supprime toute la colonne
*/

static int estV(char *c)
{
    if (c == NULL) return 0;
    if (c[0] == 'V' && c[1] == '\0') return 1;
    return 0;
}

int gererPermutationNiveau2(JeuState *jeu)
{
    int dx, dy;
    int x1, y1, x2, y2;
    int idx1, idx2;
    char *a, *b;

    if (jeu == NULL) return -2;

    dx = valeur_absolue(jeu->curseur_x - jeu->selection_x);
    dy = valeur_absolue(jeu->curseur_y - jeu->selection_y);

    /* adjacent ? */
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;
        return -1;
    }

    x1 = jeu->selection_x;
    y1 = jeu->selection_y;
    x2 = jeu->curseur_x;
    y2 = jeu->curseur_y;

    idx1 = x1 * jeu->colonnes + y1;
    idx2 = x2 * jeu->colonnes + y2;

    a = jeu->plateau[idx1];
    b = jeu->plateau[idx2];

    /* CAS 1 : un spécial est impliqué -> effet colonne */
    if (estV(a) || estV(b)) {

        permuterBonbons(jeu->plateau, jeu->colonnes, x1, y1, x2, y2);
        jeu->nbcoups--;

        /* après permutation : on cherche où est le V */
        if (estV(jeu->plateau[idx1])) {
            effetSpecialColonne(jeu, x1, y1);
        } else if (estV(jeu->plateau[idx2])) {
            effetSpecialColonne(jeu, x2, y2);
        }

        traiterAlignementsCascadeNiveau2(jeu);

        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;

        return 0;
    }

    /* CAS 2 : permutation normale -> doit être valide niveau 2 */
    if (!permutationValideNiveau2(jeu->plateau, jeu->lignes, jeu->colonnes, x1, y1, x2, y2)) {
        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;
        return -2;
    }

    permuterBonbons(jeu->plateau, jeu->colonnes, x1, y1, x2, y2);
    jeu->nbcoups--;

    traiterAlignementsCascadeNiveau2(jeu);

    jeu->mode_selection = 0;
    jeu->selection_x = -1;
    jeu->selection_y = -1;

    return 0;
}

void boucleJeuNiveau2(JeuState *jeu)
{
    if (jeu == NULL) return;

    while (jeu->continuer) {

        if (jeu->nbcoups <= 0) break;

        if (verifierVictoire(jeu)) {
            jeu->victoire = 1;
            break;
        }

        afficherJeu(jeu);

        {
            char c = lireCommandeLigne();

            if (c == '\n') {
                if (jeu->mode_selection) {
                    int r = gererPermutationNiveau2(jeu);
                    if (r == -1) afficherErreur("Les bonbons doivent etre adjacents !");
                    if (r == -2) afficherErreur("Permutation invalide (aucune combinaison) !");
                }
                continue;
            }

            appliquerCommande(jeu, c);
        }
    }

    jeu->victoire = verifierVictoire(jeu);
}
