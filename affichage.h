#ifndef AFFICHAGE_H                                         
#define AFFICHAGE_H                                         

#include "structures.h"                                     

/* action sur le terminal  */               
void nettoyer_ecran(void);                                  /* nettoie le terminal */
void pause_entree(void);                          

/* affichage du jeu  */                
void afficher_plateau(Jeu *jeu);                            
void afficher_infos(Jeu *jeu);                              
void afficher_controles(Jeu *jeu);                          
void afficher_jeu(Jeu *jeu);                           
void afficher_fin(Jeu *jeu);           

/* affichage des menus */          
void afficher_titre(void);                                
void afficher_menu_avec_sauvegarde(int selection, Sauvegarde *sauv);  
void afficher_menu_sans_sauvegarde(int selection);          
void afficher_parametres(void);                             

/* avancement du niveau */        
void afficher_niveau_debloque(const char *pseudo, int niveau);  
void afficher_jeu_termine(const char *pseudo);              /* Message jeu termine victoire finale */
void afficher_game_over(const char *pseudo);                /* plus de vies */
void afficher_niveau_echoue(int vies_restantes);            /* Message echec niveau-> vie -1  */
void afficher_nouvelle_partie_warning(const char *ancien_pseudo);    /* Warning ecrasement sauvegarde */
void afficher_nouvelle_partie(void);                     
void afficher_au_revoir(void);                              

/* affichage saisie  */      
char lire_menu(void);                                       /* action d'une touche dans le menu  */
void lire_pseudo(char *pseudo);                             

#endif                                                    
