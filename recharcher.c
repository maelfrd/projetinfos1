#include <stdlib.h>

#define NB_LIGNES 9
#define NB_COLONNES 9

int genererBonbon() {
    return (rand() % 5) + 1;      // 5 types de bonbons
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
