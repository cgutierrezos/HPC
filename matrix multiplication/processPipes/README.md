# Parallel Matrix Multiplication with Pipes

## Introduction

This segment of the Matrix Multiplication Project delves into the parallel processing realm, utilizing pipes for inter-process communication. By dividing the matrix multiplication task among multiple processes and enabling efficient data exchange via pipes, this approach seeks to harness the computational power of modern multicore systems, aiming to significantly improve execution times compared to the sequential method.

## Features

- **Parallel Execution**: Leverages multiple processes to perform matrix multiplication in parallel, reducing overall computation time.
- **Inter-Process Communication**: Utilizes pipes for efficient data transfer between parent and child processes, ensuring cohesive operation and result compilation.
- **Scalability Analysis**: Tests the algorithm with various matrix sizes and numbers of processes to evaluate scalability and identify optimal configurations.
- **Performance Insights**: Gathers and reports detailed performance metrics, highlighting the speedup achieved through parallelization.

## System Requirements

- **Operating System**: Primarily designed for Linux/Unix environments but can be adapted for others.
- **Compiler**: Requires GCC or a compatible C compiler to compile the source code.
- **Shell**: Bash or a similar shell environment is recommended for executing scripts.

## Installation Instructions

1. **Clone the Repository**: Obtain a copy of the project repository on your local system.

    ```bash
    git clone [git@github.com:cgutierrezos/HPC.git]
    ```
2. **Make Scripts Executable**: Ensure that the execution scripts have the necessary permissions.

    ```bash
    chmod +x run_tests.sh
    chmod +x run_compile.sh
    ```

3. **Compile the Source Code**: Use the provided script to compile the parallel algorithm with pipes.

    ```bash
    ./run_compile.sh
    ```

## Detailed Usage Guide

### Running a Single Test

Execute the parallel matrix multiplication program with a specified matrix size and number of processes:

```bash
./process_pipes.bin <N> <num_processes> [-v]
