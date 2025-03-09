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

/* 
 * This structure defines the parameters needed to check 
 * a row, a column, or a 3x3 subgrid of the Sudoku board.
 */
typedef struct {
    int row;
    int col;
    int checkType;  /* 0 for row, 1 for column, 2 for 3x3 subgrid */
} parameters;

/* 
 * sudWorker:
 * Checks a single row, column, or 3x3 subgrid based on the 
 * parameters passed in. Returns 1 if valid, 0 otherwise.
 */
int sudWorker(parameters *data, int sudoku[SIZE][SIZE]) {
    /* 
     * make an array of size 10 so we can track digits 1 through 9. 
     * Using index 0 for convenience.
     */
    int found[SIZE + 1] = {0};
    
    /* Check a single row */
    if (data->checkType == 0) {
        int r = data->row;
        for (int c = 0; c < SIZE; c++) {
            int val = sudoku[r][c];
            /* If the value is out of range or already found, it's invalid. */
            if (val < 1 || val > 9 || found[val]) {
                return 0;
            }
            found[val] = 1;
        }
    }
    /* Check a single column */
    else if (data->checkType == 1) {
        int c = data->col;
        for (int r = 0; r < SIZE; r++) {
            int val = sudoku[r][c];
            if (val < 1 || val > 9 || found[val]) {
                return 0;
            }
            found[val] = 1;
        }
    }
    /* Check a 3x3 subgrid */
    else if (data->checkType == 2) {
        int startRow = data->row;
        int startCol = data->col;
        for (int r = 0; r < SQUARE; r++) {
            for (int c = 0; c < SQUARE; c++) {
                int val = sudoku[startRow + r][startCol + c];
                if (val < 1 || val > 9 || found[val]) {
                    return 0;
                }
                found[val] = 1;
            }
        }
    }

    /* If no duplicates or out-of-range values were found, return valid. */
    return 1;
}

/*
 * isSudValidSingle:
 * Performs a basic check of the entire Sudoku board 
 * runs checks through the worker
 * in a single-threaded. Returns 1 if all checks pass, 0 otherwise.
 */
int isSudValidSingle(int sudoku[SIZE][SIZE]) {
    /* Check all rows */
    for (int i = 0; i < SIZE; i++) {
        parameters rowParam = { .row = i, .col = 0, .checkType = 0 };
        if (!sudWorker(&rowParam, sudoku)) {
            return 0; /* Invalid row */
        }
    }

    /* Check all columns */
    for (int j = 0; j < SIZE; j++) {
        parameters colParam = { .row = 0, .col = j, .checkType = 1 };
        if (!sudWorker(&colParam, sudoku)) {
            return 0; /* Invalid column */
        }
    }

    /* Check all 3x3 subgrids */
    for (int block = 0; block < SIZE; block++) {
        int startRow = (block / 3) * 3;
        int startCol = (block % 3) * 3;
        parameters gridParam = { .row = startRow, .col = startCol, .checkType = 2 };
        if (!sudWorker(&gridParam, sudoku)) {
            return 0; /* Invalid 3x3 subgrid */
        }
    }

    /* reaching here means everything is valid */
    return 1;
}

/*
 * main:
 * 1) Read the Sudoku board from input.txt
 * 2) Checks if the user provided an option argument
 * 3) If option == 1, performs the single-threaded check
 * 4) Prints YES or NO depending on validity
 */
int main(int argc, char** argv) {
    /* Check if the user provided at least one argument (the option). */
    if (argc < 2) {
        printf("Usage: %s <option>\n", argv[0]);
        printf("Example: %s 1\n", argv[0]);
        return 1;
    }

    /* Parse the option from the first command-line argument. */
    int option = atoi(argv[1]);

    /* Open the input file */
    char *filename = "input.txt";
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: could not open %s\n", filename);
        return 1;
    }

    /* Create a 9x9 Sudoku array and read values from the file */
    int sudoku[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fscanf(file, "%d", &sudoku[i][j]);
        }
    }
    fclose(file);

    /* Print the board state */
    printf("BOARD STATE IN: %s\n", filename);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", sudoku[i][j]);
        }
        printf("\n");
    }

    /* only option 1 (single-thread) is implemented for now */
    if (option == 1) {
        int valid = isSudValidSingle(sudoku);
        printf("SOLUTION: %s\n", valid ? "YES" : "NO");
    }
    else {
        printf("Option %d not yet implemented.\n", option);
    }

    return 0;
}