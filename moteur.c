/* moteur.c - Logique du jeu Candy Crush */
#include <stdlib.h>
#include <string.h>
#include "moteur.h"

/* ========== FONCTIONS UTILITAIRES ========== */

int est_special(char *c)
{
    if (c == NULL) return 0;
    if (strcmp(c, EMOJI_ARBRE) == 0) return 1;
    if (strcmp(c, EMOJI_BOMBE) == 0) return 1;
    if (strcmp(c, EMOJI_BOOMERANG) == 0) return 1;
    if (strcmp(c, EMOJI_ARCENCIEL) == 0) return 1;
    return 0;
}

int est_joker(char *c)
{
    if (c == NULL) return 0;
    return (strcmp(c, EMOJI_JOKER) == 0);
}

int est_fruit(char *c)
{
    if (c == NULL) return 0;
    if (est_special(c)) return 0;
    if (est_joker(c)) return 0;
    return 1;
}

int index_fruit(Jeu *jeu, char *f)
{
    int i;
    if (f == NULL) return -1;
    for (i = 0; i < NB_FRUITS; i++)
        if (strcmp(f, jeu->fruits[i]) == 0) return i;
    return -1;
}

/* ========== ALIGNEMENTS DE BASE ========== */

/* Cherche tous les alignements de 3+ fruits identiques */
int chercher_alignements(Jeu *jeu, int marques[])
{
    int x, y, k, nb, idx, trouve = 0;
    int total = jeu->lignes * jeu->colonnes;
    char *f;
    
    /* Initialise les marques a 0 */
    for (k = 0; k < total; k++) marques[k] = 0;
    
    /* Recherche horizontale */
    for (x = 0; x < jeu->lignes; x++) {
        for (y = 0; y < jeu->colonnes - 2; y++) {
            idx = x * jeu->colonnes + y;
            f = jeu->plateau[idx];
            if (!est_fruit(f)) continue;
            
            /* Compte les fruits identiques vers la droite */
            nb = 1;
            for (k = y + 1; k < jeu->colonnes; k++) {
                if (strcmp(jeu->plateau[x * jeu->colonnes + k], f) != 0) break;
                nb++;
            }
            
            /* Si 3 ou plus, on marque */
            if (nb >= 3) {
                for (k = 0; k < nb; k++)
                    marques[x * jeu->colonnes + y + k] = 1;
                trouve = 1;
            }
        }
    }
    
    /* Recherche verticale */
    for (y = 0; y < jeu->colonnes; y++) {
        for (x = 0; x < jeu->lignes - 2; x++) {
            idx = x * jeu->colonnes + y;
            f = jeu->plateau[idx];
            if (!est_fruit(f)) continue;
            
            nb = 1;
            for (k = x + 1; k < jeu->lignes; k++) {
                if (strcmp(jeu->plateau[k * jeu->colonnes + y], f) != 0) break;
                nb++;
            }
            
            if (nb >= 3) {
                for (k = 0; k < nb; k++)
                    marques[(x + k) * jeu->colonnes + y] = 1;
                trouve = 1;
            }
        }
    }
    
    return trouve;
}

/* Supprime les cases marquees et compte les points */
void supprimer_marques(Jeu *jeu, int marques[])
{
    int i, idx;
    int total = jeu->lignes * jeu->colonnes;
    
    for (i = 0; i < total; i++) {
        if (marques[i]) {
            idx = index_fruit(jeu, jeu->plateau[i]);
            if (idx >= 0) jeu->score[idx]++;
            jeu->plateau[i] = NULL;
        }
    }
}

/* Fait tomber les bonbons (gravite) */
void faire_tomber(Jeu *jeu)
{
    int col, lig, dest;
    
    for (col = 0; col < jeu->colonnes; col++) {
        dest = jeu->lignes - 1;
        for (lig = jeu->lignes - 1; lig >= 0; lig--) {
            if (jeu->plateau[lig * jeu->colonnes + col] != NULL) {
                jeu->plateau[dest * jeu->colonnes + col] = jeu->plateau[lig * jeu->colonnes + col];
                if (dest != lig) jeu->plateau[lig * jeu->colonnes + col] = NULL;
                dest--;
            }
        }
    }
}

