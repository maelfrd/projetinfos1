#include <stdio.h>

#include "structures.h"
#include "gestion_partie.h"
#include "affichage.h"

int main(void) {
    JeuState jeu;

    int lignes = 10;
    int colonnes = 10;

    initialiserPartie(&jeu, lignes, colonnes);

    boucleJeu(&jeu);

    afficherFinPartie(&jeu);
    libererPartie(&jeu);

    return 0;
}
