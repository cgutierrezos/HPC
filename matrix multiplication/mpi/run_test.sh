#!/bin/bash

HOSTS=$1

echo "HOSTS = $HOSTS"

# Name of the executable
EXECUTABLE="./mmmpi.bin"

# Output file for CSV results
OUTPUT_FILE="mmmpi_results.csv"


echo "threads, N, CPU_Time" > $OUTPUT_FILE

# Execute the program for different values of N and NUM_PROCESS, then collect the results
for NUM_THREADS in 8 4
do
    for N in 200 500 1000
    do
        
        echo "Running for N = $N with $NUM_THREADS threads in hosts $HOSTS"

        # Run the executable with the specified number of processes and wait for it to finish
        mpiexec -n $NUM_THREADS -hosts $HOSTS $EXECUTABLE $N >> $OUTPUT_FILE 

    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"
