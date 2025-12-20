#include <stdio.h>                                         
#include <stdlib.h>                                         
#include <string.h>                                        
#include <time.h>                                           
#include "gestion_partie.h"                                
#include "moteur.h"                                         
#include "affichage.h"                                     
#include "saisie.h"                                        

static int abs_val(int x) { return x < 0 ? -x : x; }        /* renvoie la valeur absolue de x
si l'on rentre -X cela permet de renvoyer x qui est alors positifs pour un deplacement de case */

void init_partie(Jeu *jeu, int lignes, int colonnes)       
{
    static int seed = 0;                                    
    int i;                                                  
    if (!seed) { srand(time(NULL)); seed = 1; }                         /* Prépare le générateur de hasard en l'initialisant une seule fois 
                                                                        (grâce à la variable seed qui garde en mémoire si c'est déjà fait) 
                                                                        et déclare un compteur i pour les boucles */
    jeu->lignes = lignes;                                               /* Definit le nombre de lignes */
    jeu->colonnes = colonnes;                                           /* Definit le nombre de colonnes */
    jeu->plateau = (char **)malloc(sizeof(char *) * lignes * colonnes);  /* creer un tableau pour ligne et collone et chaque 
                                                                            case contiendra un pointeur vers un fruit */
    jeu->fruits[0] = "🍏";                                                  
    jeu->fruits[1] = "🍋";                                                  
    jeu->fruits[2] = "🍇";                                                 
    jeu->fruits[3] = "🍊";                                                
    jeu->fruits[4] = "🥥";                                 
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;      /* Initialise les scores a 0 */
    if (jeu->niveau == 1) {                                     /* Si niveau 1 */
        jeu->coups = COUPS_N1;                                  /* 40 coups */
        jeu->objectif = OBJECTIF_N1;                            /* 25 fruits/type */
        jeu->temps_limite = TEMPS_N1;                           /* 240 secondes */
        jeu->objectif_arbres = 0;                               /* Pas d'objectif d'arbres */
    } else if (jeu->niveau == 2) {                                  /* Si niveau 2 */
        jeu->coups = COUPS_N2;                                      /* 35 coups */
        jeu->objectif = OBJECTIF_N2;                                /* 30 fruits/type */
        jeu->temps_limite = TEMPS_N2;                               /* 210 secondes */
        jeu->objectif_arbres = 0;                                   /* Pas d'objectif d'arbres */
    } else {                                                            /* Niveau 3 */
        jeu->coups = COUPS_N3;                                          /* 30 coups */
        jeu->objectif = OBJECTIF_N3;                                    /* 35 fruits/type */
        jeu->temps_limite = TEMPS_N3;                                   /* 180 secondes */
        jeu->objectif_arbres = OBJECTIF_ARBRES;                         /* 3 arbres requis */
    }
    jeu->vies = VIES_MAX;                                   /* initialisation des vies a 3 */
    jeu->arbres_utilises = 0;                               /* 0 arbres utilises */
    jeu->temps_debut = time(NULL);                          /* temps du début */
    jeu->temps_restant = jeu->temps_limite;                     /* temps restant */
    for (i = 0; i < lignes * colonnes; i++)                     /* Pour chaque case */
        jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];          /* Place fruit aleatoire en utilisant le 
                                                                    numéro attribué a chaque fruit (aléatoire de 0 à 4)*/
    if (jeu->niveau == 1) cascade(jeu);                     /* Cascade simple Niveau 1 */
    else cascade_niveau2(jeu);                              /* Cascade avec speciaux Niveau 2/Niveau 3 */
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;      /* Remet les scores a 0 */
    jeu->curseur_x = jeu->curseur_y = 0;                    /* Place le curseur en haut à gauche  */
    jeu->selection = 0;                                     /* aucun fruit séléctionné au départ */
    jeu->select_x = jeu->select_y = -1;                     /* Position invalide */
    jeu->en_cours = 1;                                      /* Partie active */
    jeu->victoire = 0;                                      /* Pas de victoire */
}

