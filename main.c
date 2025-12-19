/* main.c - Programme principal Candy Crush */
#include <stdio.h>
#include <string.h>
#include "structures.h"
#include "gestion_partie.h"
#include "affichage.h"

static char lire_menu(void)
{
    char buf[64];
    if (fgets(buf, sizeof(buf), stdin) == NULL) return 'x';
    if (buf[0] == '\n') return '\n';
    return buf[0];
}

static void lire_pseudo(char *pseudo)
{
    int len;
    printf("Entre ton pseudo: ");
    if (fgets(pseudo, 32, stdin) == NULL) {
        strcpy(pseudo, "Joueur");
        return;
    }
    len = strlen(pseudo);
    if (len > 0 && pseudo[len-1] == '\n')
        pseudo[len-1] = '\0';
    if (pseudo[0] == '\0')
        strcpy(pseudo, "Joueur");
}

static int menu_principal(Sauvegarde *sauv)
{
    char c;
    int sel = 1;
    int nb_options;
    
    if (sauv->existe)
        nb_options = 4;
    else
        nb_options = 3;
    
    while (1) {
        nettoyer_ecran();
        printf("========== CANDY CRUSH ==========\n\n");
        
        if (sauv->existe) {
            /* Avec sauvegarde: 1=Reprendre, 2=Nouvelle, 3=Param, 4=Quitter */
            printf("%s Reprendre (%s - Niveau %d - %d vies)\n", 
                   sel == 1 ? ">" : " ", sauv->pseudo, sauv->niveau_en_cours, sauv->vies);
            printf("%s Nouvelle partie\n", sel == 2 ? ">" : " ");
            printf("%s Parametres\n", sel == 3 ? ">" : " ");
            printf("%s Quitter\n\n", sel == 4 ? ">" : " ");
        } else {
            /* Sans sauvegarde: 1=Nouvelle, 2=Param, 3=Quitter */
            printf("%s Nouvelle partie\n", sel == 1 ? ">" : " ");
            printf("%s Parametres\n", sel == 2 ? ">" : " ");
            printf("%s Quitter\n\n", sel == 3 ? ">" : " ");
        }
        
        printf("Z/S: naviguer | Entree: valider\n");
        
        c = lire_menu();
        
        if (c == 'z' || c == 'Z') {
            if (sel > 1) sel--;
        }
        else if (c == 's' || c == 'S') {
            if (sel < nb_options) sel++;
        }
        else if (c == 'x' || c == 'X') {
            return 0;  /* Quitter */
        }
        else if (c == '\n') {
            return sel;
        }
    }
}

static void jouer_niveau(Jeu *jeu, Sauvegarde *sauv)
{
    init_partie(jeu, 10, 10);
    strcpy(jeu->pseudo, sauv->pseudo);
    jeu->vies = sauv->vies;
    
    /* Lancer la boucle selon le niveau */
    if (jeu->niveau == 1)
        boucle_jeu(jeu);
    else if (jeu->niveau == 2)
        boucle_jeu_n2(jeu);
    else
        boucle_jeu_n3(jeu);
    
    afficher_fin(jeu);
    
    if (jeu->victoire) {
        /* Niveau reussi -> passe au suivant */
        if (sauv->niveau_en_cours < 3) {
            sauv->niveau_en_cours++;
            sauvegarder_partie(sauv);
            
            nettoyer_ecran();
            printf("========================================\n");
            printf("  BRAVO %s !\n", sauv->pseudo);
            printf("  Niveau %d debloque !\n", sauv->niveau_en_cours);
            printf("========================================\n");
            pause_entree();
        } else {
            /* Jeu termine ! */
            nettoyer_ecran();
            printf("========================================\n");
            printf("  FELICITATIONS %s !\n", sauv->pseudo);
            printf("  Tu as termine le jeu !\n");
            printf("========================================\n");
            supprimer_sauvegarde();
            sauv->existe = 0;
            pause_entree();
        }
    } else {
        /* Niveau perdu -> perd une vie */
        sauv->vies--;
        
        if (sauv->vies <= 0) {
            /* Plus de vies -> supprime la sauvegarde */
            nettoyer_ecran();
            printf("========================================\n");
            printf("  GAME OVER %s !\n", sauv->pseudo);
            printf("  Tu n'as plus de vies.\n");
            printf("  Ta partie est supprimee.\n");
            printf("========================================\n");
            supprimer_sauvegarde();
            sauv->existe = 0;
            pause_entree();
        } else {
            /* Sauvegarde avec vies restantes */
            sauvegarder_partie(sauv);
            
            nettoyer_ecran();
            printf("========================================\n");
            printf("  Niveau echoue !\n");
            printf("  Vies restantes: %d\n", sauv->vies);
            printf("========================================\n");
            pause_entree();
        }
    }
    
    liberer_partie(jeu);
}

int main(void)
{
    Jeu jeu;
    Sauvegarde sauv;
    int choix;
    
    /* Charger la sauvegarde si elle existe */
    charger_sauvegarde(&sauv);
    
    while (1) {
        choix = menu_principal(&sauv);
        
        /* Quitter (0 = touche X, ou dernier choix du menu) */
        if (choix == 0) {
            printf("Au revoir!\n");
            return 0;
        }
        
        if (sauv.existe) {
            /* Avec sauvegarde: 1=Reprendre, 2=Nouvelle, 3=Param, 4=Quitter */
            
            if (choix == 4) {
                printf("Au revoir!\n");
                return 0;
            }
            
            if (choix == 3) {
                nettoyer_ecran();
                printf("Parametres: rien pour l'instant\n");
                pause_entree();
                continue;
            }
            
            if (choix == 1) {
                /* Reprendre partie */
                jeu.niveau = sauv.niveau_en_cours;
                jouer_niveau(&jeu, &sauv);
                charger_sauvegarde(&sauv);
                continue;
            }
            
            if (choix == 2) {
                /* Nouvelle partie */
                nettoyer_ecran();
                printf("========== NOUVELLE PARTIE ==========\n\n");
                printf("Attention: une partie existe deja pour %s.\n", sauv.pseudo);
                printf("Creer une nouvelle partie va l'effacer!\n\n");
                
                lire_pseudo(sauv.pseudo);
                sauv.vies = VIES_MAX;
                sauv.niveau_en_cours = 1;
                sauv.existe = 1;
                sauvegarder_partie(&sauv);
                
                jeu.niveau = 1;
                jouer_niveau(&jeu, &sauv);
                charger_sauvegarde(&sauv);
                continue;
            }
        } else {
            /* Sans sauvegarde: 1=Nouvelle, 2=Param, 3=Quitter */
            
            if (choix == 3) {
                printf("Au revoir!\n");
                return 0;
            }
            
            if (choix == 2) {
                nettoyer_ecran();
                printf("Parametres: rien pour l'instant\n");
                pause_entree();
                continue;
            }
            
            if (choix == 1) {
                /* Nouvelle partie */
                nettoyer_ecran();
                printf("========== NOUVELLE PARTIE ==========\n\n");
                
                lire_pseudo(sauv.pseudo);
                sauv.vies = VIES_MAX;
                sauv.niveau_en_cours = 1;
                sauv.existe = 1;
                sauvegarder_partie(&sauv);
                
                jeu.niveau = 1;
                jouer_niveau(&jeu, &sauv);
                charger_sauvegarde(&sauv);
                continue;
            }
        }
    }
    
    return 0;
}
