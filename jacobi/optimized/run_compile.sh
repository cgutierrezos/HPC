#!/bin/bash

gcc -DUSE_CLOCK -O3 -march=native jacobi1d_optimized.c timing.c -o jacobi1d_optimized.bin
