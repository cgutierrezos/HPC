#!/bin/bash

# Name of the executable
INPUT_FILE="./process_sharedmemory.c"

# Output file for CSV results
OUTPUT_FILE="./process_sharedmemory.bin"


gcc $INPUT_FILE -o $OUTPUT_FILE