#include <stdlib.h>                                        
#include <string.h>                                         
#include <unistd.h>                                        
#include "moteur.h"                                         
#include "affichage.h"                                     
#include "saisie.h"                                         
#define DELAI_ANIMATION 500000  // met un délai de 0,5 secondes entre chaques animation pour permettre a l'utilisateur de suivre l'avancé 

 
 // Affiche le menu et retourne l’option choisie

int menu(Sauvegarde *sauv)                                  
{
    char c;                                                 
    int selection = 1;                                      
    int nb_options = sauv->existe ? 4 : 3;                  
    int action;                                             
    while (1) {  
    // verifie la présence d’une sauvegarde pour savoir quoi afficher  


        if (sauv->existe) afficher_menu_avec_sauvegarde(selection, sauv);  
        else afficher_menu_sans_sauvegarde(selection);      
        c = lire_menu();                                    
        action = appliquer_commande_menu(c);              
        if (action == -1) { if (selection > 1) selection--; }  
        else if (action == 1) { if (selection < nb_options) selection++; }  
        else if (action == -99) return 0;                   
        else if (action == 0) return selection;             
    }
}

// vérifie s'il s'agit d'un fruit spécial
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

//vérifie si c'est un fruit normal

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

// gestion de l'alignement

int chercher_alignements(Jeu *jeu, int marques[])           
{
    //gestion plateau
    int x, y, k, nb, idx, trouve = 0;                      
    int total = jeu->lignes * jeu->colonnes;                
    char *f;                                                
    for (k = 0; k < total; k++) marques[k] = 0;             
    for (x = 0; x < jeu->lignes; x++) {                     
        for (y = 0; y < jeu->colonnes - 2; y++) {           
            idx = x * jeu->colonnes + y;                    
            f = jeu->plateau[idx];                          
            if (!est_fruit(f)) continue;                    
            nb = 1;                                         
            
            // Compte combien de fruits identiques en dessous

            for (k = y + 1; k < jeu->colonnes; k++) {       
                if (strcmp(jeu->plateau[x * jeu->colonnes + k], f) != 0) break; 
                nb++;                                       
            }
            // vérifie si l'alignement est valide



            if (nb >= 3) {                                  
                for (k = 0; k < nb; k++) marques[x * jeu->colonnes + y + k] = 1;  
                trouve = 1;                                 
            }
        }
    }
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
                for (k = 0; k < nb; k++) marques[(x + k) * jeu->colonnes + y] = 1;  
                trouve = 1;                                 
            }
        }
    }

    // Indique s’il existe au moins un alignement sur le plateau

    
    return trouve;                                         
}

// Supprime toutes les cases marquées et met à jour le score

void supprimer_marques(Jeu *jeu, int marques[])             
{
    int i, idx, total = jeu->lignes * jeu->colonnes;

    // Parcourt toutes les cases du plateau        
    
    for (i = 0; i < total; i++) {                           
        if (marques[i]) {                                   
            idx = index_fruit(jeu, jeu->plateau[i]);        
            if (idx >= 0) jeu->score[idx]++;                
            jeu->plateau[i] = NULL;                         
        }
    }
}

// fait "tomber" les fruits vers le bas

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


// Remplit toutes les cases vides du plateau (pour le niveau 2 possibilité de "joker")


void remplir_vides(Jeu *jeu, int niveau2)
{
    int i, total = jeu->lignes * jeu->colonnes;
    int joker_present = 0;                                  
    if (niveau2) {                                         
        for (i = 0; i < total; i++)                         
            if (est_joker(jeu->plateau[i])) { joker_present = 1; break; }
    }

    //Parcourt toutes les cases du plateau

    for (i = 0; i < total; i++) {                           
        if (jeu->plateau[i] == NULL) {      

        //vérifie s’il y a déjà un joker                
            
            if (niveau2 && !joker_present && (rand() % 50) == 0) {  
                jeu->plateau[i] = EMOJI_JOKER;              
                joker_present = 1;                          
            } else jeu->plateau[i] = jeu->fruits[rand() % NB_FRUITS]; 
        }
    }
}

