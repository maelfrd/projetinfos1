/* moteur.c - Logique du jeu Candy Crush */                 /* En-tete fichier */
#include <stdlib.h>                                         /* Allocation memoire, rand */
#include <string.h>                                         /* strcmp, strcpy */
#include <unistd.h>                                         /* usleep */
#include "moteur.h"                                         /* Header moteur */
#include "affichage.h"                                      /* Fonctions affichage */
#include "saisie.h"                                         /* Fonctions saisie */

#define DELAI_ANIMATION 500000                              /* Delai animation: 0.5 seconde */

/* ========== MENU ========== */                            /* Section menu */
int menu(Sauvegarde *sauv)                                  /* Gere le menu principal */
{
    char c;                                                 /* Caractere lu */
    int selection = 1;                                      /* Option selectionnee */
    int nb_options = sauv->existe ? 4 : 3;                  /* Nombre d'options selon sauvegarde */
    int action;                                             /* Action a effectuer */
    while (1) {                                             /* Boucle infinie menu */
        if (sauv->existe) afficher_menu_avec_sauvegarde(selection, sauv);  /* Affiche menu avec sauv */
        else afficher_menu_sans_sauvegarde(selection);      /* Affiche menu sans sauv */
        c = lire_menu();                                    /* Lit touche */
        action = appliquer_commande_menu(c);                /* Convertit en action */
        if (action == -1) { if (selection > 1) selection--; }  /* Monte si possible */
        else if (action == 1) { if (selection < nb_options) selection++; }  /* Descend si possible */
        else if (action == -99) return 0;                   /* Quitter */
        else if (action == 0) return selection;             /* Valider */
    }
}

/* ========== FONCTIONS UTILITAIRES ========== */           /* Section utilitaires */
int est_special(char *c)                                    /* Verifie si emoji special */
{
    if (c == NULL) return 0;                                /* NULL pas special */
    if (strcmp(c, EMOJI_ARBRE) == 0) return 1;              /* Arbre special */
    if (strcmp(c, EMOJI_BOMBE) == 0) return 1;              /* Bombe special */
    if (strcmp(c, EMOJI_BOOMERANG) == 0) return 1;          /* Boomerang special */
    if (strcmp(c, EMOJI_ARCENCIEL) == 0) return 1;          /* Arc-en-ciel special */
    return 0;                                               /* Sinon pas special */
}

int est_joker(char *c)                                      /* Verifie si emoji joker */
{
    if (c == NULL) return 0;                                /* NULL pas joker */
    return (strcmp(c, EMOJI_JOKER) == 0);                   /* Compare avec emoji joker */
}

int est_fruit(char *c)                                      /* Verifie si emoji fruit normal */
{
    if (c == NULL) return 0;                                /* NULL pas fruit */
    if (est_special(c)) return 0;                           /* Special pas fruit */
    if (est_joker(c)) return 0;                             /* Joker pas fruit */
    return 1;                                               /* Sinon c'est un fruit */
}

int index_fruit(Jeu *jeu, char *f)                          /* Retourne index du fruit (0-4) */
{
    int i;                                                  /* Compteur */
    if (f == NULL) return -1;                               /* NULL retourne -1 */
    for (i = 0; i < NB_FRUITS; i++)                         /* Parcourt fruits */
        if (strcmp(f, jeu->fruits[i]) == 0) return i;       /* Si trouve, retourne index */
    return -1;                                              /* Pas trouve */
}

