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
#define SQUARE 3

typedef struct {
    int row;
    int col;
} parameters;

int main(int argc, char** argv)
{
    char *filename = "input.txt";
    FILE *file = fopen(filename, "r");
    int sudoko[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) { // loads in the sudoku into a 2d array
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoko[i][j]);
        }
    }
    fclose(file);

    printf("BOARD STATE IN: %s\n", filename);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudoko[i][j]);
        }
        printf("\n");
    }
    printf("SOLUTION: XXX\n");
    return 0;
}