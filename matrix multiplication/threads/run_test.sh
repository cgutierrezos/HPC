#!/bin/bash

# Name of the executable
EXECUTABLE="./threads.bin"

# Output file for CSV results
OUTPUT_FILE="threads_results.csv"


echo "threads, N, CPU_Time" > $OUTPUT_FILE

# Execute the program for different values of N and NUM_PROCESS, then collect the results
for NUM_THREADS in 16 12 8 6 4 2
do
    for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        
        echo "Running for N = $N with $NUM_THREADS threads"

        # Run the executable with the specified number of processes and wait for it to finish
        
        $EXECUTABLE $N $NUM_THREADS >> $OUTPUT_FILE 

    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"
