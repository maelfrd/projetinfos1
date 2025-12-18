#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"

/*
  Valeur absolue (version simple).
  Exemple : iabs(-3) = 3 ; iabs(5) = 5
*/
static int valeur_absolue(int v)
{
    if (v < 0) {
        return -v;
    }
    return v;
}

/*
  Renvoie 1 si le caractère est un "blanc" (espace / tab / retour ligne), sinon 0.
  On s'en sert pour ignorer les espaces quand l'utilisateur tape une commande.
*/
static int estBlanc(char c)
{
    if (c == ' ')  return 1;
    if (c == '\t') return 1;
    if (c == '\n') return 1;
    if (c == '\r') return 1;
    if (c == '\v') return 1;
    if (c == '\f') return 1;
    return 0;
}

/*
  Lecture simple d'une commande :
  - lit une ligne (fgets)
  - ignore les blancs
  - renvoie le premier caractère utile
  - si l'utilisateur appuie juste sur Entrée, renvoie '\n'
  IMPORTANT : on n'accepte PAS les majuscules ; seules les minuscules sont attendues.
*/
static char lireCommandeLigne(void)
{
    char buf[64];
    int i;

    if (fgets(buf, (int)sizeof(buf), stdin) == 0) {
        return '\0'; /* EOF ou erreur */
    }

    for (i = 0; buf[i] != '\0'; i++) {

        /* Entrée = valider permutation (si mode sélection) */
        if (buf[i] == '\n') {
            return '\n';
        }

        /* On ignore les espaces et séparateurs */
        if (estBlanc(buf[i])) {
            continue;
        }

        /* On renvoie le premier caractère non-blanc tel quel (sans conversion) */
        return buf[i];
    }

    /* Ligne vide -> on considère "Entrée" */
    return '\n';
}

void initialiserPartie(JeuState *jeu, int lignes, int colonnes)
{
    size_t total_cases;
    int i, x, y;

    if (jeu == 0) return;

    if (lignes <= 0) lignes = 10;
    if (colonnes <= 0) colonnes = 10;

    (*jeu).lignes = lignes;
    (*jeu).colonnes = colonnes;
    (*jeu).nbcoups = 5;

    (*jeu).curseur_x = 0;
    (*jeu).curseur_y = 0;

    (*jeu).mode_selection = 0;
    (*jeu).selection_x = -1;
    (*jeu).selection_y = -1;

    (*jeu).continuer = 1;
    (*jeu).victoire = 0;

    /* Emojis / bonbons */
    (*jeu).emojis[0] = "🥭";
    (*jeu).emojis[1] = "🍋";
    (*jeu).emojis[2] = "🍏";
    (*jeu).emojis[3] = "🍇";
    (*jeu).emojis[4] = "🍅";

    for (i = 0; i < NB_TYPES_BONBONS; i++) {
        (*jeu).nbemoji[i] = 0;
    }

    total_cases = (size_t)lignes * (size_t)colonnes;
    (*jeu).plateau = (char**)malloc(total_cases * sizeof(char*));
    if ((*jeu).plateau == 0) {
        (*jeu).continuer = 0;
        return;
    }

    srand((unsigned int)time(0));
    for (x = 0; x < lignes; x++) {
        for (y = 0; y < colonnes; y++) {
            int r = rand() % NB_TYPES_BONBONS;
            (*jeu).plateau[x * colonnes + y] = (*jeu).emojis[r];
        }
    }
}

int verifierVictoire(JeuState *jeu)
{
    int i;
    if (jeu == 0) return 0;

    for (i = 0; i < NB_TYPES_BONBONS; i++) {
        if ((*jeu).nbemoji[i] < OBJECTIF_PAR_FRUIT) {
            return 0;
        }
    }
    return 1;
}

void libererPartie(JeuState *jeu)
{
    if (jeu == 0) return;
    free((*jeu).plateau);
    (*jeu).plateau = 0;
}

/*
  Tente la permutation entre la case sélectionnée et la case du curseur.
  Retour :
    0  : permutation exécutée
   -1  : pas adjacent
   -2  : permutation invalide (pas d’alignement)
*/
int gererPermutation(JeuState *jeu)
{
    int dx, dy;
    int ok;

    if (jeu == 0) return -2;

    /* 1) Vérifier adjacency (une case de distance, horizontal OU vertical) */
    dx = valeur_absolue((*jeu).curseur_x - (*jeu).selection_x);
    dy = valeur_absolue((*jeu).curseur_y - (*jeu).selection_y);

    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        /* Pas adjacent */
        (*jeu).mode_selection = 0;
        (*jeu).selection_x = -1;
        (*jeu).selection_y = -1;
        return -1;
    }

    /* 2) Vérifier que la permutation crée un alignement */
    ok = permutationValide(
        (*jeu).plateau, (*jeu).lignes, (*jeu).colonnes,
        (*jeu).selection_x, (*jeu).selection_y,
        (*jeu).curseur_x, (*jeu).curseur_y
    );

    if (!ok) {
        (*jeu).mode_selection = 0;
        (*jeu).selection_x = -1;
        (*jeu).selection_y = -1;
        return -2;
    }

    /* 3) Exécuter la permutation + 1 coup consommé + cascades */
    permuterBonbons(
        (*jeu).plateau, (*jeu).colonnes,
        (*jeu).selection_x, (*jeu).selection_y,
        (*jeu).curseur_x, (*jeu).curseur_y
    );
    (*jeu).nbcoups--;

    traiterAlignementsCascade(jeu);

    /* 4) Fin : on sort du mode sélection */
    (*jeu).mode_selection = 0;
    (*jeu).selection_x = -1;
    (*jeu).selection_y = -1;

    return 0;
}

void boucleJeu(JeuState *jeu)
{
    if (jeu == 0) return;

    while ((*jeu).continuer) {

        if ((*jeu).nbcoups <= 0) {
            break;
        }

        (*jeu).victoire = verifierVictoire(jeu);
        if ((*jeu).victoire) {
            break;
        }

        afficherJeu(jeu);

        printf("Commande (z=haut, s=bas, q=gauche, d=droite, p=selection, Entree=valider, x=quitter) : ");
        fflush(stdout);

        /* Lecture commande */
        {
            char c = lireCommandeLigne();

            if (c == '\0') {
                (*jeu).continuer = 0;
                break;
            }

            /* Entrée : valider permutation si une sélection est active */
            if (c == '\n') {
                if ((*jeu).mode_selection) {
                    int r = gererPermutation(jeu);
                    if (r == -1) afficherErreur("Les bonbons doivent etre adjacents !");
                    if (r == -2) afficherErreur("Permutation invalide (aucun alignement) !");
                }
                continue;
            }

            /* Sinon : déplacement / sélection / quitter (minuscules seulement) */
            appliquerCommande(jeu, c);
        }
    }

    (*jeu).victoire = verifierVictoire(jeu);
}
