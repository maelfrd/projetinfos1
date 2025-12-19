/* affichage.c - Fonctions d'affichage */                   /* En-tete du fichier */
#include <stdio.h>                                          /* Bibliotheque entrees/sorties standard */
#include <string.h>                                         /* Bibliotheque manipulation chaines */
#include "affichage.h"                                      /* Header des fonctions d'affichage */

/* ========== FONCTIONS DE BASE ========== */               /* Section fonctions de base */

void nettoyer_ecran(void)                                   /* Efface l'ecran du terminal */
{                                                           /* Debut de la fonction */
    int i;                                                  /* Compteur de boucle */
    for (i = 0; i < 50; i++)                                /* Repete 50 fois */
        printf("\n");                                       /* Affiche une ligne vide */
}                                                           /* Fin de nettoyer_ecran */

void pause_entree(void)                                     /* Attend que l'utilisateur appuie Entree */
{                                                           /* Debut de la fonction */
    int c;                                                  /* Variable pour stocker caractere lu */
    printf("\nAppuie sur Entree...");                       /* Affiche message d'attente */
    while ((c = getchar()) != '\n' && c != EOF);            /* Vide le buffer jusqu'a Entree ou EOF */
}                                                           /* Fin de pause_entree */

/* ========== AFFICHAGE DU JEU ========== */                /* Section affichage jeu */

void afficher_plateau(Jeu *jeu)                             /* Affiche la grille de jeu */
{                                                           /* Debut de la fonction */
    int x, y, idx;                                          /* Coordonnees et index dans tableau */
    char *c;                                                /* Pointeur vers emoji a afficher */
                                                            /* Ligne vide pour lisibilite */
    /* Bordure haute */                                     /* Commentaire de sous-section */
    for (y = 0; y < jeu->colonnes; y++)                     /* Pour chaque colonne */
        printf("----");                                     /* Affiche segment de bordure */
    printf("-\n");                                          /* Termine la bordure haute */
                                                            /* Ligne vide pour lisibilite */
    /* Lignes du plateau */                                 /* Commentaire de sous-section */
    for (x = 0; x < jeu->lignes; x++) {                     /* Pour chaque ligne du plateau */
        printf("| ");                                       /* Affiche bordure gauche */
        for (y = 0; y < jeu->colonnes; y++) {               /* Pour chaque colonne */
            idx = x * jeu->colonnes + y;                    /* Calcule index dans tableau 1D */
            c = jeu->plateau[idx];                          /* Recupere emoji a cette position */
            if (c == NULL)                                  /* Si case vide */
                c = " ";                                    /* Affiche espace */
                                                            /* Ligne vide pour lisibilite */
            if (jeu->selection && x == jeu->select_x && y == jeu->select_y)  /* Si case selectionnee */
                printf("[%s]", c);                          /* Affiche avec crochets */
            else if (x == jeu->curseur_x && y == jeu->curseur_y)  /* Si curseur sur cette case */
                printf("<%s>", c);                          /* Affiche avec chevrons */
            else                                            /* Sinon case normale */
                printf(" %s ", c);                          /* Affiche avec espaces */
        }                                                   /* Fin boucle colonnes */
        printf("|\n");                                      /* Affiche bordure droite et newline */
    }                                                       /* Fin boucle lignes */
                                                            /* Ligne vide pour lisibilite */
    /* Bordure basse */                                     /* Commentaire de sous-section */
    for (y = 0; y < jeu->colonnes; y++)                     /* Pour chaque colonne */
        printf("----");                                     /* Affiche segment de bordure */
    printf("-\n");                                          /* Termine la bordure basse */
}                                                           /* Fin de afficher_plateau */

void afficher_infos(Jeu *jeu)                               /* Affiche informations de jeu */
{                                                           /* Debut de la fonction */
    int i;                                                  /* Compteur de boucle */
    const char *noms[5] = {"Pomme", "Citron", "Raisin", "Orange", "coco"};  /* Noms des fruits */
                                                            /* Ligne vide pour lisibilite */
    /* Vies */                                              /* Commentaire de sous-section */
    printf("\nVies: ");                                     /* Affiche label vies */
    for (i = 0; i < jeu->vies; i++)                         /* Pour chaque vie restante */
        printf("❤️ ");                                      /* Affiche emoji coeur */
    printf("(%d/%d)\n", jeu->vies, VIES_MAX);               /* Affiche compteur vies */
                                                            /* Ligne vide pour lisibilite */
    /* Temps */                                             /* Commentaire de sous-section */
    if (jeu->temps_limite > 0)                              /* Si limite de temps active */
        printf("Temps: %d s\n", jeu->temps_restant);        /* Affiche temps restant */
                                                            /* Ligne vide pour lisibilite */
    /* Coups et objectif */                                 /* Commentaire de sous-section */
    printf("Coups: %d\n", jeu->coups);                      /* Affiche coups restants */
    printf("Objectif: %d de chaque fruit\n", jeu->objectif);  /* Affiche objectif */
                                                            /* Ligne vide pour lisibilite */
    /* Arbres niveau 3 */                                   /* Commentaire de sous-section */
    if (jeu->objectif_arbres > 0)                           /* Si objectif arbres actif (niveau 3) */
        printf("Arbres 🎄: %d/%d\n", jeu->arbres_utilises, jeu->objectif_arbres);  /* Affiche compteur arbres */
                                                            /* Ligne vide pour lisibilite */
    /* Score par fruit */                                   /* Commentaire de sous-section */
    printf("\nRecolte:\n");                                 /* Affiche titre section recolte */
    for (i = 0; i < NB_FRUITS; i++) {                       /* Pour chaque type de fruit */
        printf("  %s %-7s: %2d/%d", jeu->fruits[i], noms[i], jeu->score[i], jeu->objectif);  /* Affiche score fruit */
        if (jeu->score[i] >= jeu->objectif)                 /* Si objectif atteint */
            printf(" [OK]");                                /* Affiche marqueur OK */
        printf("\n");                                       /* Nouvelle ligne */
    }                                                       /* Fin boucle fruits */
}                                                           /* Fin de afficher_infos */

