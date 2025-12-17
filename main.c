#include <stdio.h>
#include "structures.h"
#include "gestion_partie.h"
#include "affichage.h"

int main(void) {
    JeuState jeu;
    int lignes, colonnes;
    
    // Affichage du titre
    afficherEntete();


    // Saisie des dimensions
    printf("Choix du nombre de lignes du plateau : ");
    scanf("%d", &lignes);
    printf("Choix du nombre de colonnes du plateau : ");
    scanf("%d", &colonnes);
    
    // Initialisation de la partie
    initialiserPartie(&jeu, lignes, colonnes);
    
    // Lancement de la boucle de jeu
    boucleJeu(&jeu);
    
    // Affichage de fin
    afficherFinPartie(&jeu);
    
    // Libération de la mémoire
    libererPartie(&jeu);
    
    return 0;
}
