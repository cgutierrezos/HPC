# Name of the executable
EXECUTABLE="./matrixopenmp.bin"

# Output file for CSV results
OUTPUT_FILE="matrixopenmp_results.csv"

# Initialize the results file and write the header
echo "Processes, N, CPU_Time" > $OUTPUT_FILE

for num_threads in 4 2
do

# Execute the program for different values of N and collect the results
for N in 200 500 1000 2000 3000 4000 5000 6000 7000 8000
do
    echo "Running for N = $N with $num_threads parallel threads"
    $EXECUTABLE $N $num_threads >> $OUTPUT_FILE
done

done

echo "Execution completed. Results saved in $OUTPUT_FILE"
