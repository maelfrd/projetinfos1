           
#include <stdio.h>                                          
#include "saisie.h"                                        



char lire_commande(void)                                   
{                                                           
    char buf[64];                                           /* stocke la ligne saisie */
    int i;                                                  /* Indice pour parcourir le buffer */
                                                            
    if (fgets(buf, sizeof(buf), stdin) == NULL)             /* Lit une ligne depuis clavier */
        return 'x';                                         /* Si erreur ou EOF, retourne quitter */
                                                            
    if (buf[0] == '\n')                                     /* Si l'utilisateur appuie juste Entree */
        return '\n';                                        /* Retourne caractere nouvelle ligne */
                                                            
    for (i = 0; buf[i] != '\0'; i++)                        /* Parcourt chaque caractere du buffer */
        if (buf[i] != ' ' && buf[i] != '\t' && buf[i] != '\n')  /* Si pas espace/tab/newline */
            return buf[i];                                  /* Retourne premier caractere valide */
                                                           
    return '\n';                                            /* Si aucun caractere valide, retourne newline */
}                                                           

                 

int appliquer_commande_menu(char cmd)                      
{                                                          
    if (cmd == 'z' || cmd == 'Z') {                         
        return -1;                                          /* Retourne -1 pour monter */
    }                                                       
    else if (cmd == 's' || cmd == 'S') {                    
        return 1;                                           /* Retourne 1 pour descendre */
    }                                                      
    else if (cmd == 'x' || cmd == 'X') {                    
        return -99;                                         /* Retourne -99 pour quitter */
    }                                                       
    else if (cmd == '\n') {                                 
        return 0;                                           /* Retourne 0 pour valider */
    }                                                       
    return 99;                                              /* Commande non reconnue */
}                                                           

                

void appliquer_commande(Jeu *jeu, char cmd)                 
{                                                         
    if (cmd == 'x' || cmd == 'X') {                        
        jeu->en_cours = 0;                                  /* Met fin a la partie */
        return;                                             /* Sort de la fonction */
    }                                                      
                                                           
    if (cmd == 'z' || cmd == 'Z') {                        
        if (jeu->curseur_x > 0)                             /* Si pas en haut du plateau */
            jeu->curseur_x--;                               /* Deplace curseur vers le haut */
    }                                                       
    else if (cmd == 's' || cmd == 'S') {                   
        if (jeu->curseur_x < jeu->lignes - 1)               /* Si pas en bas du plateau */
            jeu->curseur_x++;                               /* Deplace curseur vers le bas */
    }                                                       
    else if (cmd == 'q' || cmd == 'Q') {                   
        if (jeu->curseur_y > 0)                             /* Si pas a gauche du plateau */
            jeu->curseur_y--;                               /* Deplace curseur vers la gauche */
    }                                                       
    else if (cmd == 'd' || cmd == 'D') {                   
        if (jeu->curseur_y < jeu->colonnes - 1)             /* Si pas a droite du plateau */
            jeu->curseur_y++;                               /* Deplace curseur vers la droite */
    }                                                       
    else if (cmd == 'p' || cmd == 'P') {                   
        if (!jeu->selection) {                              /* Si aucune selection active */
            jeu->selection = 1;                             /* Active la selection */
            jeu->select_x = jeu->curseur_x;                 /* Sauvegarde position X selectionnee */
            jeu->select_y = jeu->curseur_y;                 /* Sauvegarde position Y selectionnee */
        }                                                   
        else if (jeu->curseur_x == jeu->select_x &&         /* Si curseur sur case selectionnee */
                 jeu->curseur_y == jeu->select_y) {         /* (meme X et meme Y) */
            jeu->selection = 0;                             /* Desactive la selection */
        }                                                   /* Fin du if meme position */
    }                                                       
}                                                          
