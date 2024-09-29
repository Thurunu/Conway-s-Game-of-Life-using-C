#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cuda_runtime.h>

// Define grid dimensions and game speed
#define WIDTH 190
#define HEIGHT 50

#define SPEED 50 // Speed in milliseconds for the next generation

// Define characters used for display
#define BACKGROUND '.'
#define CELL '0'

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

// Host 2D array representing the grid of cells
Cell h_grid[HEIGHT][WIDTH] = { { {DEAD, BACKGROUND} } };

// CUDA Kernel to generate the next state of the grid based on Game of Life rules
__global__ void gen_next(Cell *d_grid, Cell *d_next_grid)
{
    int i = blockIdx.y * blockDim.y + threadIdx.y;
    int j = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < HEIGHT && j < WIDTH)
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

                int row = (i + k + HEIGHT) % HEIGHT;
                int col = (j + l + WIDTH) % WIDTH;

                if (d_grid[row * WIDTH + col].state == ALIVE)
                    alive_count++;
            }
        }

        // Apply the rules of the Game of Life based on the count of alive neighbors
        if (d_grid[i * WIDTH + j].state == ALIVE)
        {
            if (alive_count < 2 || alive_count > 3)
            {
                d_next_grid[i * WIDTH + j].state = DEAD;  // Underpopulation or overpopulation
            }
            else
            {
                d_next_grid[i * WIDTH + j].state = ALIVE; // Lives to next generation
            }
        }
        else
        {
            if (alive_count == 3)
            {
                d_next_grid[i * WIDTH + j].state = ALIVE; // Reproduction
            }
            else
            {
                d_next_grid[i * WIDTH + j].state = DEAD;
            }
        }
    }
}

// Function to initialize the grid, setting all cells to DEAD
void init_grid()
{
    for (size_t i = 0; i < HEIGHT; i++)
    {
        for (size_t j = 0; j < WIDTH; j++)
        {
            h_grid[i][j].state = DEAD; // Initialize all cells as DEAD
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
            if (h_grid[i][j].state == ALIVE)
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
    init_grid(); // Initialize the grid with all cells set to DEAD

    // Set up an initial configuration of live cells (a small block of cells is made ALIVE)
    for (size_t i = 0; i < WIDTH / 5; i++)
    {
        for (size_t j = 0; j < HEIGHT / 5; j++)
        {
            h_grid[j][i].state = ALIVE; // Set some cells in the top-left corner to ALIVE
        }
    }

    // Allocate memory on the device (GPU)
    Cell *d_grid, *d_next_grid;
    cudaMalloc((void **)&d_grid, WIDTH * HEIGHT * sizeof(Cell));
    cudaMalloc((void **)&d_next_grid, WIDTH * HEIGHT * sizeof(Cell));

    // Define the number of threads and blocks
    dim3 threadsPerBlock(16, 16);
    dim3 blocksPerGrid((WIDTH + threadsPerBlock.x - 1) / threadsPerBlock.x, (HEIGHT + threadsPerBlock.y - 1) / threadsPerBlock.y);

    system("clear");
    // Loop to run the simulation continuously until no more live cells exist
    while (print_grid() != 0)
    {
        // Copy host grid to device
        cudaMemcpy(d_grid, h_grid, WIDTH * HEIGHT * sizeof(Cell), cudaMemcpyHostToDevice);

        // Launch the kernel to compute the next generation on the GPU
        gen_next<<<blocksPerGrid, threadsPerBlock>>>(d_grid, d_next_grid);

        // Swap grids: d_next_grid will become the new d_grid in the next iteration
        Cell *temp = d_grid;
        d_grid = d_next_grid;
        d_next_grid = temp;

        // Copy the updated grid back to the host
        cudaMemcpy(h_grid, d_grid, WIDTH * HEIGHT * sizeof(Cell), cudaMemcpyDeviceToHost);

        usleep(SPEED * 1000); // Pause the program for a short period (to control the speed of the simulation)
        system("clear");      // Clear the console to show the updated grid
    }

    // Free device memory
    cudaFree(d_grid);
    cudaFree(d_next_grid);

    return 0;
}