/* ========== ALIGNEMENTS DE BASE ========== */             /* Section alignements */
int chercher_alignements(Jeu *jeu, int marques[])           /* Cherche alignements 3+ */
{
    int x, y, k, nb, idx, trouve = 0;                       /* Variables locales */
    int total = jeu->lignes * jeu->colonnes;                /* Taille plateau */
    char *f;                                                /* Fruit courant */
    for (k = 0; k < total; k++) marques[k] = 0;             /* Init marques a 0 */
    for (x = 0; x < jeu->lignes; x++) {                     /* Pour chaque ligne */
        for (y = 0; y < jeu->colonnes - 2; y++) {           /* Pour chaque colonne (sauf 2 dernieres) */
            idx = x * jeu->colonnes + y;                    /* Index dans tableau */
            f = jeu->plateau[idx];                          /* Fruit a cette position */
            if (!est_fruit(f)) continue;                    /* Ignore si pas fruit */
            nb = 1;                                         /* Compte le fruit courant */
            for (k = y + 1; k < jeu->colonnes; k++) {       /* Compte vers droite */
                if (strcmp(jeu->plateau[x * jeu->colonnes + k], f) != 0) break;  /* Stop si different */
                nb++;                                       /* Incremente compteur */
            }
            if (nb >= 3) {                                  /* Si 3 ou plus */
                for (k = 0; k < nb; k++) marques[x * jeu->colonnes + y + k] = 1;  /* Marque tous */
                trouve = 1;                                 /* Alignement trouve */
            }
        }
    }
    for (y = 0; y < jeu->colonnes; y++) {                   /* Pour chaque colonne */
        for (x = 0; x < jeu->lignes - 2; x++) {             /* Pour chaque ligne (sauf 2 dernieres) */
            idx = x * jeu->colonnes + y;                    /* Index */
            f = jeu->plateau[idx];                          /* Fruit */
            if (!est_fruit(f)) continue;                    /* Ignore si pas fruit */
            nb = 1;                                         /* Compte courant */
            for (k = x + 1; k < jeu->lignes; k++) {         /* Compte vers bas */
                if (strcmp(jeu->plateau[k * jeu->colonnes + y], f) != 0) break;  /* Stop si different */
                nb++;                                       /* Incremente */
            }
            if (nb >= 3) {                                  /* Si 3+ */
                for (k = 0; k < nb; k++) marques[(x + k) * jeu->colonnes + y] = 1;  /* Marque */
                trouve = 1;                                 /* Trouve */
            }
        }
    }
    return trouve;                                          /* Retourne si trouve */
}

void supprimer_marques(Jeu *jeu, int marques[])             /* Supprime cases marquees */
{
    int i, idx, total = jeu->lignes * jeu->colonnes;        /* Variables */
    for (i = 0; i < total; i++) {                           /* Pour chaque case */
        if (marques[i]) {                                   /* Si marquee */
            idx = index_fruit(jeu, jeu->plateau[i]);        /* Index du fruit */
            if (idx >= 0) jeu->score[idx]++;                /* Incremente score */
            jeu->plateau[i] = NULL;                         /* Supprime */
        }
    }
}

void faire_tomber(Jeu *jeu)                                 /* Applique gravite */
{
    int col, lig, dest;                                     /* Variables */
    for (col = 0; col < jeu->colonnes; col++) {             /* Pour chaque colonne */
        dest = jeu->lignes - 1;                             /* Destination = bas */
        for (lig = jeu->lignes - 1; lig >= 0; lig--) {      /* Du bas vers haut */
            if (jeu->plateau[lig * jeu->colonnes + col] != NULL) {  /* Si case non vide */
                jeu->plateau[dest * jeu->colonnes + col] = jeu->plateau[lig * jeu->colonnes + col];  /* Deplace */
                if (dest != lig) jeu->plateau[lig * jeu->colonnes + col] = NULL;  /* Vide ancienne */
                dest--;                                     /* Prochaine destination */
            }
        }
    }
}

void remplir_vides(Jeu *jeu, int niveau2)                   /* Remplit cases vides */
{
    int i, total = jeu->lignes * jeu->colonnes;             /* Variables */
    int joker_present = 0;                                  /* Flag joker */
    if (niveau2) {                                          /* Si niveau 2+ */
        for (i = 0; i < total; i++)                         /* Cherche joker existant */
            if (est_joker(jeu->plateau[i])) { joker_present = 1; break; }
    }
    for (i = 0; i < total; i++) {                           /* Pour chaque case */
        if (jeu->plateau[i] == NULL) {                      /* Si vide */
            if (niveau2 && !joker_present && (rand() % 50) == 0) {  /* 2% joker N2+ */
                jeu->plateau[i] = EMOJI_JOKER;              /* Place joker */
                joker_present = 1;                          /* Marque present */
            } else jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];  /* Sinon fruit aleatoire */
        }
    }
}

