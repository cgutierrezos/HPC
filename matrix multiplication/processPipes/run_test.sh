#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_process_pipes.bin"

# Output file for CSV results
OUTPUT_FILE="processpipes_results.csv"


echo "Processes, N, CPU_Time" > $OUTPUT_FILE

# Execute the program for different values of N and NUM_PROCESS, then collect the results
for NUM_PROCESSES in 2 4 6 8 12 16
do
    for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
    do
        
        echo "Running for N = $N with $NUM_PROCESSES parallel processes"

        # Run the executable with the specified number of processes and wait for it to finish
        
        $EXECUTABLE $N $NUM_PROCESSES >> $OUTPUT_FILE 

    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"