void afficher_controles(Jeu *jeu)                           /* Affiche les controles disponibles */
{                                                           /* Debut de la fonction */
    printf("\nCurseur: [%d,%d]\n", jeu->curseur_x, jeu->curseur_y);  /* Affiche position curseur */
                                                            /* Ligne vide pour lisibilite */
    if (jeu->selection) {                                   /* Si une selection est active */
        printf("Selection: [%d,%d] - ACTIVE\n", jeu->select_x, jeu->select_y);  /* Affiche position selectionnee */
        printf("Deplace puis P pour echanger, ou P pour annuler\n");  /* Instructions echange */
    } else {                                                /* Si pas de selection */
        printf("Z/Q/S/D: deplacer | P: selectionner | X: quitter\n");  /* Instructions normales */
    }                                                       /* Fin du if selection */
}                                                           /* Fin de afficher_controles */

void afficher_jeu(Jeu *jeu)                                 /* Affiche l'ecran de jeu complet */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("              CANDY CRUSH \n");                 /* Affiche titre */
    afficher_plateau(jeu);                                  /* Affiche la grille */
    afficher_infos(jeu);                                    /* Affiche les informations */
    afficher_controles(jeu);                                /* Affiche les controles */
}                                                           /* Fin de afficher_jeu */

void afficher_fin(Jeu *jeu)                                 /* Affiche l'ecran de fin de niveau */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("\n");                                           /* Ligne vide */
    if (jeu->victoire)                                      /* Si le joueur a gagne */
        printf("            VICTOIRE !\n");                 /* Affiche message victoire */
    else                                                    /* Si le joueur a perdu */
        printf("            PARTIE TERMINEE\n\n");          /* Affiche message fin */
    printf("Coups restants: %d\n", jeu->coups);             /* Affiche coups restants */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_fin */

/* ========== AFFICHAGE DES MENUS ========== */             /* Section affichage menus */

void afficher_titre(void)                                   /* Affiche le titre du jeu */
{                                                           /* Debut de la fonction */
    printf("              CANDY CRUSH \n\n");               /* Affiche titre centre */
}                                                           /* Fin de afficher_titre */

void afficher_menu_avec_sauvegarde(int selection, Sauvegarde *sauv)  /* Affiche menu avec sauvegarde */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    afficher_titre();                                       /* Affiche titre */
                                                            /* Ligne vide pour lisibilite */
    printf("%s Reprendre (%s - Niveau %d - %d vies)\n",     /* Option 1: Reprendre */
           selection == 1 ? ">" : " ",                      /* Fleche si selectionne */
           sauv->pseudo,                                    /* Pseudo sauvegarde */
           sauv->niveau_en_cours,                           /* Niveau sauvegarde */
           sauv->vies);                                     /* Vies sauvegardees */
    printf("%s Nouvelle partie\n", selection == 2 ? ">" : " ");  /* Option 2: Nouvelle partie */
    printf("%s Parametres\n", selection == 3 ? ">" : " ");  /* Option 3: Parametres */
    printf("%s Quitter\n\n", selection == 4 ? ">" : " ");   /* Option 4: Quitter */
                                                            /* Ligne vide pour lisibilite */
    printf("Z/S: naviguer | Entree: valider\n");            /* Instructions navigation */
}                                                           /* Fin de afficher_menu_avec_sauvegarde */

