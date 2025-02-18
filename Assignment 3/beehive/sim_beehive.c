#include <stdio.h>
#include <stdlib.h>
#include "beehive.h"
#include <unistd.h>
#include <omp.h>

int **allocate_grid() {
    int **grid = malloc(3000 * sizeof(int *));
    if (!grid) {
        fprintf(stderr, "Error: Memory allocation failed for grid rows.\n");
        exit(1);
    }

    for (int i = 0; i < 3000; i++) {
        grid[i] = malloc(3000 * sizeof(int));
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
    for (int i = 0; i < 3000; i++) {
        for (int j = 0; j < 3000; j++) {
            grid[i][j] = 0;
        }
    }
}

// Copy the pattern into a specific location in the grid
void place_pattern(int **grid, int start_row, int start_col) {
    for (int i = 0; i < BEEHIVE_HEIGHT; i++) {
        for (int j = 0; j < BEEHIVE_WIDTH; j++) {
            grid[start_row + i][start_col + j] = beehive[i][j];
        }
    }
}

// Print a subsection of the grid
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

// Free the allocated memory for a 3000x3000 grid
void free_grid(int **grid) {
    for (int i = 0; i < 3000; i++) {
        free(grid[i]);
    }
    free(grid);
}

int main(int argc, char *argv[]){
    double start, end; 
    int new_population = 0;
    printf("Starting the program...\n");

    // Allocate two grids for double buffering
    int **old_grid = allocate_grid();
    int **new_grid = allocate_grid();

    // Initialize both grids to zero
    init_grid(old_grid);
    init_grid(new_grid);

    int start_row = 1500;
    int start_col = 1500;

    // Safety check: ensure the pattern fits
    if (start_row + BEEHIVE_HEIGHT > 3000 || start_col + BEEHIVE_WIDTH > 3000) {
        fprintf(stderr, "Error: Pattern does not fit within the grid bounds.\n");
        free_grid(old_grid);
        free_grid(new_grid);
        return 1;
    }

    // Place the pattern in old_grid
    place_pattern(old_grid, start_row, start_col);

    // printf("Initial grid (partial view):\n");
    // print_grid(old_grid, start_row - 50, start_col - 50, GROWER_HEIGHT + 100, GROWER_WIDTH + 100);

    int iter = 5000;
    start = omp_get_wtime();
    for (int curr = 0; curr < iter; curr++) {
        // For each cell, count neighbors from old_grid and write the new state to new_grid
        new_population = 0;
        
        #pragma omp parallel for reduction(+:new_population)
        for (int i = 1; i < 2999; i++) { // dead border
            for (int j = 1; j < 2999; j++) {// dead border
                int cell = old_grid[i][j];
                int live_neighbors = 0;

                // The 8 possible neighbor directions
                int directions[8][2] = {
                    {-1, -1}, {-1, 0}, {-1, 1},
                    { 0, -1},          { 0, 1},
                    { 1, -1}, { 1, 0}, { 1, 1}
                };

                // Count neighbors in old_grid
                for (int d = 0; d < 8; d++) {
                    int ni = i + directions[d][0];
                    int nj = j + directions[d][1];
                    if (ni >= 0 && ni < 3000 && nj >= 0 && nj < 3000) {
                        live_neighbors += old_grid[ni][nj];
                    }
                }

                if (cell == 1) { // alive
                    if (live_neighbors == 2 || live_neighbors == 3) {
                        new_grid[i][j] = 1;
                        new_population ++;
                    } else {
                        new_grid[i][j] = 0;
                    }
                } else { // dead
                    if (live_neighbors == 3) {
                        new_grid[i][j] = 1;
                        new_population ++;
                    } else {
                        new_grid[i][j] = 0;
                    }
                }
            }
        }
        // print_grid(new_grid, start_row - 50, start_col - 50, GROWER_HEIGHT + 50, GROWER_WIDTH+50);
        printf("\nIteration %d; Population %d", curr + 1, new_population);
 
        // Swap old_grid and new_grid pointers
        int **temp = old_grid;
        old_grid = new_grid;
        new_grid = temp;
    }

    end = omp_get_wtime();
    // Free both grids
    free_grid(old_grid);
    free_grid(new_grid);

    printf("Program completed successfully. Took %f seconds\n", end-start);
    return 0;
}