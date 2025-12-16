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
    jeu->lignes = lignes;
    jeu->colonnes = colonnes;
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
    desactiver_mode_canonique();
    
    while(jeu->continuer && jeu->nbcoups > 0) {
        // Vérifie la victoire
        if(verifierVictoire(jeu)) {
            jeu->victoire = 1;
            jeu->continuer = 0;
            break;
        }
        
        // Efface l'écran
        printf("\033[2J\033[H");
        
        // Affichage
        afficherEntete();
        afficherPlateau(jeu);
        afficherInfos(jeu);
        afficherControles(jeu);
        
        // Gestion des entrées
        char touche = deplacerCurseur(&jeu->curseur_x, &jeu->curseur_y, 
                                      jeu->lignes, jeu->colonnes);
        
        if(touche == 'q' || touche == 'Q') {
            jeu->continuer = 0;
        }
        else if(touche == 'p' || touche == 'P') {
            gererPermutation(jeu);
        }
        
        usleep(50000);
    }
    
    reactiver_mode_canonique();
}
