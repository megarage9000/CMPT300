#include <stdio.h>

int main() {

    // Count digits, white space, and others
    // int c, i, nwhite, nother;

    // int ndigit[10];


    // nwhite = nother = 0;

    // for (i = 0; i < 10; ++i) 
    //     ndigit[i] = 0;

    // while ((c = getchar()) != EOF) 
    //     if (c >= '0' && c <= '9') // chars are small integers, we can find the internal representation via small ints
    //         ++ndigit[c-'0'];
    //     else if (c == ' ' || c == '\n' || c == '\t')
    //         ++nwhite;
    //     else
    //         ++nother;
    // printf("digits= ");
    // for(i = 0; i < 10; ++i)
    //     printf("digit %d = %d ", i, ndigit[i]);
    // printf(", white space = %d, other = %d\n", nwhite, nother);

    // Exercise 1-13.
    // Printing the length of words as a Hortizontal Histogram
    // int c;
    // while((c = getchar()) != EOF) {
    //     if(c == ' ' || c == '\n' || c == '\t')
    //         putchar('\n');
    //     else 
    //         putchar('*');
    // }

    // Exercise 1-14
    // Print histogram of character frequencies
    int c;
    int chars[128];

    for(int i = 0; i < 128; ++i) {
        chars[i] = 0;
    }

    while ((c = getchar()) != EOF) {
        ++chars[c];
    }

    for(int i = 0; i < 128; i++) {
        putchar(i);
        for(int j = 0; j < chars[i]; j++)
            putchar('*');
        putchar('\n');
    }

}