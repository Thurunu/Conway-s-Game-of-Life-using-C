
# Game of Life - CPU and CUDA Implementations

This repository contains two implementations of Conway's Game of Life:
1. A CPU-based version (original code provided by the user).
2. A CUDA-accelerated version (code modified for parallel execution on a GPU).

The Game of Life is a cellular automaton devised by mathematician John Conway, in which cells evolve over discrete time steps based on a set of rules that simulate life and death.

## 1. CPU Version

### Overview
The CPU version uses a 2D grid of cells, where each cell can be either **ALIVE** or **DEAD**. The evolution of the grid over time follows the standard Game of Life rules:
- Any live cell with fewer than two live neighbors dies (underpopulation).
- Any live cell with two or three live neighbors survives.
- Any live cell with more than three live neighbors dies (overpopulation).
- Any dead cell with exactly three live neighbors becomes alive (reproduction).

### Files
- `game_of_life_cpu.c`: Contains the CPU implementation of the Game of Life.

### Compilation and Execution
To compile and run the CPU version:
\`\`\`bash
gcc game_of_life_cpu.c -o game_of_life_cpu
./game_of_life_cpu
\`\`\`

### Customization
- You can modify the grid dimensions by changing the `WIDTH` and `HEIGHT` constants in the code.
- You can control the speed of the simulation by adjusting the `SPEED` constant (in milliseconds).

---

## 2. CUDA Version

### Overview
The CUDA version is a GPU-accelerated version of the Game of Life. By using CUDA, we can run the computation of the next state of the grid in parallel on many threads, significantly speeding up the simulation, especially for larger grids.

### Files
- `game_of_life_cuda.cu`: Contains the CUDA implementation of the Game of Life.

### CUDA-Specific Changes
- The core logic for generating the next grid state has been moved to a CUDA kernel (`gen_next_kernel`), which runs on the GPU.
- Memory for the grid is allocated on both the host (CPU) and device (GPU), and data is copied between them using CUDA API functions.
- Grid updates are processed in parallel on the GPU, with each thread responsible for updating one cell.

### Compilation and Execution
To compile and run the CUDA version, you will need a CUDA-enabled GPU and the CUDA toolkit installed on your machine.

To compile:
\`\`\`bash
nvcc game_of_life_cuda.cu -o game_of_life_cuda
\`\`\`

To run the executable:
\`\`\`bash
./game_of_life_cuda
\`\`\`

### Customization
- Modify the `WIDTH`, `HEIGHT`, and `SPEED` constants to change the grid size and simulation speed.
- The CUDA kernel uses a block size of `(16, 16)`. You can adjust the block size depending on the number of CUDA cores on your GPU for better performance.

---

## Game of Life Rules

1. **Underpopulation**: Any live cell with fewer than two live neighbors dies.
2. **Survival**: Any live cell with two or three live neighbors lives on to the next generation.
3. **Overpopulation**: Any live cell with more than three live neighbors dies.
4. **Reproduction**: Any dead cell with exactly three live neighbors becomes alive.

## Requirements

### CPU Version:
- C Compiler (e.g., GCC)
- Terminal with ANSI escape sequence support (for clearing the screen)

### CUDA Version:
- CUDA-enabled GPU
- NVIDIA CUDA Toolkit
- C Compiler (e.g., GCC)

## How to Use

1. **Compile and Run**:
   - For CPU: Follow the instructions in the "CPU Version" section.
   - For CUDA: Follow the instructions in the "CUDA Version" section.
   
2. **Grid Display**:
   - The grid will be displayed in the terminal, where:
     - **'E'** represents an alive cell.
     - **'.'** represents a dead cell.
   
3. **Simulation End**:
   - The simulation continues running until no more live cells remain on the grid.

---

## License

This project is released under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Original CPU code was provided by the user and modified for CUDA acceleration.
- CUDA implementation leverages parallelism to enhance performance for larger grid sizes.
