#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define WIDTH 100
#define HEIGHT 50

#define SPEED 10 // Speed in milliseconds

#define CELL 'E'

// Define the possible states of a cell
typedef enum
{
    DEAD,
    ALIVE
} State;

// Structure representing a cell
typedef struct
{
    State state;
} Cell;

// 2D array representing the grid
Cell grid[HEIGHT][WIDTH] = {0};

// Function to initialize the grid with random cells alive
void init_grid()
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            if(rand() %2 == 0 && rand() % 4 == 0){
                grid[i][j].state = ALIVE;
            } else {
                grid[i][j].state = DEAD;
            }
        }
    }
}

// Function to generate the next state of the grid
void gen_next()
{
    Cell next_grid[HEIGHT][WIDTH] = {0};

    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            int alive_count = 0;

            // Loop through neighbors
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    if (k == 0 && l == 0)
                        continue;
                    int row = (i + k + HEIGHT) % HEIGHT;
                    int col = (j + l + WIDTH) % WIDTH;

                    if (grid[row][col].state == ALIVE)
                        alive_count++;
                }
            }

            // Apply Game of Life rules
            if (grid[i][j].state == ALIVE && (alive_count == 2 || alive_count == 3))
            {
                next_grid[i][j].state = ALIVE;
            }
            else if (grid[i][j].state == DEAD && alive_count == 3)
            {
                next_grid[i][j].state = ALIVE;
            }
            else
            {
                next_grid[i][j].state = DEAD;
            }
        }
    }

    // Copy next generation back to the main grid
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            grid[i][j] = next_grid[i][j];
        }
    }
}

// Function to draw the grid using ncurses
void draw_grid()
{
    clear(); // Clear the screen before redrawing
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            // If cell is alive, draw it, otherwise leave it blank
            if (grid[i][j].state == ALIVE)
            {
                mvaddch(i, j, CELL); // Move to position and add character
            }
            else
            {
                mvaddch(i, j, ' '); // Empty space for dead cells
            }
        }
    }
    refresh(); // Refresh the screen to show changes
}

int main()
{
    initscr();      // Initialize ncurses mode
    curs_set(FALSE); // Hide the cursor

    init_grid(); // Initialize the grid with random cells

    // Main game loop
    while (1)
    {
        draw_grid();     // Draw the current state of the grid
        gen_next();      // Calculate the next generation
        usleep(SPEED * 1000); // Control the speed of the simulation
    }

    endwin(); // End ncurses mode

    return 0;
}
