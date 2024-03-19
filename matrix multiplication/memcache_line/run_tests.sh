#!/bin/bash

NAME="sequential_memcache_line"

# Name of the executable
EXECUTABLE="./$NAME.bin"

# Output file for CSV results
OUTPUT_FILE=$NAME"_results.csv"

# Initialize the results file and write the header
echo "N, CPU_Time" > $OUTPUT_FILE

# Execute the program for different values of N and collect the results
for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Running for N = $N"
    $EXECUTABLE $N >> $OUTPUT_FILE
done

echo "Execution completed. Results saved in $OUTPUT_FILE"