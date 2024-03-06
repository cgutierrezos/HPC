#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_process_pipes.bin"

# Output file for CSV results
OUTPUT_FILE="processpipes_results.csv"

# Initial value of N
N=100

# Number of loops (10 iterations to double N each time)
LOOPS=0

# Scaling factor for N in each iteration
FACTOR=2

# Number of processes to use
NUM_PROCESSES=4

# Execute the program for different values of N and collect the results
while [ $LOOPS -lt 10 ]
do
    echo "Running for N = $N with $NUM_PROCESSES parallel processes"
    
    # Run the executable with the specified number of processes and wait for it to finish
    $EXECUTABLE $N $NUM_PROCESSES >> $OUTPUT_FILE &
    wait
    
    N=$((N * FACTOR))
    LOOPS=$((LOOPS + 1))
done

echo "Execution completed. Results saved in $OUTPUT_FILE"

