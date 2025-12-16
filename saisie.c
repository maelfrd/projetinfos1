#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include "saisie.h"

char deplacerCurseur(int *curseur_x, int *curseur_y, int lignes, int colonnes) {
    if(!kbhit()) {
        return 0;
    }
    
    char buf[3];
    int nread = read(STDIN_FILENO, buf, 3);  // ← Lit 3 caractères d'un coup
    
    if(nread == 0) return 0;
    
    // Vérifie si c'est une flèche : ESC [ X
    if(buf[0] == 27 && nread >= 3) {
        if(buf[1] == '[') {
            switch(buf[2]) {
                case 'A': if(*curseur_x > 0) (*curseur_x)--; return 'A';
                case 'B': if(*curseur_x < lignes - 1) (*curseur_x)++; return 'B';
                case 'D': if(*curseur_y > 0) (*curseur_y)--; return 'D';
                case 'C': if(*curseur_y < colonnes - 1) (*curseur_y)++; return 'C';
            }
        }
    }
    
    return buf[0];  // ← Pour P, Q, etc.
}