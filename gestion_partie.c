/* gestion_partie.c - Gestion des parties */                /* En-tete du fichier */
#include <stdio.h>                                          /* Bibliotheque entrees/sorties standard */
#include <stdlib.h>                                         /* Bibliotheque allocation memoire */
#include <string.h>                                         /* Bibliotheque manipulation chaines */
#include <time.h>                                           /* Bibliotheque gestion du temps */
#include "gestion_partie.h"                                 /* Header gestion partie */
#include "moteur.h"                                         /* Header moteur de jeu */
#include "affichage.h"                                      /* Header fonctions affichage */
#include "saisie.h"                                         /* Header fonctions saisie */

static int abs_val(int x) { return x < 0 ? -x : x; }        /* Valeur absolue d'un entier */

void init_partie(Jeu *jeu, int lignes, int colonnes)        /* Initialise une nouvelle partie */
{
    static int seed = 0;                                    /* Variable statique pour srand */
    int i;                                                  /* Compteur de boucle */
    if (!seed) { srand(time(NULL)); seed = 1; }             /* Init generateur aleatoire une fois */
    jeu->lignes = lignes;                                   /* Definit nombre de lignes */
    jeu->colonnes = colonnes;                               /* Definit nombre de colonnes */
    jeu->plateau = (char **)malloc(sizeof(char *) * lignes * colonnes);  /* Alloue plateau */
    jeu->fruits[0] = "🍏";                                  /* Emoji pomme verte */
    jeu->fruits[1] = "🍋";                                  /* Emoji citron */
    jeu->fruits[2] = "🍇";                                  /* Emoji raisin */
    jeu->fruits[3] = "🍊";                                  /* Emoji orange */
    jeu->fruits[4] = "🥥";                                  /* Emoji noix de coco */
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;      /* Init scores a 0 */
    if (jeu->niveau == 1) {                                 /* Si niveau 1 */
        jeu->coups = COUPS_N1;                              /* 40 coups */
        jeu->objectif = OBJECTIF_N1;                        /* 25 fruits/type */
        jeu->temps_limite = TEMPS_N1;                       /* 240 secondes */
        jeu->objectif_arbres = 0;                           /* Pas d'arbres */
    } else if (jeu->niveau == 2) {                          /* Si niveau 2 */
        jeu->coups = COUPS_N2;                              /* 35 coups */
        jeu->objectif = OBJECTIF_N2;                        /* 30 fruits/type */
        jeu->temps_limite = TEMPS_N2;                       /* 210 secondes */
        jeu->objectif_arbres = 0;                           /* Pas d'arbres */
    } else {                                                /* Niveau 3 */
        jeu->coups = COUPS_N3;                              /* 30 coups */
        jeu->objectif = OBJECTIF_N3;                        /* 35 fruits/type */
        jeu->temps_limite = TEMPS_N3;                       /* 180 secondes */
        jeu->objectif_arbres = OBJECTIF_ARBRES;             /* 3 arbres requis */
    }
    jeu->vies = VIES_MAX;                                   /* Init vies au max */
    jeu->arbres_utilises = 0;                               /* 0 arbres utilises */
    jeu->temps_debut = time(NULL);                          /* Timestamp debut */
    jeu->temps_restant = jeu->temps_limite;                 /* Init temps restant */
    for (i = 0; i < lignes * colonnes; i++)                 /* Pour chaque case */
        jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];  /* Place fruit aleatoire */
    if (jeu->niveau == 1) cascade(jeu);                     /* Cascade simple N1 */
    else cascade_niveau2(jeu);                              /* Cascade avec speciaux N2/N3 */
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;      /* Remet scores a 0 */
    jeu->curseur_x = jeu->curseur_y = 0;                    /* Curseur en (0,0) */
    jeu->selection = 0;                                     /* Pas de selection */
    jeu->select_x = jeu->select_y = -1;                     /* Position invalide */
    jeu->en_cours = 1;                                      /* Partie active */
    jeu->victoire = 0;                                      /* Pas de victoire */
}

void liberer_partie(Jeu *jeu)                               /* Libere memoire plateau */
{
    if (jeu->plateau) {                                     /* Si plateau alloue */
        free(jeu->plateau);                                 /* Libere memoire */
        jeu->plateau = NULL;                                /* Evite pointeur sauvage */
    }
}

int verifier_victoire(Jeu *jeu)                             /* Verifie si objectifs atteints */
{
    int i;                                                  /* Compteur */
    for (i = 0; i < NB_FRUITS; i++)                         /* Pour chaque fruit */
        if (jeu->score[i] < jeu->objectif) return 0;        /* Si pas atteint, pas victoire */
    if (jeu->objectif_arbres > 0 && jeu->arbres_utilises < jeu->objectif_arbres)  /* Si arbres requis */
        return 0;                                           /* Et pas atteint */
    return 1;                                               /* Victoire */
}

