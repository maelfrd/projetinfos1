#include <stdio.h>                                         
#include <string.h>                                       
#include "affichage.h"                                      

/*action sur le terminal */              

void nettoyer_ecran(void)                                   
{                                                    
    for (i = 0; i < 50; i++)                                /* Repete 50 fois pour 50 lignes */
        printf("\n");                                       
}                                                      

void pause_entree(void)                                     /* Attend que l'utilisateur appuie Entree */
{                                                         
    int c;                                                  
    printf("\nAppuie sur Entree...");                       /* Affiche message d'attente */
    while ((c = getchar()) != '\n' && c != EOF);            /* Vide le buffer jusqu'a Entree ou EOF */
}                                                           

/* affichage du jeu (plateau + fruits ) */                

void afficher_plateau(Jeu *jeu)                            
{                                                           
    int x, y, idx;                                          /* Coordonnees et index dans tableau */
    char *c;                                                
                                                          
    /* Bordure haute */                                    
    for (y = 0; y < jeu->colonnes; y++)                     
        printf("----");                                     /* Affiche segment de bordure */
    printf("-\n");                                        
                                                            
    /* Lignes du plateau */                                
    for (x = 0; x < jeu->lignes; x++) {                     /* Pour chaque ligne du plateau */
        printf("| ");                                       /* Affiche bordure gauche */
        for (y = 0; y < jeu->colonnes; y++) {              
            idx = x * jeu->colonnes + y;                    
            c = jeu->plateau[idx];                          
            if (c == NULL)                                  
                c = " ";                                    /* Affiche espace */
                                                           
            if (jeu->selection && x == jeu->select_x && y == jeu->select_y)  /* Si case selectionnee */
                printf("[%s]", c);                          /* Affiche avec crochets */
            else if (x == jeu->curseur_x && y == jeu->curseur_y)  /* Si curseur sur cette case */
                printf("<%s>", c);                          /* Affiche avec <> */
            else                                            /* Sinon case normale */
                printf(" %s ", c);                          /* Affiche avec espaces */
        }                                                   
        printf("|\n");                                     
    }                                                       
                                                            
    /* Bordure basse */                                   
    for (y = 0; y < jeu->colonnes; y++)                 
        printf("----");                                     
    printf("-\n");                                          
}                                                           

void afficher_infos(Jeu *jeu)                           
{                                                           
    int i;                                         
    const char *noms[5] = {"Pomme", "Citron", "Raisin", "Orange", "coco"};  /* Noms des fruits */
                                                           
    /* Vies */                                         
    printf("\nVies: ");                                     /* Affiche label vies */
    for (i = 0; i < jeu->vies; i++)                         /* Pour chaque vie restante */
        printf("❤️ ");                                      /* Affiche emoji coeur */
    printf("(%d/%d)\n", jeu->vies, VIES_MAX);            
                                                           
    /* Temps */                                            
    if (jeu->temps_limite > 0)                              /* Si limite de temps active */
        printf("Temps: %d s\n", jeu->temps_restant);        /* Affiche temps restant */
                                                            /* Ligne vide pour lisibilite */
    /* Coups et objectif */                                 
    printf("Coups: %d\n", jeu->coups);                      /* Affiche coups restants */
    printf("Objectif: %d de chaque fruit\n", jeu->objectif);  /* Affiche objectif */
                                                            
    /* Arbres niveau 3 */                                   /* Commentaire de sous-section */
    if (jeu->objectif_arbres > 0)                           /* Si objectif arbres actif (niveau 3) */
        printf("Arbres 🎄: %d/%d\n", jeu->arbres_utilises, jeu->objectif_arbres);  /* Affiche compteur arbres */
                                                            /* Ligne vide pour lisibilite */
    /* Score par fruit */                              
    printf("\nRecolte:\n");                                 /* Affiche titre section recolte */
    for (i = 0; i < NB_FRUITS; i++) {                       /* Pour chaque type de fruit */
        printf("  %s %-7s: %2d/%d", jeu->fruits[i], noms[i], jeu->score[i], jeu->objectif);  /* Affiche score fruit */
        if (jeu->score[i] >= jeu->objectif)                 /* Si objectif atteint */
            printf(" [OK]");                                /* Affiche marqueur OK */
        printf("\n");                                      
    }                                               
}                                                      

