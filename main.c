#include <stdio.h>

int main() {
   int y, x;


for (y = 0; y <= 20; y++) {
      for (x = 0; x <= 25; x++) {
         if (y == 0 || y == 20) {
          printf("-");

        } else if (x == 0 || x == 25) {
          printf("|");

        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
}


