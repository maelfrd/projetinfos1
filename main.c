

/* main.c - Programme principal Candy Crush */

#include <stdio.h>                                          
#include <string.h>                                         
#include "structures.h"                                     
#include "gestion_partie.h"                                 
#include "affichage.h"                                      
#include "moteur.h"                                         


// fonction accessible uniquement depuis ce fichier//
static void boucle_principale(Sauvegarde *sauv);            /* Prototype boucle principale */
static void traiter_choix_avec_sauvegarde(int choix, Sauvegarde *sauv);  /* Prototype avec sauv */
static void traiter_choix_sans_sauvegarde(int choix, Sauvegarde *sauv);  /* Prototype sans sauv */
static void reprendre_partie(Sauvegarde *sauv);             /* Prototype reprendre */
static void nouvelle_partie(Sauvegarde *sauv, int avec_warning);  /* Prototype nouvelle */
static void jouer_niveau(Jeu *jeu, Sauvegarde *sauv);       /* Prototype jouer */
static void gerer_resultat_niveau(Jeu *jeu, Sauvegarde *sauv);  /* Prototype resultat */

int main(void)                                              
{
    Sauvegarde sauv;                                        /* Variable sauvegarde */
    charger_sauvegarde(&sauv);                              /* Charge sauvegarde */
    boucle_principale(&sauv);                               /* Lance boucle principale */
    afficher_au_revoir();                                   /* Affiche au revoir */
    return 0;                                               /* Retourne succes */
}

static void boucle_principale(Sauvegarde *sauv)             /* Boucle principale du jeu */
{
    int choix;                                              /* Choix utilisateur */
    while (1) {                                             /* Boucle infinie */
        choix = menu(sauv);                                 /* Affiche menu */
        if (choix == 0) return;                             /* Quitter si 0 */
        if (sauv->existe) traiter_choix_avec_sauvegarde(choix, sauv);  /* Avec sauvegarde */
        else traiter_choix_sans_sauvegarde(choix, sauv);    /* Sans sauvegarde */
    }
}

static void traiter_choix_avec_sauvegarde(int choix, Sauvegarde *sauv)  /* Traite choix avec sauv */
{
    switch (choix) {                                        /* Selon choix */
        case 1: reprendre_partie(sauv); break;              /* 1: Reprendre */
        case 2: nouvelle_partie(sauv, 1); break;            /* 2: Nouvelle avec warning */
        case 3: afficher_parametres(); break;               /* 3: Parametres */
        case 4: sauv->existe = 0; break;                    /* 4: Quitter */
    }
}

static void traiter_choix_sans_sauvegarde(int choix, Sauvegarde *sauv)  /* Traite choix sans sauv */
{
    switch (choix) {                                        /* Selon choix */
        case 1: nouvelle_partie(sauv, 0); break;            /* 1: Nouvelle sans warning */
        case 2: afficher_parametres(); break;               /* 2: Parametres */
        case 3: sauv->existe = -1; break;                   /* 3: Quitter */
    }
}

static void reprendre_partie(Sauvegarde *sauv)              /* Reprend partie sauvegardee */
{
    Jeu jeu;                                                /* Structure jeu */
    jeu.niveau = sauv->niveau_en_cours;                     /* Recupere niveau */
    jouer_niveau(&jeu, sauv);                               /* Joue le niveau */
    charger_sauvegarde(sauv);                               /* Recharge sauvegarde */
}

static void nouvelle_partie(Sauvegarde *sauv, int avec_warning)  /* Nouvelle partie */
{
    Jeu jeu;                                                
    if (avec_warning) afficher_nouvelle_partie_warning(sauv->pseudo);  /* Warning si demande */
    else afficher_nouvelle_partie();                        /* Sinon ecran normal */
    lire_pseudo(sauv->pseudo);                              /* Lit pseudo */
    sauv->vies = VIES_MAX;                                  /* Init vies */
    sauv->niveau_en_cours = 1;                              /* Niveau 1 */
    sauv->existe = 1;                                       /* Existe */
    sauvegarder_partie(sauv);                               /* Sauvegarde */
    jeu.niveau = 1;                                         /* Niveau 1 */
    jouer_niveau(&jeu, sauv);                               /* Joue */
    charger_sauvegarde(sauv);                               /* Recharge */
}

static void jouer_niveau(Jeu *jeu, Sauvegarde *sauv)        /* Execute un niveau */
{
    init_partie(jeu, 10, 10);                               /* Init plateau 10x10 */
    strcpy(jeu->pseudo, sauv->pseudo);                      /* Copie pseudo */
    jeu->vies = sauv->vies;                                 /* Copie vies */
    switch (jeu->niveau) {                                  /* Selon niveau */
        case 1: boucle_jeu(jeu); break;                     /* N1: boucle simple */
        case 2: boucle_jeu_n2(jeu); break;                  /* N2: boucle N2 */
        default: boucle_jeu_n3(jeu); break;                 /* N3: boucle N3 */
    }
    afficher_fin(jeu);                                      /* Affiche fin */
    gerer_resultat_niveau(jeu, sauv);                       /* Gere resultat */
    liberer_partie(jeu);                                    /* Libere memoire */
}

static void gerer_resultat_niveau(Jeu *jeu, Sauvegarde *sauv)  /* Gere fin niveau */
{
    if (jeu->victoire) {                                    /* Si victoire */
        if (sauv->niveau_en_cours < 3) {                    /* Si pas dernier niveau */
            sauv->niveau_en_cours++;                        /* Niveau suivant */
            sauvegarder_partie(sauv);                       /* Sauvegarde */
            afficher_niveau_debloque(sauv->pseudo, sauv->niveau_en_cours);  /* Message */
        } else {                                            /* Si dernier niveau */
            supprimer_sauvegarde();                         /* Supprime sauvegarde */
            sauv->existe = 0;                               /* N'existe plus */
            afficher_jeu_termine(sauv->pseudo);             /* Felicitations */
        }
    } else {                                                /* Si defaite */
        sauv->vies--;                                       /* Perd une vie */
        if (sauv->vies <= 0) {                              /* Si plus de vies */
            supprimer_sauvegarde();                         /* Supprime */
            sauv->existe = 0;                               /* N'existe plus */
            afficher_game_over(sauv->pseudo);               /* Game over */
        } else {                                            /* Si encore des vies */
            sauvegarder_partie(sauv);                       /* Sauvegarde */
            afficher_niveau_echoue(sauv->vies);             /* Message echec */
        }
    }
}

