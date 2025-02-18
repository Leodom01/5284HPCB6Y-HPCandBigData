#include <stdio.h>
#include <stdlib.h>
#include "grower.h"
#include <unistd.h>
#include <omp.h>

#define N 3000

/**
 * Allocates a 2D grid of size N x N in memory.
 * 
 * @return Pointer to the allocated grid.
 * @throws Exits the program if memory allocation fails and print to stderr.
 */
int **allocate_grid() {
    int **grid = malloc(N * sizeof(int *));
    if (!grid) {
        fprintf(stderr, "Error: Memory allocation failed for grid rows.\n");
        exit(1);
    }
    for (int i = 0; i < N; i++) {
        grid[i] = malloc(N * sizeof(int));
        if (!grid[i]) {
            fprintf(stderr, "Error: Memory allocation failed for grid columns.\n");
            exit(1);
        }
    }
    return grid;
}

/**
 * Initializes all elements of the grid to 0 in a parallelized fashion
 * 
 * @param grid Pointer to the grid to initialize.
 */
void init_grid(int **grid) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }
}

/**
 * Places the predefined pattern (taken from grower.h) on the grid starting at the given position.
 * 
 * @param grid Pointer to the grid.
 * @param start_row Row index where the pattern should spawn on the grid.
 * @param start_col Column index where the pattern should spawn on the grid.
 */
void place_pattern(int **grid, int start_row, int start_col) {
    for (int i = 0; i < GROWER_HEIGHT; i++) {
        for (int j = 0; j < GROWER_WIDTH; j++) {
            grid[start_row + i][start_col + j] = grower[i][j];
        }
    }
}

/**
 * Prints a portion of the grid (used for debugging purposes).
 * 
 * @param grid Pointer to the grid.
 * @param start_row Starting row index for printing.
 * @param start_col Starting column index for printing.
 * @param rows_to_print Number of rows to print.
 * @param cols_to_print Number of columns to print.
 */
void print_grid(int **grid, int start_row, int start_col, int rows_to_print, int cols_to_print) {
    for (int i = start_row; i < start_row + rows_to_print; i++) {
        for (int j = start_col; j < start_col + cols_to_print; j++) {
            if (grid[i][j] == 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/**
 * Frees the memory allocated for a grid.
 * 
 * @param grid Pointer to the grid to free.
 */
void free_grid(int **grid) {
    for (int i = 0; i < N; i++) {
        free(grid[i]);
    }
    free(grid);
}

/**
 * Main function that runs the simulation.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return Exit status code (0 for success, 1 for error).
 */
int main(int argc, char *argv[]) {
    printf("Starting the program...\n");
    double start = omp_get_wtime();

    // Allocate memory for grids
    int **old_grid = allocate_grid();
    int **new_grid = allocate_grid();

    // Initialize both grids to zero
    init_grid(old_grid);
    init_grid(new_grid);

    int start_row = 1500;
    int start_col = 1500;

    // Ensure the pattern fits within the grid bounds
    if (start_row + GROWER_HEIGHT > N || start_col + GROWER_WIDTH > N) {
        fprintf(stderr, "Error: Grower does not fit within the grid bounds.\n");
        free_grid(old_grid);
        free_grid(new_grid);
        return 1;
    }

    // Place the pattern on the grid
    place_pattern(old_grid, start_row, start_col);

    // Initial bounding box for the pattern
    int minRow = start_row; 
    int maxRow = start_row + GROWER_HEIGHT - 1;
    int minCol = start_col;
    int maxCol = start_col + GROWER_WIDTH - 1;

    int iter = 50000;

    // Define the 8 possible neighbor directions
    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    int global_population;

    // Main simulation loop, one cycle per iteration
    for (int curr = 0; curr < iter; curr++) {
        // Reset new_grid to zero
        init_grid(new_grid);

        // Expand the bounding box for neighbor checking
        int scanMinRow = (minRow > 0) ? (minRow - 1) : 0;
        int scanMaxRow = (maxRow < N - 1) ? (maxRow + 1) : (N - 1);
        int scanMinCol = (minCol > 0) ? (minCol - 1) : 0;
        int scanMaxCol = (maxCol < N - 1) ? (maxCol + 1) : (N - 1);

        global_population = 0;

        // Thread-local variables for bounding box and population
        int newMinRow = N, newMaxRow = -1;
        int newMinCol = N, newMaxCol = -1;

        #pragma omp parallel
        {
            int localMinRow = N, localMaxRow = -1;
            int localMinCol = N, localMaxCol = -1;
            int localPop = 0;

            #pragma omp for
            for (int i = scanMinRow; i <= scanMaxRow; i++) {
                for (int j = scanMinCol; j <= scanMaxCol; j++) {
                    int cell = old_grid[i][j];
                    int live_neighbors = 0;

                    // Count live neighbors
                    for (int d = 0; d < 8; d++) {
                        int ni = i + directions[d][0];
                        int nj = j + directions[d][1];
                        if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
                            live_neighbors += old_grid[ni][nj];
                        }
                    }

                    // Apply Game of Life rules
                    if (cell == 1 && (live_neighbors == 2 || live_neighbors == 3)) {
                        new_grid[i][j] = 1;
                        localPop++;
                        if (i < localMinRow) localMinRow = i;
                        if (i > localMaxRow) localMaxRow = i;
                        if (j < localMinCol) localMinCol = j;
                        if (j > localMaxCol) localMaxCol = j;
                    } else if (cell == 0 && live_neighbors == 3) {
                        new_grid[i][j] = 1;
                        localPop++;
                        if (i < localMinRow) localMinRow = i;
                        if (i > localMaxRow) localMaxRow = i;
                        if (j < localMinCol) localMinCol = j;
                        if (j > localMaxCol) localMaxCol = j;
                    }
                }
            }

            // Merge thread-local results into global variables
            #pragma omp critical
            {
                // Merge population
                global_population += localPop;

                // Merge bounding box
                if (localMinRow < newMinRow) newMinRow = localMinRow;
                if (localMaxRow > newMaxRow) newMaxRow = localMaxRow;
                if (localMinCol < newMinCol) newMinCol = localMinCol;
                if (localMaxCol > newMaxCol) newMaxCol = localMaxCol;
            }
        }

        // Update bounding box for the next iteration
        if (newMinRow <= newMaxRow && newMinCol <= newMaxCol) {
            minRow = newMinRow;
            maxRow = newMaxRow;
            minCol = newMinCol;
            maxCol = newMaxCol;
        } else {
            // No cells were alive
            minRow = minCol = 0;
            maxRow = maxCol = -1;
        }
        printf("\nIteration %d; Population %d", iter, global_population);
        // Swap the grids
        int **temp = old_grid;
        old_grid = new_grid;
        new_grid = temp;

        // Break if all cells are dead
        if (global_population == 0) {
            printf("\nAll cells died at iteration %d.\n", curr + 1);
            break;
        }
    }

    

    double end = omp_get_wtime();

    // Free allocated memory
    free_grid(old_grid);
    free_grid(new_grid);

    printf("\nProgram completed successfully. Took %f seconds\n", end - start);
    return 0;
}