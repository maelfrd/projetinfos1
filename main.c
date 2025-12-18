#include <stdio.h>
#include <string.h>

#include "structures.h"
#include "gestion_partie.h"
#include "affichage.h"

/* Lit une ligne et enlève le '\n' final. */
static void lire_ligne(char *buf, int taille)
{
    int len;

    if (fgets(buf, taille, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }

    len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
}

static char lire_commande_menu(void)
{
    char buf[64];

    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return 'x';
    }

    if (buf[0] == '\n' || buf[0] == '\0') {
        return '\n';
    }

    return buf[0]; /* minuscules recommandées */
}

static int menu_principal(int partie_disponible)
{
    const char *items[3] = { "Reprendre partie", "Nouvelle partie", "Parametres" };
    int selection = 0;

    while (1) {
        int i;
        char c;

        nettoyer_ecran();

        printf("============== MENU ==============\n\n");

        for (i = 0; i < 3; i++) {
            if (i == 0 && !partie_disponible) {
                if (i == selection) printf(" > %s (indisponible)\n", items[i]);
                else                printf("   %s (indisponible)\n", items[i]);
            } else {
                if (i == selection) printf(" > %s\n", items[i]);
                else                printf("   %s\n", items[i]);
            }
        }

        printf("\nCommandes : z=haut, s=bas, Entrée=valider, x=quitter\n");
        printf("----------------------------------\n");

        c = lire_commande_menu();

        if (c == 'z') {
            if (selection > 0) selection--;
        } else if (c == 's') {
            if (selection < 2) selection++;
        } else if (c == 'x') {
            return -1;
        } else if (c == '\n') {
            if (selection == 0 && !partie_disponible) {
                printf("\nAucune partie a reprendre.\n");
                pause_entree();
            } else {
                return selection;
            }
        }
    }
}

static void demander_pseudo(char *pseudo, int taille)
{
    nettoyer_ecran();
    printf("=========== NOUVELLE PARTIE ===========\n\n");
    printf("Entre ton pseudo :\n> ");

    lire_ligne(pseudo, taille);

    if (pseudo[0] == '\0') {
        strcpy(pseudo, "Joueur");
    }
}

static int menu_niveaux(const char *pseudo)
{
    const char *items[3] = { "Niveau 1", "Niveau 2 (special)", "Niveau 3 (non disponible)" };
    int selection = 0;

    while (1) {
        int i;
        char c;

        nettoyer_ecran();
        printf("============= CHOIX DU NIVEAU =============\n\n");
        printf("Pseudo : %s\n\n", pseudo);

        for (i = 0; i < 3; i++) {
            if (i == selection) printf(" > %s\n", items[i]);
            else                printf("   %s\n", items[i]);
        }

        printf("\nCommandes : z=haut, s=bas, Entrée=valider, x=retour menu\n");
        printf("------------------------------------------\n");

        c = lire_commande_menu();

        if (c == 'z') {
            if (selection > 0) selection--;
        } else if (c == 's') {
            if (selection < 2) selection++;
        } else if (c == 'x') {
            return -1;
        } else if (c == '\n') {
            return selection;
        }
    }
}

static void ecran_parametres(void)
{
    nettoyer_ecran();
    printf("============== PARAMETRES ==============\n\n");
    printf("Ecran vide pour le moment.\n");
    pause_entree();
}

int main(void)
{
    JeuState jeu = {0};
    int partie_initialisee = 0;

    char pseudo[32];
    pseudo[0] = '\0';

    while (1) {
        int choix = menu_principal(partie_initialisee);

        if (choix == -1) {
            if (partie_initialisee) {
                libererPartie(&jeu);
            }
            nettoyer_ecran();
            printf("Au revoir.\n");
            return 0;
        }

        if (choix == 2) {
            ecran_parametres();
            continue;
        }

        if (choix == 1) {
            if (partie_initialisee) {
                libererPartie(&jeu);
                partie_initialisee = 0;
            }

            demander_pseudo(pseudo, (int)sizeof(pseudo));

            while (1) {
                int niveau = menu_niveaux(pseudo);

                if (niveau == -1) {
                    break;
                }

                if (niveau == 0) {
                    initialiserPartie(&jeu, 10, 10);
                    partie_initialisee = 1;

                    boucleJeu(&jeu);
                    afficherFinPartie(&jeu);
                    break;
                } else if (niveau == 1) {
                    initialiserPartie(&jeu, 10, 10);
                    partie_initialisee = 1;

                    boucleJeuNiveau2(&jeu);
                    afficherFinPartie(&jeu);
                    break;
                } else {
                    printf("\nCe niveau n'est pas encore disponible.\n");
                    pause_entree();
                }
            }

            continue;
        }

        if (choix == 0) {
            if (!partie_initialisee) {
                printf("\nAucune partie a reprendre.\n");
                pause_entree();
                continue;
            }

            jeu.continuer = 1;

            /* Reprendre : on reprend le niveau courant.
               (Pour simplifier : on reprend en niveau 1 ; si tu veux,
                on peut mémoriser le niveau plus tard.) */
            boucleJeu(&jeu);
            afficherFinPartie(&jeu);
            continue;
        }
    }
}
