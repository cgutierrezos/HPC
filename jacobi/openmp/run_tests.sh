EXECUTABLE="./jacobi1d_optimizedOPENMP.bin"

# Output file for CSV results
OUTPUT_FILE="jacobi1d_optimizedOPENMP_results.csv"

# Initialize the results file and write the header
echo "NUM_PROCESS, N, STEPS, CPU_Time" > $OUTPUT_FILE

for NUM_PROCESS in 2 4 6 8 12 16
do
    for STEPS in 200 500 1000 2000 3000
    do
        # Execute the program for different values of N and collect the results
        for N in 2000000 5000000 10000000 20000000 30000000 
        do
            echo "Running for N = $N, STEPS = $STEPS  NUM_PROCESS = $NUM_PROCESS"
            $EXECUTABLE $N $STEPS $NUM_PROCESS >> $OUTPUT_FILE
        done
    done
done

echo "Execution completed. Results saved in $OUTPUT_FILE"