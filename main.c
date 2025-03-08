/***
 * Chris Toala & Eric Rivera
 * Project 2: Multithreaded Programming
 * Dr. Yoon
 * 2/4/25
 * Description: XXX
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE 9

typedef struct {
    int row;
    int col;
} parameters;

int main(int argc, char** argv)
{
    char *filename = "input.txt";
    FILE *file = fopen(filename, "r");
    int numSud;
    printf("BOARD STATE IN: %s\n", filename);
    while ((numSud = fgetc(file)) != EOF) 
        putchar(numSud);
        printf("\n");
        printf("SOLUTION: XXX\n");
    fclose(file);




    
    return 0;
}