/* ========== PERMUTATION ET CASCADE ========== */          /* Section permutation */
void echanger(Jeu *jeu, int x1, int y1, int x2, int y2)     /* Echange deux cases */
{
    int i1 = x1 * jeu->colonnes + y1;                       /* Index case 1 */
    int i2 = x2 * jeu->colonnes + y2;                       /* Index case 2 */
    char *tmp = jeu->plateau[i1];                           /* Sauvegarde case 1 */
    jeu->plateau[i1] = jeu->plateau[i2];                    /* Case 1 = Case 2 */
    jeu->plateau[i2] = tmp;                                 /* Case 2 = ancien Case 1 */
}

int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2)  /* Verifie si echange valide */
{
    int *marques, ok, total = jeu->lignes * jeu->colonnes;  /* Variables */
    echanger(jeu, x1, y1, x2, y2);                          /* Echange temporaire */
    marques = (int *)calloc(total, sizeof(int));            /* Alloue marques */
    ok = chercher_alignements(jeu, marques);                /* Cherche alignements */
    free(marques);                                          /* Libere marques */
    echanger(jeu, x1, y1, x2, y2);                          /* Annule echange */
    return ok;                                              /* Retourne validite */
}

void cascade(Jeu *jeu)                                      /* Cascade niveau 1 */
{
    int *marques, total = jeu->lignes * jeu->colonnes;      /* Variables */
    marques = (int *)calloc(total, sizeof(int));            /* Alloue marques */
    while (chercher_alignements(jeu, marques)) {            /* Tant qu'alignements */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche + delai */
        supprimer_marques(jeu, marques);                    /* Supprime */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche + delai */
        faire_tomber(jeu);                                  /* Gravite */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche + delai */
        remplir_vides(jeu, 0);                              /* Remplit */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche + delai */
    }
    free(marques);                                          /* Libere */
}

/* ========== NIVEAU 2/3 : COMBINAISONS SPECIALES ========== */  /* Section N2/N3 */
static int correspond(char *c, char *type)                  /* Verifie correspondance ou joker */
{
    if (c == NULL || type == NULL) return 0;                /* NULL = pas correspond */
    if (est_special(c)) return 0;                           /* Special pas correspond */
    if (est_joker(c)) return 1;                             /* Joker correspond toujours */
    return (strcmp(c, type) == 0);                          /* Compare types */
}

static int compter_dir(Jeu *jeu, int x, int y, int dx, int dy, char *type)  /* Compte dans direction */
{
    int nb = 0;                                             /* Compteur */
    x += dx; y += dy;                                       /* Avance */
    while (x >= 0 && x < jeu->lignes && y >= 0 && y < jeu->colonnes) {  /* Tant que dans grille */
        if (!correspond(jeu->plateau[x * jeu->colonnes + y], type)) break;  /* Stop si different */
        nb++; x += dx; y += dy;                             /* Incremente et avance */
    }
    return nb;                                              /* Retourne compte */
}