/* Remplit les cases vides avec des bonbons aleatoires */
void remplir_vides(Jeu *jeu, int niveau2)
{
    int i, total = jeu->lignes * jeu->colonnes;
    int joker_present = 0;
    
    /* Verifie si un joker existe deja */
    if (niveau2) {
        for (i = 0; i < total; i++)
            if (est_joker(jeu->plateau[i])) { joker_present = 1; break; }
    }
    
    for (i = 0; i < total; i++) {
        if (jeu->plateau[i] == NULL) {
            /* 2% de chance de joker en niveau 2+ */
            if (niveau2 && !joker_present && (rand() % 50) == 0) {
                jeu->plateau[i] = EMOJI_JOKER;
                joker_present = 1;
            } else {
                jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];
            }
        }
    }
}

/* ========== PERMUTATION ET CASCADE ========== */

void echanger(Jeu *jeu, int x1, int y1, int x2, int y2)
{
    int i1 = x1 * jeu->colonnes + y1;
    int i2 = x2 * jeu->colonnes + y2;
    char *tmp = jeu->plateau[i1];
    jeu->plateau[i1] = jeu->plateau[i2];
    jeu->plateau[i2] = tmp;
}

int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2)
{
    int *marques, ok, total = jeu->lignes * jeu->colonnes;
    
    echanger(jeu, x1, y1, x2, y2);
    marques = (int *)calloc(total, sizeof(int));
    ok = chercher_alignements(jeu, marques);
    free(marques);
    echanger(jeu, x1, y1, x2, y2);
    
    return ok;
}

void cascade(Jeu *jeu)
{
    int *marques, total = jeu->lignes * jeu->colonnes;
    marques = (int *)calloc(total, sizeof(int));
    
    while (chercher_alignements(jeu, marques)) {
        supprimer_marques(jeu, marques);
        faire_tomber(jeu);
        remplir_vides(jeu, 0);
    }
    free(marques);
}

/* ========== NIVEAU 2/3 : COMBINAISONS SPECIALES ========== */

/* Verifie si une case correspond a un type ou est un joker */
static int correspond(char *c, char *type)
{
    if (c == NULL || type == NULL) return 0;
    if (est_special(c)) return 0;
    if (est_joker(c)) return 1;
    return (strcmp(c, type) == 0);
}

/* Compte les bonbons dans une direction */
static int compter_dir(Jeu *jeu, int x, int y, int dx, int dy, char *type)
{
    int nb = 0;
    x += dx; y += dy;
    while (x >= 0 && x < jeu->lignes && y >= 0 && y < jeu->colonnes) {
        if (!correspond(jeu->plateau[x * jeu->colonnes + y], type)) break;
        nb++;
        x += dx; y += dy;
    }
    return nb;
}

