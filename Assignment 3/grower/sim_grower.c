#include <stdio.h>
#include <stdlib.h>
#include "grower.h"
#include <unistd.h>
#include <omp.h>

#define N 3000

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

// Initialize the entire grid to 0
void init_grid(int **grid) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = 0;
        }
    }
}

void place_pattern(int **grid, int start_row, int start_col) {
    for (int i = 0; i < GROWER_HEIGHT; i++) {
        for (int j = 0; j < GROWER_WIDTH; j++) {
            grid[start_row + i][start_col + j] = grower[i][j];
        }
    }
}

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

void free_grid(int **grid) {
    for (int i = 0; i < N; i++) {
        free(grid[i]);
    }
    free(grid);
}

int main(int argc, char *argv[]){
    printf("Starting the program...\n");
    double start = omp_get_wtime();

    int **old_grid = allocate_grid();
    int **new_grid = allocate_grid();

    // Initialize both grids to zero
    init_grid(old_grid);
    init_grid(new_grid);

    int start_row = 1500;
    int start_col = 1500;

    // Safety check: ensure the pattern fits
    if (start_row + GROWER_HEIGHT > N || start_col + GROWER_WIDTH > N) {
        fprintf(stderr, "Error: Grower does not fit within the grid bounds.\n");
        free_grid(old_grid);
        free_grid(new_grid);
        return 1;
    }

    place_pattern(old_grid, start_row, start_col);

    // Initial bounding box for the pattern
    int minRow = start_row; 
    int maxRow = start_row + GROWER_HEIGHT - 1;
    int minCol = start_col;
    int maxCol = start_col + GROWER_WIDTH - 1;

    int iter = 5000;

    // 8 possible neighbor directions
    static const int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (int curr = 0; curr < iter; curr++) {

        // Zero out new_grid to avoid stale cells (especially if bounding box shrinks)
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                new_grid[i][j] = 0;
            }
        }

        // Expand bounding box by 1 so we consider neighbors
        int scanMinRow = (minRow > 0)       ? (minRow - 1) : 0;
        int scanMaxRow = (maxRow < N - 1)   ? (maxRow + 1) : (N - 1);
        int scanMinCol = (minCol > 0)       ? (minCol - 1) : 0;
        int scanMaxCol = (maxCol < N - 1)   ? (maxCol + 1) : (N - 1);

        int global_population = 0;

        // We'll store boundaries in global variables but update them via local variables in each thread
        int newMinRow = N,    newMaxRow = -1;
        int newMinCol = N,    newMaxCol = -1;

        #pragma omp parallel
        {
            // Each thread's local bounding box
            int localMinRow = N,    localMaxRow = -1;
            int localMinCol = N,    localMaxCol = -1;
            int localPop = 0;

            #pragma omp for
            for (int i = scanMinRow; i <= scanMaxRow; i++) {
                for (int j = scanMinCol; j <= scanMaxCol; j++) {
                    int cell = old_grid[i][j];
                    int live_neighbors = 0;

                    // Count neighbors
                    for (int d = 0; d < 8; d++) {
                        int ni = i + directions[d][0];
                        int nj = j + directions[d][1];
                        if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
                            live_neighbors += old_grid[ni][nj];
                        }
                    }

                    // Apply Game of Life rules
                    if (cell == 1) { // alive
                        if (live_neighbors == 2 || live_neighbors == 3) {
                            new_grid[i][j] = 1;
                            localPop++;
                            // Update local bounding box
                            if (i < localMinRow) localMinRow = i;
                            if (i > localMaxRow) localMaxRow = i;
                            if (j < localMinCol) localMinCol = j;
                            if (j > localMaxCol) localMaxCol = j;
                        }
                    } else { // dead
                        if (live_neighbors == 3) {
                            new_grid[i][j] = 1;
                            localPop++;
                            // Update local bounding box
                            if (i < localMinRow) localMinRow = i;
                            if (i > localMaxRow) localMaxRow = i;
                            if (j < localMinCol) localMinCol = j;
                            if (j > localMaxCol) localMaxCol = j;
                        }
                    }
                }
            } 

            // Merge local bounding box + population into the global variables
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

        printf("\nIteration %d; Population %d", curr + 1, global_population);

        // Update bounding box for next iteration
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

        // Swap old_grid and new_grid
        int **temp = old_grid;
        old_grid = new_grid;
        new_grid = temp;

        // If population is zero, we can break out early to save time
        if (global_population == 0) {
            printf("\nAll cells died at iteration %d.\n", curr + 1);
            break;
        }
    }

    double end = omp_get_wtime();

    // Cleanup
    free_grid(old_grid);
    free_grid(new_grid);

    printf("\nProgram completed successfully. Took %f seconds\n", end - start);
    return 0;
}