int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[])  /* Detecte formations speciales */
{
    int x, y, i, j, idx, trouve = 0;                        /* Variables */
    int total = jeu->lignes * jeu->colonnes;                /* Taille */
    int haut, bas, gauche, droite, len_h, len_v;            /* Compteurs directions */
    char *c, *a, *b, *d;                                    /* Emojis */
    for (i = 0; i < total; i++) { marques[i] = 0; speciaux[i] = 0; }  /* Init tableaux */
    for (x = 0; x < jeu->lignes; x++) {                     /* Pour chaque ligne */
        for (y = 0; y < jeu->colonnes; y++) {               /* Pour chaque colonne */
            idx = x * jeu->colonnes + y;                    /* Index */
            c = jeu->plateau[idx];                          /* Emoji */
            if (!est_fruit(c)) continue;                    /* Ignore non-fruits */
            haut = compter_dir(jeu, x, y, -1, 0, c);        /* Compte vers haut */
            bas = compter_dir(jeu, x, y, 1, 0, c);          /* Compte vers bas */
            gauche = compter_dir(jeu, x, y, 0, -1, c);      /* Compte vers gauche */
            droite = compter_dir(jeu, x, y, 0, 1, c);       /* Compte vers droite */
            len_v = 1 + haut + bas;                         /* Longueur verticale */
            len_h = 1 + gauche + droite;                    /* Longueur horizontale */
            if (len_v >= 3 && len_h >= 3) {                 /* Croix/T/L = Arbre */
                for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1;  /* Marque vertical */
                for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;  /* Marque horizontal */
                speciaux[idx] = 1; marques[idx] = 0; trouve = 1;  /* Arbre au centre */
            }
            else if (len_h >= 6 || len_v >= 6) {            /* 6+ = Arc-en-ciel */
                int pos_arcenciel;                          /* Position arc-en-ciel */
                if (len_h >= 6) {                           /* Si horizontal */
                    for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;  /* Marque */
                    pos_arcenciel = x * jeu->colonnes + (y - gauche + 2);  /* Position 3 */
                    speciaux[pos_arcenciel] = 4; marques[pos_arcenciel] = 0;  /* Arc-en-ciel */
                }
                if (len_v >= 6) {                           /* Si vertical */
                    for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1;  /* Marque */
                    pos_arcenciel = (x - haut + 2) * jeu->colonnes + y;  /* Position 3 */
                    speciaux[pos_arcenciel] = 4; marques[pos_arcenciel] = 0;  /* Arc-en-ciel */
                }
                trouve = 1;                                 /* Trouve */
            }
            else if (len_h == 5 || len_v == 5) {            /* 5 = Boomerang */
                int pos_boomerang;                          /* Position boomerang */
                if (len_h == 5) {                           /* Si horizontal */
                    for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;  /* Marque */
                    pos_boomerang = x * jeu->colonnes + (y - gauche + 2);  /* Position 3 */
                    speciaux[pos_boomerang] = 3; marques[pos_boomerang] = 0;  /* Boomerang */
                }
                if (len_v == 5) {                           /* Si vertical */
                    for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1;  /* Marque */
                    pos_boomerang = (x - haut + 2) * jeu->colonnes + y;  /* Position 3 */
                    speciaux[pos_boomerang] = 3; marques[pos_boomerang] = 0;  /* Boomerang */
                }
                trouve = 1;                                 /* Trouve */
            }
        }
    }
    for (x = 0; x < jeu->lignes - 1; x++) {                 /* Carre 2x2 = Bombe */
        for (y = 0; y < jeu->colonnes - 1; y++) {           /* Pour chaque position */
            int pos_bombe;                                  /* Position bombe */
            a = jeu->plateau[x * jeu->colonnes + y];        /* Coin haut-gauche */
            b = jeu->plateau[x * jeu->colonnes + y + 1];    /* Coin haut-droit */
            c = jeu->plateau[(x+1) * jeu->colonnes + y];    /* Coin bas-gauche */
            d = jeu->plateau[(x+1) * jeu->colonnes + y + 1];  /* Coin bas-droit */
            if (!est_fruit(a)) continue;                    /* Ignore si pas fruit */
            if (!est_fruit(b) || strcmp(a, b) != 0) continue;  /* Verifie b */
            if (!est_fruit(c) || strcmp(a, c) != 0) continue;  /* Verifie c */
            if (!est_fruit(d) || strcmp(a, d) != 0) continue;  /* Verifie d */
            marques[x * jeu->colonnes + y] = 1;             /* Marque haut-gauche */
            marques[x * jeu->colonnes + y + 1] = 1;         /* Marque haut-droit */
            marques[(x+1) * jeu->colonnes + y] = 1;         /* Marque bas-gauche */
            marques[(x+1) * jeu->colonnes + y + 1] = 1;     /* Marque bas-droit */
            pos_bombe = (x+1) * jeu->colonnes + (y+1);      /* Bombe bas-droit */
            speciaux[pos_bombe] = 2; marques[pos_bombe] = 0; trouve = 1;  /* Cree bombe */
        }
    }
    return trouve;                                          /* Retourne resultat */
}

