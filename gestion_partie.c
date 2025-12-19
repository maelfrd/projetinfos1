/* gestion_partie.c - Gestion des parties */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"

static int abs_val(int x) { return x < 0 ? -x : x; }

void init_partie(Jeu *jeu, int lignes, int colonnes)
{
    static int seed = 0;
    int i;
    
    if (!seed) { srand(time(NULL)); seed = 1; }
    
    jeu->lignes = lignes;
    jeu->colonnes = colonnes;
    jeu->plateau = (char **)malloc(sizeof(char *) * lignes * colonnes);
    
    /* Fruits */
    jeu->fruits[0] = "🍎";
    jeu->fruits[1] = "🍋";
    jeu->fruits[2] = "🍇";
    jeu->fruits[3] = "🍊";
    jeu->fruits[4] = "🍒";
    
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;
    
    /* Parametres selon niveau */
    if (jeu->niveau == 1) {
        jeu->coups = COUPS_N1;
        jeu->objectif = OBJECTIF_N1;
        jeu->temps_limite = TEMPS_N1;
        jeu->objectif_arbres = 0;
    } else if (jeu->niveau == 2) {
        jeu->coups = COUPS_N2;
        jeu->objectif = OBJECTIF_N2;
        jeu->temps_limite = TEMPS_N2;
        jeu->objectif_arbres = 0;
    } else {
        jeu->coups = COUPS_N3;
        jeu->objectif = OBJECTIF_N3;
        jeu->temps_limite = TEMPS_N3;
        jeu->objectif_arbres = OBJECTIF_ARBRES;
    }
    
    jeu->vies = VIES_MAX;
    jeu->arbres_utilises = 0;
    jeu->temps_debut = time(NULL);
    jeu->temps_restant = jeu->temps_limite;
    
    /* Remplir le plateau */
    for (i = 0; i < lignes * colonnes; i++)
        jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];
    
    /* Eliminer les alignements de depart */
    if (jeu->niveau == 1)
        cascade(jeu);
    else
        cascade_niveau2(jeu);
    
    /* Remettre les scores a zero apres le nettoyage initial */
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;
    
    jeu->curseur_x = jeu->curseur_y = 0;
    jeu->selection = 0;
    jeu->select_x = jeu->select_y = -1;
    jeu->en_cours = 1;
    jeu->victoire = 0;
}

void liberer_partie(Jeu *jeu)
{
    if (jeu->plateau) { free(jeu->plateau); jeu->plateau = NULL; }
}

int verifier_victoire(Jeu *jeu)
{
    int i;
    for (i = 0; i < NB_FRUITS; i++)
        if (jeu->score[i] < jeu->objectif) return 0;
    if (jeu->objectif_arbres > 0 && jeu->arbres_utilises < jeu->objectif_arbres)
        return 0;
    return 1;
}

int gerer_echange(Jeu *jeu)
{
    int dx = abs_val(jeu->curseur_x - jeu->select_x);
    int dy = abs_val(jeu->curseur_y - jeu->select_y);
    
    /* Pas adjacent */
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        jeu->selection = 0;
        return -1;
    }
    
    /* Pas valide */
    if (!permutation_valide(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {
        jeu->selection = 0;
        return -1;
    }
    
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);
    jeu->coups--;
    cascade(jeu);
    jeu->selection = 0;
    return 0;
}

