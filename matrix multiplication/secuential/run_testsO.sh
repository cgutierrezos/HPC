#!/bin/bash

# Name of the executable
EXECUTABLE="./matrix_multiplication_sequential"

# Output file for CSV results
OUTPUT_FILE="sequential_resultsO.csv"

# Initialize the results file and write the header
echo "N, CPU_Time" > $OUTPUT_FILE

# Execute the program for different values of N and collect the results

for i in {1..3}
do
    echo "with -o$i" >> $OUTPUT_FILE
    for N in 500 1000
    do
        echo "Running for N = $N"
        $EXECUTABLE$i".bin" $N >> $OUTPUT_FILE
    done
done



echo "Execution completed. Results saved in $OUTPUT_FILE"