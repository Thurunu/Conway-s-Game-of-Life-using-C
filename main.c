#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

// Define grid dimensions and game speed
#define WIDTH 150
#define HEIGHT 50

#define SPEED 1000 // Speed in milliseconds for the next generation

// Define characters used for display
#define BACKGROUND '.'
#define CELL 'E'

// Define the possible states of a cell (DEAD or ALIVE)
typedef enum
{
    DEAD,
    ALIVE
} State;

// Structure representing a cell, containing its state and symbol
typedef struct
{
    State state;
    char symbol;
} Cell;

// 2D array representing the grid of cells
Cell grid[HEIGHT][WIDTH] = {0};

// Function to initialize the grid, setting all cells to DEAD
void init_grid()
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            if (rand() % 2 == 0 && rand() % 3 == 0)
            {
                grid[i][j].state = ALIVE;
                continue;
            }
            grid[i][j].state = DEAD; // Initialize all cells as DEAD
        }
    }
}

// Function to generate the next state of the grid based on Game of Life rules
void gen_next()
{
    // new_grid[HEIGHT][WIDTH] = {0};
    Cell new_grid[HEIGHT][WIDTH] = {0};
    
    // Loop through each cell in the grid
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            int alive_count = 0; // Counter to track number of alive neighbors

            // Loop through the neighbors (3x3 grid around the current cell)
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    // Skip the current cell itself
                    if (k == 0 && l == 0)
                        continue;
                    int row = (i + k) % HEIGHT;
                    int col = (j + l) % HEIGHT;

                    if (grid[row][col].state == ALIVE)
                            alive_count++;
                        
                }
            }

            // Apply the rules of the Game of Life based on the count of alive neighbors
            switch (alive_count)
            {
            case 0:
            case 1:
                // Any live cell with fewer than two live neighbors dies (underpopulation)
                new_grid[i][j].state = DEAD;
                break;
            case 2:
            case 3:
                // Any live cell with two or three live neighbors lives on to the next generation
                // Any dead cell with exactly three live neighbors becomes a live cell (reproduction)
                if (grid[i][j].state == DEAD && alive_count == 3)
                {
                    new_grid[i][j].state = ALIVE;
                }
                break;

            default:
                // Any live cell with more than three live neighbors dies (overpopulation)
                new_grid[i][j].state = DEAD;
                break;
            }
        }
    }
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            grid[i][j] = new_grid[i][j];
        }
    }
}

// Function to print the grid to the console
int print_grid()
{
    int alive_count = 0; // Counter to track the number of alive cells
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            // Print the CELL symbol if the cell is alive, otherwise print the BACKGROUND
            if (grid[i][j].state == ALIVE)
            {
                alive_count++;
                printf("%c", CELL);
            }
            else
            {
                printf("%c", BACKGROUND);
            }
        }
        printf("\n"); // New line after each row
    }
    return alive_count; // Return the total number of alive cells
}

// Main function
int main()
{
    srand(time(NULL)); // Seed the random number generator
    init_grid(); // Initialize the grid with all cells set to DEAD

    // Set up an initial configuration of live cells (a small block of cells is made ALIVE)
    for(size_t i = 0; i < WIDTH/5; i++){
        for(size_t j = 0; j < HEIGHT/5; j++){
            grid[j][i].state = ALIVE; // Set some cells in the top-left corner to ALIVE
        }
    }
    system("clear");
    // Loop to run the simulation continuously until no more live cells exist
    while (print_grid() != 0)
    {
        usleep(SPEED * 1000); // Pause the program for a short period (to control the speed of the simulation)
        gen_next();           // Generate the next state of the grid
        system("clear");      // Clear the console to show the updated grid
    }
}
