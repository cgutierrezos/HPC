#!/bin/bash

# Name of the executable

string_list=("-o1" "-o2" "-o3")

for str in "${string_list[@]}"
do
    INPUT_FILE="./matrix_multiplication_sequential.c"

    # Output file for CSV results
    OUTPUT_FILE="./matrix_multiplication_sequential${str:2}.bin"

    gcc $INPUT_FILE $str -o $OUTPUT_FILE
done