static int chercher_avec_jokers(Jeu *jeu, int marques[])    /* Cherche alignements avec jokers */
{
    int t, x, y, k, debut, len, trouve = 0, a_type;         /* Variables */
    char *type, *c;                                         /* Type et emoji courant */
    for (t = 0; t < NB_FRUITS; t++) {                       /* Pour chaque type de fruit */
        type = jeu->fruits[t];                              /* Type courant */
        for (x = 0; x < jeu->lignes; x++) {                 /* Horizontal */
            y = 0;                                          /* Debut colonne */
            while (y < jeu->colonnes) {                     /* Parcourt colonnes */
                debut = y; len = 0; a_type = 0;             /* Init */
                while (y < jeu->colonnes) {                 /* Compte sequence */
                    c = jeu->plateau[x * jeu->colonnes + y];  /* Emoji */
                    if (!correspond(c, type)) break;        /* Stop si pas correspond */
                    if (!est_joker(c)) a_type = 1;          /* Au moins un vrai fruit */
                    len++; y++;                             /* Incremente */
                }
                if (len >= 3 && a_type) {                   /* Si 3+ avec vrai fruit */
                    for (k = debut; k < debut + len; k++) marques[x * jeu->colonnes + k] = 1;  /* Marque */
                    trouve = 1;                             /* Trouve */
                }
                if (len == 0) y++;                          /* Avance si rien */
            }
        }
        for (y = 0; y < jeu->colonnes; y++) {               /* Vertical */
            x = 0;                                          /* Debut ligne */
            while (x < jeu->lignes) {                       /* Parcourt lignes */
                debut = x; len = 0; a_type = 0;             /* Init */
                while (x < jeu->lignes) {                   /* Compte sequence */
                    c = jeu->plateau[x * jeu->colonnes + y];  /* Emoji */
                    if (!correspond(c, type)) break;        /* Stop si pas correspond */
                    if (!est_joker(c)) a_type = 1;          /* Vrai fruit */
                    len++; x++;                             /* Incremente */
                }
                if (len >= 3 && a_type) {                   /* Si 3+ */
                    for (k = debut; k < debut + len; k++) marques[k * jeu->colonnes + y] = 1;  /* Marque */
                    trouve = 1;                             /* Trouve */
                }
                if (len == 0) x++;                          /* Avance */
            }
        }
    }
    return trouve;                                          /* Retourne resultat */
}

int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2)  /* Validation N2 */
{
    char *a = jeu->plateau[x1 * jeu->colonnes + y1];        /* Emoji 1 */
    char *b = jeu->plateau[x2 * jeu->colonnes + y2];        /* Emoji 2 */
    int *marques, *spec, i, ok = 0;                         /* Variables */
    int total = jeu->lignes * jeu->colonnes;                /* Taille */
    if (est_special(a) || est_special(b)) return 1;         /* Special toujours valide */
    echanger(jeu, x1, y1, x2, y2);                          /* Echange temporaire */
    marques = (int *)calloc(total, sizeof(int));            /* Alloue marques */
    spec = (int *)calloc(total, sizeof(int));               /* Alloue speciaux */
    detecter_speciaux(jeu, marques, spec);                  /* Detecte speciaux */
    chercher_avec_jokers(jeu, marques);                     /* Cherche jokers */
    for (i = 0; i < total; i++) if (marques[i] || spec[i]) { ok = 1; break; }  /* Verifie */
    free(marques); free(spec);                              /* Libere */
    echanger(jeu, x1, y1, x2, y2);                          /* Annule echange */
    return ok;                                              /* Retourne validite */
}

