# Parallel Matrix Multiplication with Shared Memory

## Introduction

The Parallel Matrix Multiplication with Shared Memory segment of this project delves into the advanced computational technique of utilizing shared memory for parallel processing in matrix multiplication. This approach leverages the concurrent execution capabilities of modern multi-core processors to enhance the efficiency of matrix operations, particularly for large-scale computations.

## Features

- **Parallel Execution**: Implements matrix multiplication using multiple processes that share memory space, enabling concurrent computations.
- **Scalability Analysis**: Tests the algorithm with various matrix sizes and numbers of processes to assess scalability and performance in parallel environments.
- **Performance Insights**: Measures and compares the execution time of parallel processing against the sequential approach, highlighting the speedup achieved.
- **Customization Options**: Users can specify the number of processes to be used in addition to adjusting matrix sizes and iterations, allowing for a tailored testing experience.

## System Requirements

- **Operating System**: Linux/Unix environment, suitable for handling shared memory operations and process management.
- **Compiler**: GCC or an equivalent C compiler capable of compiling multi-process code.
- **Shell**: Bash shell or a compatible command-line interface for script execution.

## Installation Instructions

1. **Obtain the Repository**: Start by cloning this repository to your system.

    ```bash
    git clone [git@github.com:cgutierrezos/HPC.git]
    ```

2. **Script Execution Permission**: Make sure that the execution scripts are set to be executable.

    ```bash
    chmod +x run_tests.sh
    chmod +x run_compile.sh
    ```

3. **Source Code Compilation**: In the project's root directory, use the provided script to compile the parallel matrix multiplication code.

    ```bash
    ./run_compile.sh
    ```

## Usage Instructions

### Executing Parallel Tests

To perform matrix multiplication using the parallel algorithm with shared memory, specify the desired matrix size and the number of processes:

```bash
./process_sharedmemory.bin <N> <Number_of_Processes>
