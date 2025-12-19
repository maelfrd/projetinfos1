
#include <stdio.h>      
#include <stdlib.h>    
#include <string.h>   
#include <time.h>       
#include "gestion_partie.h"
#include "moteur.h"
#include "affichage.h"
#include "saisie.h"

 valeur absolue d'un entier
static int abs_val(int x) { return x < 0 ? -x : x; } //lkjojhiubh

// Initialisation d'une partie
void init_partie(Jeu *jeu, int lignes, int colonnes)
{
    static int seed = 0;    // Permet d'initialiser srand une seule fois
    int i;
    
    // Initialisation du générateur aléatoire
    if (!seed) { srand(time(NULL)); seed = 1; }
    
    // Dimensions du plateau
    jeu->lignes = lignes;
    jeu->colonnes = colonnes;

    // Allocation mémoire du plateau
    jeu->plateau = (char **)malloc(sizeof(char *) * lignes * colonnes);
    
    /* Fruits */
    jeu->fruits[0] = "🍎";
    jeu->fruits[1] = "🍋";
    jeu->fruits[2] = "🍇";
    jeu->fruits[3] = "🍊";
    jeu->fruits[4] = "🍒";
    
    // Initialisation des scores à 0
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;
    
    /* Parametres selon niveau */
    if (jeu->niveau == 1) {
        jeu->coups = COUPS_N1;          // Nombre de coups autorisés
        jeu->objectif = OBJECTIF_N1;    // Objectif de score
        jeu->temps_limite = TEMPS_N1;   // Temps limite
        jeu->objectif_arbres = 0;       // Pas d'arbres requis
    } else if (jeu->niveau == 2) {
        jeu->coups = COUPS_N2;
        jeu->objectif = OBJECTIF_N2;
        jeu->temps_limite = TEMPS_N2;
        jeu->objectif_arbres = 0;
    } else {
        jeu->coups = COUPS_N3;
        jeu->objectif = OBJECTIF_N3;
        jeu->temps_limite = TEMPS_N3;
        jeu->objectif_arbres = OBJECTIF_ARBRES; // Objectif spécial arbres
    }
    
    // Initialisation des paramètres de jeu
    jeu->vies = VIES_MAX;
    jeu->arbres_utilises = 0;
    jeu->temps_debut = time(NULL);
    jeu->temps_restant = jeu->temps_limite;
    
    /* Remplir le plateau avec des fruits aléatoires */
    for (i = 0; i < lignes * colonnes; i++)
        jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS];
    
    /* Eliminer les alignements de depart */
    if (jeu->niveau == 1)
        cascade(jeu);              // Cascade simple
    else
        cascade_niveau2(jeu);      // Cascade avancée
    
    /* Remettre les scores a zero apres le nettoyage initial */
    for (i = 0; i < NB_FRUITS; i++) jeu->score[i] = 0;
    
    // Initialisation du curseur et de la sélection
    jeu->curseur_x = jeu->curseur_y = 0;
    jeu->selection = 0;
    jeu->select_x = jeu->select_y = -1;

    // Etat de la partie
    jeu->en_cours = 1;
    jeu->victoire = 0;
}

// Libération de la mémoire du plateau
void liberer_partie(Jeu *jeu)
{
    if (jeu->plateau) { 
        free(jeu->plateau);        // Libère la mémoire
        jeu->plateau = NULL;       // Évite un pointeur sauvage
    }
}

// Vérifie si les conditions de victoire sont remplies
int verifier_victoire(Jeu *jeu)
{
    int i;

    // Vérifie que tous les objectifs de fruits sont atteints
    for (i = 0; i < NB_FRUITS; i++)
        if (jeu->score[i] < jeu->objectif) return 0;

    // Vérifie l'objectif des arbres si présent
    if (jeu->objectif_arbres > 0 && jeu->arbres_utilises < jeu->objectif_arbres)
        return 0;

    return 1;   // Victoire
}

// Gestion d'un échange (niveau 1)
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
    if (!permutation_valide(jeu, jeu->select_x, jeu->select_y,
                            jeu->curseur_x, jeu->curseur_y)) {
        jeu->selection = 0;
        return -1;
    }
    
    // Échange des cases
    echanger(jeu, jeu->select_x, jeu->select_y,
                  jeu->curseur_x, jeu->curseur_y);

    jeu->coups--;      // Consomme un coup
    cascade(jeu);      // Applique les cascades
    jeu->selection = 0;
    return 0;
}