void cascade_niveau2(Jeu *jeu)                              /* Cascade niveau 2 */
{
    int *marques, *spec, i, trouve, any;                    /* Variables */
    int total = jeu->lignes * jeu->colonnes;                /* Taille */
    marques = (int *)calloc(total, sizeof(int));            /* Alloue marques */
    spec = (int *)calloc(total, sizeof(int));               /* Alloue speciaux */
    while (1) {                                             /* Boucle cascade */
        trouve = detecter_speciaux(jeu, marques, spec);     /* Detecte */
        if (chercher_avec_jokers(jeu, marques)) trouve = 1; /* Jokers */
        for (i = 0; i < total; i++) if (spec[i]) marques[i] = 0;  /* Protege speciaux */
        any = 0; for (i = 0; i < total; i++) if (marques[i]) { any = 1; break; }  /* Verifie */
        if (!any) break;                                    /* Sort si rien */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche */
        for (i = 0; i < total; i++) {                       /* Cree speciaux */
            if (spec[i] == 1) jeu->plateau[i] = EMOJI_ARBRE;  /* Arbre */
            if (spec[i] == 2) jeu->plateau[i] = EMOJI_BOMBE;  /* Bombe */
            if (spec[i] == 3) jeu->plateau[i] = EMOJI_BOOMERANG;  /* Boomerang */
            if (spec[i] == 4) jeu->plateau[i] = EMOJI_ARCENCIEL;  /* Arc-en-ciel */
        }
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche speciaux */
        supprimer_marques(jeu, marques);                    /* Supprime */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche */
        faire_tomber(jeu);                                  /* Gravite */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche */
        remplir_vides(jeu, 1);                              /* Remplit */
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         /* Affiche */
    }
    free(marques); free(spec);                              /* Libere */
}

/* ========== EFFETS DES BONBONS SPECIAUX ========== */     /* Section effets */
void effet_colonne(Jeu *jeu, int y)                         /* Arbre: supprime colonne */
{
    int x, idx;                                             /* Variables */
    for (x = 0; x < jeu->lignes; x++) {                     /* Pour chaque ligne */
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);  /* Index fruit */
        if (idx >= 0) jeu->score[idx]++;                    /* Incremente score */
        jeu->plateau[x * jeu->colonnes + y] = NULL;         /* Supprime */
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche suppression */
    faire_tomber(jeu);                                      /* Gravite */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    remplir_vides(jeu, 1);                                  /* Remplit */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
}

void effet_ligne(Jeu *jeu, int x)                           /* Boomerang: supprime ligne */
{
    int y, idx;                                             /* Variables */
    for (y = 0; y < jeu->colonnes; y++) {                   /* Pour chaque colonne */
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);  /* Index */
        if (idx >= 0) jeu->score[idx]++;                    /* Score */
        jeu->plateau[x * jeu->colonnes + y] = NULL;         /* Supprime */
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    faire_tomber(jeu);                                      /* Gravite */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    remplir_vides(jeu, 1);                                  /* Remplit */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
}

void effet_bombe(Jeu *jeu, int x, int y)                    /* Bombe: supprime 3x3 */
{
    int i, j, idx;                                          /* Variables */
    for (i = x - 1; i <= x + 1; i++) {                      /* Lignes -1 a +1 */
        for (j = y - 1; j <= y + 1; j++) {                  /* Colonnes -1 a +1 */
            if (i >= 0 && i < jeu->lignes && j >= 0 && j < jeu->colonnes) {  /* Si dans grille */
                idx = index_fruit(jeu, jeu->plateau[i * jeu->colonnes + j]);  /* Index */
                if (idx >= 0) jeu->score[idx]++;            /* Score */
                jeu->plateau[i * jeu->colonnes + j] = NULL; /* Supprime */
            }
        }
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    faire_tomber(jeu);                                      /* Gravite */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    remplir_vides(jeu, 1);                                  /* Remplit */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
}

void effet_arcenciel(Jeu *jeu, char *cible)                 /* Arc-en-ciel: supprime type */
{
    int i, idx, total = jeu->lignes * jeu->colonnes;        /* Variables */
    char *type = cible;                                     /* Type cible */
    if (!est_fruit(type)) type = jeu->fruits[rand() % NB_FRUITS];  /* Si pas fruit, aleatoire */
    for (i = 0; i < total; i++) {                           /* Pour chaque case */
        if (est_fruit(jeu->plateau[i]) && strcmp(jeu->plateau[i], type) == 0) {  /* Si meme type */
            idx = index_fruit(jeu, jeu->plateau[i]);        /* Index */
            if (idx >= 0) jeu->score[idx]++;                /* Score */
            jeu->plateau[i] = NULL;                         /* Supprime */
        }
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    faire_tomber(jeu);                                      /* Gravite */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
    remplir_vides(jeu, 1);                                  /* Remplit */
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             /* Affiche */
}