int gerer_echange_n2(Jeu *jeu)
{
    int dx, dy, i1, i2;
    char *a, *b;
    
    dx = abs_val(jeu->curseur_x - jeu->select_x);
    dy = abs_val(jeu->curseur_y - jeu->select_y);
    
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        jeu->selection = 0;
        return -1;
    }
    
    i1 = jeu->select_x * jeu->colonnes + jeu->select_y;
    i2 = jeu->curseur_x * jeu->colonnes + jeu->curseur_y;
    a = jeu->plateau[i1];
    b = jeu->plateau[i2];
    
    /* Si un bonbon special est implique */
    if (est_special(a) || est_special(b)) {
        echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);
        jeu->coups--;
        
        /* Apres l'echange */
        a = jeu->plateau[i1];
        b = jeu->plateau[i2];
        
        /* Arc-en-ciel en priorite */
        if (strcmp(a, EMOJI_ARCENCIEL) == 0) {
            effet_arcenciel(jeu, b);
            jeu->plateau[i1] = NULL;
        } else if (strcmp(b, EMOJI_ARCENCIEL) == 0) {
            effet_arcenciel(jeu, a);
            jeu->plateau[i2] = NULL;
        } else {
            if (strcmp(a, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->select_x, jeu->select_y);
            else if (strcmp(a, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->select_x);
            else if (strcmp(a, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->select_y); jeu->arbres_utilises++; }
            
            if (strcmp(b, EMOJI_BOMBE) == 0) effet_bombe(jeu, jeu->curseur_x, jeu->curseur_y);
            else if (strcmp(b, EMOJI_BOOMERANG) == 0) effet_ligne(jeu, jeu->curseur_x);
            else if (strcmp(b, EMOJI_ARBRE) == 0) { effet_colonne(jeu, jeu->curseur_y); jeu->arbres_utilises++; }
        }
        
        cascade_niveau2(jeu);
        jeu->selection = 0;
        return 0;
    }
    
    /* Permutation normale */
    if (!permutation_valide_n2(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y)) {
        jeu->selection = 0;
        return -1;
    }
    
    echanger(jeu, jeu->select_x, jeu->select_y, jeu->curseur_x, jeu->curseur_y);
    jeu->coups--;
    cascade_niveau2(jeu);
    jeu->selection = 0;
    return 0;
}

void boucle_jeu(Jeu *jeu)
{
    char cmd;
    
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {
        /* Temps */
        if (jeu->temps_limite > 0) {
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);
            if (jeu->temps_restant <= 0) break;
        }
        
        afficher_jeu(jeu);
        cmd = lire_commande();
        appliquer_commande(jeu, cmd);
        
        /* Tente l'echange si selection active et case differente */
        if ((cmd == 'p' || cmd == 'P') && jeu->selection &&
            (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y)) {
            gerer_echange(jeu);
        }
    }
    jeu->victoire = verifier_victoire(jeu);
}

void boucle_jeu_n2(Jeu *jeu)
{
    char cmd;
    
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {
        if (jeu->temps_limite > 0) {
            jeu->temps_restant = jeu->temps_limite - (int)(time(NULL) - jeu->temps_debut);
            if (jeu->temps_restant <= 0) break;
        }
        
        afficher_jeu(jeu);
        cmd = lire_commande();
        appliquer_commande(jeu, cmd);
        
        if ((cmd == 'p' || cmd == 'P') && jeu->selection &&
            (jeu->curseur_x != jeu->select_x || jeu->curseur_y != jeu->select_y)) {
            gerer_echange_n2(jeu);
        }
    }
    jeu->victoire = verifier_victoire(jeu);
}

void boucle_jeu_n3(Jeu *jeu)
{
    boucle_jeu_n2(jeu);  /* Meme logique, objectifs differents */
}

/* ========== SAUVEGARDE ========== */

void sauvegarder_partie(Sauvegarde *sauv)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");
    if (f == NULL) return;
    
    fprintf(f, "%s\n", sauv->pseudo);
    fprintf(f, "%d\n", sauv->vies);
    fprintf(f, "%d\n", sauv->niveau_en_cours);
    
    fclose(f);
}

int charger_sauvegarde(Sauvegarde *sauv)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "r");
    if (f == NULL) {
        sauv->existe = 0;
        return 0;
    }
    
    if (fgets(sauv->pseudo, 32, f) == NULL) {
        fclose(f);
        sauv->existe = 0;
        return 0;
    }
    /* Enlever le \n */
    int len = strlen(sauv->pseudo);
    if (len > 0 && sauv->pseudo[len-1] == '\n')
        sauv->pseudo[len-1] = '\0';
    
    if (fscanf(f, "%d", &sauv->vies) != 1) {
        fclose(f);
        sauv->existe = 0;
        return 0;
    }
    
    if (fscanf(f, "%d", &sauv->niveau_en_cours) != 1) {
        fclose(f);
        sauv->existe = 0;
        return 0;
    }
    
    fclose(f);
    sauv->existe = 1;
    return 1;
}

void supprimer_sauvegarde(void)
{
    remove(FICHIER_SAUVEGARDE);
}
