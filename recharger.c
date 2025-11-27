#include <stdlib.h>

#define NB_LIGNES 15
#define NB_COLONNES 15

int genererBonbon() {
    return (rand() % 4) + 1;      // 4 types de bonbons
}

void remplirCasesVides(int grille[NB_LIGNES][NB_COLONNES]) {

    for (int col = 0; col < NB_COLONNES; col++) {

        int ligneEcriture = NB_LIGNES - 1;  

        for (int ligne = NB_LIGNES - 1; ligne >= 0; ligne--) {
            if (grille[ligne][col] != 0) {
                grille[ligneEcriture][col] = grille[ligne][col];
                ligneEcriture--;
            }
        }

        while (ligneEcriture >= 0) {
            grille[ligneEcriture][col] = genererBonbon();
            ligneEcriture--;
        }
    }
}
