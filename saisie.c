/* saisie.c - Gestion des entrees clavier */                /* En-tete du fichier */
#include <stdio.h>                                          /* Bibliotheque entrees/sorties standard */
#include "saisie.h"                                         /* Header des fonctions de saisie */

/* ====
====== LECTURE COMMANDE ========== */                /* Section lecture commande */

char lire_commande(void)                                    /* Lit une touche clavier */
{                                                           /* Debut de la fonction */
    char buf[64];                                           /* Buffer pour stocker la ligne saisie */
    int i;                                                  /* Indice pour parcourir le buffer */
                                                            /* Ligne vide pour lisibilite */
    if (fgets(buf, sizeof(buf), stdin) == NULL)             /* Lit une ligne depuis clavier */
        return 'x';                                         /* Si erreur ou EOF, retourne quitter */
                                                            /* Ligne vide pour lisibilite */
    if (buf[0] == '\n')                                     /* Si utilisateur appuie juste Entree */
        return '\n';                                        /* Retourne caractere nouvelle ligne */
                                                            /* Ligne vide pour lisibilite */
    for (i = 0; buf[i] != '\0'; i++)                        /* Parcourt chaque caractere du buffer */
        if (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')  /* Si pas espace/tab/newline */
            return buf[i];                                  /* Retourne premier caractere valide */
                                                            /* Ligne vide pour lisibilite */
    return '\n';                                            /* Si aucun caractere valide, retourne newline */
}                                                           /* Fin de lire_commande */

/* ========== COMMANDE MENU ========== */                   /* Section commande menu */

int appliquer_commande_menu(char cmd)                       /* Traite commande dans le menu */
{                                                           /* Debut de la fonction */
    if (cmd == 'z' || cmd == 'Z') {                         /* Si touche Z (haut) */
        return -1;                                          /* Retourne -1 pour monter */
    }                                                       /* Fin du if Z */
    else if (cmd == 's' || cmd == 'S') {                    /* Si touche S (bas) */
        return 1;                                           /* Retourne 1 pour descendre */
    }                                                       /* Fin du if S */
    else if (cmd == 'x' || cmd == 'X') {                    /* Si touche X (quitter) */
        return -99;                                         /* Retourne -99 pour quitter */
    }                                                       /* Fin du if X */
    else if (cmd == '\n') {                                 /* Si touche Entree */
        return 0;                                           /* Retourne 0 pour valider */
    }                                                       /* Fin du if Entree */
    return 99;                                              /* Commande non reconnue */
}                                                           /* Fin de appliquer_commande_menu */

/* ========== COMMANDE JEU ========== */                    /* Section commande jeu */

void appliquer_commande(Jeu *jeu, char cmd)                 /* Applique commande pendant le jeu */
{                                                           /* Debut de la fonction */
    if (cmd == 'x' || cmd == 'X') {                         /* Si touche X (quitter) */
        jeu->en_cours = 0;                                  /* Met fin a la partie */
        return;                                             /* Sort de la fonction */
    }                                                       /* Fin du if X */
                                                            /* Ligne vide pour lisibilite */
    if (cmd == 'z' || cmd == 'Z') {                         /* Si touche Z (haut) */
        if (jeu->curseur_x > 0)                             /* Si pas en haut du plateau */
            jeu->curseur_x--;                               /* Deplace curseur vers le haut */
    }                                                       /* Fin du if Z */
    else if (cmd == 's' || cmd == 'S') {                    /* Si touche S (bas) */
        if (jeu->curseur_x < jeu->lignes - 1)               /* Si pas en bas du plateau */
            jeu->curseur_x++;                               /* Deplace curseur vers le bas */
    }                                                       /* Fin du if S */
    else if (cmd == 'q' || cmd == 'Q') {                    /* Si touche Q (gauche) */
        if (jeu->curseur_y > 0)                             /* Si pas a gauche du plateau */
            jeu->curseur_y--;                               /* Deplace curseur vers la gauche */
    }                                                       /* Fin du if Q */
    else if (cmd == 'd' || cmd == 'D') {                    /* Si touche D (droite) */
        if (jeu->curseur_y < jeu->colonnes - 1)             /* Si pas a droite du plateau */
            jeu->curseur_y++;                               /* Deplace curseur vers la droite */
    }                                                       /* Fin du if D */
    else if (cmd == 'p' || cmd == 'P') {                    /* Si touche P (selection) */
        if (!jeu->selection) {                              /* Si aucune selection active */
            jeu->selection = 1;                             /* Active la selection */
            jeu->select_x = jeu->curseur_x;                 /* Sauvegarde position X selectionnee */
            jeu->select_y = jeu->curseur_y;                 /* Sauvegarde position Y selectionnee */
        }                                                   /* Fin du if pas de selection */
        else if (jeu->curseur_x == jeu->select_x &&         /* Si curseur sur case selectionnee */
                 jeu->curseur_y == jeu->select_y) {         /* (meme X et meme Y) */
            jeu->selection = 0;                             /* Desactive la selection */
        }                                                   /* Fin du if meme position */
    }                                                       /* Fin du if P */
}                                                           /* Fin de appliquer_commande */