void afficher_menu_sans_sauvegarde(int selection)           /* Affiche menu sans sauvegarde */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    afficher_titre();                                       /* Affiche titre */
                                                            /* Ligne vide pour lisibilite */
    printf("%s Nouvelle partie\n", selection == 1 ? ">" : " ");  /* Option 1: Nouvelle partie */
    printf("%s Parametres\n", selection == 2 ? ">" : " ");  /* Option 2: Parametres */
    printf("%s Quitter\n\n", selection == 3 ? ">" : " ");   /* Option 3: Quitter */
                                                            /* Ligne vide pour lisibilite */
    printf("Z/S: naviguer | Entree: valider\n");            /* Instructions navigation */
}                                                           /* Fin de afficher_menu_sans_sauvegarde */

void afficher_parametres(void)                              /* Affiche l'ecran des parametres */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("Parametres: rien pour l'instant\n");            /* Message placeholder */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_parametres */

/* ========== MESSAGES DE PROGRESSION ========== */         /* Section messages progression */

void afficher_niveau_debloque(const char *pseudo, int niveau)  /* Affiche message niveau debloque */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf(" \n");                                          /* Ligne vide */
    printf("  BRAVO %s !\n", pseudo);                       /* Felicitations avec pseudo */
    printf("  Niveau %d debloque !\n", niveau);             /* Annonce nouveau niveau */
    printf("\n");                                           /* Ligne vide */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_niveau_debloque */

void afficher_jeu_termine(const char *pseudo)               /* Affiche message jeu termine */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("\n");                                           /* Ligne vide */
    printf("  FELICITATIONS %s !\n", pseudo);               /* Felicitations avec pseudo */
    printf("  Tu as termine le jeu !\n");                   /* Message victoire finale */
    printf("\n");                                           /* Ligne vide */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_jeu_termine */

void afficher_game_over(const char *pseudo)                 /* Affiche message game over */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("\n");                                           /* Ligne vide */
    printf("  GAME OVER %s !\n", pseudo);                   /* Game over avec pseudo */
    printf("  Tu n'as plus de vies.\n");                    /* Explication */
    printf("  Ta partie est supprimee.\n");                 /* Information suppression */
    printf("\n");                                           /* Ligne vide */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_game_over */

void afficher_niveau_echoue(int vies_restantes)             /* Affiche message echec niveau */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("\n");                                           /* Ligne vide */
    printf("  Niveau echoue !\n");                          /* Message echec */
    printf("  Vies restantes: %d\n", vies_restantes);       /* Affiche vies restantes */
    printf("\n");                                           /* Ligne vide */
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_niveau_echoue */

void afficher_nouvelle_partie_warning(const char *ancien_pseudo)  /* Affiche warning ecrasement */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf("            NOUVELLE PARTIE \n\n");             /* Titre */
    printf("Attention: une partie existe deja pour %s.\n", ancien_pseudo);  /* Avertissement */
    printf("Creer une nouvelle partie va l'effacer!\n\n");  /* Consequence */
}                                                           /* Fin de afficher_nouvelle_partie_warning */

void afficher_nouvelle_partie(void)                         /* Affiche ecran nouvelle partie */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       /* Efface l'ecran */
    printf(".          NOUVELLE PARTIE \n\n");              /* Titre nouvelle partie */
}                                                           /* Fin de afficher_nouvelle_partie */

void afficher_au_revoir(void)                               /* Affiche message de sortie */
{                                                           /* Debut de la fonction */
    printf("Au revoir!\n");                                 /* Message de fin */
}                                                           /* Fin de afficher_au_revoir */

/* ========== SAISIE LIEE A L'AFFICHAGE ========== */       /* Section saisie affichage */

char lire_menu(void)                                        /* Lit une touche dans le menu */
{                                                           /* Debut de la fonction */
    char buf[64];                                           /* Buffer pour la saisie */
    if (fgets(buf, sizeof(buf), stdin) == NULL)             /* Lit ligne depuis clavier */
        return 'x';                                         /* Si erreur, retourne quitter */
    if (buf[0] == '\n')                                     /* Si juste Entree */
        return '\n';                                        /* Retourne newline */
    return buf[0];                                          /* Retourne premier caractere */
}                                                           /* Fin de lire_menu */

void lire_pseudo(char *pseudo)                              /* Demande et lit le pseudo */
{                                                           /* Debut de la fonction */
    int len;                                                /* Variable pour longueur chaine */
    printf("Entre ton pseudo: ");                           /* Affiche prompt */
    if (fgets(pseudo, 32, stdin) == NULL) {                 /* Lit pseudo (max 31 chars) */
        strcpy(pseudo, "Joueur");                           /* Si erreur, pseudo par defaut */
        return;                                             /* Sort de la fonction */
    }                                                       /* Fin du if erreur */
    len = strlen(pseudo);                                   /* Calcule longueur du pseudo */
    if (len > 0 && pseudo[len-1] == '\n')                   /* Si termine par newline */
        pseudo[len-1] = '\0';                               /* Remplace par fin de chaine */
    if (pseudo[0] == '\0')                                  /* Si pseudo vide */
        strcpy(pseudo, "Joueur");                           /* Utilise pseudo par defaut */
}                                                           /* Fin de lire_pseudo */
}
