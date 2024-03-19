#!/bin/bash

NAME="sequential_memcache_line"

# Name of the executable
INPUT_FILE="./$NAME.c"

# Output file for CSV results
OUTPUT_FILE="./$NAME.bin"


gcc $INPUT_FILE -o $OUTPUT_FILE
