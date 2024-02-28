#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_process_sharedmemory.bin"

# Output file for CSV results
OUTPUT_FILE="sequential_results.csv"

# Initialize the results file and write the header
echo "N, CPU_Time" > $OUTPUT_FILE

# Initial value of N
N=200

# Number of loops (10 iterations to double N each time)
LOOPS=0

# Scaling factor for N in each iteration
FACTOR=2

# Execute the program for different values of N and collect the results
while [ $LOOPS -lt 10 ]
do
    echo "Running for N = $N"
    $EXECUTABLE $N >> $OUTPUT_FILE
    N=$((N * FACTOR))
    LOOPS=$((LOOPS + 1))
done

echo "Execution completed. Results saved in $OUTPUT_FILE"