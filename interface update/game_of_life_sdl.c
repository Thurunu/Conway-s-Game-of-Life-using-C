#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Define grid dimensions
#define WIDTH 150
#define HEIGHT 100
#define SPEED 75

// Define the size of each cell in pixels
#define CELL_SIZE 10

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

// Function to initialize the grid
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
            else
            {
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

// Function to draw the grid using SDL
void draw_grid(SDL_Renderer *renderer)
{
    // Set background color (black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Set color for live cells (white)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            if (grid[i][j].state == ALIVE)
            {
                SDL_Rect cell_rect = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
    }

    SDL_RenderPresent(renderer); // Present the updated frame
}

int main(int argc, char *argv[])
{
    srand(time(NULL)); // Seed the random number generator

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Ask user for how long to run the simulation (in minutes)
    int minutes = 3;
    

    // Convert minutes to milliseconds
    int run_time = minutes * 60 * 1000;

    init_grid(); // Initialize the grid with random cells

    // Get the starting time
    Uint32 start_time = SDL_GetTicks();

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // Main loop
    while (!quit)
    {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        // Check if the time has passed
        if (SDL_GetTicks() - start_time >= run_time)
        {
            printf("Time's up! Exiting simulation...\n");
            quit = 1;
        }

        draw_grid(renderer);  // Draw the grid
        gen_next();           // Calculate the next generation
        SDL_Delay(SPEED);        // Delay to control the speed of simulation
    }

    // Destroy the renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
