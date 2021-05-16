#include <stdio.h>

// THIS FILE IS MODIFIED TO USE EXTERN

#define MAXLINE 1000 

int max;

char line[MAXLINE];
char longest[MAXLINE];

int getLine(void);

void copy(void);

int main() {
    int len;
    extern int max;
    extern char longest[];

    max = 0;
    while((len = getLine()) > 0) {
        if(len > max) {
            max = len;
            copy();
        }
    }

    if (max > 0)
        printf("%s", longest);
    return 0;
}

int getLine(void) {
    int c, i;

    extern char maxLine[]; // Adding extern refers to the "external definition"

    for(i = 0; i < MAXLINE- 1 && (c=getchar()) != EOF && c != '\n'; i++)
        line[i] = c;

    if(c == '\n') {
        line[i] = c;
        ++i;
    }

    line[i] = '\0'; // Marking the end of a string
    return i;
}

void copy(void) {
    int i = 0;

    extern char line[], longest[]; // Adding extern refers to the "external definition"
    i = 0;

    while ((longest[i] = line[i]) != '\0')
        ++i;
}