/* Detecte les combinaisons speciales */
int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[])
{
    int x, y, i, j, idx, trouve = 0;
    int total = jeu->lignes * jeu->colonnes;
    int haut, bas, gauche, droite, len_h, len_v;
    char *c, *a, *b, *d;
    
    for (i = 0; i < total; i++) { marques[i] = 0; speciaux[i] = 0; }
    
    /* Parcours du plateau */
    for (x = 0; x < jeu->lignes; x++) {
        for (y = 0; y < jeu->colonnes; y++) {
            idx = x * jeu->colonnes + y;
            c = jeu->plateau[idx];
            if (!est_fruit(c)) continue;
            
            /* Compte dans les 4 directions */
            haut = compter_dir(jeu, x, y, -1, 0, c);
            bas = compter_dir(jeu, x, y, 1, 0, c);
            gauche = compter_dir(jeu, x, y, 0, -1, c);
            droite = compter_dir(jeu, x, y, 0, 1, c);
            
            len_v = 1 + haut + bas;
            len_h = 1 + gauche + droite;
            
            /* Croix/T/L -> Arbre */
            if (len_v >= 3 && len_h >= 3) {
                for (i = x - haut; i <= x + bas; i++)
                    marques[i * jeu->colonnes + y] = 1;
                for (j = y - gauche; j <= y + droite; j++)
                    marques[x * jeu->colonnes + j] = 1;
                speciaux[idx] = 1; /* Arbre */
                marques[idx] = 0;
                trouve = 1;
            }
            /* 6+ -> Arc-en-ciel */
            else if (len_h >= 6 || len_v >= 6) {
                if (len_h >= 6)
                    for (j = y - gauche; j <= y + droite; j++)
                        marques[x * jeu->colonnes + j] = 1;
                if (len_v >= 6)
                    for (i = x - haut; i <= x + bas; i++)
                        marques[i * jeu->colonnes + y] = 1;
                speciaux[idx] = 4; /* Arc-en-ciel */
                marques[idx] = 0;
                trouve = 1;
            }
            /* 5 -> Boomerang */
            else if (len_h == 5 || len_v == 5) {
                if (len_h == 5)
                    for (j = y - gauche; j <= y + droite; j++)
                        marques[x * jeu->colonnes + j] = 1;
                if (len_v == 5)
                    for (i = x - haut; i <= x + bas; i++)
                        marques[i * jeu->colonnes + y] = 1;
                speciaux[idx] = 3; /* Boomerang */
                marques[idx] = 0;
                trouve = 1;
            }
        }
    }
    
    /* Carre 2x2 -> Bombe */
    for (x = 0; x < jeu->lignes - 1; x++) {
        for (y = 0; y < jeu->colonnes - 1; y++) {
            a = jeu->plateau[x * jeu->colonnes + y];
            b = jeu->plateau[x * jeu->colonnes + y + 1];
            c = jeu->plateau[(x+1) * jeu->colonnes + y];
            d = jeu->plateau[(x+1) * jeu->colonnes + y + 1];
            
            if (!est_fruit(a)) continue;
            if (!est_fruit(b) || strcmp(a, b) != 0) continue;
            if (!est_fruit(c) || strcmp(a, c) != 0) continue;
            if (!est_fruit(d) || strcmp(a, d) != 0) continue;
            
            idx = x * jeu->colonnes + y;
            marques[x * jeu->colonnes + y + 1] = 1;
            marques[(x+1) * jeu->colonnes + y] = 1;
            marques[(x+1) * jeu->colonnes + y + 1] = 1;
            speciaux[idx] = 2; /* Bombe */
            marques[idx] = 0;
            trouve = 1;
        }
    }
    
    return trouve;
}

/* Cherche alignements avec jokers */
static int chercher_avec_jokers(Jeu *jeu, int marques[])
{
    int t, x, y, k, debut, len, trouve = 0;
    int a_type;
    char *type, *c;
    
    for (t = 0; t < NB_FRUITS; t++) {
        type = jeu->fruits[t];
        
        /* Horizontal */
        for (x = 0; x < jeu->lignes; x++) {
            y = 0;
            while (y < jeu->colonnes) {
                debut = y; len = 0; a_type = 0;
                while (y < jeu->colonnes) {
                    c = jeu->plateau[x * jeu->colonnes + y];
                    if (!correspond(c, type)) break;
                    if (!est_joker(c)) a_type = 1;
                    len++; y++;
                }
                if (len >= 3 && a_type) {
                    for (k = debut; k < debut + len; k++)
                        marques[x * jeu->colonnes + k] = 1;
                    trouve = 1;
                }
                if (len == 0) y++;
            }
        }
        
        /* Vertical */
        for (y = 0; y < jeu->colonnes; y++) {
            x = 0;
            while (x < jeu->lignes) {
                debut = x; len = 0; a_type = 0;
                while (x < jeu->lignes) {
                    c = jeu->plateau[x * jeu->colonnes + y];
                    if (!correspond(c, type)) break;
                    if (!est_joker(c)) a_type = 1;
                    len++; x++;
                }
                if (len >= 3 && a_type) {
                    for (k = debut; k < debut + len; k++)
                        marques[k * jeu->colonnes + y] = 1;
                    trouve = 1;
                }
                if (len == 0) x++;
            }
        }
    }
    return trouve;
}