//PERMUTATION ET CASCADE
void echanger(Jeu *jeu, int x1, int y1, int x2, int y2)     {
    int i1 = x1 * jeu->colonnes + y1;                       
    int i2 = x2 * jeu->colonnes + y2;                       
    char *tmp = jeu->plateau[i1];                           
    jeu->plateau[i1] = jeu->plateau[i2];                    
    jeu->plateau[i2] = tmp;                         
}

//vérifie si la permutation est possible

int permutation_valide(Jeu *jeu, int x1, int y1, int x2, int y2)  
{
    int *marques, ok, total = jeu->lignes * jeu->colonnes;  
    echanger(jeu, x1, y1, x2, y2);                          
    marques = (int *)calloc(total, sizeof(int));  

    // Vérifie si l’échange crée au moins un alignement          
    
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
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        supprimer_marques(jeu, marques);                    
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        faire_tomber(jeu);                                  
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        remplir_vides(jeu, 0);                              
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
    }
    free(marques);                                          
}

//NIVEAU 2/3 COMBINAISONS SPECIALES 

// Vérifie si la case correspond au type recherché

static int correspond(char *c, char *type)                  
{
    if (c == NULL || type == NULL) return 0;                
    if (est_special(c)) return 0;                          
    if (est_joker(c)) return 1;                            
    return (strcmp(c, type) == 0);                          
}

// Compte le nombre de fruits identiques dans une direction donnée

static int compter_dir(Jeu *jeu, int x, int y, int dx, int dy, char *type)  
{
    int nb = 0;                                             
    x += dx; y += dy;                                       
    while (x >= 0 && x < jeu->lignes && y >= 0 && y < jeu->colonnes) { 
        if (!correspond(jeu->plateau[x * jeu->colonnes + y], type)) break; 
        nb++; x += dx; y += dy;                            
    }
    return nb;                                             
}

//détection des combinaisons spéciales

