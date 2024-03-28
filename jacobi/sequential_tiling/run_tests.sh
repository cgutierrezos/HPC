#!/bin/bash

# Name of the executable
EXECUTABLE="./jacobi1d.bin"

# Output file for CSV results
OUTPUT_FILE="jacobi1d_results.csv"

# Initialize the results file and write the header
echo "N, STEPS, CPU_Time" > $OUTPUT_FILE

for STEPS in 2000 3000
do
    # Execute the program for different values of N and collect the results
    for N in 2000000 5000000 10000000 20000000 30000000
    do
        echo "Running for N = $N, STEPS = $STEPS"
        $EXECUTABLE $N $STEPS >> $OUTPUT_FILE
    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"