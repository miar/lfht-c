#!/bin/sh

# compile on with gcc 

rm bench.out
del output/correct_hash output/result_hash

gcc  -Wall -Wmissing-prototypes -Wmissing-declarations bench_pthreads.c -lpthread -g -o bench.out

#compile and check assembly code

#gcc -c -g -Wa,-a,-ad -lpthread bench_pthreads.c -o bench.out > foo.lst

# compile on with gcc 
#clang -Wall -lpthread bench_pthreads.c -o bench.out

#and run for the moment...

./bench.out

#tkdiff output/correct_hash output/result_hash
