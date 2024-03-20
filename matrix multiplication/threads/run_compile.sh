#!/bin/bash

# Name of the executable
INPUT_FILE="./threads.c"

# Output file for CSV results
OUTPUT_FILE="./threads.bin"

gcc $INPUT_FILE -o $OUTPUT_FILE -pthread

