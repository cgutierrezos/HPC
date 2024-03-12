#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_process_sharedmemory.bin"

# Output file for CSV results
OUTPUT_FILE="sequential_results.csv"

# Initialize the results file and write the header
echo "Processes, N, CPU_Time" > $OUTPUT_FILE

for num_processes in 16 12 8 6 4 2
do

# Execute the program for different values of N and collect the results
for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Running for N = $N with $num_processes parallel processes"
    $EXECUTABLE $N $num_processes >> $OUTPUT_FILE
done

done

echo "Execution completed. Results saved in $OUTPUT_FILE"