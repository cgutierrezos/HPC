#!/bin/bash

# Name of the executable
INPUT_FILE="./matrix_multiplication_process_sharedmemory.c"

# Output file for CSV results
OUTPUT_FILE="./matrix_multiplication_process_sharedmemory.bin"


gcc $INPUT_FILE -o $OUTPUT_FILE