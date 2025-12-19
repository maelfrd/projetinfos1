/* saisie.c - Gestion des entrees clavier */
#include <stdio.h>
#include "saisie.h"

char lire_commande(void)
{
    char buf[64];
    int i;
    
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 'x';
    if (buf[0] == '\n') return '\n';
    
    /* Cherche le premier caractere non-espace */
    for (i = 0; buf[i] != '\0'; i++)
        if (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
            return buf[i];
    
    return '\n';
}

void appliquer_commande(Jeu *jeu, char cmd)
{
    /* Quitter */
    if (cmd == 'x' || cmd == 'X') {
        jeu->en_cours = 0;
        return;
    }
    
    /* Deplacements */
    if (cmd == 'z' || cmd == 'Z') {
        if (jeu->curseur_x > 0) jeu->curseur_x--;
    }
    else if (cmd == 's' || cmd == 'S') {
        if (jeu->curseur_x < jeu->lignes - 1) jeu->curseur_x++;
    }
    else if (cmd == 'q' || cmd == 'Q') {
        if (jeu->curseur_y > 0) jeu->curseur_y--;
    }
    else if (cmd == 'd' || cmd == 'D') {
        if (jeu->curseur_y < jeu->colonnes - 1) jeu->curseur_y++;
    }
    /* Selection */
    else if (cmd == 'p' || cmd == 'P') {
        if (!jeu->selection) {
            jeu->selection = 1;
            jeu->select_x = jeu->curseur_x;
            jeu->select_y = jeu->curseur_y;
        } else if (jeu->curseur_x == jeu->select_x && jeu->curseur_y == jeu->select_y) {
            jeu->selection = 0;
        }
    }
}