void liberer_partie(Jeu *jeu)                               /* Libere la memoire plateau */
{
    if (jeu->plateau) {                                     
        free(jeu->plateau);                                 /* Libere la memoire */
        jeu->plateau = NULL;                              
    }
}

int verifier_victoire(Jeu *jeu)                             /* verification des objectifs*/
{
    int i;                                                 
    for (i = 0; i < NB_FRUITS; i++)                         
        if (jeu->score[i] < jeu->objectif) return 0;        /* vérification pour chaque fruit si les objs sont atteints
                                                                si l'un d'entre eux n'est pas atteint alors defaite*/
    if (jeu->objectif_arbres > 0 && jeu->arbres_utilises < jeu->objectif_arbres)  /* pareil pour les arbres au niveau 3 */
        return 0;                                           /* defaite */
    return 1;                                               /* Victoire */
}

int gerer_echange(Jeu *jeu)                                 
{
    int dx = abs_val(jeu->curseur_x - jeu->select_x);       
    int dy = abs_val(jeu->curseur_y - jeu->select_y);       /* calcule la distance entre X et Y */
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {  /* Si pas adjacent */
        jeu->selection = 0;                                 /* Annule selection */
        return -1;                                          /* Echec */
    }
    if (!permutation_valide(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {  /* Si invalide */
        jeu->selection = 0;                                 /* Annule selection */
        return -1;                                          /* Echec */
    }
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange cases */
    jeu->coups--;                                           /* nb de coups -1 */
    cascade(jeu);                                           /* Lance cascade */
    jeu->selection = 0;                                     /* Desactive selection */
    return 0;                                               /* Succes */
}

int gerer_echange_n2(Jeu *jeu)                              /* Gere echange niveau 2/3 */
{
    int dx, dy, i1, i2;                                     /* Déclare les variables nécessaires et calcule la distance 
                                                                entre les deux fruits sélectionnés */
    char *a, *b;                                            /* Pointeurs emojis */
    dx = abs_val(jeu->curseur_x - jeu->select_x);           /* pareil que Niveau 1*/
    dy = abs_val(jeu->curseur_y - jeu->select_y);           
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {  
        jeu->selection = 0;                                 
        return -1;                                         
    }
    i1 = jeu->select_x * jeu->colonnes + jeu->select_y;     /* Index case 1 */
    i2 = jeu->curseur_x * jeu->colonnes + jeu->curseur_y;   /* Index case 2 */
    a = jeu->plateau[i1];                                   /* Emoji case 1 */
    b = jeu->plateau[i2];                                   /* Emoji case 2 */
    if (est_special(a) || est_special(b)) {                 /* si l'un des deux est un caractère spécial */
        echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange */
        jeu->coups--;                                       /* coups -1 */
        a = jeu->plateau[i1];                               /* Nouveau emoji 1 */
        b = jeu->plateau[i2];                               /* Nouveau emoji 2 */
        if (strcmp(a, EMOJI_ARCENCIEL) == 0) {              /* Si arc-en-ciel en a */
            effet_arcenciel(jeu, b);                        /* Effet sur type b */
            jeu->plateau[i1] = NULL;                        /* détruit tous les fruits du même type que b) */
        } else if (strcmp(b, EMOJI_ARCENCIEL) == 0) {       /* pareil si arc-en-ciel en b */
            effet_arcenciel(jeu, a);                        
            jeu->plateau[i2] = NULL;                        
        } else {                                            
            if (strcmp(a, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->select_x, jeu->select_y);  /* Bombe 1 */
            else if (strcmp(a, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->select_x);  /* Boomerang 1 */
            else if (strcmp(a, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->select_y); jeu->arbres_utilises++; }  /* Arbre 1 */
            if (strcmp(b, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->curseur_x, jeu->curseur_y);  /* Bombe 2 */
            else if (strcmp(b, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->curseur_x);  /* Boomerang 2 */
            else if (strcmp(b, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->curseur_y); jeu->arbres_utilises++; }  /* Arbre 2 */
        }
        cascade_niveau2(jeu);                              
        jeu->selection = 0;                                 /* Desactive la séléction */
        return 0;                                           /* Succes */
    }
    if (!permutation_valide_n2(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {  /* Si invalide */
        jeu->selection = 0;                                 /* Annule */
        return -1;                                          /* Echec */
    }
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange */
    jeu->coups--;                                           /* coups -1 */
    cascade_niveau2(jeu);                                   /* Cascade */
    jeu->selection = 0;                                     /* Desactive la selection */
    return 0;                                               /* Succes */
}

void boucle_jeu(Jeu *jeu)                                   /* Boucle principale N1 */
{
    char cmd;                                               /* les Commandes au clavier */
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {  /* continue tant que le jeu est actif */
        if (jeu->temps_limite > 0) {                        /* Si temps limite */
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);  /* Calcule restant */
            if (jeu->temps_restant <= 0) break;             /* si le temps est écoulé on sort du jeu */
        }
        afficher_jeu(jeu);                                  /* Affiche plateau */
        cmd = lire_commande();                              /* Lit la commande */
        appliquer_commande(jeu, cmd);                       /* Applique cette derniere */
        if ((cmd == 'p' || cmd == 'P') && jeu->selection && (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y))  /* Si P et selection */
            gerer_echange(jeu);                             /* on essaye de faire un echange */
    }
    jeu->victoire = verifier_victoire(jeu);                 /* Verifie la victoire finale */
}

void boucle_jeu_n2(Jeu *jeu)                                /* Boucle principale Niveau 2 */
{
    char cmd;                                               /* pareil que pour le niveau 1 */
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {  
        if (jeu->temps_limite > 0) {                        /* Si temps limite */
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);  
            if (jeu->temps_restant <= 0) break;             
        }
        afficher_jeu(jeu);                                
        cmd = lire_commande();                             
        appliquer_commande(jeu, cmd);                       
        if ((cmd == 'p' || cmd == 'P') && jeu->selection && (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y))  
            gerer_echange_n2(jeu);                          /* effectue l'échange selon les critères des echanges du niveau 2 */
    }
    jeu->victoire = verifier_victoire(jeu);                 /* Verifie */
}

void boucle_jeu_n3(Jeu *jeu) { boucle_jeu_n2(jeu); }        /* Niveau 3 = Niveau 2 avec objectifs differents */

void sauvegarder_partie(Sauvegarde *sauv)                   /* Sauvegarde dans fichier */
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");               /* Ouvre en ecriture */
    if (f == NULL) return;                                  /* Si echec alors on sort */
    fprintf(f, "%s\n", sauv->pseudo);                       /* Ecrit pseudo */
    fprintf(f, "%d\n", sauv->vies);                         /* Ecrit vies */
    fprintf(f, "%d\n", sauv->niveau_en_cours);              /* Ecrit niveau */
    fclose(f);                                              /* Ferme fichier */
}

int charger_sauvegarde(Sauvegarde *sauv)                    /* Charge depuis fichier */
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");               /* Ouvre en lecture */
    int len;                                                /* Longueur chaine */
    if (f == NULL) { sauv->existe = 0; return 0; }          /* Si pas de fichier */
    if (fgets(sauv->pseudo, 32, f) == NULL) { fclose(f); sauv->existe = 0; return 0; }  /* Lit pseudo */
    len = strlen(sauv->pseudo);                             /* Calcule longueur */
    if (len > 0 && sauv->pseudo[len-1] == '\n') sauv->pseudo[len-1] = '\0';  /* Enleve newline */
    if (fscanf(f, "%d", &sauv->vies) != 1) { fclose(f); sauv->existe = 0; return 0; }  /* Lit vies */
    if (fscanf(f, "%d", &sauv->niveau_en_cours) != 1) { fclose(f); sauv->existe = 0; return 0; }  /* Lit niveau */
    fclose(f);                                              /* Ferme fichier */
    sauv->existe = 1;                                       /* Sauvegarde valide */
    return 1;                                               /* Succes */
}

void supprimer_sauvegarde(void) { remove(FICHIER_SAUVEGARDE); }  /* Supprime fichier sauvegarde */
