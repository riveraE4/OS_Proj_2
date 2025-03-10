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
#include <pthread.h> 

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

/* global arrays for multi-threaded approach */
int globalSudoku[SIZE][SIZE];
int threadResults[11] = {0};


/* 
 * sudWorker:
 * Checks a single row, column, or 3x3 subgrid based on the 
 * parameters passed in. Returns 1 if valid, 0 otherwise.
 */
int sudWorker(parameters *data, int sudoku[SIZE][SIZE]) {
    /* 
     * make an array of size 10 so we can track digits 1 through 9. 
     * Using index 0 for start
     */
    int found[SIZE + 1] = {0};
    
    /* check only single rows */
    if (data->checkType == 0) {
        int r = data->row;
        for (int c = 0; c < SIZE; c++) {
            int val = sudoku[r][c];
            /* here if the val is out of range or already found, it's invalid. */
            if (val < 1 || val > 9 || found[val]) {
                return 0;
            }
            found[val] = 1;
        }
    }
    /* checking for single columns */
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
    /* checking the 3x3 grids */
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
 * threadParameters:
 * holds parameters + index for thread assignment
 */
typedef struct {
    parameters param;
    int index;
} threadParameters;

/* 
 * mtThreadWorker:
 * thread worker calls sudWorker on the globalSudoku
 * sets threadResults[index] to 1 if valid, 0 otherwise
 */
void *mtThreadWorker(void *arg) {
    threadParameters *tpar = (threadParameters*)arg;
    int valid = sudWorker(&tpar->param, globalSudoku);

    if (valid == 1) {
        threadResults[tpar->index] = 1;
    } else {
        threadResults[tpar->index] = 0;
    }

    pthread_exit(NULL);
}

/*
 * isSudValidMulti:
 * creates multiple threads to check rows, columns, and subgrids
 * 1 thread checks all rows, 1 thread checks all columns, 9 threads for subgrids
 * returns 1 if all valid, 0 otherwise
 */
int isSudValidMulti() {
    pthread_t threads[11];
    threadParameters tparams[11];

    /* reset threadResults to 0 */
    for (int i = 0; i < 11; i++) {
        threadResults[i] = 0;
    }

    /* create thread for checking all rows (index 0) */
    tparams[0].param.row = 0;
    tparams[0].param.col = 0;
    tparams[0].param.checkType = 0;
    tparams[0].index = 0;
    pthread_create(&threads[0], NULL, mtThreadWorker, (void*)&tparams[0]);

    /* create thread for checking all columns (index 1) */
    tparams[1].param.row = 0;
    tparams[1].param.col = 0;
    tparams[1].param.checkType = 1;
    tparams[1].index = 1;
    pthread_create(&threads[1], NULL, mtThreadWorker, (void*)&tparams[1]);

    /* create threads for each 3x3 subgrid (indices 2 to 10) */
    for (int i = 0; i < 9; i++) {
        int startRow = (i / 3) * 3;
        int startCol = (i % 3) * 3;
        tparams[i + 2].param.row = startRow;
        tparams[i + 2].param.col = startCol;
        tparams[i + 2].param.checkType = 2;
        tparams[i + 2].index = i + 2;
        pthread_create(&threads[i + 2], NULL, mtThreadWorker, (void*)&tparams[i + 2]);
    }

    /* wait for all 11 threads to complete */
    for (int i = 0; i < 11; i++) {
        pthread_join(threads[i], NULL);
    }

    /* check threadResults array to see if everything was valid */
    for (int i = 0; i < 11; i++) {
        if (threadResults[i] == 0) {
            return 0; /* if any check is invalid, return 0 */
        }
    }

    return 1; /* all checks passed */
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
    /* minimal changes: option 2 calls multi-threaded function */
    else if (option == 2) {
        /* copy sudoku board into the globalSudoku before multi-threading */
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                globalSudoku[i][j] = sudoku[i][j];
            }
        }
        int valid = isSudValidMulti();
        printf("SOLUTION: %s\n", valid ? "YES" : "NO");
    }
    else {
        printf("Option %d not yet implemented.\n", option);
    }

    return 0;
}