int detecter_speciaux(Jeu *jeu, int marques[], int speciaux[])  
{
    int x, y, i, j, idx, trouve = 0;                        
    int total = jeu->lignes * jeu->colonnes;                
    int haut, bas, gauche, droite, len_h, len_v;           
    char *c, *a, *b, *d;

    // Réinitialise les tableaux marques et speciaux                                    
    
    for (i = 0; i < total; i++) { marques[i] = 0; speciaux[i] = 0; } 

        //Détection alignements en croix / lignes 
    
    for (x = 0; x < jeu->lignes; x++) {                    
        for (y = 0; y < jeu->colonnes; y++) {               
            idx = x * jeu->colonnes + y;                   
            c = jeu->plateau[idx];                         
            if (!est_fruit(c)) continue; 

            // Compte dans les 4 directions

            haut = compter_dir(jeu, x, y, -1, 0, c);        
            bas = compter_dir(jeu, x, y, 1, 0, c);          
            gauche = compter_dir(jeu, x, y, 0, -1, c);      
            droite = compter_dir(jeu, x, y, 0, 1, c);       
            len_v = 1 + haut + bas;                         
            len_h = 1 + gauche + droite;                    
            if (len_v >= 3 && len_h >= 3) { 

            // Marque la ligne verticale

                for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1; 

                // Marque la ligne horizontale 

                for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;  
                speciaux[idx] = 1; marques[idx] = 0; trouve = 1; 
            }

            //6 OU PLUS

            else if (len_h >= 6 || len_v >= 6) {            
                int pos_arcenciel;                          
                if (len_h >= 6) {                          
                    for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;  
                    pos_arcenciel = x * jeu->colonnes + (y - gauche + 2);  
                    speciaux[pos_arcenciel] = 4; marques[pos_arcenciel] = 0;  
                }
                if (len_v >= 6) {                           
                    for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1;  
                    pos_arcenciel = (x - haut + 2) * jeu->colonnes + y;  
                    speciaux[pos_arcenciel] = 4; marques[pos_arcenciel] = 0;  
                }
                trouve = 1;                                 
            }

            //5 ALIGNÉS

            else if (len_h == 5 || len_v == 5) {            
                int pos_boomerang;                         
                if (len_h == 5) {                          
                    for (j = y - gauche; j <= y + droite; j++) marques[x * jeu->colonnes + j] = 1;
                    pos_boomerang = x * jeu->colonnes + (y - gauche + 2); 
                    speciaux[pos_boomerang] = 3; marques[pos_boomerang] = 0;  
                }
                if (len_v == 5) {                           
                    for (i = x - haut; i <= x + bas; i++) marques[i * jeu->colonnes + y] = 1;  
                    pos_boomerang = (x - haut + 2) * jeu->colonnes + y;  
                    speciaux[pos_boomerang] = 3; marques[pos_boomerang] = 0;
                }
                trouve = 1;                                
            }
        }
    }

    //Détection carré

    for (x = 0; x < jeu->lignes - 1; x++) {                 
        for (y = 0; y < jeu->colonnes - 1; y++) {           
            int pos_bombe;                                  
            a = jeu->plateau[x * jeu->colonnes + y];        
            b = jeu->plateau[x * jeu->colonnes + y + 1];    
            c = jeu->plateau[(x+1) * jeu->colonnes + y];    
            d = jeu->plateau[(x+1) * jeu->colonnes + y + 1];

            // Vérifie que les 4 cases sont identiques
            
            if (!est_fruit(a)) continue;                    
            if (!est_fruit(b) || strcmp(a, b) != 0) continue;  
            if (!est_fruit(c) || strcmp(a, c) != 0) continue;  
            if (!est_fruit(d) || strcmp(a, d) != 0) continue; 

            // Marque les 4 cases 
            
            marques[x * jeu->colonnes + y] = 1;             
            marques[x * jeu->colonnes + y + 1] = 1;         
            marques[(x+1) * jeu->colonnes + y] = 1;         
            marques[(x+1) * jeu->colonnes + y + 1] = 1;     
            pos_bombe = (x+1) * jeu->colonnes + (y+1);      
            speciaux[pos_bombe] = 2; marques[pos_bombe] = 0; trouve = 1;  
        }
    }

    // Indique si un spécial a été détecté

    return trouve;                                          
}

// Cherche des alignements en tenant compte des jokers

static int chercher_avec_jokers(Jeu *jeu, int marques[])    
{
    int t, x, y, k, debut, len, trouve = 0, a_type;        
    char *type, *c;                                         
    for (t = 0; t < NB_FRUITS; t++) {                       
        type = jeu->fruits[t];  

        //Recherche horizontale 

        for (x = 0; x < jeu->lignes; x++) {                 
            y = 0;                                          
            while (y < jeu->colonnes) {                     
                debut = y; len = 0; a_type = 0; 

                // Compte la séquence compatible            
                
                while (y < jeu->colonnes) {                 
                    c = jeu->plateau[x * jeu->colonnes + y];  
                    if (!correspond(c, type)) break;        
                    if (!est_joker(c)) a_type = 1;          
                    len++; y++;                             
                }

                // Alignement valide : 3+ avec au moins un vrai fruit

                if (len >= 3 && a_type) {                  
                    for (k = debut; k < debut + len; k++) marques[x * jeu->colonnes + k] = 1;  
                    trouve = 1;                             
                }
                // Avance s'il ne trouve rien

                if (len == 0) y++;                          
            }
        }

        //Recherche verticale
        
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
                    for (k = debut; k < debut + len; k++) marques[k * jeu->colonnes + y] = 1;  
                    trouve = 1;                            
                }
                if (len == 0) x++;                          
            }
        }
    }
    return trouve;                                          
}

//VALIDATION DES PERMUTATIONS NIVEAU 2

