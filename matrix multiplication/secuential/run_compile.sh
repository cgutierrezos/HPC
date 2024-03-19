#!/bin/bash

# Name of the executable
INPUT_FILE="./sequential.c"

# Output file for CSV results
OUTPUT_FILE="./sequential.bin"


gcc $INPUT_FILE -o $OUTPUT_FILE
