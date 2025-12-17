#include <stdio.h>
#include <ctype.h>

#include "saisie.h"
#include "gestion_partie.h" // gererPermutation()

static char lireTouche(void) {
    char buf[32];

    if (!fgets(buf, sizeof(buf), stdin)) {
        return '\0'; // EOF
    }

    // Cherche le 1er caractère non-blanc
    for (int i = 0; buf[i] != '\0'; i++) {
        if (!isspace((unsigned char)buf[i])) {
            return (char)tolower((unsigned char)buf[i]);
        }
    }

    return '\0'; // ligne vide
}

void gererSaisie(JeuState *jeu) {
    printf("Commande (z=haut, s=bas, q=gauche, d=droite, p=select/permuter, x=quitter) : ");
    fflush(stdout);

    char c = lireTouche();

    // EOF => quitter proprement
    if (c == '\0') {
        jeu->continuer = 0;
        return;
    }

    switch (c) {
        case 'x':
            jeu->continuer = 0;
            return;

        // IMPORTANT : axes cohérents avec l’affichage :
        // curseur_x = ligne (haut/bas), curseur_y = colonne (gauche/droite)
        case 'z':
            if (jeu->curseur_x > 0) jeu->curseur_x--;
            return;

        case 's':
            if (jeu->curseur_x < jeu->lignes - 1) jeu->curseur_x++;
            return;

        case 'q':
            if (jeu->curseur_y > 0) jeu->curseur_y--;
            return;

        case 'd':
            if (jeu->curseur_y < jeu->colonnes - 1) jeu->curseur_y++;
            return;

        case 'p':
            // 1er P : sélection (mode_selection=1)
            // 2e P : tentative de permutation + reset sélection
            gererPermutation(jeu);
            return;

        default:
            // ignore
            return;
    }
}