void afficher_controles(Jeu *jeu)                           /* Affiche les controles disponibles */
{                                                   
    printf("\nCurseur: [%d,%d]\n", jeu->curseur_x, jeu->curseur_y);  /* Affiche position curseur */
                                                            
    if (jeu->selection) {                                   /* Si une selection est active */
        printf("Selection: [%d,%d] - ACTIVE\n", jeu->select_x, jeu->select_y);  /* Affiche position selectionnee */
        printf("Deplace puis P pour echanger, ou P pour annuler\n");  /* Instructions pour effectuer un  echange */
    } else {                                                
        printf("Z/Q/S/D: deplacer | P: selectionner | X: quitter\n");  /* Instructions deplacement  */
    }                                                       /* Fin du if selection */
}                                                           /* Fin de afficher_controles */

void afficher_jeu(Jeu *jeu)                                 /* Affiche l'ecran de jeu complet */
{                                           
    nettoyer_ecran();                                  
    printf("              CANDY CRUSH \n");      
    afficher_plateau(jeu);                                  
    afficher_infos(jeu);                                  
    afficher_controles(jeu);                         
}                                                       

void afficher_fin(Jeu *jeu)                                 /* Affiche l'ecran de fin de niveau */
{
    nettoyer_ecran();                                      
    printf("\n");                                           
    if (jeu->victoire)                                      
        printf("            VICTOIRE !\n");                 /* Affiche message victoire */
    else                                                   
        printf("            PARTIE TERMINEE\n\n");          
    printf("Coups restants: %d\n", jeu->coups);             /* Affiche coups restants */
    pause_entree();                                        
}                                                           

/*        AFFICHAGE DES MENUS */             

void afficher_titre(void)                                  
{                                                           
    printf("              CANDY CRUSH \n\n");               
}                                                          

void afficher_menu_avec_sauvegarde(int selection, Sauvegarde *sauv)  /* Affiche menu avec sauvegarde */
{                                                        
    nettoyer_ecran();                                      
    afficher_titre();                             
                                                           
    printf("%s Reprendre (%s - Niveau %d - %d vies)\n",     /* Option 1: Reprendre */
           selection == 1 ? ">" : " ",                      /* Fleche si selectionne */
           sauv->pseudo,                                   
           sauv->niveau_en_cours,                 
           sauv->vies);                                    
    printf("%s Nouvelle partie\n", selection == 2 ? ">" : " ");  /* Option 2: Nouvelle partie */
    printf("%s Parametres\n", selection == 3 ? ">" : " ");  /* Option 3: Parametres */
    printf("%s Quitter\n\n", selection == 4 ? ">" : " ");   /* Option 4: Quitter */
                                                         
    printf("Z/S: naviguer | Entree: valider\n");            
}                                                          

void afficher_menu_sans_sauvegarde(int selection)           
{                                                           
    nettoyer_ecran();                               
    afficher_titre();                                       
                                                           
    printf("%s Nouvelle partie\n", selection == 1 ? ">" : " ");  /* Option 1: Nouvelle partie */
    printf("%s Parametres\n", selection == 2 ? ">" : " ");  /* Option 2: Parametres */
    printf("%s Quitter\n\n", selection == 3 ? ">" : " ");   /* Option 3: Quitter */
                                                           
    printf("Z/S: naviguer | Entree: valider\n");            /* Instructions  de deplacement */
}                                                           

void afficher_parametres(void)                              
{                                                     
    nettoyer_ecran();                                      
    printf("Parametres: rien pour l'instant\n");           
    pause_entree();                                     
}                                                         

