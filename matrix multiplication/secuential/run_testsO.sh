#!/bin/bash



# Execute the program for different values of N and collect the results

for i in {1..3}
do
    # Name of the executable
    EXECUTABLE="./sequentialO$i.bin"

    # Output file for CSV results
    OUTPUT_FILE="sequential_results_O$i.csv"

    # Initialize the results file and write the header
    echo "N, CPU_Time" > $OUTPUT_FILE
    for N in 500 1000
    do
        echo "Running for N = $N"
        $EXECUTABLE $N >> $OUTPUT_FILE
    done

    echo "Execution completed. Results saved in $OUTPUT_FILE"
done