int permutation_valide_n2(Jeu *jeu, int x1, int y1, int x2, int y2)  
{
    char *a = jeu->plateau[x1 * jeu->colonnes + y1];        
    char *b = jeu->plateau[x2 * jeu->colonnes + y2];        
    int *marques, *spec, i, ok = 0;                         
    int total = jeu->lignes * jeu->colonnes;                
    if (est_special(a) || est_special(b)) return 1;         
    echanger(jeu, x1, y1, x2, y2);                          
    marques = (int *)calloc(total, sizeof(int));            
    spec = (int *)calloc(total, sizeof(int)); 

    // Détection des alignements et des bonbons spéciaux              
    
    detecter_speciaux(jeu, marques, spec);                  
    chercher_avec_jokers(jeu, marques);                     
    for (i = 0; i < total; i++) if (marques[i] || spec[i]) { ok = 1; break; }  
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

        // Protège les cases où un bonbon spécial va apparaître 
        
        for (i = 0; i < total; i++) if (spec[i]) marques[i] = 0;  

            // Vérifie s’il reste quelque chose à supprimer

        any = 0; for (i = 0; i < total; i++) if (marques[i]) { any = 1; break; }  
        if (!any) break;                                    
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);

        // Crée les bonbons spéciaux         
        
        for (i = 0; i < total; i++) {                       
            if (spec[i] == 1) jeu->plateau[i] = EMOJI_ARBRE;  
            if (spec[i] == 2) jeu->plateau[i] = EMOJI_BOMBE;  
            if (spec[i] == 3) jeu->plateau[i] = EMOJI_BOOMERANG;  
            if (spec[i] == 4) jeu->plateau[i] = EMOJI_ARCENCIEL;  
        }
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        supprimer_marques(jeu, marques);                    
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        faire_tomber(jeu);                                  
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
        remplir_vides(jeu, 1);                              
        afficher_jeu(jeu); usleep(DELAI_ANIMATION);         
    }
    free(marques); free(spec);                              
}

// EFFETS DES BONBONS SPECIAUX 

// Arbre : supprime toute une colonne

void effet_colonne(Jeu *jeu, int y)                        
{
    int x, idx;                                             
    for (x = 0; x < jeu->lignes; x++) {                     
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);
        if (idx >= 0) jeu->score[idx]++;                    
        jeu->plateau[x * jeu->colonnes + y] = NULL;         
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    remplir_vides(jeu, 1);                                  
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
}

// Boomerang : supprime toute une ligne

void effet_ligne(Jeu *jeu, int x)                           
{
    int y, idx;                                             
    for (y = 0; y < jeu->colonnes; y++) {                   
        idx = index_fruit(jeu, jeu->plateau[x * jeu->colonnes + y]);  
        if (idx >= 0) jeu->score[idx]++;                    
        jeu->plateau[x * jeu->colonnes + y] = NULL;         
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    faire_tomber(jeu);                                      
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    remplir_vides(jeu, 1);                                  
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
}

// Bombe : supprime une zone 3x3

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
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    faire_tomber(jeu);                                      
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    remplir_vides(jeu, 1);                                  
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
}

// Arc-en-ciel : supprime tous les fruits d’un type

void effet_arcenciel(Jeu *jeu, char *cible)                 
{
    int i, idx, total = jeu->lignes * jeu->colonnes;        
    char *type = cible;

    // Si cible invalide, choisit un type aléatoire                                     
    
    if (!est_fruit(type)) type = jeu->fruits[rand() % NB_FRUITS];  
    for (i = 0; i < total; i++) {                           
        if (est_fruit(jeu->plateau[i]) && strcmp(jeu->plateau[i], type) == 0) {  
            idx = index_fruit(jeu, jeu->plateau[i]);        
            if (idx >= 0) jeu->score[idx]++;                
            jeu->plateau[i] = NULL;                         
        }
    }
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    faire_tomber(jeu);                                      
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
    remplir_vides(jeu, 1);                                  
    afficher_jeu(jeu); usleep(DELAI_ANIMATION);             
}
