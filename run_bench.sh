#!/bin/sh

# compile on with gcc 

#gcc -O3 -Wall -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations bench.c -g -o bench.out

# compile on with gcc 
clang bench.c -o bench.out

#and run for the moment...

./bench.out
