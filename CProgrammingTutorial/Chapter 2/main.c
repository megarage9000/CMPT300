#include <stdio.h>

int main() {
    char in_use = 0 << 5;

    for (int i = 0; i < 8; i++) {
        if(in_use & 1 << i) {
            printf("Current car in use = %d", i);
        }
    }
    


}