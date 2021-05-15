#include <stdio.h>

#define LOWER 0     // lower limit
#define UPPER 300   // upper limit
#define STEP 20     // step

#define IN 1 // inside a word
#define OUT 0 // outside a word

int main() {
    printf("hello, world\n");
    
    // Converting Fahrenheit temps to Celsius
    /* print Fahrenheit-Celsius table

    for fahr = 0, 20, ..., 300 */

    /*
        The type int means that the variables listed are integers, by contrast 
        with float, which means floating point, i.e., numbers that may have a 
        fractional part. The range of both int and float depends on the machine 
        you are using; 16-bit ints, which lie between −32768 and +32767, are 
        common, as are 32-bit ints. A float number is typically a 32-bit quantity, 
        with at least six significant digits and magnitude generally between about 
        10^(−38) and 10^(+38)
    */
    float fahr, celsius;

    fahr = LOWER;

    while(fahr < UPPER) {
        celsius = 5 * (fahr - 32) / 9; // Multiplying the number then dividing due to int truncation
        printf("%3.0f\t%6.2f\n", fahr, celsius); // '/t' = tab, you can apply numbers to give a fix width / height
        // (if using floats, the number after number determine decimals)
        fahr = fahr + STEP;
    }

    for(celsius = 0; celsius <= UPPER; celsius = celsius + STEP) {
        printf("%3.0f\t%6.2f\n", ((celsius * 9) / 5) + 32, celsius);
    }

    // // inputting and outputing
    // int c; // We use an int to store returned getchar() values so we can compare
    //        // returned values to EOF constant in stdio.h

    // while((c = getchar())!= EOF) { // Precedence
    //     putchar(c);                // EOF info: https://stackoverflow.com/questions/4358728/end-of-file-eof-in-c
    // }

    // // counting characters
    // double nc;

    // for(nc = 0; getchar() != EOF; ++nc); // Do CTRL + D to simulate EOF
    
    // printf("%.0f\n" ,nc); 

    // // line counting 
    // int c, nl, b, t;

    // nl = 0;
    // while((c = getchar()) != EOF){
    //     if(c == '\n') // Character constant
    //         ++nl;
    //     else if(c == ' ')
    //         ++b;
    //     if(c == '\t')
    //         ++t;
    // }
    // printf("new lines = %d\n", nl);
    // printf("blanks = %d\n", b);
    // printf("tabs = %d\n", t);

    // exercise 1.9 
    // int c, prev;
    // while((c = getchar()) != EOF){
    //     if(prev != ' ')
    //         putchar(c);
    //     prev = c;
    // }
    
    // exercise 1.10
    // int c;
    // while((c = getchar()) != EOF) {
    //     if(c == '\t'){
    //         putchar('\\');
    //         putchar('t');
    //     }
    //     else if(c == '\b'){
    //         putchar('\\');
    //         putchar('b');
    //     }
    //     else if(c == '\\'){
    //         putchar('\\');
    //         putchar('\\');
    //     }
    //      else {
    //          putchar(c);
    //      }
        
    // }
     
     // Word counting

     int c, nl, nw, nc, state;

     state = OUT;

     nl = nw = nc = 0;

     while((c = getchar()) != EOF) {
        ++nc; 

        if(c == '\n') 
            ++nl;

        if (c == ' ' || c == '\n' || c == '\t')
            state = OUT;

        else if(state == OUT) {
            state = IN;
            ++nw;
        }
    }

    printf("%d %d %d\n", nl, nw, nc);
}