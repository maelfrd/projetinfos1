#include "saisie.h"
#include <stdlib.h>


void appliquerCommande(JeuState *jeu, char commande)
{
    if (jeu == NULL) return;

    /* Quitter */
    if (commande == 'x') {
        jeu->continuer = 0;
        return;
    }

    /* Déplacements */
    if (commande == 'z') {
        if (jeu->curseur_x > 0) jeu->curseur_x--;
        return;
    }
    if (commande == 's') {
        if (jeu->curseur_x < jeu->lignes - 1) jeu->curseur_x++;
        return;
    }
    if (commande == 'q') {
        if (jeu->curseur_y > 0) jeu->curseur_y--;
        return;
    }
    if (commande == 'd') {
        if (jeu->curseur_y < jeu->colonnes - 1) jeu->curseur_y++;
        return;
    }

    /* Sélection / annulation */
    if (commande == 'p') {
        if (!jeu->mode_selection) {
            jeu->mode_selection = 1;
            jeu->selection_x = jeu->curseur_x;
            jeu->selection_y = jeu->curseur_y;
        } else {
            jeu->mode_selection = 0;
            jeu->selection_x = -1;
            jeu->selection_y = -1;
        }
        return;
    }
}