/*           progression d'un niveau  */        

void afficher_niveau_debloque(const char *pseudo, int niveau)  
{                                                          
    nettoyer_ecran();                               
    printf(" \n");                                       
    printf("  BRAVO %s !\n", pseudo);                       /* Felicitations avec pseudo */
    printf("  Niveau %d debloque !\n", niveau);            
    printf("\n");                                           
    pause_entree();                                         
}                                                         

void afficher_jeu_termine(const char *pseudo)               /* Affiche message jeu termine */
{                                                           
    nettoyer_ecran();                                       
    printf("\n");                                          
    printf("  FELICITATIONS %s !\n", pseudo);               /* Felicitations avec pseudo */
    printf("  Tu as termine le jeu !\n");                   /* Message victoire finale */
    printf("\n");                                    
    pause_entree();                                        
}                                                          

void afficher_game_over(const char *pseudo)                 /* Affiche message game over */
{                                                          
    nettoyer_ecran();                          
    printf("\n");                                           
    printf("  GAME OVER %s !\n", pseudo);                   
    printf("  Tu n'as plus de vies.\n");                  
    printf("  Ta partie est supprimee.\n");                 
    printf("\n");                                        
    pause_entree();                                       
}                                                           

void afficher_niveau_echoue(int vies_restantes)             /* Affiche message echec niveau */
{                                                           /* Debut de la fonction */
    nettoyer_ecran();                                       
    printf("\n");                                         
    printf("  Niveau echoue !\n");                          /* Message echec */
    printf("  Vies restantes: %d\n", vies_restantes);       /* Affiche vies restantes */
    printf("\n");                                          
    pause_entree();                                         /* Attend appui Entree */
}                                                           /* Fin de afficher_niveau_echoue */

void afficher_nouvelle_partie_warning(const char *ancien_pseudo)  /* Affiche warning ecrasement */
{                                                           
    nettoyer_ecran();                                     
    printf("            NOUVELLE PARTIE \n\n");             /* Titre */
    printf("Attention: une partie existe deja pour %s.\n", ancien_pseudo);  /* Avertissement */
    printf("Creer une nouvelle partie va l'effacer!\n\n");  
}                                                           

void afficher_nouvelle_partie(void)                     
{                                                 
    nettoyer_ecran();                                       
    printf(".          NOUVELLE PARTIE \n\n");              /* Titre nouvelle partie */
}                                                           /* Fin de afficher_nouvelle_partie */

void afficher_au_revoir(void)                               /* Affiche message de sortie */
{                                                           
    printf("Au revoir!\n");                               
}                                                           

/* affichage saisie */      

char lire_menu(void)                                        /* Lit une touche dans le menu */
{                                                          
    char buf[64];                                           /* Buffer pour la saisie */
    if (fgets(buf, sizeof(buf), stdin) == NULL)             /* Lit ligne depuis clavier */
        return 'x';                                         /* Si erreur, retourne quitter */
    if (buf[0] == '\n')                                     /* Si juste Entree */
        return '\n';                                        
    return buf[0];                                          /* Retourne premier caractere */
}                                                          

void lire_pseudo(char *pseudo)                              /* Demande et lit le pseudo */
{                                                  
    int len;                                                /* Variable pour longueur chaine */
    printf("Entre ton pseudo: ");                           
    if (fgets(pseudo, 32, stdin) == NULL) {                 /* Lit pseudo (max 31 chars) */
        strcpy(pseudo, "Joueur");                           
        return;                                             
    }                                                      
    len = strlen(pseudo);                                   /* Calcule longueur du pseudo */
    if (len > 0 && pseudo[len-1] == '\n')                  
        pseudo[len-1] = '\0';                               
    if (pseudo[0] == '\0')                                  
        strcpy(pseudo, "Joueur");                        
}                                                          
}