int gerer_echange(Jeu *jeu)                                 /* Gere echange niveau 1 */
{
    int dx = abs_val(jeu->curseur_x - jeu->select_x);       /* Distance X */
    int dy = abs_val(jeu->curseur_y - jeu->select_y);       /* Distance Y */
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {  /* Si pas adjacent */
        jeu->selection = 0;                                 /* Annule selection */
        return -1;                                          /* Echec */
    }
    if (!permutation_valide(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {  /* Si invalide */
        jeu->selection = 0;                                 /* Annule selection */
        return -1;                                          /* Echec */
    }
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange cases */
    jeu->coups--;                                           /* Decremente coups */
    cascade(jeu);                                           /* Lance cascade */
    jeu->selection = 0;                                     /* Desactive selection */
    return 0;                                               /* Succes */
}

int gerer_echange_n2(Jeu *jeu)                              /* Gere echange niveau 2/3 */
{
    int dx, dy, i1, i2;                                     /* Variables locales */
    char *a, *b;                                            /* Pointeurs emojis */
    dx = abs_val(jeu->curseur_x - jeu->select_x);           /* Distance X */
    dy = abs_val(jeu->curseur_y - jeu->select_y);           /* Distance Y */
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {  /* Si pas adjacent */
        jeu->selection = 0;                                 /* Annule */
        return -1;                                          /* Echec */
    }
    i1 = jeu->select_x * jeu->colonnes + jeu->select_y;     /* Index case 1 */
    i2 = jeu->curseur_x * jeu->colonnes + jeu->curseur_y;   /* Index case 2 */
    a = jeu->plateau[i1];                                   /* Emoji case 1 */
    b = jeu->plateau[i2];                                   /* Emoji case 2 */
    if (est_special(a) || est_special(b)) {                 /* Si special implique */
        echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange */
        jeu->coups--;                                       /* Decremente */
        a = jeu->plateau[i1];                               /* Nouveau emoji 1 */
        b = jeu->plateau[i2];                               /* Nouveau emoji 2 */
        if (strcmp(a, EMOJI_ARCENCIEL) == 0) {              /* Si arc-en-ciel en 1 */
            effet_arcenciel(jeu, b);                        /* Effet sur type b */
            jeu->plateau[i1] = NULL;                        /* Supprime arc-en-ciel */
        } else if (strcmp(b, EMOJI_ARCENCIEL) == 0) {       /* Si arc-en-ciel en 2 */
            effet_arcenciel(jeu, a);                        /* Effet sur type a */
            jeu->plateau[i2] = NULL;                        /* Supprime */
        } else {                                            /* Autres speciaux */
            if (strcmp(a, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->select_x, jeu->select_y);  /* Bombe 1 */
            else if (strcmp(a, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->select_x);  /* Boomerang 1 */
            else if (strcmp(a, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->select_y); jeu->arbres_utilises++; }  /* Arbre 1 */
            if (strcmp(b, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->curseur_x, jeu->curseur_y);  /* Bombe 2 */
            else if (strcmp(b, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->curseur_x);  /* Boomerang 2 */
            else if (strcmp(b, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->curseur_y); jeu->arbres_utilises++; }  /* Arbre 2 */
        }
        cascade_niveau2(jeu);                               /* Cascade N2 */
        jeu->selection = 0;                                 /* Desactive */
        return 0;                                           /* Succes */
    }
    if (!permutation_valide_n2(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {  /* Si invalide */
        jeu->selection = 0;                                 /* Annule */
        return -1;                                          /* Echec */
    }
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);  /* Echange */
    jeu->coups--;                                           /* Decremente */
    cascade_niveau2(jeu);                                   /* Cascade */
    jeu->selection = 0;                                     /* Desactive */
    return 0;                                               /* Succes */
}

void boucle_jeu(Jeu *jeu)                                   /* Boucle principale N1 */
{
    char cmd;                                               /* Commande clavier */
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {  /* Tant que actif */
        if (jeu->temps_limite > 0) {                        /* Si temps limite */
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);  /* Calcule restant */
            if (jeu->temps_restant <= 0) break;             /* Si ecoule, sort */
        }
        afficher_jeu(jeu);                                  /* Affiche plateau */
        cmd = lire_commande();                              /* Lit commande */
        appliquer_commande(jeu, cmd);                       /* Applique */
        if ((cmd == 'p' || cmd == 'P') && jeu->selection && (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y))  /* Si P et selection */
            gerer_echange(jeu);                             /* Tente echange */
    }
    jeu->victoire = verifier_victoire(jeu);                 /* Verifie victoire finale */
}

void boucle_jeu_n2(Jeu *jeu)                                /* Boucle principale N2 */
{
    char cmd;                                               /* Commande clavier */
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {  /* Tant que actif */
        if (jeu->temps_limite > 0) {                        /* Si temps limite */
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);  /* Calcule */
            if (jeu->temps_restant <= 0) break;             /* Sort si ecoule */
        }
        afficher_jeu(jeu);                                  /* Affiche */
        cmd = lire_commande();                              /* Lit */
        appliquer_commande(jeu, cmd);                       /* Applique */
        if ((cmd == 'p' || cmd == 'P') && jeu->selection && (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y))  /* Si echange */
            gerer_echange_n2(jeu);                          /* Gere N2 */
    }
    jeu->victoire = verifier_victoire(jeu);                 /* Verifie */
}

void boucle_jeu_n3(Jeu *jeu) { boucle_jeu_n2(jeu); }        /* N3 = N2 avec objectifs differents */

void sauvegarder_partie(Sauvegarde *sauv)                   /* Sauvegarde dans fichier */
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");               /* Ouvre en ecriture */
    if (f == NULL) return;                                  /* Si echec, sort */
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
