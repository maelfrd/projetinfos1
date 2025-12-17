#ifndef SAISIE_H
#define SAISIE_H

#include "structures.h"

/*
  Applique une commande au jeu, SANS I/O (lecture clavier faite dans boucleJeu).
  Commandes:
    z = haut, s = bas, q = gauche, d = droite
    p = sélectionner / annuler la sélection
    x = quitter
*/
void appliquerCommande(JeuState *jeu, char commande);

#endif
