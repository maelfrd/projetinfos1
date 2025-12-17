#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"

void initialiserPartie(JeuState *jeu, int lignes, int colonnes) {
    // Initialisation de la structure
    jeu->lignes = 10;
    jeu->colonnes = 10;
    jeu->nbcoups = 50;
    jeu->curseur_x = 0;
    jeu->curseur_y = 0;
    jeu->mode_selection = 0;
    jeu->selection_x = -1;
    jeu->selection_y = -1;
    jeu->continuer = 1;
    jeu->victoire = 0;
    
    // Initialisation des emojis
    jeu->emojis[0] = "🥭";
    jeu->emojis[1] = "🍋";
    jeu->emojis[2] = "🍏";
    jeu->emojis[3] = "🍇";
    jeu->emojis[4] = "🍅";
    
    // Initialisation des compteurs
    for(int i = 0; i < NB_TYPES_BONBONS; i++) {
        jeu->nbemoji[i] = 0;
    }
    
    // Allocation du plateau
    size_t total_cases = (size_t)lignes * (size_t)colonnes;
    jeu->plateau = malloc(total_cases * sizeof(char*));
    
    // Génération aléatoire du plateau
    srand((unsigned int)time(NULL));
    for(int x = 0; x < lignes; x++) {
        for(int y = 0; y < colonnes; y++) {
            int r = rand() % NB_TYPES_BONBONS;
            jeu->plateau[x * colonnes + y] = jeu->emojis[r];
        }
    }
}

int verifierVictoire(JeuState *jeu) {
    for(int i = 0; i < NB_TYPES_BONBONS; i++) {
        if(jeu->nbemoji[i] < OBJECTIF_PAR_FRUIT) {
            return 0;
        }
    }
    return 1;
}

void libererPartie(JeuState *jeu) {
    free(jeu->plateau);
}

void gererPermutation(JeuState *jeu) {
    if(!jeu->mode_selection) {
        // Première sélection
        jeu->mode_selection = 1;
        jeu->selection_x = jeu->curseur_x;
        jeu->selection_y = jeu->curseur_y;
    } else {
        // Deuxième sélection
        int dx = abs(jeu->curseur_x - jeu->selection_x);
        int dy = abs(jeu->curseur_y - jeu->selection_y);
        
        // Vérifie l'adjacence
        if((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            // Vérifie la validité
            if(permutationValide(jeu->plateau, jeu->lignes, jeu->colonnes, 
                                jeu->selection_x, jeu->selection_y, 
                                jeu->curseur_x, jeu->curseur_y)) {
                
                // Effectue la permutation
                permuterBonbons(jeu->plateau, jeu->colonnes, 
                               jeu->selection_x, jeu->selection_y,
                               jeu->curseur_x, jeu->curseur_y);
                jeu->nbcoups--;
                
                // Traite les alignements
                traiterAlignementsCascade(jeu);
                
            } else {
                afficherErreur("Permutation invalide ! Aucun alignement créé.");
            }
        } else {
            afficherErreur("Les bonbons doivent être adjacents (pas en diagonale) !");
        }
        
        // Réinitialise la sélection
        jeu->mode_selection = 0;
        jeu->selection_x = -1;
        jeu->selection_y = -1;
    }
}


 void boucleJeu(JeuState *jeu) {

    int attente_entree = 0;
    char buf[32];

    while (jeu->continuer) {

        afficherJeu(jeu);

        printf("Commande (zqsd/wasd, p puis Entrée, Entrée seule=valider, x=quitter) : ");
        fflush(stdout);

        if (!fgets(buf, sizeof(buf), stdin)) {
            // EOF => on quitte proprement
            jeu->continuer = 0;
            break;
        }

        char c = buf[0]; // 1er caractère de la ligne

        // Quitter
        if (c == 'x' || c == 'X') {
            jeu->continuer = 0;
            continue;
        }

        // Déplacements (tour par tour)
        if (c == 'z' || c == 'Z' || c == 'w' || c == 'W') {
            if (jeu->curseur_y > 0) jeu->curseur_y--;
            attente_entree = 0;
            continue;
        }
        if (c == 's' || c == 'S') {
            if (jeu->curseur_y < jeu->lignes - 1) jeu->curseur_y++;
            attente_entree = 0;
            continue;
        }
        if (c == 'q' || c == 'Q' || c == 'a' || c == 'A') {
            if (jeu->curseur_x > 0) jeu->curseur_x--;
            attente_entree = 0;
            continue;
        }
        if (c == 'd' || c == 'D') {
            if (jeu->curseur_x < jeu->colonnes - 1) jeu->curseur_x++;
            attente_entree = 0;
            continue;
        }

        // Armement sélection : P + Entrée
        if (!jeu->mode_selection) {
            if (c == 'p' || c == 'P') {
                attente_entree = 1;      // on attend Entrée seule
                continue;
            }
            // Entrée seule => valide la 1ère sélection si on a armé avec P
            if ((c == '\n' || c == '\r') && attente_entree) {
                gererPermutation(jeu);   // 1ère sélection (prend la case du curseur)
                attente_entree = 0;
                continue;
            }
        } else {
            // Déjà sélectionné : Entrée seule => tente la permutation avec la case du curseur
            if (c == '\n' || c == '\r') {
                gererPermutation(jeu);   // 2e sélection + swap si adjacent
                continue;
            }
        }

        // Toute autre commande : on ignore et on désarme
        attente_entree = 0;
    }
}

