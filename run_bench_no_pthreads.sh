#!/bin/sh

# compile on with gcc 

rm bench.out
#gcc  -Wall  -Wmissing-prototypes -Wmissing-declarations bench_no_pthreads.c -g -o bench.out

#compile and check assembly code

#gcc -c -g -Wa,-a,-ad bench_no_pthreads.c -o bench.out > foo.lst

# compile on with gcc 
clang -Wall bench_no_pthreads.c -o bench.out

#and run for the moment...

./bench.out
