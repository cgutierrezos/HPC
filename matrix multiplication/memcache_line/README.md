# Sequential Matrix Multiplication

## Introduction

The Sequential Matrix Multiplication project focuses on the fundamental operation of matrix multiplication, a cornerstone in numerical analysis, computer graphics, optimization, and more. This project implements a basic yet efficient algorithm for multiplying two NxN matrices in a sequential manner, without parallelization, to understand the performance implications of matrix operations at scale.

## Features

- **Scalability Testing**: Executes matrix multiplication across a range of matrix sizes, starting from N=200 and doubling up to 10 iterations, to evaluate performance at various scales.
- **Performance Metrics**: Records CPU time for each operation, offering insights into computational complexity and resource utilization.
- **Ease of Use**: Simple command-line interface for executing tests and generating performance data.
- **Customizability**: Offers the flexibility to adjust the starting matrix size, the number of iterations, and the scaling factor.

## System Requirements

- **Operating System**: Linux/Unix environment.
- **Compiler**: GCC or any standard C compiler.
- **Shell**: Bash shell for running the automation script.

## Installation Instructions

1. **Clone the Repository**: Clone or download the project repository to your local machine.

    ```bash
    git clone [repository-url]
    ```
2. **Mark Scripts as Executable**: Ensure the shell script is executable.

    ```bash
    chmod +x run_tests.sh
    chmod +x run_compile.sh
    ```

3. **Compile the Source Code**: Navigate to the project directory and compile the source code using GCC.

    ```bash
    ./run_compile.sh
    ```


## Detailed Usage Guide

### Running a Single Test

To run the matrix multiplication program for a specific matrix size:

```bash
./sequential_memcache_line.bin <N> [-v]



### Running a Single Test

To run the matrix multiplication program for performance test:

```bash
./run_tests.sh

