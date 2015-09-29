#!/bin/sh

# compile on with gcc 

rm bench.out
#gcc  -Wall  -Wmissing-prototypes -Wmissing-declarations bench.c -g -o bench.out

#compile and check assembly code

#gcc -c -g -Wa,-a,-ad bench.c -o bench.out > foo.lst

# compile on with gcc 
clang -Wall bench.c -o bench.out

#and run for the moment...

./bench.out
