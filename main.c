#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void afficher_interface();

int main(void)
{
    afficher_interface();
    return 0;
}

void afficher_interface()
{
    srand(time(NULL)); 

    int lignes, colonnes;

    printf("Choix du nombre de lignes du plateau : ");
    scanf("%d", &lignes);

    printf("Choix du nombre de colonnes du plateau : ");
    scanf("%d", &colonnes);

    // Emojis disponibles
    char *emojis[5] = {"🥭", "🍋", "🍏", "🍇", "🍅"};

    // Contrat à remplir
    char *contrat[5] = {"🥭", "🍋", "🍏", "🍇", "🍅"};

    // Tableau du plateau
    char *face_de_jeux[lignes][colonnes];

    // Compteur de bonbons collectés 
    int nbemoji[5] = {0, 0, 0, 0, 0};

    // Génération du plateau + comptage
    for(int x = 0; x < lignes; x++)
    {
        for(int y = 0; y < colonnes; y++)
        {
            int r = rand() % 5;
            face_de_jeux[x][y] = emojis[r];
        }
    }

    int nbcoups = 50;
    int temps = 120;

    printf("\nPlateau généré :\n\n");

    // Bordure du haut
    
    for(int y = 0; y < colonnes; y++)
        printf("---");
    printf("-\n");

    // Corps du plateau
    for(int x = 0; x < lignes; x++)
    {
        printf("| ");
        for(int y = 0; y < colonnes; y++)
        {
            printf("%s ", face_de_jeux[x][y]);
        }
        printf("|\n");
    }

    // Bordure du bas
    
    for(int y = 0; y < colonnes; y++)
        printf("---");
    printf("-\n");

   
    //     INFORMATIONS DU NIVEAU

    printf("\nIl vous reste : %d coups\n", nbcoups);
    printf("Temps : %d secondes\n", temps);

    printf("\nContrat à remplir : ");
    for(int i = 0; i < 5; i++)
    {
        printf("%s ", contrat[i]);
    }
     
    printf("\n");

    // NIVEAU RENDU = total de bonbons collectés
    printf("\nNiveau rendu (bonbons collectés) :\n");
    for(int i = 0; i < 5; i++)
    {
        printf("%s : %d  ", emojis[i], nbemoji[i]);
    }
    printf("\n");
}