// Gestion d'un échange (niveau 2 et 3)
int gerer_echange_n2(Jeu *jeu)
{
    int dx, dy, i1, i2;
    char *a, *b;
    
    dx = abs_val(jeu->curseur_x - jeu->select_x);
    dy = abs_val(jeu->curseur_y - jeu->select_y);
    
    // Vérifie l'adjacence
    if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1))) {
        jeu->selection = 0;
        return -1;
    }
    
    // Indices linéaires dans le plateau
    i1 = jeu->select_x * jeu->colonnes + jeu->select_y;
    i2 = jeu->curseur_x * jeu->colonnes + jeu->curseur_y;

    a = jeu->plateau[i1];
    b = jeu->plateau[i2];
    
    /* Si un bonbon special est implique */
    if (est_special(a) || est_special(b)) {
        echanger(jeu, jeu->select_x, jeu->select_y,
                      jeu->curseur_x, jeu->curseur_y);
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
            // Effets spéciaux pour a
            if (strcmp(a, EMOJI_BOMBE) == 0)
                effet_bombe(jeu, jeu->select_x, jeu->select_y);
            else if (strcmp(a, EMOJI_BOOMERANG) == 0)
                effet_ligne(jeu, jeu->select_x);
            else if (strcmp(a, EMOJI_ARBRE) == 0) {
                effet_colonne(jeu, jeu->select_y);
                jeu->arbres_utilises++;
            }
            
            // Effets spéciaux pour b
            if (strcmp(b, EMOJI_BOMBE) == 0)
                effet_bombe(jeu, jeu->curseur_x, jeu->curseur_y);
            else if (strcmp(b, EMOJI_BOOMERANG) == 0)
                effet_ligne(jeu, jeu->curseur_x);
            else if (strcmp(b, EMOJI_ARBRE) == 0) {
                effet_colonne(jeu, jeu->curseur_y);
                jeu->arbres_utilises++;
            }
        }
        
        cascade_niveau2(jeu);
        jeu->selection = 0;
        return 0;
    }
    
    /* Permutation normale */
    if (!permutation_valide_n2(jeu, jeu->select_x, jeu->select_y,
                               jeu->curseur_x, jeu->curseur_y)) {
        jeu->selection = 0;
        return -1;
    }
    
    echanger(jeu, jeu->select_x, jeu->select_y,
                  jeu->curseur_x, jeu->curseur_y);
    jeu->coups--;
    cascade_niveau2(jeu);
    jeu->selection = 0;
    return 0;
}

// Boucle principale du jeu (niveau 1)
void boucle_jeu(Jeu *jeu)
{
    char cmd;
    
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {
        /* Gestion du temps */
        if (jeu->temps_limite > 0) {
            jeu->temps_restant = jeu->temps_limite -
                                 (int)(time(NULL) - jeu->temps_debut);
            if (jeu->temps_restant <= 0) break;
        }
        
        afficher_jeu(jeu);          // Affiche le plateau
        cmd = lire_commande();      // Lit la commande clavier
        appliquer_commande(jeu, cmd); // Applique la commande
        
        /* Tente l'echange si selection active */
        if ((cmd == 'p' || cmd == 'P') && jeu->selection &&
            (jeu->curseur_x != jeu->select_x ||
             jeu->curseur_y != jeu->select_y)) {
            gerer_echange(jeu);
        }
    }
    jeu->victoire = verifier_victoire(jeu);
}

// Boucle de jeu niveau 2
void boucle_jeu_n2(Jeu *jeu)
{
    char cmd;
    
    while (jeu->en_cours && jeu->coups > 0 && !verifier_victoire(jeu)) {
        if (jeu->temps_limite > 0) {
            jeu->temps_restant = jeu->temps_limite -
                                 (int)(time(NULL) - jeu->temps_debut);
            if (jeu->temps_restant <= 0) break;
        }
        
        afficher_jeu(jeu);
        cmd = lire_commande();
        appliquer_commande(jeu, cmd);
        
        if ((cmd == 'p' || cmd == 'P') && jeu->selection &&
            (jeu->curseur_x != jeu->select_x ||
             jeu->curseur_y != jeu->select_y)) {
            gerer_echange_n2(jeu);
        }
    }
    jeu->victoire = verifier_victoire(jeu);
}

// Boucle niveau 3 : même logique que niveau 2
void boucle_jeu_n3(Jeu *jeu)
{
    boucle_jeu_n2(jeu);  // Meme logique, objectifs differents
}

/* ========== SAUVEGARDE ========== */

// Sauvegarde de la partie dans un fichier
void sauvegarder_partie(Sauvegarde *sauv)
{
    FILE *f = fopen(FICHIER_SAUVEGARDE, "w");
    if (f == NULL) return;
    
    fprintf(f, "%s\n", sauv->pseudo);
    fprintf(f, "%d\n", sauv->vies);
    fprintf(f, "%d\n", sauv->niveau_en_cours);
    
    fclose(f);
}

// Chargement de la sauvegarde
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

    // Supprime le retour à la ligne du pseudo
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
    sauv->existe = 1;   // Sauvegarde valide
    return 1;
}

// Supprime le fichier de sauvegarde
void supprimer_sauvegarde(void)
{
    remove(FICHIER_SAUVEGARDE);
}
