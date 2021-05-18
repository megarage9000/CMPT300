#include <stdio.h>

/*
    From: https://www.cprogramming.com/tutorial/bitwise_operators.html
    Bitwise operator examples:

    We will use a char to store an "item" in use. Char's are
    typically stored in 2 bytes: 

    00000000 

    We will manipulate these bytes to determine which "item" is 
    in use, and we will have only 8 items. 
    
    These items are numbered from 0 - 7, and these will determine
    where the 1 is located on the char from the right. For example, 
    5 will be:

    00100000
*/
char in_use = 1 << 5;

void swap(int * a, int * b);
int main() {
    for (int i = 0; i < 8; i++) {
        if(in_use & 1 << i) {
            printf("Current car in use = %d\n", i);
        }
    }

    int a = 3;
    int b = 2;
    printf("Nums before swap: a = %d, b = %d \n", a, b);
    swap(&a, &b);
    printf("Nums after swap: a = %d, b = %d \n", a, b);
}

void swap(int * a, int * b) {
    *a = *a ^ *b;
    // Now, we can recover *a_orig by applying *a XOR *b_orig
    *b = *a ^ *b;
    // So we have the following situation:
    // The value originally stored in *a, a_orig, is now in *b
    // and *a still stores a_orig ^ b_orig
    // This means that we can recover the value of b_orig by applying 
    // the XOR operation to *a and a_orig.  Since *b stores a_orig..
    *a = *a ^ *b;
}

/*
    ex. 
    num = 1
    in_use = 00000000 (in bits)

    1 << 1 -> 00000010

     00000000
    |00000010
    __________
   = 00000010
*/

void set_in_use(int num) { 
    in_use = in_use | 1 << num;
}

/*
    ex. 
    num = 3
    in_use = 00001000 (in bits)

    ~(1 << 3) -> ~(00001000) -> 11110111

     00001000
    &11110111
    __________
   = 00000000
*/

void set_unused(int num) {
    in_use = in_use & ~(1<< num);
}