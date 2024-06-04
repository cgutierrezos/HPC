#!/bin/bash

# Name of the executable
INPUT_FILE="./mmmpi.c"

# Output file for CSV results
OUTPUT_FILE="./mmmpi.bin"

mpicc $INPUT_FILE -o $OUTPUT_FILE

