#!/bin/bash

gcc -DUSE_CLOCK -O3 jacobi1d.c timing.c -o jacobi1d.bin
