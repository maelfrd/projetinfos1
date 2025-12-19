/* saisie.c - Gestion des entrees clavier */

#include <stdio.h>      // Pour fgets() et stdin
#include "saisie.h"     // Déclarations et structure Jeu

// Lit une commande clavier et retourne un caractère
char lire_commande(void)
{
    char buf[64];       // Buffer pour stocker la ligne saisie
    int i;              // Indice pour parcourir le buffer
    
    // Lecture d'une ligne depuis l'entrée standard (clavier)
    // Si erreur ou fin de fichier, on retourne 'x' (quitter)
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 'x';

    // Si l'utilisateur appuie juste sur Entrée
    if (buf[0] == '\n') return '\n';
    
    // Recherche du premier caractère non vide (ni espace, ni tab, ni retour ligne)
    for (i = 0; buf[i] != '\0'; i++)
        if (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')
            return buf[i];   // Retourne la commande trouvée
    
    // Si aucun caractère valide n'a été trouvé
    return '\n';
}

// Applique la commande au jeu
void appliquer_commande(Jeu *jeu, char cmd)
{
    // ----- Quitter le jeu -----
    if (cmd == 'x' || cmd == 'X') {
        jeu->en_cours = 0;   // Arrête la boucle principale du jeu
        return;             // Sort de la fonction
    }
    
    // ----- Déplacement vers le haut (Z) -----
    if (cmd == 'z' || cmd == 'Z') {
        if (jeu->curseur_x > 0)          // Vérifie qu'on ne sort pas de la grille
            jeu->curseur_x--;             // Déplace le curseur vers le haut
    }
    // ----- Déplacement vers le bas (S) -----
    else if (cmd == 's' || cmd == 'S') {
        if (jeu->curseur_x < jeu->lignes - 1)
            jeu->curseur_x++;             // Déplace le curseur vers le bas
    }
    // ----- Déplacement vers la gauche (Q) -----
    else if (cmd == 'q' || cmd == 'Q') {
        if (jeu->curseur_y > 0)
            jeu->curseur_y--;             // Déplace le curseur vers la gauche
    }
    // ----- Déplacement vers la droite (D) -----
    else if (cmd == 'd' || cmd == 'D') {
        if (jeu->curseur_y < jeu->colonnes - 1)
            jeu->curseur_y++;             // Déplace le curseur vers la droite
    }
    // ----- Sélection (P) -----
    else if (cmd == 'p' || cmd == 'P') {
        // Si aucune sélection n'est active
        if (!jeu->selection) {
            jeu->selection = 1;           // Active la sélection
            jeu->select_x = jeu->curseur_x; // Sauvegarde la position X
            jeu->select_y = jeu->curseur_y; // Sauvegarde la position Y
        }
        // Si on appuie sur P au même endroit, on annule la sélection
        else if (jeu->curseur_x == jeu->select_x &&
                 jeu->curseur_y == jeu->select_y) {
            jeu->selection = 0;           // Désactive la sélection
        }
    }
}
