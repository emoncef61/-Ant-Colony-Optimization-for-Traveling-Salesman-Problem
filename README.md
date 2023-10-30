# Ant Colony Optimization for Traveling Salesman Problem

This C program implements the Ant Colony Optimization (ACO) algorithm to solve the Traveling Salesman Problem (TSP). The TSP is a classic optimization problem that seeks to find the shortest possible route that visits a given set of cities and returns to the original city.

The ACO algorithm simulates the foraging behavior of ants to find an approximate solution to the TSP. It iteratively improves the tour length by adjusting the pheromone levels on the paths between cities.

## Prerequisites

- C compiler (e.g., GCC)
- SDL2 library for visualization

## Getting Started

1. Clone this repository to your local machine.
2. Compile the source code using your C compiler. You can use the following command:
   ```shell
   gcc -o main main.c -lSDL2 -lm

3. run ./a.out

Usage

Upon running the program, you will see a window displaying a visualization of the TSP and the ACO algorithm.
The program will display the best tour found by the ants and the length of that tour.
You can close the window to exit the program.
Configuration

You can adjust the following parameters in the C code for your specific use case:

NUM_ANTS: Number of ants in the colony.
ALPHA and BETA: Influence factors for pheromone and visibility, respectively.
EVAPORATION: Pheromone evaporation rate.
Q: Pheromone quantity to deposit.
MAX_ITERATIONS: Maximum number of iterations for the ACO algorithm.
N: Number of cities in the TSP.
Other constants and data structures as needed for your problem.
