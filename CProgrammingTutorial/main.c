#include <stdio.h>

#define LOWER 0     // lower limit
#define UPPER 300   // upper limit
#define STEP 20     // step

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
}