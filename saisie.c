#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include "saisie.h"

// Sauvegarde du mode terminal original
struct termios orig_termios;

static void set_raw_mode(void) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= (tcflag_t)~(ICANON | ECHO);   // Mode non canonique, pas d'écho
    raw.c_cc[VMIN] = 0;                // Lecture non bloquante
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void desactiver_mode_canonique(void) {
    set_raw_mode();
}

void reactiver_mode_canonique(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int kbhit(void) {
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

char deplacerCurseur(int *curseur_x, int *curseur_y, int lignes, int colonnes) {
    if (!kbhit()) {
        return 0;
    }

    int c = getchar();

    // Séquence des flèches: ESC [ A/B/C/D
    if (c == '\033') {
        if (kbhit() && getchar() == '[') {
            int dir = getchar();
            switch (dir) {
                case 'A': // Haut
                    if (*curseur_x > 0) (*curseur_x)--;
                    break;
                case 'B': // Bas
                    if (*curseur_x < lignes - 1) (*curseur_x)++;
                    break;
                case 'C': // Droite
                    if (*curseur_y < colonnes - 1) (*curseur_y)++;
                    break;
                case 'D': // Gauche
                    if (*curseur_y > 0) (*curseur_y)--;
                    break;
                default:
                    break;
            }
        }
        return 0;
    }

    // Retourne la touche pour gestion (p/P/q/Q)
    return (char)c;
}