int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2)
{
    char *a = jeu->plateau[x1 * jeu->colonnes + y1];
    char *b = jeu->plateau[x2 * jeu->colonnes + y2];
    int *marques, *spec, i, ok = 0;
    int total = jeu->lignes * jeu->colonnes;
    
    /* Echange avec special toujours valide */
    if (est_special(a) || est_special(b)) return 1;
    
    echanger(jeu, x1, y1, x2, y2);
    marques = (int *)calloc(total, sizeof(int));
    spec = (int *)calloc(total, sizeof(int));
    
    detecter_speciaux(jeu, marques, spec);
    chercher_avec_jokers(jeu, marques);
    
    for (i = 0; i < total; i++)
        if (marques[i] || spec[i]) { ok = 1; break; }
    
    free(marques); free(spec);
    echanger(jeu, x1, y1, x2, y2);
    return ok;
}

void cascade_niveau2(Jeu *jeu)
{
    int *marques, *spec, i, trouve, any;
    int total = jeu->lignes * jeu->colonnes;
    
    marques = (int *)calloc(total, sizeof(int));
    spec = (int *)calloc(total, sizeof(int));
    
    while (1) {
        trouve = detecter_speciaux(jeu, marques, spec);
        if (chercher_avec_jokers(jeu, marques)) trouve = 1;
        
        /* Ne pas supprimer ou creer un special */
        for (i = 0; i < total; i++)
            if (spec[i]) marques[i] = 0;
        
        /* Verifie s'il y a quelque chose a supprimer */
        any = 0;
        for (i = 0; i < total; i++)
            if (marques[i]) { any = 1; break; }
        
        if (!any) break;
        
        /* Cree les speciaux */
        for (i = 0; i < total; i++) {
            if (spec[i] == 1) jeu->plateau[i] = EMOJI_ARBRE;
            if (spec[i] == 2) jeu->plateau[i] = EMOJI_BOMBE;
            if (spec[i] == 3) jeu->plateau[i] = EMOJI_BOOMERANG;
            if (spec[i] == 4) jeu->plateau[i] = EMOJI_ARCENCIEL;
        }
        
        supprimer_marques(jeu, marques);
        faire_tomber(jeu);
        remplir_vides(jeu, 1);
    }
    
    free(marques); free(spec);
}

/* ========== EFFETS DES BONBONS SPECIAUX ========== */

void effet_colonne(Jeu *jeu, int y)
{
    int x, idx;
    for (x = 0; x < jeu->lignes; x++) {
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);
        if (idx >= 0) jeu->score[idx]++;
        jeu->plateau[x * jeu->colonnes + y] = NULL;
    }
    faire_tomber(jeu);
    remplir_vides(jeu, 1);
}

void effet_ligne(Jeu *jeu, int x)
{
    int y, idx;
    for (y = 0; y < jeu->colonnes; y++) {
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);
        if (idx >= 0) jeu->score[idx]++;
        jeu->plateau[x * jeu->colonnes + y] = NULL;
    }
    faire_tomber(jeu);
    remplir_vides(jeu, 1);
}

void effet_bombe(Jeu *jeu, int x, int y)
{
    int i, j, idx;
    for (i = x - 1; i <= x + 1; i++) {
        for (j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i < jeu->lignes && j >= 0 && j < jeu->colonnes) {
                idx = index_fruit(jeu, jeu->plateau[i * jeu->colonnes + j]);
                if (idx >= 0) jeu->score[idx]++;
                jeu->plateau[i * jeu->colonnes + j] = NULL;
            }
        }
    }
    faire_tomber(jeu);
    remplir_vides(jeu, 1);
}

void effet_arcenciel(Jeu *jeu, char *cible)
{
    int i, idx, total = jeu->lignes * jeu->colonnes;
    char *type = cible;
    
    if (!est_fruit(type)) type = jeu->fruits[rand() % NB_FRUITS];
    
    for (i = 0; i < total; i++) {
        if (est_fruit(jeu->plateau[i]) && strcmp(jeu->plateau[i], type) == 0) {
            idx = index_fruit(jeu, jeu->plateau[i]);
            if (idx >= 0) jeu->score[idx]++;
            jeu->plateau[i] = NULL;
        }
    }
    faire_tomber(jeu);
    remplir_vides(jeu, 1);
}
