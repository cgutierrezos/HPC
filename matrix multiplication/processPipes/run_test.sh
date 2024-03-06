#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_process_pipes.bin"

# Output file for CSV results
OUTPUT_FILE="processpipes_results.csv"

# Initial value of N

# Number of loops (10 iterations to double N each time)
#LOOPS=0

# Scaling factor for N in each iteration
#FACTOR=2

# Number of processes to use

# Execute the program for different values of N and collect the results
#while [ $LOOPS -lt 10 ]
#do
#    echo unning for N = $N with $NUM_PROCESSES parallel processes
    
    # Run the executable with the specified number of processes and wait for it to finish
#    $EXECUTABLE $N $NUM_PROCESSES >> $OUTPUT_FILE &
#    wait
    
#    N=$((N * FACTOR))
#    LOOPS=$((LOOPS + 1))
#done

for NUM_PROCESSES in 8 4 2
do
 for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
 do
    
     echo "Running for N = $N with $NUM_PROCESSES parallel processes" >> $OUTPUT_FILE

    # Run the executable with the specified number of processes and wait for it to finish
    
    $EXECUTABLE $N $NUM_PROCESSES >> $OUTPUT_FILE 

 done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"

