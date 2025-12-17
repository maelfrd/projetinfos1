#include "saisie.h"

void appliquerCommande(JeuState *jeu, char commande) {
    if (jeu == 0) return;

    if (commande == 'x' || commande == 'X') {
        (*jeu).continuer = 0;
        return;
    }

    if (commande == 'z' || commande == 'Z') {
        if ((*jeu).curseur_x > 0) (*jeu).curseur_x--;
        return;
    }
    if (commande == 's' || commande == 'S') {
        if ((*jeu).curseur_x < (*jeu).lignes - 1) (*jeu).curseur_x++;
        return;
    }
    if (commande == 'q' || commande == 'Q') {
        if ((*jeu).curseur_y > 0) (*jeu).curseur_y--;
        return;
    }
    if (commande == 'd' || commande == 'D') {
        if ((*jeu).curseur_y < (*jeu).colonnes - 1) (*jeu).curseur_y++;
        return;
    }

    if (commande == 'p' || commande == 'P') {
        if (!(*jeu).mode_selection) {
            (*jeu).mode_selection = 1;
            (*jeu).selection_x = (*jeu).curseur_x;
            (*jeu).selection_y = (*jeu).curseur_y;
        } else {
            (*jeu).mode_selection = 0;
            (*jeu).selection_x = -1;
            (*jeu).selection_y = -1;
        }
        return;
    }
}
