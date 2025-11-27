#include <stdio.h>
#include <stdlib.h>

#define NB_LIGNES 20
#define NB_COLONNES 25

void faireTomber(int grille[NB_LIGNES][NB_COLONNES]) {

    for (int col = 0; col < NB_COLONNES; col++) {

        int ligneEcriture = NB_LIGNES - 1;

        for (int ligne = NB_LIGNES - 1; ligne >= 0; ligne--) {

            if (grille[ligne][col] != 0) {

                grille[ligneEcriture][col] = grille[ligne][col];

                if (ligneEcriture != ligne) {
                    grille[ligne][col] = 0;
                }

                ligneEcriture--;
            }
        }
    }
}
