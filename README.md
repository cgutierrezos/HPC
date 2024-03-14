# Matrix Multiplication Project

## Introduction

The Matrix Multiplication Project is a comprehensive exploration of different approaches to performing matrix multiplication, a fundamental operation in numerous computational fields such as numerical analysis, computer graphics, and machine learning. This project encompasses implementations ranging from a basic sequential algorithm to more advanced parallel processing techniques utilizing shared memory and pipes, aiming to investigate and compare their performance across various conditions.

## Project Structure

The project is organized into three main directories, each focusing on a distinct approach to matrix multiplication:

- **Sequential Matrix Multiplication**: Implements the traditional method of multiplying two NxN matrices in a sequential manner, serving as a baseline for performance comparison.
- **Parallel Matrix Multiplication with Shared Memory**: Utilizes multiprocessing with shared memory to parallelize the multiplication task, seeking to leverage the power of modern multicore processors for improved performance.
- **Parallel Matrix Multiplication with Pipes**: Employs pipes for inter-process communication in a parallelized environment, offering an alternative method to shared memory for distributing the computation workload.

## Key Features

- **Versatile Implementations**: From the straightforward sequential algorithm to the more complex parallel versions, the project showcases a range of strategies for matrix multiplication.
- **Performance Evaluation**: Each implementation includes performance testing scripts that measure execution times across different matrix sizes, providing valuable insights into scalability and efficiency.
- **Documentation and Ease of Use**: Detailed README files in each directory guide users through installation, usage, and understanding of each algorithm, ensuring accessibility for users with varying levels of expertise.

## Getting Started

### Prerequisites

- A Linux/Unix environment is recommended for optimal compatibility, especially for parallel implementations.
- GCC or a similar C compiler installed on your system for compiling the source code.
- Basic familiarity with terminal/shell commands for navigating directories and running scripts.

### Installation

1. Clone the repository to your local machine:

    ```bash
    git clone [git@github.com:cgutierrezos/HPC.git]
    ```

2. Navigate to the desired directory (sequential, shared memory, or pipes) and follow the specific installation and usage instructions provided in the README.md file within that directory.

## Usage Overview

Each directory contains a `run_compile.sh` script to compile the source code and a `run_tests.sh` script to execute a series of performance tests. For detailed instructions on running individual tests or customizing test parameters, refer to the README.md file in the respective directory.

## Contributing

Contributions to the Matrix Multiplication Project are warmly welcomed. Whether it's through submitting bug reports, suggesting improvements, or proposing new features, your input helps make this project more valuable for everyone. Feel free to fork the repository, make your changes, and submit a pull request.

## License

This project is open-sourced under the MIT License. See the LICENSE file for more details.

## Acknowledgments

- Thanks to all contributors and reviewers who have helped improve and refine this project.
- Special acknowledgment to the open-source community for providing the tools and platforms that make projects like this possible.

We hope this project serves as a valuable resource for those interested in the intricacies of matrix multiplication algorithms and their performance implications. Happy coding!
