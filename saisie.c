#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include "saisie.h"

struct termios orig_termios;

void desactiver_mode_canonique() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
}

void reactiver_mode_canonique() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

int kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char deplacerCurseur(int *curseur_x, int *curseur_y, int lignes, int colonnes) {
    if(!kbhit()) {
        return 0;
    }
    
    char buf[3];
    int nread = read(STDIN_FILENO, buf, 3);
    
    if(nread == 0) return 0;
    
    if(buf[0] == 27 && nread >= 3) {
        if(buf[1] == '[') {
            switch(buf[2]) {
                case 'A':
                    if(*curseur_x > 0) (*curseur_x)--;
                    return 'A';
                case 'B':
                    if(*curseur_x < lignes - 1) (*curseur_x)++;
                    return 'B';
                case 'D':
                    if(*curseur_y > 0) (*curseur_y)--;
                    return 'D';
                case 'C':
                    if(*curseur_y < colonnes - 1) (*curseur_y)++;
                    return 'C';
            }
        }
    }
    
    return buf[0];
}